
// CVS magic symbols for voodoo practitioners
// $Id: doxygenMain.h,v 1.4 2004/11/15 22:33:49 rstelzleni Exp $

/** \mainpage IntersectionLocator
 *
 * \section intro Introduction
 * <P>
 * The intersection locator is an attempt to automatically correct vector
 * road maps without human intevention.  The basic idea is to use a raster
 * image, like an aerial photograph of an area, to locate the true
 * locations of the intersections, and then to use a rubber sheeting 
 * transformation with the newly identified better intersection locations
 * as the control points to correct the vector roads.  
 * </P><P>
 * The new intersection locations are found using a pattern matching 
 * technique.  First the locations of the intersection in the vector image
 * are found in the raster image.  Next the vector image is used to create
 * a temporary in-memory raster image (also called a template) of how
 * we expect the intersection to look.  This image is based on the angles
 * of the intersecting roads and the width of the roads, which is gleaned
 * from the vector data.  The next step is to find a spot on the raster
 * that is near the intersection in the vector data and that matches the
 * temporary raster image best.  This spot is then used as the new 
 * better location for the intersection.
 * </P><P>
 * That is a very brief overview of the process that skims over the 
 * road pixel identification, and control point filtering.  
 * </P>
 *
 * \section usage Usage
 * \ref preparingForNewData Instructions for preparing for a new dataset
 *
 * It is probably worth pointing out here that the 
 * IntersectionMap::findControlPoints function will print out a bunch of
 * messages that say "processing intersection 1 of 543" or some other 
 * numbers.  The total number of intersections isn't correct in this
 * output, what it actually uses as the total number of intersections
 * is the total number of points in all the lines.  This could be fixed
 * by looping through the vControl vector once and counting the number of
 * points that are intersections, but I didn't go to the trouble, so ... 
 * sorry.
 *
 * \section installation Installation
 * <P>
 * This project was developed in VC6.  I haven't tried to build it in
 * any other environments, but in theory I didn't use anything that
 * isn't portable.  To build the project you will need the GDAL/OGR 
 * library, which is available at 
 * <a href="http://www.remotesensing.org/gdal/">www.remotesensing.org/gdal</a>
 * </P><P>
 * I built the project using version 1.2.0 of that library.  If you
 * download the pre-built gdal-120-ntbin.zip package it includes 
 * everything you will need.  If you can't find that package then feel
 * free to try the latest version.  It "should" work, but of course,
 * there are no guarantees.  If whatever version of gdal you get doesn't
 * include the proj dynamic library then you will have to find a copy
 * of that too.
 * </P><P>
 * To build the project the include files for gdal will need to be in
 * your include path, and you will need to link against the static library
 * gdal_i.lib.  The easiest way to handle that in VC6 is to add it to the
 * "Source Files" folder.  To run the built executable you will need to
 * have the gdal12.dll and proj.dll (both can be easily acquired from the
 * website mentioned above, in the mentined package for instance)
 * somewhere where the program can find them.  In the project directory 
 * is a simple solution.
 * </P><P>
 * Running the program is a whole 'nother story.  Because the requirements
 * have been changing so quickly my solution has been to write a specialized
 * driver for each run.  To do this you will need to do some reading up
 * on how the program works, and that's what this documentation is for.
 * Good Luck.
 * </P>
 *
 * \subsection linuxInstallation Installation in Linux
 * \ref buildingOnLinux Linux Installation Guide
 */
