/* THis procedure dumps parts of an image to screen
   in decimal form */

/*
void pixdump(unsigned char *buf, long nl, long ns, long minx, long miny, long maxx, long maxy)
{
 long i, j;

printf("\n");
 for(i = miny; i <= maxy; ++i)
 {
  for(j = minx; j <= maxx; ++j)
	printf("%4d", buf[i*ns+j]);
  printf("\n");
 }
printf("\n");

}
*/

void pixdump(double box[][2])
{
	printf("%lf,%lf\t%lf,%lf\t\t",box[0][0],box[0][1],box[1][0],box[1][1]);
	printf("%lf,%lf\n",box[4][0],box[4][1]);
	printf("%lf,%lf\t%lf,%lf\n\n",box[3][0],box[3][1],box[2][0],box[2][1]);
}
