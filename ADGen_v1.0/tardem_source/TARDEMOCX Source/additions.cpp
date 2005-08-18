#include "lsm.h"
CTardemCtrl *ocxCtrl;  //use for event handling

//this is needed in order to run netsetup without user input
void MaxAreaD8Coords(char* areafile, long *x, long *y)
{
	int err, max=0,middle,dist;
	long xx=0,yy=0,i,j;
	int **arr;
	
	//initialize values
	*x = 0;
	*y = 0;
	
	err = gridread(areafile,(void ***)&arr,RPINTDTYPE,&nx,&ny,&dx,&dy,bndbox,&csize,&ndv,&filetype);
    if(err != 0) {
		free(arr[0]);
		free(arr);
		return;
	}

	//find max coord
	for(i=0; i<ny; i++) {
		for(j=0; j<nx; j++) {
			if(arr[j][i] > max) {
				max = arr[j][i];
				xx = j;
				yy = i;
			}
		}
	}

	//flip y coordinate 	
	middle = ny/2;
	dist = middle-yy;
	yy += dist*2;
	if(ny%2 == 0)
		yy--;

	//set to world coordinates
	utme=bndbox[0]+dx*0.5;
    utmn=bndbox[1]+dy*0.5;
	(*x) = utme + csize * xx;
	(*y) = utmn + csize * yy;
}

//used for event calling throughout Tardem procedures
void setCtrl(CTardemCtrl *ctrl)
{
	ocxCtrl = ctrl;
}