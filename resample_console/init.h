#ifndef INIT_H
#define INIT_H

#include "imgio.h"

/*
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
*/


void parse_input(int *argc, char *argv[], float *fill);
void get_image(unsigned char *buf);
void clear_buffer(unsigned char* buf, long bufsize);
unsigned char* init_input( struct IMGINFO *inimg);
unsigned char* init_output( struct IMGINFO *outimg);
void init_class(int bufsize);
void init_stats(int bufsize);






void get_image_line( FILE* file, unsigned int lineNumber, unsigned int lineSize, unsigned int *buffer );

#endif



