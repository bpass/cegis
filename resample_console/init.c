#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "imgio.h"
#include "init.h"
#include "getgeoinfo.h"
#include "getprojinfo.h"

/*---------------------------------*\
\* init.c                          */
/*---------------------------------*\
\* Various initialization routines */
/* Replaces imgio.c from mapimg    *\
\*---------------------------------*/

//---------- External Variables ----------//

extern int dodump;

extern char infile_name[];	extern char outfile_name[];
extern char inimgFile[];
extern int class2data[][2];	extern int num_classes;
extern int preferred;		extern int unpreferred;
extern int domax;		extern int domin;
extern int dobands;		extern int dochoice;
extern int donn;		extern int doout;
extern int dopreferred;		extern int dounpreferred;

extern FILE *inptr;		extern FILE *outptr;
extern FILE *maxfile;		extern FILE *minfile;
extern FILE *choicefile;	extern FILE *nnfile;
extern FILE *classptr[];	extern unsigned char *classbuf[];
extern unsigned char *maxbuf;	extern unsigned char *minbuf;
extern unsigned char *choicebuf;	extern unsigned char *nnbuf;

long insize;
long outsize;

  /*----------------------------------------------------*/
 /* Parse the input arguments and initalize processing */
/*----------------------------------------------------*/
void parse_input(int *argc, char *argv[], float *fill) 
{
int i;
char ch[200];
char histofile_name[200];
float tempf;
int tempi;
FILE *histfile;
if (*argc < 6) 
   { 
   printf("Syntax: resample inimg outimg histofile fillval {MmNbCoPpD}\n"); 
   printf("options:\nM\tcalculate maximum class per pixel\n");
   printf("m\tcalculate minimum class per pixel\n");
   printf("N\twrite nearest neighbor output image\n");
   printf("b\tcalculate pixel use of each band\n");
   printf("C\tcalculate how many bands are in each pixel\n");
   printf("o\twrite the output image\n");
   printf("P\tmost preferred class for output image\n");
   printf("p\tleast preferred class for output image\n");
   printf("\n");
   exit(-1); 
   }
//----- copy params to appropriate variables -----//
sprintf(infile_name,"%s",argv[1]);  /*infilename*/
printf("Input file:\t%s.img\n",infile_name);
sprintf(outfile_name,"%s",argv[2]);/*outfile name*/
printf("Output file:\t%s.img\n",outfile_name);
sprintf(histofile_name,"%s%s",argv[3], ".histo"); //histo file
printf("Histo file:\t%s\n",histofile_name);
sscanf(argv[4],"%f",fill);

//----- flag options -----//
strcpy(ch, argv[5]);
i = 0;
while(ch[i] != '\0')
{
 switch(ch[i])
 {
 case '-':	break;
 case 'N':	donn = 1;     printf("Creating %s.nn.img\n",    outfile_name);	break;
 case 'M':	domax = 1;    printf("Creating %s.max.img\n",   outfile_name);	break;
 case 'm':	domin = 1;    printf("Creating %s.min.img\n",   outfile_name);	break;
 case 'b':	dobands = 1;  printf("Creating band images\n");			break;
 case 'C':	dochoice = 1; printf("Creating %s.choice.img\n",outfile_name);	break;
 case 'o':	doout = 1;    printf("Creating %s.img\n", outfile_name);break;
 case 'P':	dopreferred = 1;break;
 case 'p':	dounpreferred = 1;break;
 case 'D':	dodump = 1;break;
 default:	printf("Error: \'%c\' - invalid parameter\n\n", ch[i]); 
		exit(-1); break;
  }
 i++; 
}//while

//----- Check histogram file to find valid class numbers -----//
num_classes = 1;
histfile = fopen(histofile_name, "r");
if(!histfile){printf("Error: Could not open file %s\n\n", 
histofile_name);exit(-1);}
fscanf(histfile, "%d %d %f%%", &tempi, &class2data[0], &tempf);
for(i = 1; i < 256; ++i)
{
 fscanf(histfile, "%d %d %f%%", &tempi, &class2data[i], &tempf);
 if(class2data[i][0])
 {
	class2data[i][0] = num_classes;
	class2data[num_classes][1] = i;
	num_classes++;
 }
 else
	class2data[i][0] = 0;
}
fclose(histfile);

//----- Special case: non-zero fill value -----//
if(*fill != 0)
{
	class2data[0][0] = num_classes;
	class2data[num_classes][1] = (int)*fill;
	num_classes++;
}


//----- find preferred & unpreferred classes -----//
if(dopreferred)
{
 if(*argc < 7)
	{printf("Most preferred class must be named.\n\n");exit(-1);}
 sscanf(argv[6],"%d",&preferred);
 printf("Class %d is most preferred.\n",preferred);
 if(dounpreferred)
 {
  if(*argc < 8)
	{printf("Least preferred class must be named.\n\n");exit(-1);}
  sscanf(argv[7],"%d",&unpreferred);
  printf("Class %d is least preferred.\n",unpreferred);
 }
}
else
 if(dounpreferred)
 {
  if(*argc < 7)
	{printf("Least preferred class must be named.\n\n");exit(-1);}
  if(*argc == 8)		// least & most preferred classes are given, but only p option
	sscanf(argv[7], "%d",&unpreferred);
  else
	sscanf(argv[6],"%d",&unpreferred);
  printf("Class %d is least preferred.\n",unpreferred);
 } 
 
 return;
}// parse_input


// Read the entire input image
// ---------------------------
void get_image(unsigned char *buf)
{ 
char filename[255];
sprintf(filename, "%s.img",infile_name);
inptr = fopen(filename,"r");	// Open input file
if(!inptr)
{
	printf("ERROR opening input file!\n");
	exit(-1);
}
fread(buf,1,insize,inptr); 
fclose(inptr);
return;
}

// set all values in buffer to zero
// --------------------------------
void clear_buffer(unsigned char* buf, long bufsize)
{
	long i;
	for(i = 0; i < bufsize; ++i)
	{
		buf[i] = 0;
	}
	
	return;
}

   /*---------------------------------------------------------------*/
  /* init_input   --  edited 6/2001 to reflect changes in .geoinfo */
 /* and .proj file formats                                        */
/*---------------------------------------------------------------*/
unsigned char *init_input( struct IMGINFO* inimg)
{
unsigned char *bufptr;			// Ptr to input buffer
long i;					// Loop counter 

// Open info file and get image geometry information
// -------------------------------------------------
// get image geometry information
// ------------------------------
get_geoInfo(infile_name,&inimg->nl,&inimg->ns,&i,
	&inimg->ul_x,&inimg->ul_y,&inimg->pixsize);

get_projInfo(infile_name,&inimg->sys,&inimg->zone,&inimg->unit,&inimg->datum,
	inimg->pparm);

insize = inimg->nl * inimg->ns;
bufptr = (unsigned char *) malloc(insize);
if (!bufptr) 
   {printf("Error allocating input image buffer!  Image too large!\n");exit(-1);}
return(bufptr);
}

   /*-----------------------------------------------------------------*/
  /* init_output   --  edited to reflect changes in .geoinfo & .proj */
 /* file formats, 6/2001                                            */
/*-----------------------------------------------------------------*/
unsigned char* init_output( struct IMGINFO* outimg)
{
long i;                                 /* Loop counter */

// get image geometry information
// ------------------------------
get_geoInfo(outfile_name,&outimg->nl,&outimg->ns,&i,
	&outimg->ul_x,&outimg->ul_y,&outimg->pixsize);

get_projInfo(outfile_name,&outimg->sys,&outimg->zone,&outimg->unit,&outimg->datum,
	outimg->pparm);

outsize = outimg->ns;
return((unsigned char *) malloc(outsize));
}

  /*--------------------------------*/
 /* Open the Class files & buffers */
/*--------------------------------*/
void init_class(int bufsize)
{
int i;
char filenames[256];

for(i = 0; i < num_classes; ++i)
{
	sprintf(filenames, "%s%c%03d%s", outfile_name, '.', class2data[i][1], ".img");
	classptr[i] = fopen(filenames, "w");
	if(!classptr[i])
	 {printf("Error creating file %s\n\n", filenames);exit(-1);}
}

for(i = 0; i < num_classes; ++i)
{
	classbuf[i] = (unsigned char *)malloc(bufsize);
	if(!classbuf[i])
	  {printf("Error creating class buffer %d\n\n", i);exit(-1);}
}
}// init_class

  /*-----------------------------------------------------*/
 /* initialize buffers & files for statistical analysis */
/*-----------------------------------------------------*/
void init_stats(int bufsize)
{
int i;
char filename[255];

if(doout)
{
sprintf(filename, "%s%s", outfile_name, ".img");
outptr = fopen(filename, "w");
if(!outptr){printf("Error opening the output file\n\n");exit(-1);}
}

if(domax)
{
maxbuf = (unsigned char*)malloc(bufsize);
if(!maxbuf) {printf("ERROR: Could not create buffer\n\n");exit(-1);}
sprintf(filename, "%s%s", outfile_name, ".max.img");
maxfile = fopen(filename, "w");
if(!maxfile){printf("Error opening maximum output file\n\n");exit(-1);}
}

if(domin)
{
minbuf = (unsigned char*)malloc(bufsize);
if(!minbuf) {printf("ERROR: Could not create buffer\n\n");exit(-1);}
sprintf(filename, "%s%s", outfile_name, ".min.img");
minfile = fopen(filename, "w");
if(!minfile){printf("Error opening minimum output file\n\n");exit(-1);}
}

choicebuf = (unsigned char*)malloc(bufsize);
if(!choicebuf) {printf("ERROR: Could not create buffer\n\n");exit(-1);}
if(dochoice)
{
sprintf(filename, "%s%s", outfile_name, ".choice.img");
choicefile = fopen(filename, "w");
if(!choicefile){printf("Error opening choices file\n\n");exit(-1);}
}

nnbuf = (unsigned char*)malloc(bufsize);
if(!nnbuf){printf("ERROR: Could not create buffer\n\n");exit(-1);}
if(donn)
{
sprintf(filename, "%s%s", outfile_name, ".nn.img");
nnfile = fopen(filename, "w");
if(!nnfile){printf("Error opening file %s\n\n", filename);exit(-1);}
}

if(dobands)
	init_class(bufsize);

}// init_stats



void get_image_line( FILE* file, unsigned int lineNumber, unsigned int lineSize, unsigned int *buffer )
{ 
	if( file && buffer )
	{
		if( ftell( file ) != (long)(lineNumber)*lineSize )
		{
			if( fseek( file, SEEK_SET, lineNumber * lineSize ) != 0 )
			{
				fprintf( stderr, "Error getting line: Could not seek to location %i in file.\n", lineNumber*lineSize );
				fflush( stdout );
			}
		}
		
		if( ftell( file ) == (long)(lineNumber)*lineSize )
		{
			if( fread( buffer, sizeof( unsigned int ), lineSize, file ) != lineSize )
			{
				fprintf( stderr, "Error getting line: could not read %i elements from file starting at %i.\n", lineSize, lineNumber*lineSize );
				fflush( stdout );
			}
		}
	}
	else
	{
		fprintf( stderr, "Error getting line: Not a valid file pointer or buffer.\n" );
		fflush( stdout );
	}
	
	return;	
}




