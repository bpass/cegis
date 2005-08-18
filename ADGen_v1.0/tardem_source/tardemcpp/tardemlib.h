/***********************************************************/
/*                                                         */
/* tardem.h                                                */
/*                                                         */
/* TARDEM callable functions -- header file                */
/*                                                         */
/*                                                         */
/* David Tarboton                                          */
/* Utah Water Research Laboratory                          */
/* Utah State University                                   */
/* Logan, UT 84322-8200                                    */
/* http://www.engineering.usu.edu/dtarb/                   */
/*                                                         */
/***********************************************************/

/*  Includes   */
#include <stdio.h>
#include <stdlib.h>

/*  Constants    */
#define MAXLN 4096

/*  Tardem functions    */

int nameadd(char *full,char *arg,char *suff);
/*  Adds the suffix from the 3rd argument (input) to the name in the second argument (input), 
returned as first argument (output). Suffix is added prior to the extension preceded by the 
last "." if any  */

int flood(char *demfile, char *newfile, char * flowfile, short useflowfile, 
		  char *newflowfile);
/*  Removes pits using the flooding algorithm.  Original data in demfile (input), pit filled 
elevations in newfile (output). */

int setdird8(char *demfile, char *pointfile, char *slopefile, char * flowFile,
			 short useflowfile); 
/*  Sets D8 flow directions.  Elevation data in demfile (input),  D8 flow directions in 
pointfile (output).  D8 slopes on slopefile (output).  */ 

int setdir(char *demfile, char *angfile, char *slopefile, char * flowfile, 
			 short useflowfile);   
/*  Sets Dinf flow directions.  Elevation data in demfile (input),  Dinf flow angles in 
angfile (output).  Dinf slopes on slopefile (output).  */ 

int aread8(char *pfile, char *afile, long x, long y, int doall, 
		   char *wfile, int usew, int contcheck);
/*  Computes D8 contributing areas, using D8 pointers from pfile (input).  Result is returned
in afile (output).  x and y are outlet coordinates which may be optionally supplied.  
doall is flag:  0 means use outlet coordinates as outlet, 1 means compute whole grid.
wfile is optional weight file for area computations.  
usew is flag:  0 means do not use weight file, 1 means use weight file
contcheck is flag:  0 means do not check for edge contamination, 
                    1 means check for edge contamination  */

int area(char *pfile, char *afile, long x, long y, int doall, 
		   char *wfile, int usew, int contcheck);
/*  Computes Dinf contributing areas, using Dinf angles from pfile (input).  Result is returned
in afile (output).  x and y are outlet coordinates which may be optionally supplied.  
doall is flag:  0 means use outlet coordinates as outlet, 1 means compute whole grid.
wfile is optional weight file for area computations.  
usew is flag:  0 means do not use weight file, 1 means use weight file
contcheck is flag:  0 means do not check for edge contamination, 
                    1 means check for edge contamination  */

int gridnet(char *pfile,char *plenfile,char *tlenfile,char *gordfile,char *maskfile,
			long x, long y, int mask, int outlet, int thresh);
/*  Uses D8 flow drections in pfile (input) with optional basin maskfile (input)
to compute grid of longest flow length upstream of each point, plenfile (output),
total path length upstream of each point, tlenfile (output) and grid strahler order
gordfile (output).
x, y are optional outlet coordinates.
mask is a flag with value 0 if there is no mask file, 1 if there is.
outlet is a flag with value 0 if no outlet is specifies, 1 if it is.
thresh is the mask threshold used in >= test.  */

int netsetup(char *demfile, int method,
			 float p1,float p2,float p3,float p4, long xr,long yr, int contcheck);
/*  Extracts channel network from grids and saves in vector format.
areafile (input):  grid with D8 contributing area 'ad8'.
slopefile (input):  slopes used only in method 2 ('slp' recommended)
plenfile (input):  path length grid used only in method 3 'plen'
pfile (input):  D8 flow directions file 'p'
srcfile (output):  Grid of contributing area of channel network sources
elevfile (input):  Elevation data used only in method 4 'fel'
gordfile (input):  Grid order file used only in method 5 'gord'
treefile (output):  list of links in channel network tree
coordfile (output):  list of coordinates in channel network tree
ordfile (output): grid of channel network Strahler order
scafile (input): grid with Dinf contributing area 'sca' used in methods 2 and 3.
method (input): integer designating method.  
  1. Catchment area threshold A >= p[0]. 
  2. Area-Slope threshold A S^p[1] >= p[0]. 
  3. Length-Area threshold A >= p[0] L^p[1]. Here L is the maximum drainage length to each cell 
  4. Accumulation area of upward curved grid cells.  The DEM is first smoothed by a kernel 
  with value p[0] at its center, p[1] on its edges, and p[2] on diagonals.  The Peuker and 
  Douglas (1975) method is then used to identify upwards curved grid cells and contributing 
  area computed using only these cells.  A threshold, Auc >= p[3] on these cells is used to 
  map the channel network. 
  5. Grid order threshold  O >= p[0]. 
p[4] (input):  4 element array containing parameters
xr, yr (input):  x and y coordinates of outlet.   
contcheck:  flag to indicate edge contamination checking  */

void arclinks(char *coordfile,char *treefile,char *outfile, int *ilink, float *amin);
/*  Reads tree data files and outputs Arc Export format file.  An element for each link
coordfile (input):  list of coordinates in channel network '*coord.dat'
treefile (input):  list of links in channel network tree '*tree.dat'
outfile (output):  Arc Export format file '*.e00'
ilink (input):  Link number to define outlet if a subset is required (use 0 for whole tree)
amin (input):  Area threshold to prune grid (use 0 for whole tree)   */

void arcstreams(char *coordfile,char *treefile,char *outfile, int *ilink, float *amin);
/*  Reads tree data files and outputs Arc Export format file, an elemnt for each stream.
coordfile (input):  list of coordinates in channel network '*coord.dat'
treefile (input):  list of links in channel network tree '*tree.dat'
outfile (output):  Arc Export format file '*.e00'
ilink (input):  Link number to define outlet if a subset is required (use 0 for whole tree)
amin (input):  Area threshold to prune grid (use 0 for whole tree)   */

int subbasinsetup(char *pfile, char *wfile,char *treefile,char *coordfile,char *shpfile, int ordert);
/*  Sets up subwatersheds each with their unique number
pfile (input): grid of D8 flow directions 'p'
wfile (output):  grid of watershed identifiers 'w'
treefile (input):  list of links in channel network tree '*tree.dat'
coordfile (input):  list of coordinates in channel network '*coord.dat'
shpfile(output):  shape file of resultant channel network '*.shp'
ordert (input):  Strahler order threshold for watershed delineation.   */

int depgrd(char *angfile,char *dgfile, char *depfile);
/* Computes dependence.  Dependence is defined as the contribution to specific catchment 
area at one or a set of grid cells, from each grid cell.
angfile (input):  The grid of Dinf angles.
dgfile (input):  The set of cells whose dependence is to be evaluated.  This is a grid of 1's and 0's.
depfile (output):  The grid containing the dependence function.  */
  


