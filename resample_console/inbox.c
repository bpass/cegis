/* These procedures are used to determine if a point is inside a polygon */

int onLine(double p1[2], double p2[2], double test[2])
{
float dx, dy, dxt, dyt;

if(test == p1 || test == p2) return 1;

if( !((p1[0] <= test[0] && test[0] < p2[0]) || (p2[0] <= test[0] && test[0] < p1[0])) )
	return 0;

dx = p2[1] - p1[1];	dy = p2[0]-p1[0];
dxt = test[1] - p1[1];	dyt = test[0] - p1[0];

if(dx*dyt - dy*dxt)
	return 0;
else
	return 1;
}

int inBox(double box[4][2], double test[2])
{
int intersections = 0;	// # of times test line crosses a polgon line
int i;			// loop counter
int j = 3;		// another placeholder

for(i = 0; i < 4; ++i)
{
  if(onLine(box[j], box[i], test))
	return 1;
  if( (test[1] < box[j][1] || test[1] < box[i][1]) &&
    ( (box[j][0] <= test[0] && test[0] < box[i][0]) ||
      (box[i][0] <= test[0] && test[0] < box[j][0]) ) )
	intersections++;
  j = i;
}

return ( intersections % 2 );
}
