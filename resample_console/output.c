/*---------------------------------------------------------------------*\
\* output.c  -- unsigned char find_output(long)                        */
/*---------------------------------------------------------------------*\
\* Determine the output pixel by checking for preferred/anti-preferred */
/* classes. Until the algorithm develops further, NN is usually used.  *\
\*---------------------------------------------------------------------*/

extern unsigned char *choicebuf;	// choiceimg buffer
extern unsigned char *nnbuf;		// NN buffer
extern int classcount[][2];		// pixel stat storage
extern int class2data[][2];		// more stat storage
extern int dopreferred;			// P flag
extern int dounpreferred;		// p flag
extern int preferred;
extern int unpreferred;

unsigned char find_output(long out_samp)
{
int i;
int choices = choicebuf[out_samp];
unsigned char temp;

if(choices == 1)
{
	temp = classcount[0][1];
	return class2data[temp][1];
}

if(dopreferred)
 for(i = 0; i < choices; ++i)
  if(classcount[i][1] == preferred)
	return preferred;
	// check for preferred class
if(dounpreferred)
{
 for(i = 0; i < choices; ++i)
  if(classcount[i][1] == unpreferred)
  {
	// move unpreferred to end of list & decrement choices
	temp = classcount[i][1];
	classcount[i][1] = classcount[choices-1][1];
	classcount[choices-1][1] = temp;
	choices--;
  }
  if(choices == 1)
	return classcount[0][1];
}

return nnbuf[out_samp];
}//find_output
