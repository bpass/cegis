// RESAMPLE does statistical analysis on the input spaces represented by one
// pixel in the output image. It is loosely based on MAPIMG.
// EMN June - August, 2001
// ---------------------------------------------------------------------------
// MAPIMG remaps imagery from one projection to another using nearest neighbor
// resampling.  Each coordinate is evaluated thru the GCTP; there is no grid
// based interpolation here...  DRS/Science/EROS 1/95
// ---------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>



#include "imgio.h"
#include "init.h"




#define DELTA_LS 0.00005
#define DELTA_METERS outimg.pixsize/2

//----- Global Variables -- performance is crucial here -----//
char infile_name[200];		/* Name of input file */
char outfile_name[200];		/* Name of output file */
char inimgFile[200];		// input image filename
struct IMGINFO inimg;		/* Image information--input */
struct IMGINFO outimg;		/* Image information--output */
double in[2];			/* Input projection coordinates of a point */
double out[2];			/* Output projection coordinates of a point */
double corner[2];		// for computing values at pixel corners
double coord[2];		// random coordinates
double in_line, in_samp;	/* Input image coordinates of a point */
long status;			/* Return status flag for gctp() call */
long zero=0;			/* Constant of 0 */
long num_classes=255;		// number of classes used...
int preferred;		// most preferred class
int unpreferred;	// least preferred class

//---------- file pointers & buffers ----------//
FILE *inptr;			/* Input file pointer */
FILE *outptr;			/* Output file pointer */
FILE *classptr[256];	// array of ptrs to class files
FILE *maxfile, *minfile, *choicefile;	// random statistics
FILE *nnfile;			// nearest neighbor resample
unsigned int *inbuf;		/* Ptr to the input image (all in memory) */
unsigned int *outbuf;		/* Ptr to one line of output image data */
unsigned int *classbuf[256];	// array of ptrs to class buffers
unsigned int *maxbuf;		// buffer to store which class was used most in pixel
unsigned int *minbuf;		// stores which class was used least in pixel
unsigned int *nnbuf;		// stores NN output
unsigned int *choicebuf;	// stores how many classes were in pixel


//---------- flags ----------//
int dodump=0;		// whether or not to dump minbox to screen
int domax=0;		// image of most common value per pixel
int domin=0;		// image of least common value per pixel
int donn=0;		// create traditional mapimg image
int dobands=0;		// images of how many hits per pixel per band
int dochoice=0;		// image of how many classes to choose from per pixel
int doout=0;		// write "the" output file
int dopreferred=0;	// most preferred class
int dounpreferred=0;	// least preferred class
int boxerr=0;		// true if no minbox pixels are in polygon
int find2corners=0;	// true if previous pixel's corners can be reused
int classcount[256][2]={0};// flags which classes are used per output pixel
	// classcount[x][0] is true if class x is used
	// classcount[][1] is a list of the classes used in current pixel
int class2data[256][2]={0};// flags which classes are used in input image
	// class2data[x][0] converts data value x to its internal class number
	// class2data[x][1] converts internal class number x to its data value

//---------- procedure prototypes ----------//
unsigned char *init_input(), *init_output();
void init_stats(int);
void init_class(int);
int get_coords(double[][2], long, long);
unsigned char find_output(long);


//////////////////////////
//                      //
//     THIS IS MAIN     //
//                      //
//////////////////////////
main(argc, argv) int argc; char *argv[]; 
{
float fill;			/* Fill value */
double inbox[5][2];		// stores coords of input pixel's corners & center
				// in the order of ul, ur, lr, ll, center
double minx, maxx, miny, maxy;
long out_line, out_samp;	/* Output image coordinates of a point */
int curx, cury, i;		// loop counters
unsigned char temp;		// temporaray var
unsigned char t2;

printf("\n");
printf("Resample version 5.04 9/4/02\n\n");

  /*-------------------------*/
 /* Stretching exercises... */
/*-------------------------*/
//----- read parameters -----//
parse_input(&argc,argv,&fill);

//----- Initialize buffers & files -----//
inbuf  = init_input(&inimg);
outbuf = init_output(&outimg);
init_stats(outimg.ns);

//----- Read input image into memory -----//
printf("Openning input image...\n");
char temp_filename[255];
sprintf(temp_filename, "%s.img",infile_name);

inptr = fopen( temp_filename. "rb" ); 
if(!inptr)
{
	printf("ERROR opening input file!\n");
	exit(-1);
}


printf("Generating output...\n");
  /*-------------*/
 /* The workout */
/*-------------*/
for(out_line = 0; out_line < outimg.nl; out_line++) 
{
 printf("%cLine %6d of %6d", 13, out_line+1, outimg.nl);
 out[1] = outimg.ul_y - (out_line * outimg.pixsize); // set y coordinate for whole line

//----- clear buffers -----//
 if(domax)
	clear_buffer(maxbuf, outimg.ns);
 if(domin)
	clear_buffer(minbuf, outimg.ns);
clear_buffer(choicebuf, outimg.ns);
clear_buffer(nnbuf, outimg.ns);
 if(dobands)
 {
	for(i = 0; i < num_classes; ++i)
	  clear_buffer(classbuf[i], outimg.ns);
 }
find2corners = 0;	// must get all 4 corners at beg. of line

 for(out_samp = 0; out_samp < outimg.ns; out_samp++) 
 {
  out[0] = outimg.ul_x + (out_samp * outimg.pixsize); //set pixel's x coordinate

//----- get pixel center & corner coordinates -----//
  temp = get_coords(inbox, out_line, out_samp);
  
  // ******************** Coordinate Dump *********************
  //printf("%d %d %f %f %f %f %f %f %f %f %f %f\n", out_line, out_samp,
  //	inbox[4][1],inbox[4][0],
  //	inbox[0][1],inbox[0][0], inbox[1][1],inbox[1][0],
  //	inbox[2][1],inbox[2][0], inbox[3][1],inbox[3][0]);
  // ******************** End Coord Dump **********************
  
  if(!temp)
  {
     find2corners = 0;
     nnbuf[out_samp] = fill;
     if(doout)
       outbuf[out_samp] = fill;
  }
  else
  {
	   find2corners = 1;
	   nnbuf[out_samp] = inbuf[(long)(inbox[4][1]*inimg.ns+inbox[4][0])];
//----- clear the classcount array -----//
	   for(i = 0; i < num_classes; ++i)
	   {
		classcount[i][0] = 0;
		classcount[i][1] = 0;
	   }

//----- compute minbox -----//
	   maxx = maxy = 0; minx = inimg.ns; miny = inimg.ns;
	   for(i = 0; i < 4; ++i)
	   {
	    if(maxx < inbox[i][0])
		maxx = inbox[i][0];
	    else
	    {
	     if(minx > inbox[i][0])
		minx = inbox[i][0];
	    }
	    if(maxy < inbox[i][1])
		maxy = inbox[i][1];
	    else
	    {
	     if(miny > inbox[i][1])
		miny = inbox[i][1];
	    }
	   } //for i

//----- check each pixel in minbox & compile stats -----//
	   boxerr = 1;
	   for(cury = (long)(miny+0.5); cury <= (long)(maxy+0.5); ++cury)
	   {
	    coord[1] = cury;
	    for(curx = (long)(minx+0.5); curx <= (long)(maxx+0.5); ++curx)
	    {
	     coord[0] = curx; 
	     if(inBox(inbox, coord))
	     {
	      boxerr = 0;
	      temp = inbuf[(long)(coord[1]*inimg.ns + coord[0])];
	      if(classcount[class2data[temp][0]][0]<255)
		classcount[class2data[temp][0]][0]++;
	
	       if(dobands)
	         classbuf[class2data[temp][0]][out_samp]++;  
       
	      }//if inBox
	    }// for curx
	   }//for cury

//----- finish statistical analysis -----//
	  if(boxerr)	//no pixels from rectangle in the minbox, get NN.
	  {
		choicebuf[out_samp]=255;
		if(doout)
		  outbuf[out_samp] = nnbuf[out_samp];
		if(domax)
		  maxbuf[out_samp]=nnbuf[out_samp];
		if(domin)
		  minbuf[out_samp]=nnbuf[out_samp];
	  }
	  else	//!boxerr
	  { 
	   temp = t2 = 0;
	   for(i = 0; i < num_classes; ++i)
	     if(classcount[i][0])
	     {
		classcount[t2][1] = i;	// make list of used classes
		t2++;			// increment current list subscript
		temp++;			// increment number of choices
	     }
 	  choicebuf[out_samp] = temp;

 	  if(domax)
 	  {
 	   temp = t2 = 0;
 	   for(i = 0; i < num_classes; ++i)
 	    if(classcount[i][0] > temp)
 	      {temp = classcount[i][0]; t2 = i;}
 	   maxbuf[out_samp] = class2data[t2][1];
 	  }//if doMax

	   if(domin)
	   {
	    temp = 255;
	    for(i = 0; i < num_classes; ++i)
	     if(classcount[i][0] > 0 && classcount[i][0] < temp)
		{temp = classcount[i][0];t2 = i;}
	    minbuf[out_samp] = class2data[t2][1];
	  }//domin

	  if(doout)// this is where we find "the" output pixel
		outbuf[out_samp] = find_output(out_samp);
	
	  }	//!boxerr
  }// else get_coords == true
 }// for out_samp

//----- end of line, write to files -----//
if(doout)
{
 if(fwrite(outbuf, 1, outimg.ns, outptr)!= outimg.ns)
	{printf("Error writing to output file\n\n");exit(-1);}
}
if(donn)
{
 if(fwrite(nnbuf, 1, outimg.ns, nnfile)!= outimg.ns)
	{printf("Error writing to NN output file\n\n");exit(-1);}
}
if(domax)
{
 if(fwrite(maxbuf, 1, outimg.ns, maxfile)!= outimg.ns)
	{printf("Error writing to maxfile\n\n");exit(-1);}
}
if(domin)
{
 if(fwrite(minbuf, 1, outimg.ns, minfile)!= outimg.ns)
	{printf("Error writing to minfile\n\n");exit(-1);}
}
if(dochoice)
{
 if(fwrite(choicebuf, 1, outimg.ns, choicefile)!= outimg.ns)
	{printf("Error writing to choicefile\n\n");exit(-1);}
}
if(dobands)
{
 for(i = 0; i < num_classes; ++i)
  if(fwrite(classbuf[i], 1, outimg.ns, classptr[i]) != outimg.ns)
	{printf("Error writing to band file\n\n");exit(-1);} 
}

}// out_line

printf("\nRESAMPLE complete.\n\n"); 	/* The END */
}

/*--------------------------------------------------------*\
\* get_coords                                             */
/*--------------------------------------------------------*\
\* use output pixel coordinate to find the coordinates of */
/* the corners of the corresponding input pixel           *\
\*--------------------------------------------------------*/
int get_coords(double inbox[5][2], long out_line, long out_samp)
{
long sys=0;
long zone=62;
long unit=4;
long datum=19;
double pparm[15]={0};
double coordt1[2];
double coordt2[2];
double temp1;
double temp2;
int dumpflag=0;

  /*-------------------------*/
 /* get "center" coordinate */
/*-------------------------*/
// convert input to geographic coordinates, check for wrapping,
// convert geographic coords to output.

coordt1[1] = coordt2[1] = out[1];
coordt1[0] = coordt2[0] = out[0];

if( outimg.sys == 24 )
// Goode's already masks fill areas, Equirectangular freaks out...
{
// This is the "normal" mapimg gctp call, output => input
 gctp(out,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
      &zero,"",&zero,"",in,&inimg.sys,&inimg.zone,inimg.pparm,&inimg.unit,
      &inimg.datum,"","",&status); 
 if(status != OK)
 {
  if(dodump)
	dumpflag = 1;
  else
	return 0;
 }
}//if Goode's
else
{
//------ output => lat/long ------//
gctp(out,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
     &zero,"",&zero,"",coordt1,&sys,&zone,pparm,&unit,
     &datum,"","",&status); 
//----- lat/long => output -----//
gctp(coordt1,&sys,&zone,pparm,&unit,&datum,&zero,"",&zero,"",coordt2,&outimg.sys,
     &outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,"","",&status);
//----- compare values... if different we're in a fill area -----//
temp1=fabs(coordt2[0] - out[0]);
temp2=fabs(coordt2[1] - out[1]);
if(temp1 > DELTA_METERS || temp2 > DELTA_METERS)
	return 0;
//----- lat/long => input -----//
gctp(coordt1,&sys,&zone,pparm,&unit,&datum,&zero,"",&zero,"",in,&inimg.sys,
&inimg.zone,inimg.pparm,&inimg.unit,&inimg.datum,"","",&status); 
if(status != OK)
 {
  if(dodump)
	dumpflag = 1;
  else
	return 0;
 }
} //else not goode's

in_line = (inimg.ul_y - in[1]) / inimg.pixsize+0.5;
in_samp = (in[0] - inimg.ul_x) / inimg.pixsize+0.5;

if(in_line < 0 && in_line < -DELTA_LS)
	in_line = 0;
if(in_samp < 0 && in_samp < -DELTA_LS)
	in_samp = 0;
//----- Are we in bounds? -----//
if( (long)in_line <0 || (long)in_samp <0 ||
    (long)in_line >= inimg.nl || (long)in_samp >= inimg.ns )
	return 0;

inbox[4][1] = (long)(in_line);
inbox[4][0] = (long)(in_samp);

//----- last pixel's right side is this pixel's left side -----//
if(find2corners)
{
	inbox[0][0] = inbox[1][0];	// old ur = new ul
	inbox[0][1] = inbox[1][1];
	inbox[3][0] = inbox[2][0];
	inbox[3][1] = inbox[2][1];	// old lr = new ll
}
else	// find ul & ll
{
  /*------------------------*/
 /* find upper left corner */
/*------------------------*/
 corner[0] = out[0] - outimg.pixsize/2;
 corner[1] = out[1] + outimg.pixsize/2;
if(outimg.sys == 24)
{
gctp(corner,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
    &zero,"",&zero,"",in,&inimg.sys,&inimg.zone,inimg.pparm,&inimg.unit,
    &inimg.datum,"","",&status); 
if(status != OK)
 {
  if(dodump)
	dumpflag = 1;
  else
	return 0;
 }
}
else
{
//------ output => lat/long ------//
gctp(corner,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
     &zero,"",&zero,"",coordt1,&sys,&zone,pparm,&unit,
     &datum,"","",&status); 
//----- lat/long => output -----//
gctp(coordt1,&sys,&zone,pparm,&unit,&datum,&zero,"",&zero,"",coordt2,&outimg.sys,
     &outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,"","",&status);
//----- compare values... if different we're in a fill area -----//
temp1=fabs(coordt2[0] - corner[0]);
temp2=fabs(coordt2[1] - corner[1]);
if(temp1 > DELTA_METERS || temp2 > DELTA_METERS)
	return 0;
//----- lat/long => input -----//
gctp(coordt1,&sys,&zone,pparm,&unit,&datum,&zero,"",&zero,"",in,&inimg.sys,
&inimg.zone,inimg.pparm,&inimg.unit,&inimg.datum,"","",&status); 
 if(status != OK)
 {
  if(dodump)
	dumpflag = 1;
  else
 	return 0;
 }
}	// not goode's
in_line = (inimg.ul_y - in[1]) / inimg.pixsize;
in_samp = (in[0] - inimg.ul_x) / inimg.pixsize;

//----- adjust for tolerance around zero -----//
if(in_line < 0 && in_line < -DELTA_LS)
	in_line = 0;
if(in_samp < 0 && in_samp < -DELTA_LS)
	in_samp = 0;
//----- bounds checking -----//
if((long)in_line+DELTA_LS < 0 || (long)in_samp+DELTA_LS < 0 ||
  (long)in_line > inimg.nl   || (long)in_samp > inimg.ns)
	return 0;
//----- save coordinates -----//
 inbox[0][1] = in_line;
 inbox[0][0] = in_samp;

  /*------------------------*/
 /* find lower left corner */
/*------------------------*/
 corner[0] = out[0] - outimg.pixsize/2;
 corner[1] = out[1] - outimg.pixsize/2;
if(outimg.sys == 24)
{
gctp(corner,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
    &zero,"",&zero,"",in,&inimg.sys,&inimg.zone,inimg.pparm,&inimg.unit,
    &inimg.datum,"","",&status); 
if(status != OK)
 {
  if(dodump)
	dumpflag = 1;
  else
	return 0;
 }
}	else	{
//------ output => lat/long ------//
gctp(corner,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
     &zero,"",&zero,"",coordt1,&sys,&zone,pparm,&unit,
     &datum,"","",&status); 
//----- lat/long => output -----//
gctp(coordt1,&sys,&zone,pparm,&unit,&datum,&zero,"",&zero,"",coordt2,&outimg.sys,
     &outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,"","",&status);
//----- compare values... if different we're in a fill area -----//
temp1=fabs(coordt2[0] - corner[0]);
temp2=fabs(coordt2[1] - corner[1]);
if(temp1 > DELTA_METERS || temp2 > DELTA_METERS)
	return 0;
//----- lat/long => input -----//
gctp(coordt1,&sys,&zone,pparm,&unit,&datum,&zero,"",&zero,"",in,&inimg.sys,
&inimg.zone,inimg.pparm,&inimg.unit,&inimg.datum,"","",&status); 
 if(status != OK)
 {
  if(dodump)
	dumpflag = 1;
  else
	return 0;
 }
}
in_line = (inimg.ul_y - in[1]) / inimg.pixsize;
in_samp = (in[0] - inimg.ul_x) / inimg.pixsize;

//----- adjust for tolerance around zero -----//
if(in_line < 0 && in_line < -DELTA_LS)
	in_line = 0;
if(in_samp < 0 && in_samp < -DELTA_LS)
	in_samp = 0;
//----- bounds checking -----//
if((long)in_line+DELTA_LS < 0 || (long)in_samp+DELTA_LS < 0 ||
   (long)in_line > inimg.nl   || (long)in_samp > inimg.ns)
	return 0;
//----- save coordinates -----//
 inbox[3][1] = in_line;
 inbox[3][0] = in_samp;
}//else(need to get ul&ll corners)


  /*-------------------------*/
 /* find upper right corner */
/*-------------------------*/
corner[0] = out[0] + outimg.pixsize/2;
corner[1] = out[1] + outimg.pixsize/2;
gctp(corner,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
    &zero,"",&zero,"",in,&inimg.sys,&inimg.zone,inimg.pparm,&inimg.unit,
    &inimg.datum,"","",&status); 
if(status != OK)
 {
  if(dodump)
	dumpflag = 1;
  else
	return 0;
 }
in_line = (inimg.ul_y - in[1]) / inimg.pixsize;
in_samp = (in[0] - inimg.ul_x) / inimg.pixsize;
//----- adjust for tolerance around zero -----//
if(in_line < 0 && in_line < -DELTA_LS)
	in_line = 0;
if(in_samp < 0 && in_samp < -DELTA_LS)
	in_samp = 0;
//----- bounds checking -----//
if((long)in_line < 0 || (long)in_samp < 0 ||
   (long)in_line > inimg.nl || (long)in_samp > inimg.ns)
	return 0;
//----- save coordinates -----//
inbox[1][1] = in_line;
inbox[1][0] = in_samp;


  /*-------------------------*/
 /* find lower right corner */
/*-------------------------*/
//----- adjust for tolerance around zero -----//
corner[0] = out[0] + outimg.pixsize/2;
corner[1] = out[1] - outimg.pixsize/2;
gctp(corner,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
     &zero,"",&zero,"",in,&inimg.sys,&inimg.zone,inimg.pparm,&inimg.unit,
     &inimg.datum,"","",&status); 
if(status != OK)
 {
  if(dodump)
	dumpflag = 1;
  else
	return 0;
 }
in_line = (inimg.ul_y - in[1]) / inimg.pixsize;
in_samp = (in[0] - inimg.ul_x) / inimg.pixsize;
if(in_line < 0 && in_line < -DELTA_LS)
	in_line = 0;
if(in_samp < 0 && in_samp < -DELTA_LS)
	in_samp = 0;
//----- bounds checking -----//
if((long)in_line < 0 || (long)in_samp < 0 ||
   (long)in_line > inimg.nl || (long)in_samp > inimg.ns)
	return 0;
//----- save coordinates -----//
inbox[2][1] = in_line;
inbox[2][0] = in_samp;
//printf("\n");

//----- Success! Got all four corners, OK to reuse these next time
find2corners = 1;

if(dodump && dumpflag)
	return 0;
else
	return 1;
}// get_coords
