/* ****************************************************************************
 * Purpose:  GDAL Image Splitting Program
 * Author:   Homme Zwaagstra, geodata at soton.ac.uk
 *
 * This program is heavily derived from the GDAL gdal_translate utility
 *
 * ****************************************************************************
 * Copyright (c) 1998, 2002, Frank Warmerdam
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
 * ****************************************************************************
 */

#include "gdal.h"
#include "cpl_string.h"
#include "vrtdataset.h"
#include <cstring>

/* compilation on WIN32 machines has not been tested! */
//#ifdef WIN32
//#define PATH_SEP_CHAR '\\'
//#else
#define PATH_SEP_CHAR '/'
//#endif

static CPLErr CopyRasterBand(GDALDataset *poSrcDS, GDALDataset *poDstDS, 
                             int nBand, int nSrcXOff, int nSrcYOff, 
                             int nSrcXSize, int nSrcYSize);
static VRTDataset * CopyRasterWindow(GDALDatasetH hSrcDS, 
                                     int nSrcXOff, int nSrcYOff, 
                                     int nSrcXSize, int nSrcYSize);
static void AttachMetadata( GDALDatasetH, char ** );
static int SplitFile(const char *pszSource, 
                     int cols, int rows, int bQuiet, 
                     GDALProgressFunc pfnProgress);
static char * CreateString(const char *fmt, ...);
static char * CreateFileName(const char *pszSource, int col, int row);
static char * GetExtension(const char *basename);
static char * GetDirname(const char *string);
static char * GetBasename(const char *string);


/* ******************************************************************** */
/*                               Usage()                                */
/* ******************************************************************** */

static void Usage()

{
    int	iDr;
        
    printf( "Usage: gdalsplit [--version]\n"
            "       [-c cols]\n"
            "       [-r rows]\n"
            "       [-quiet]\n"
            "       src_dataset...\n\n" );

    printf( "%s\n\n", GDALVersionInfo( "--version" ) );
    printf( "The following format drivers are configured and support output:\n" );
    for( iDr = 0; iDr < GDALGetDriverCount(); iDr++ )
		{
			GDALDriverH hDriver = GDALGetDriver(iDr);
        
			if( GDALGetMetadataItem( hDriver, GDAL_DCAP_CREATE, NULL ) != NULL
				|| GDALGetMetadataItem( hDriver, GDAL_DCAP_CREATECOPY,
										NULL ) != NULL )
				{
					printf( "  %s: %s\n",
							GDALGetDriverShortName( hDriver ),
							GDALGetDriverLongName( hDriver ) );
				}
		}
}


/* ******************************************************************** */
/*                                main()                                */
/* ******************************************************************** */

int main( int argc, char ** argv )

{
	int bQuiet = FALSE, i, j = 0;
	int cols = 2, rows = 2;
    GDALProgressFunc pfnProgress = GDALTermProgress;

/* -------------------------------------------------------------------- */
/*      Register standard GDAL drivers, and identify output driver.     */
/* -------------------------------------------------------------------- */

    GDALAllRegister();

/* -------------------------------------------------------------------- */
/*      Handle command line arguments.                                  */
/* -------------------------------------------------------------------- */

	for( i = 1; i < argc; i++ ) {
		if( EQUAL(argv[i],"--version") )
			{
				printf( "%s\n", GDALVersionInfo( "--version" ) );
			}

        else if( EQUAL(argv[i],"-quiet") )
			{
				bQuiet = TRUE;
				pfnProgress = GDALDummyProgress;
			}	

		else if( EQUAL(argv[i],"-c") )
			{
				if( atoi(argv[i+1]) < 2 )
					{
						printf( "There must be more columns than %s.\n", argv[i+1] );
						Usage();
						GDALDestroyDriverManager();
						exit( 2 );
					}

				cols = atoi(argv[++i]);
			}

		else if( EQUAL(argv[i],"-r") )
			{
				if( atoi(argv[i+1]) < 2 )
					{
						printf( "There must be more rows than %s.\n", argv[i+1] );
						Usage();
						GDALDestroyDriverManager();
						exit( 2 );
					}

				rows = atoi(argv[++i]);
			}

        else if( argv[i][0] == '-' )
			{
				printf( "Option %s incomplete, or not recognised.\n\n", 
						argv[i] );
				Usage();
				GDALDestroyDriverManager();
				exit( 2 );
			}
		
        else {
			SplitFile(argv[i], cols, rows, bQuiet, pfnProgress);
			j++;
		}
	}

	/* At least one file must be provided */
	if (j == 0) {
		Usage();
		GDALDestroyDriverManager();
		exit( 2 );
	}

/* -------------------------------------------------------------------- */
/*      Clean up.                                                       */
/* -------------------------------------------------------------------- */

	GDALDumpOpenDatasets( stderr );
    GDALDestroyDriverManager();

	exit(0);
}


/************************************************************************/
/*                           AttachMetadata()                           */
/************************************************************************/

static void AttachMetadata( GDALDatasetH hDS, char **papszMetadataOptions )

{
    int nCount = CSLCount(papszMetadataOptions);
    int i;

    for( i = 0; i < nCount; i++ )
		{
			char    *pszKey = NULL;
			const char *pszValue;
        
			pszValue = CPLParseNameValue( papszMetadataOptions[i], &pszKey );
			GDALSetMetadataItem(hDS,pszKey,pszValue,NULL);
			CPLFree( pszKey );
		}

    CSLDestroy( papszMetadataOptions );
}					


/************************************************************************/
/*                           CopyRasterBand()                           */
/*                                                                      */
/*      Add a copy of the specified source band to the destination      */
/*      dataset using only data from a pixel window.                    */
/************************************************************************/

static CPLErr CopyRasterBand(GDALDataset *poSrcDS, GDALDataset *poDstDS, int nBand, int nSrcXOff, int nSrcYOff, int nSrcXSize, int nSrcYSize)
{
	VRTSourcedRasterBand   *poVRTBand;
	GDALRasterBand  *poSrcBand;
	GDALDataType    eBandType;
	double          dfNoData;
	int             bSuccess;

	poSrcBand = poSrcDS->GetRasterBand( nBand );

/* -------------------------------------------------------------------- */
/*      Select output data type to match source.                        */
/* -------------------------------------------------------------------- */
	eBandType = poSrcBand->GetRasterDataType();

/* -------------------------------------------------------------------- */
/*      Create this band.                                               */
/* -------------------------------------------------------------------- */
	if (poDstDS->AddBand( eBandType, NULL ) == CE_Failure)
		return CE_Failure;
	
	poVRTBand = (VRTSourcedRasterBand *) poDstDS->GetRasterBand( nBand );           

/* -------------------------------------------------------------------- */
/*      Create a simple data source.                                    */
/* -------------------------------------------------------------------- */
	poVRTBand->AddSimpleSource( poSrcBand,
								nSrcXOff, nSrcYOff, 
								nSrcXSize, nSrcYSize, 
								0, 0,
								nSrcXSize, nSrcYSize );

/* -------------------------------------------------------------------- */
/*      copy over some other information of interest.                   */
/* -------------------------------------------------------------------- */
	poVRTBand->SetMetadata( poSrcBand->GetMetadata() );
	poVRTBand->SetColorTable( poSrcBand->GetColorTable() );
	poVRTBand->SetColorInterpretation(poSrcBand->GetColorInterpretation());
	if( strlen(poSrcBand->GetDescription()) > 0 )
		poVRTBand->SetDescription( poSrcBand->GetDescription() );
	dfNoData = poSrcBand->GetNoDataValue( &bSuccess );
	if ( bSuccess )
		poVRTBand->SetNoDataValue( dfNoData );

	return CE_None;
}


/************************************************************************/
/*                           CopyRasterWindow()                         */
/*                                                                      */
/*      Return a virtual dataset representing the specified pixel       */
/*      window of the source dataset.                                   */
/************************************************************************/

static VRTDataset * CopyRasterWindow(GDALDatasetH hSrcDS, int nSrcXOff, int nSrcYOff, int nSrcXSize, int nSrcYSize) 
{
	VRTDataset *poVDS = NULL;
    int nBandCount = 0, i;
    double adfGeoTransform[6];
    const char *pszProjection;
    char **papszMetadataOptions = NULL;
	
	poVDS = new VRTDataset( nSrcXSize, nSrcYSize );

	pszProjection = GDALGetProjectionRef( hSrcDS );
	if( pszProjection != NULL && strlen(pszProjection) > 0 )
		poVDS->SetProjection( pszProjection );
    
	if( GDALGetGeoTransform( hSrcDS, adfGeoTransform ) == CE_None)
		{
			adfGeoTransform[0] += nSrcXOff * adfGeoTransform[1]
				+ nSrcYOff * adfGeoTransform[2];
			adfGeoTransform[3] += nSrcXOff * adfGeoTransform[4]
				+ nSrcYOff * adfGeoTransform[5];
        
			poVDS->SetGeoTransform( adfGeoTransform );
		}

	else if( GDALGetGCPCount( hSrcDS ) > 0 )
		{
			GDAL_GCP *pasGCPs;
			int       nGCPs = GDALGetGCPCount( hSrcDS );

			pasGCPs = GDALDuplicateGCPs( nGCPs, GDALGetGCPs( hSrcDS ) );

			for( i = 0; i < nGCPs; i++ )
				{
					pasGCPs[i].dfGCPPixel -= nSrcXOff;
					pasGCPs[i].dfGCPLine  -= nSrcYOff;
				}
            
			poVDS->SetGCPs( nGCPs, pasGCPs,
							GDALGetGCPProjection( hSrcDS ) );

			GDALDeinitGCPs( nGCPs, pasGCPs );
			CPLFree( pasGCPs );
		}

	poVDS->SetMetadata( ((GDALDataset*)hSrcDS)->GetMetadata() );
	AttachMetadata( (GDALDatasetH) poVDS, papszMetadataOptions );

	nBandCount = GDALGetRasterCount( hSrcDS );
	for( i = 1; i <= nBandCount; i++ )
		{
			CopyRasterBand((GDALDataset *) hSrcDS, poVDS, i, nSrcXOff, nSrcYOff, nSrcXSize, nSrcYSize);
		}

	return poVDS;
}


/************************************************************************/
/*                           SplitFile()                                */
/*                                                                      */
/*      Split the specified file into the required number of tiles.     */
/************************************************************************/

static int SplitFile(const char * pszSource, int cols, int rows, int bQuiet, GDALProgressFunc pfnProgress)
{
    GDALDatasetH hDataset, hOutDS;
    VRTDataset *poVDS;
    int i,x,z;
    int nRasterXSize, nRasterYSize,
		nSrcXOff = 0, nSrcYOff = 0,
		nRoundedXSize, nRoundedYSize,
		nSrcXSize = 0, nSrcYSize = 0;
    GDALDriverH	hDriver;
	char * pszDest;
	
/* -------------------------------------------------------------------- */
/*      Attempt to open source file.                                    */
/* -------------------------------------------------------------------- */

    hDataset = GDALOpen( pszSource, GA_ReadOnly );
    
    if( hDataset == NULL )
		{
			fprintf( stderr,
					 "GDALOpen failed - %d\n%s\n",
					 CPLGetLastErrorNo(), CPLGetLastErrorMsg() );
			return ( 0 );
		}

    if( CSLCount(GDALGetMetadata( hDataset, "SUBDATASETS" )) > 0 )
		{
			fprintf( stderr,
					 "Input file contains subdatasets. Please, select one of them for reading.\n" );
			GDALClose( hDataset );
			return ( 0 );
		}

    nRasterXSize = GDALGetRasterXSize( hDataset );
    nRasterYSize = GDALGetRasterYSize( hDataset );

    if( cols > nRasterXSize )
		{
			fprintf( stderr,
					 "The column count cannot be greater than the image width in pixels.\n" );
			GDALClose( hDataset );
			return ( 0 );
		}

    if( rows > nRasterYSize )
		{
			fprintf( stderr,
					 "The row count cannot be greater than the image height in pixels.\n" );
			GDALClose( hDataset );
			return ( 0 );
		}
	
	if( !bQuiet )
		printf( "Splitting %s size %d, %d into %d files...\n"
				, pszSource, nRasterXSize, nRasterYSize, cols * rows);

/* -------------------------------------------------------------------- */
/*      Find the output driver.                                         */
/* -------------------------------------------------------------------- */

	hDriver = GDALGetDatasetDriver( hDataset );
	
/* -------------------------------------------------------------------- */
/*      Establish some parameters.                                      */
/* -------------------------------------------------------------------- */

	nRoundedXSize = (int) round(nRasterXSize / (double) cols);
	nRoundedYSize = (int) round(nRasterYSize / (double) rows);

/* -------------------------------------------------------------------- */
/*      Make a virtual clone.                                           */
/* -------------------------------------------------------------------- */

	for (i = 1, z = 1, nSrcXOff = 0; i <= cols; i++, nSrcXOff += nSrcXSize) {
		nSrcXSize = (i != cols) ? nRoundedXSize : nRasterXSize - nRoundedXSize * (cols - 1);

		for (x = 1, nSrcYOff = 0; x <= rows; x++, z++, nSrcYOff += nSrcYSize) {
			pszDest = CreateFileName(pszSource, i, x);

			if (!pszDest)
				return FALSE;

			nSrcYSize = (x != rows) ? nRoundedYSize : nRasterYSize - nRoundedYSize * (rows - 1);

			if (!bQuiet)
				printf("%d. %s size is %d, %d\n  ", z, pszDest, nSrcXSize, nSrcYSize);

			poVDS = CopyRasterWindow(hDataset, nSrcXOff, nSrcYOff, nSrcXSize, nSrcYSize);

/* -------------------------------------------------------------------- */
/*      Write to the output file using CopyCreate().                    */
/* -------------------------------------------------------------------- */
			hOutDS = GDALCreateCopy( hDriver, pszDest, (GDALDatasetH) poVDS,
									 TRUE, NULL, 
									 pfnProgress, NULL );
			if ( hOutDS != NULL )
				{
					GDALClose( hOutDS );
				}

			GDALClose( (GDALDatasetH) poVDS );

			free(pszDest);
		}
	}
	
    GDALClose( hDataset );
    
    return ( hOutDS != NULL );
}


/************************************************************************/
/*                           CreateString()                             */
/*                                                                      */
/*      Takes printf() arguments and formats output to appropriately    */
/*      allocated string.                                               */
/*      The code is adapted from that found in the glibc vsnprintf      */
/*      man page.                                                       */
/*      It is up to the caller to free the result.                      */
/************************************************************************/

static char * CreateString(const char *fmt, ...) {
	/* Guess we need no more than 100 bytes. */
	int n, size = 100;
	char *p;
	va_list ap;
	if ((p = (char *) malloc (size)) == NULL)
		return NULL;
	while (1) {
		/* Try to print in the allocated space. */
		va_start(ap, fmt);
		n = vsnprintf (p, size, fmt, ap);
		va_end(ap);
		/* If that worked, return the string. */
		if (n > -1 && n < size)
			return p;
		/* Else try again with more space. */
		if (n > -1)    /* glibc 2.1 */
			size = n+1; /* precisely what is needed */
		else           /* glibc 2.0 */
			size *= 2;  /* twice the old size */
		if ((p = (char *) realloc (p, size)) == NULL)
			return NULL;
	}
}


/************************************************************************/
/*                           CreateFileName()                           */
/*                                                                      */
/*      Create filename based on source filename but specifying         */
/*		required column and row.                                        */
/*      It is up to the caller to free the result.                      */
/************************************************************************/

static char * CreateFileName(const char *pszSource, int col, int row) 
{
	char *basename = GetBasename(pszSource);
	char *dirname = GetDirname(pszSource);
	char *ext = GetExtension(basename);
	char *filename;

	if (ext) {
		basename[(strlen(basename) - strlen(ext))-1] = '\0';
		filename = CreateString("%s%c%s_c%d_r%d.%s", dirname, PATH_SEP_CHAR, basename, col, row, ext);
	} else {
		filename = CreateString("%s%c%s_c%d_r%d", dirname, PATH_SEP_CHAR, basename, col, row);
	}

	free(dirname);
	free(basename);
	free(ext);

	return filename;
}


/************************************************************************/
/*                           GetExtension()                             */
/*                                                                      */
/*      Return the extension portion of a filename or NULL if none      */
/*		is present.                                                     */
/*      It is up to the caller to free the result.                      */
/************************************************************************/

static char * GetExtension(const char *basename) 
{
	char *ext;
	char *p = strrchr(basename, '.');
	if (!p)
		return NULL;

	if (p < strrchr(basename, PATH_SEP_CHAR))
		return NULL;

	if ((ext = (char *) malloc (strlen(p))) == NULL) /* no need to do strlen(p + 1) because we are going to disregard the '.' */
		return NULL;

	strcpy(ext, ++p);

	return ext;
}


/************************************************************************/
/*                           GetDirname()                               */
/*                                                                      */
/*      Return the directory portion of a filename or '.' if none       */
/*		is present.                                                     */
/*      It is up to the caller to free the result.                      */
/*      Code adapted from bash dirname command.                         */
/************************************************************************/

static char * GetDirname(const char *string)
{
	int slen;
	char * psTmp;
	char *dirname;

	slen = strlen (string);

	/* Strip trailing slashes */
	while (slen > 0 && string[slen - 1] == PATH_SEP_CHAR)
		slen--;

	/* (2) If string consists entirely of slash characters, string shall be
	   set to a single slash character.  In this case, skip steps (3)
	   through (8). */
	if (slen == 0)
		return CreateString("%c", PATH_SEP_CHAR);

	if ((psTmp = (char *) malloc (slen+1)) == NULL)
		return NULL;

	strcpy(psTmp, string);
	
	/* (3) If there are any trailing slash characters in string, they
	   shall be removed. */
	psTmp[slen] = '\0';

	/* (4) If there are no slash characters remaining in string, string
	   shall be set to a single period character.  In this case, skip
	   steps (5) through (8).

	   (5) If there are any trailing nonslash characters in string,
	   they shall be removed. */

	while (--slen >= 0)
		if (psTmp[slen] == PATH_SEP_CHAR)
			break;

	if (slen < 0)
		{
			free(psTmp);
			return CreateString(".");
		}

	/* (7) If there are any trailing slash characters in string, they
	   shall be removed. */
	while (--slen >= 0)
		if (psTmp[slen] != PATH_SEP_CHAR)
			break;
	psTmp[++slen] = '\0';

	/* (8) If the remaining string is empty, string shall be set to a single
	   slash character. */
	if (slen == 0)
		dirname = CreateString("%c", PATH_SEP_CHAR);
	else
		dirname = CreateString("%s", psTmp);
	free(psTmp);

	return dirname;
}


/************************************************************************/
/*                           GetBasename()                              */
/*                                                                      */
/*      Return the basename of a filename, including extension.         */
/*      It is up to the caller to free the result.                      */
/*      Code adapted from bash basename command.                        */
/************************************************************************/

static char * GetBasename(const char *string)
{
	int slen;
	char *psTmp, *psTmp2, *psTmp3;
	
	slen = strlen (string);

	/* Strip trailing slashes */
	while (slen > 0 && string[slen - 1] == PATH_SEP_CHAR)
		slen--;

	/* (2) If string consists entirely of slash characters, string shall be
	   set to a single slash character.  In this case, skip steps (3)
	   through (5). */
	if (slen == 0)
		return CreateString("%c", PATH_SEP_CHAR);
	
	if ((psTmp = (char *) malloc (slen+1)) == NULL)
		return NULL;

	strcpy(psTmp, string);
	
	/* (3) If there are any trailing slash characters in string, they
	   shall be removed. */
	psTmp[slen] = '\0';

	/* (4) If there are any slash characters remaining in string, the prefix
	   of string up to an including the last slash character in string
	   shall be removed. */
	while (--slen >= 0)
		if (psTmp[slen] == PATH_SEP_CHAR)
			break;

	if (slen == 0)
		return psTmp;

	for (psTmp2 = &psTmp[slen+1], psTmp3 = psTmp; *psTmp2 != '\0'; psTmp2++, psTmp3++)
		*psTmp3 = *psTmp2;

	*(psTmp3++) = '\0';

	return psTmp;
}

