/******************************************************************************
 * $Id: shputils.cpp,v 1.1 2005/08/18 01:00:41 ahartman Exp $
 *
 * Project:  Shapelib
 * Purpose:  
 *   Altered "shpdump" and "dbfdump" to allow two files to be appended.
 *   Other Functions:
 *     Selecting from the DBF before the write occurs.
 *     Change the UNITS between Feet and Meters and Shift X,Y.
 *     Clip and Erase boundary.
 *
 *   Bill Miller   NC-DOT -- Feb. 1997 -- bmiller@doh.dot.state.nc.us
 *         There was not a lot of time to debug hidden problems;
 *         And the code is not very well organized or documented.
 *         The clip/erase function was not well tested.
 *
 *    PURPOSE: I needed a program to Append, Select, Change Unit, and 
 *             Clip boundaries.  The program only passes thru the
 *             data once.
 *
 * Author:   Bill Miller (bmiller@doh.dot.state.nc.us)
 *
 ******************************************************************************
 * Copyright (c) 1999, Frank Warmerdam
 *
 * This software is available under the following "MIT Style" license,
 * or at the option of the licensee under the LGPL (see LICENSE.LGPL).  This
 * option is discussed in more detail in shapelib.html.
 *
 * --
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 *
 * $Log: shputils.cpp,v $
 * Revision 1.1  2005/08/18 01:00:41  ahartman
 * Initial commit of the source code from the Cartographic Research web page.
 *
 * Revision 1.4  1999/11/05 14:12:05  warmerda
 * updated license terms
 *
 * Revision 1.3  1998/12/03 15:47:39  warmerda
 * Did a bunch of rewriting to make it work with the V1.2 API.
 *
 * Revision 1.2  1998/06/18 01:19:49  warmerda
 * Made C++ compilable.
 *
 * Revision 1.1  1997/05/27 20:40:27  warmerda
 * Initial revision
 */

//# include <memory.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

static char rcsid[] = 
  "$Id: shputils.cpp,v 1.1 2005/08/18 01:00:41 ahartman Exp $";

#include "shapefil.h"
#include "string.h"
#ifndef FALSE
#  define FALSE		0
#  define TRUE		1
#endif

char            infile[80], outfile[80], temp[400];

/* Variables for shape files */
SHPHandle	hSHP;
SHPHandle	hSHPappend;
int		nShapeType, nEntities, iPart;
int		nShapeTypeAppend, nEntitiesAppend;
SHPObject	*psCShape;
double		adfBoundsMin[4], adfBoundsMax[4];


/* Variables for DBF files */
DBFHandle	hDBF;
DBFHandle	hDBFappend;
    
DBFFieldType    iType;
DBFFieldType    jType;
    
char	iszTitle[12];
char	jszTitle[12];

int	*pt;
char	iszFormat[32], iszField[1024];
char	jszFormat[32], jszField[1024];
int	i, ti, iWidth, iDecimals, iRecord;
int	j, tj, jWidth, jDecimals, jRecord;
int     found, newdbf;

void openfiles(void);
void setext(char *pt, char *ext);
int strncasecmp2(char *s1, char *s2, int n);
void mergefields(void);
void findselect(void);
void showitems(void);
int selectrec();
int check_theme_bnd();
int clip();
void error();


/* -------------------------------------------------------------------- */
/* Variables for the SELECT function */
/* -------------------------------------------------------------------- */
   char    selectitem[40], *cpt;
   int     selectvalues[150];
   int     iselect = FALSE, iselectitem = -1, selcount=0;
   int     iunselect = FALSE;

/* -------------------------------------------------------------------- */
/* Variables for the CLIP and ERASE functions */
/* -------------------------------------------------------------------- */
   double  cxmin, cymin, cxmax, cymax; 
   int     iclip = FALSE, ierase = FALSE;
   int     itouch = FALSE, iinside = FALSE, icut = FALSE;
   int     ibound = FALSE, ipoly = FALSE;
   char    clipfile[80];

/* -------------------------------------------------------------------- */
/* Variables for the UNIT function */
/* -------------------------------------------------------------------- */
   double  factor = 1;  /* NO FACTOR */
   int     iunit = FALSE;
   
   
/* -------------------------------------------------------------------- */
/* Variables for the SHIFT function */
/* -------------------------------------------------------------------- */
   double  xshift = 0, yshift = 0;  /* NO SHIFT */
      



/************************************************************************/
/*                             openfiles()                              */
/************************************************************************/

void openfiles() {
/* -------------------------------------------------------------------- */
/*      Open the DBF file.                                              */
/* -------------------------------------------------------------------- */
    setext(infile, "dbf");
    hDBF = DBFOpen( infile, "rb" );
    if( hDBF == NULL )
    {
	printf( "ERROR: Unable to open the input DBF:%s\n", infile );
	exit( 1 );
    }
/* -------------------------------------------------------------------- */
/*      Open the append DBF file.                                       */
/* -------------------------------------------------------------------- */
    if (strcmp(outfile,"")) {
        setext(outfile, "dbf");
        hDBFappend = DBFOpen( outfile, "rb+" );
        newdbf=0;
        if( hDBFappend == NULL )
        {
            newdbf=1;
            hDBFappend = DBFCreate( outfile );
            if( hDBFappend == NULL )
            {
                printf( "ERROR: Unable to open the append DBF:%s\n", outfile );
                exit( 1 );
            }
        }
    }
/* -------------------------------------------------------------------- */
/*      Open the passed shapefile.                                      */
/* -------------------------------------------------------------------- */
    setext(infile, "shp");
    hSHP = SHPOpen( infile, "rb" );

    if( hSHP == NULL )
    {
	printf( "ERROR: Unable to open the input shape file:%s\n", infile );
	exit( 1 );
    }

    SHPGetInfo( hSHP, &nEntities, &nShapeType, NULL, NULL );

/* -------------------------------------------------------------------- */
/*      Open the passed append shapefile.                               */
/* -------------------------------------------------------------------- */
    if (strcmp(outfile,"")) {
        setext(outfile, "shp");
        hSHPappend = SHPOpen( outfile, "rb+" );

        if( hSHPappend == NULL )
        {
            hSHPappend = SHPCreate( outfile, nShapeType );
            if( hSHPappend == NULL )
            {
                printf( "ERROR: Unable to open the append shape file:%s\n",
                        outfile );
                exit( 1 );
            }
        }
        SHPGetInfo( hSHPappend, &nEntitiesAppend, &nShapeTypeAppend,
                    NULL, NULL );

        if (nShapeType != nShapeTypeAppend) 
        {
            puts( "ERROR: Input and Append shape files are of different types.");
            exit( 1 );
        }
    }
}

/* -------------------------------------------------------------------- */
/*	Change the extension.  If there is any extension on the 	*/
/*	filename, strip it off and add the new extension                */
/* -------------------------------------------------------------------- */
void setext(char *pt, char *ext)
{
int i;
    for( i = strlen(pt)-1; 
	 i > 0 && pt[i] != '.' && pt[i] != '/' && pt[i] != '\\';
	 i-- ) {}

    if( pt[i] == '.' )
        pt[i] = '\0';
        
    strcat(pt,".");
    strcat(pt,ext);
}



/* -------------------------------------------------------------------- */
/*	Find matching fields in the append file.                        */
/*      Output file must have zero records to add any new fields.       */
/* -------------------------------------------------------------------- */
void mergefields()
{
    int i,j;
    ti = DBFGetFieldCount( hDBF );
    tj = DBFGetFieldCount( hDBFappend );
    /* Create a pointer array for the max # of fields in the output file */
    pt = (int *) malloc( (ti+tj+1) * sizeof(int) ); 
    
    for( i = 0; i < ti; i++ )
    {
       pt[i]= -1;  /* Initial pt values to -1 */
    }
    /* DBF must be empty before adding items */
    jRecord = DBFGetRecordCount( hDBFappend );
    for( i = 0; i < ti; i++ )
    {
	iType = DBFGetFieldInfo( hDBF, i, iszTitle, &iWidth, &iDecimals );
        found=0;
        {
      	    for( j = 0; j < tj; j++ )   /* Search all field names for a match */
    	    {
	        jType = DBFGetFieldInfo( hDBFappend, j, jszTitle, &jWidth, &jDecimals );
	        if (iType == jType && (strcmp(iszTitle, jszTitle) == 0) )
	        {
	            if (found == 1  ||  newdbf == 1)
	            {
	                if (i == j)  pt[i]=j;
	                printf("Warning: Duplicate field name found (%s)\n",iszTitle);
	                /* Duplicate field name
	                   (Try to guess the correct field by position) */
	            }
	            else
	            {
	            	pt[i]=j;  found=1; 
	            }
	        }
	    }
	}
	
	if (pt[i] == -1  && found == 0)  /* Try to force into an existing field */
	{                                /* Ignore the field name, width, and decimal places */
	    jType = DBFGetFieldInfo( hDBFappend, j, jszTitle, &jWidth, &jDecimals );
	    if (iType == jType) 
	    {
	    	pt[i]=i;  found=1;
	    }
	}
	if (found == 0  &&  jRecord == 0)  /* Add missing field to the append table */
	{                 /* The output DBF must be is empty */
	    pt[i]=tj;
	    tj++;
	    if( !DBFAddField( hDBFappend, iszTitle, iType, iWidth, iDecimals ))
	    {
		printf( "Warning: DBFAddField(%s, TYPE:%d, WIDTH:%d  DEC:%d, ITEM#:%d of %d) failed.\n",
		         iszTitle, iType, iWidth, iDecimals, (i+1), (ti+1) );
		pt[i]=-1;
	    }
	}
    }
}


void findselect()
{
    /* Find the select field name */
    iselectitem = -1;
    for( i = 0; i < ti  &&  iselectitem < 0; i++ )
    {
	iType = DBFGetFieldInfo( hDBF, i, iszTitle, &iWidth, &iDecimals );
        if (strncasecmp2(iszTitle, selectitem, 0) == 0) iselectitem = i;
    }
    if (iselectitem == -1) 
    {
        printf("Warning: Item not found for selection (%s)\n",selectitem);
        iselect = 0;
	showitems();
        printf("Continued... (Selecting entire file)\n");
    }
    /* Extract all of the select values (by field type) */
    
}

void showitems()
{
        printf("Available Items: ");
        for( i = 0; i < ti; i++ )
        {
	    iType = DBFGetFieldInfo( hDBF, i, iszTitle, &iWidth, &iDecimals );
	    printf("%s, ",iszTitle);
        }
        printf("(total=%d)\n",ti);
}

int selectrec()
{
int value, ty;

   ty = DBFGetFieldInfo( hDBF, iselectitem, NULL, &iWidth, &iDecimals);
      switch(ty)
      {
      case FTString:
	break;
      case FTInteger:
        value = DBFReadIntegerAttribute( hDBF, iRecord, iselectitem );
        for (j = 0; j<selcount; j++)
          {
          if (selectvalues[j] == value)
               if (iunselect) return(0);  /* Keep this record */
                        else  return(1);  /* Skip this record */
          }
	break;
      case FTDouble:
        break;
      }
      if (iunselect) return(1);  /* Skip this record */
               else  return(0);  /* Keep this record */
}


int check_theme_bnd()
{
    if ( (adfBoundsMin[0] >= cxmin) && (adfBoundsMax[0] <= cxmax) &&
         (adfBoundsMin[1] >= cymin) && (adfBoundsMax[1] <= cymax) )
    {   /** Theme is totally inside clip area **/
        if (ierase) nEntities=0; /** SKIP THEME  **/
        else   iclip=FALSE; /** WRITE THEME (Clip not needed) **/
    }
            
    if ( ( (adfBoundsMin[0] < cxmin) && (adfBoundsMax[0] < cxmin) ) ||
         ( (adfBoundsMin[1] < cymin) && (adfBoundsMax[1] < cymin) ) ||
         ( (adfBoundsMin[0] > cxmax) && (adfBoundsMax[0] > cxmax) ) ||
         ( (adfBoundsMin[1] > cymax) && (adfBoundsMax[1] > cymax) ) )
    {   /** Theme is totally outside clip area **/
        if (ierase) iclip=FALSE; /** WRITE THEME (Clip not needed) **/
        else   nEntities=0; /** SKIP THEME  **/
    }
            
    if (nEntities == 0)
        puts("WARNING: Theme is outside the clip area."); /** SKIP THEME  **/
	return 0;
}

int clip()
{
    int  outside=FALSE;
    int  j2=0, i2=0;

    if ( (psCShape->dfXMin >= cxmin) && (psCShape->dfXMax <= cxmax) &&
         (psCShape->dfYMin >= cymin) && (psCShape->dfYMax <= cymax) )
    {   /** Feature is totally inside clip area **/
        if (ierase) return(0); /** SKIP  RECORD **/
        else   return(1); /** WRITE RECORD **/
    }
            
    if ( ( psCShape->dfXMax < cxmin ) ||
         ( psCShape->dfYMax < cymin ) ||
         ( psCShape->dfXMin > cxmax ) ||
         ( psCShape->dfYMin > cymax ) )
    {   /** Feature is totally outside clip area **/
        if (ierase) return(1); /** WRITE RECORD **/
        else   return(0); /** SKIP  RECORD **/
    }
       
    if (itouch)
    {
        if (ierase) return(0); /** SKIP  RECORD **/
        else   return(1); /** WRITE RECORD  **/
    }
            
    if (iinside)
    {
        if (ierase) return(1); /** WRITE RECORD **/
        else   return(0); /** SKIP  RECORD **/
    }
           
    /*** SECOND check each vertex in the feature ***/
    for( j2 = 0; j2 < psCShape->nVertices; j2++ ) 
    {
        if (psCShape->padfX[j2] < cxmin  || psCShape->padfX[j2] > cxmax)
        {
            outside=TRUE;
        }
        else
        {
            if (psCShape->padfY[j2] < cymin  || psCShape->padfY[j2] > cymax)
                outside=TRUE;
            else
                outside=FALSE;
        }
        
        
        if (icut)
        {
            if (outside)
            {
            } else {
                if (i2 != j2)
                {
                    /* write vertex */
                    psCShape->padfX[i2] = psCShape->padfX[j2];
                    psCShape->padfY[i2] = psCShape->padfY[j2];
                }
                i2++;
            }
        }
        else
            if (outside)  /* vertex is outside boundary */
            {
                if (iinside)
                {
                    if (ierase) return(1); /** WRITE RECORD **/
                    else   return(0); /** SKIP RECORD **/
                }
            }
            else         /* vertex is inside boundary */
            {
                if (itouch) 
                {
                    if (ierase) return(0); /** SKIP RECORD  **/
                    else   return(1); /** WRITE RECORD **/
                }
            }
    }
    
    if (icut)
    {
        j2 = psCShape->nVertices;
        if (i2 < 2) return(0); /** SKIP RECORD **/

        psCShape->nVertices = i2;

        printf("Vertices:%d   OUT:%d   Number of Parts:%d\n",
               j2, psCShape->nVertices, psCShape->nParts );
    }
    if (itouch)
    {
        if (ierase) return(1); /** WRITE RECORD **/
        else   return(0); /** SKIP RECORD  **/
    }
    if (iinside)   
    {
        if (ierase) return(0); /** SKIP RECORD  **/
        else   return(1); /** WRITE RECORD **/
    }
	return 0;
}

/* -------------------------------------------------------------------- */
/*      Display a usage message.                                        */
/* -------------------------------------------------------------------- */
void error()
    {
	puts( "USAGE: shputils  <DescribeShape>");
	puts( "USAGE: shputils  <InputShape>  <AppendShape>" );
	puts( "      { <SELECT>   <Item> <valuelist> }" );
	puts( "      { <UNSELECT> <Item> <valuelist> }" );
	puts( "      { <CLIP>   <xmin> <ymin> <xmax> <ymax> <TOUCH|INSIDE|CUT> }" );
	puts( "      { <CLIP>   <Theme>  <BOUNDARY|POLYGON> <TOUCH|INSIDE|CUT> }" );
	puts( "      Clip functions for Cut and Polygon are not supported yet..." );
	puts( "      { <ERASE>  <xmin> <ymin> <xmax> <ymax> <TOUCH|INSIDE|CUT> }" );
	puts( "      { <ERASE>  <Theme>  <BOUNDARY|POLYGON> <TOUCH|INSIDE|CUT> }" );
	puts( "      { <UNIT>   <FEET|METERS|factor>  }" );
	puts( "      { <SHIFT>  <xshift> <yshift> }\n" );
	
	
	puts( "The program will append to an existing shape file or it will" );
	puts( "create a new file if needed." );
	puts( "Only the items in the first output file will be preserved." );
	puts( "When an item does not match with the append theme then the item");
	puts( "might be placed to an existing item at the same position and type." );
	puts( "  OTHER FUNCTIONS:" );
	puts( "  - Select a group of shapes from a comma separated selection list.");
	puts( "  - UnSelect a group of shapes from a comma separated selection list.");
	puts( "  - Clip boundary extent or by theme boundary." );
	puts( "      Touch writes all the shapes that touch the boundary.");
	puts( "      Inside writes all the shapes that are completely within the boundary.");
	puts( "      *(N/A) Cut will cookie-cut shapes that are touching the boundary.");
	puts( "      Boundary clips are only the min and max of a theme boundary." );
	puts( "      *(N/A) Polygon clips use the polygons within a theme.");
	puts( "  - Erase boundary extent or by theme boundary." );
	puts( "      Erase is the direct opposite of the Clip function." );
	puts( "  - Change coordinate value units between meters and feet.");
	puts( "      There is no way to determine the input unit of a shape file.");
	puts( "      Skip this function if the shape file is already in the correct unit.");
	puts( "      Clip and Erase will be done before the unit is changed.");
	puts( "      A shift will be done after the unit is changed."); 
	puts( "  - Shift X and Y coordinates.\n" );
	puts( "Finally, There can only be one select or unselect in the command line.");
	puts( "         There can only be one clip or erase in the command line.");
	puts( "         There can only be one unit and only one shift in the command line.");
	puts( "EX: shputils in.shp out.shp  CLIP 10 10 90 90 Touch  UNIT Feet  SHIFT 40 40");
	puts( "    shputils in.shp out.shp  SELECT countycode 3,5,9,13,17,27");
	exit( 1 );
    }

/************************************************************************/
/*                            strncasecmp2()                            */
/*                                                                      */
/*      Compare two strings up to n characters                          */
/*      If n=0 then s1 and s2 must be an exact match                    */
/************************************************************************/

int strncasecmp2(char *s1, char *s2, int n)

{
int j,i;
   if (n<1) n=strlen(s1)+1;
   for (i=0; i<n; i++)
   {
      if (*s1 != *s2)
      {
         if (*s1 >= 'a' && *s1 <= 'z') {
            j=*s1-32;
            if (j != *s2) return(1);
         }
         else
         {
            j=*s1+32;
            if (j != *s2) return(1);
         }
      }
      s1++;
      s2++;
   }
   return(0);
}
