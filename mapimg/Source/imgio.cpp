// $Id: imgio.cpp,v 1.5 2006/05/19 18:55:18 lwoodard Exp $


//Copyright 2002 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mapimg I/O functions:  These functions read and write files associated with mapimg
// Written by D. Steinwand
//
// Note: These functions are used by the IMGIO class but not actually members of it.
//
// Updated by Stephan Posch for GUI implementation -- 8/02
// Updated by Jason Trent for templating to allow multiple data types -- 7/03
// Updated again by Jason Trent for object oriented approach with IMGIO class -- 11/04
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <QMessageBox>
#include <QString>
#include <QFile>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "imgio.h"

#include "mapimg.h"


int get_coords( IMGINFO outimg, IMGINFO inimg, double out[2], double inbox[5][2], long, long, FILE* paramfile, bool centerOnly )
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

   double in_line, in_samp;	/* Input image coordinates of a point */
   long in_line_temp, in_samp_temp;
   long status;		/* Return status flag for gctp() call */
   long zero=0;		/* Constant of 0 */
   double in[2];		/* Input projection coordinates of a point */
   double corner[2];		/* for computing values at pixel corners */

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
         &zero,"",&zero,"",paramfile,in,&inimg.sys,&inimg.zone,inimg.pparm,&inimg.unit,
         &inimg.datum,"","",&status);

      if(status != OK)
         return 0;

   }//if Goode's
   else
   {
      //------ output => lat/long ------//
      gctp(out,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
         &zero,"",&zero,"",0,coordt1,&sys,&zone,pparm,&unit,
         &datum,"","",&status);

      //----- lat/long => output -----//
      gctp(coordt1,&sys,&zone,pparm,&unit,&datum,&zero,"",&zero,"",0,coordt2,&outimg.sys,
         &outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,"","",&status);

      //----- compare values... if different we're in a fill area -----//
      temp1=fabs(coordt2[0] - out[0]);
      temp2=fabs(coordt2[1] - out[1]);

      if(temp1 > DELTA_METERS || temp2 > DELTA_METERS)
         return 0;

      //----- lat/long => input -----//
      gctp(coordt1,&sys,&zone,pparm,&unit,&datum,&zero,"",&zero,"",paramfile,in,&inimg.sys,
         &inimg.zone,inimg.pparm,&inimg.unit,&inimg.datum,"","",&status);

      if(status != OK)
         return 0;
   } //else not goode's

   in_line = (inimg.ul_y - in[1]) / inimg.pixsize;
   in_samp = (in[0] - inimg.ul_x) / inimg.pixsize;

   if(in_line < 0 && in_line < -DELTA_LS)
      in_line = 0;
   if(in_samp < 0 && in_samp < -DELTA_LS)
      in_samp = 0;

   //----- Are we in bounds? -----//
   in_line_temp = (long)mapimg::round(in_line);
   in_samp_temp = (long)mapimg::round(in_samp);

   if( in_line_temp <0 || in_samp_temp <0 ||
      in_line_temp >= inimg.nl || in_samp_temp >= inimg.ns )
      return 0;

   inbox[4][1] = in_line_temp;
   inbox[4][0] = in_samp_temp;

   //----- Are we only looking for the center? -----//
   if( centerOnly )
   {
      inbox[0][0] = inbox[0][1] = inbox[1][0] = inbox[1][1] = 0;
      inbox[2][0] = inbox[2][1] = inbox[3][0] = inbox[3][1] = 0;
      return 1;
   }

   //----- last pixel's right side is this pixel's left side -----//
   find2corners = 0;
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
            &zero,"",&zero,"",paramfile,in,&inimg.sys,&inimg.zone,inimg.pparm,&inimg.unit,
            &inimg.datum,"","",&status);

         if(status != OK)
            return 0;
      }
      else
      {
         //------ output => lat/long ------//
         gctp(corner,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
            &zero,"",&zero,"",0,coordt1,&sys,&zone,pparm,&unit,
            &datum,"","",&status);

         //----- lat/long => output -----//
         gctp(coordt1,&sys,&zone,pparm,&unit,&datum,&zero,"",&zero,"",0,coordt2,&outimg.sys,
            &outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,"","",&status);

         //----- compare values... if different we're in a fill area -----//
         temp1=fabs(coordt2[0] - corner[0]);
         temp2=fabs(coordt2[1] - corner[1]);
         if(temp1 > DELTA_METERS || temp2 > DELTA_METERS)
            return 0;

         //----- lat/long => input -----//
         gctp(coordt1,&sys,&zone,pparm,&unit,&datum,&zero,"",&zero,"",paramfile,in,&inimg.sys,
            &inimg.zone,inimg.pparm,&inimg.unit,&inimg.datum,"","",&status);

         if(status != OK)
            return 0;
      }	// not goode's

      in_line = (inimg.ul_y - in[1]) / inimg.pixsize;
      in_samp = (in[0] - inimg.ul_x) / inimg.pixsize;

      //----- adjust for tolerance around zero -----//
      if(in_line < 0 && in_line < -DELTA_LS)
         in_line = 0;

      if(in_samp < 0 && in_samp < -DELTA_LS)
         in_samp = 0;

      //----- bounds checking -----//
      in_line_temp = (long)mapimg::round(in_line);
      in_samp_temp = (long)mapimg::round(in_samp);

      if(in_line_temp+DELTA_LS < 0 || in_samp_temp+DELTA_LS < 0 ||
         in_line_temp >= inimg.nl   || in_samp_temp >= inimg.ns)
         return 0;

      //----- save coordinates -----//
      inbox[0][1] = in_line_temp;
      inbox[0][0] = in_samp_temp;


      /*------------------------*/
      /* find lower left corner */
      /*------------------------*/

      corner[0] = out[0] - outimg.pixsize/2;
      corner[1] = out[1] - outimg.pixsize/2;
      if(outimg.sys == 24)
      {
         gctp(corner,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
            &zero,"",&zero,"",paramfile,in,&inimg.sys,&inimg.zone,inimg.pparm,&inimg.unit,
            &inimg.datum,"","",&status);

         if(status != OK)
            return 0;
      }
      else
      {
         //------ output => lat/long ------//
         gctp(corner,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
            &zero,"",&zero,"",0,coordt1,&sys,&zone,pparm,&unit,
            &datum,"","",&status);

         //----- lat/long => output -----//
         gctp(coordt1,&sys,&zone,pparm,&unit,&datum,&zero,"",&zero,"",0,coordt2,&outimg.sys,
            &outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,"","",&status);


         //----- compare values... if different we're in a fill area -----//
         temp1=fabs(coordt2[0] - corner[0]);
         temp2=fabs(coordt2[1] - corner[1]);
         if(temp1 > DELTA_METERS || temp2 > DELTA_METERS)
            return 0;

         //----- lat/long => input -----//
         gctp(coordt1,&sys,&zone,pparm,&unit,&datum,&zero,"",&zero,"",paramfile,in,&inimg.sys,
            &inimg.zone,inimg.pparm,&inimg.unit,&inimg.datum,"","",&status);

         if(status != OK)
            return 0;
      }

      in_line = (inimg.ul_y - in[1]) / inimg.pixsize;   //to match working center
      in_samp = (in[0] - inimg.ul_x) / inimg.pixsize;

      //----- adjust for tolerance around zero -----//
      if(in_line < 0 && in_line < -DELTA_LS)
         in_line = 0;

      if(in_samp < 0 && in_samp < -DELTA_LS)
         in_samp = 0;

      //----- bounds checking -----//
      in_line_temp = (long)mapimg::round(in_line);
      in_samp_temp = (long)mapimg::round(in_samp);

      if(in_line_temp+DELTA_LS < 0 || in_samp_temp+DELTA_LS < 0 ||
         in_line_temp >= inimg.nl   || in_samp_temp >=inimg.ns)
         return 0;

      //----- save coordinates -----//
      inbox[3][1] = in_line_temp;
      inbox[3][0] = in_samp_temp;

   } //else(need to get ul&ll corners)


   /*-------------------------*/
   /* find upper right corner */
   /*-------------------------*/
   corner[0] = out[0] + outimg.pixsize/2;
   corner[1] = out[1] + outimg.pixsize/2;

   gctp(corner,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
      &zero,"",&zero,"",paramfile,in,&inimg.sys,&inimg.zone,inimg.pparm,&inimg.unit,
      &inimg.datum,"","",&status);

   if(status != OK)
      return 0;

   in_line = (inimg.ul_y - in[1]) / inimg.pixsize;      //to match working center
   in_samp = (in[0] - inimg.ul_x) / inimg.pixsize;

   //----- adjust for tolerance around zero -----//
   if(in_line < 0 && in_line < -DELTA_LS)
      in_line = 0;

   if(in_samp < 0 && in_samp < -DELTA_LS)
      in_samp = 0;

   //----- bounds checking -----//
   in_line_temp = (long)mapimg::round(in_line);
   in_samp_temp = (long)mapimg::round(in_samp);

   if(in_line_temp < 0 || in_samp_temp < 0 ||
      in_line_temp >= inimg.nl || in_samp_temp >= inimg.ns)
      return 0;

   //----- save coordinates -----//
   inbox[1][1] = in_line_temp;
   inbox[1][0] = in_samp_temp;



   /*-------------------------*/
   /* find lower right corner */
   /*-------------------------*/

   //----- adjust for tolerance around zero -----//
   corner[0] = out[0] + outimg.pixsize/2;
   corner[1] = out[1] - outimg.pixsize/2;

   gctp(corner,&outimg.sys,&outimg.zone,outimg.pparm,&outimg.unit,&outimg.datum,
      &zero,"",&zero,"",paramfile,in,&inimg.sys,&inimg.zone,inimg.pparm,&inimg.unit,
      &inimg.datum,"","",&status);

   if(status != OK)
      return 0;

   in_line = (inimg.ul_y - in[1]) / inimg.pixsize;        //to match working cneter
   in_samp = (in[0] - inimg.ul_x) / inimg.pixsize;

   if(in_line < 0 && in_line < -DELTA_LS)
      in_line = 0;

   if(in_samp < 0 && in_samp < -DELTA_LS)
      in_samp = 0;

   //----- bounds checking -----//
   in_line_temp = (long)mapimg::round(in_line);
   in_samp_temp = (long)mapimg::round(in_samp);

   if(in_line_temp < 0 || in_samp_temp < 0 ||
      in_line_temp >= inimg.nl || in_samp_temp >= inimg.ns)
      return 0;

   //----- save coordinates -----//
   inbox[2][1] = in_line_temp;
   inbox[2][0] = in_samp_temp;


   //----- Success! Got all four corners, OK to reuse these next time
   find2corners = 1;

   return 1;
}// get_coords


int onLine(double p1[2], double p2[2], double test[2])
{
   float dx, dy, dxt, dyt;

   if( (test[0] == p1[0] && test[1] == p1[1]) ||
      (test[0] == p2[0] && test[1] == p2[1]) )
      return 1;

   if( !((p1[0] <= test[0] && test[0] < p2[0]) || (p2[0] <= test[0] && test[0] < p1[0])) )
      return 0;

   dx = p2[1] - p1[1];
   dy = p2[0]-p1[0];

   dxt = test[1] - p1[1];
   dyt = test[0] - p1[0];

   if(dx*dyt - dy*dxt)
      return 0;
   else
      return 1;
}      //onLine

int inBox(double box[5][2], double test[2])
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
}//inBox


