
// CVS magic symbols for voodoo practitioners
// $Id: doxygenMain.h,v 1.10 2004/11/19 18:23:36 rstelzleni Exp $

/** \mainpage IntersectionLocator
 *
 * \section intro Introduction
 * 
 * The intersection locator is an attempt to automatically correct vector
 * road maps without human intervention.  The basic idea is to use a raster
 * image, like an aerial photograph of an area, to locate the true
 * locations of the intersections, and then to use a rubber sheeting 
 * transformation with the newly identified better intersection locations
 * as the control points to correct the vector roads.  
 * 
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
 * 
 * That is a very brief overview of the process that skims over the 
 * road pixel identification, and control point filtering.  
 * 
 *
 * \section usage Usage
 * \ref preparingForNewData Instructions for preparing for a new dataset <BR>
 * \ref overallProcess Explanation of the overall process <BR>
 * \ref relatedLinks Web pages that you might find helpful<BR><BR>
 * It is probably worth pointing out here that the 
 * IntersectionMap::findControlPoints function will print out a bunch of
 * messages that say "processing intersection 1 of 543" or some other 
 * numbers.  The total number of intersections isn't correct in this
 * output, what it actually uses as the total number of intersections
 * is the total number of points in all the lines.  This could be fixed
 * by looping through the mIntersectMap once and counting the number of
 * points that are intersections, but I didn't go to the trouble, so ... 
 * sorry.
 *
 * \section installation Installation
 * 
 * This project was developed in VC6.  I haven't tried to build it in
 * any other environments, but in theory I didn't use anything that
 * isn't portable.  To build the project you will need the GDAL/OGR 
 * library, which is available at 
 * <a href="http://www.remotesensing.org/gdal/">www.remotesensing.org/gdal</a>
 * 
 * I built the project using version 1.2.0 of that library.  If you
 * download the pre-built gdal-120-ntbin.zip package it includes 
 * everything you will need.  If you can't find that package then feel
 * free to try the latest version.  It "should" work, but of course,
 * there are no guarantees.  If whatever version of gdal you get doesn't
 * include the proj dynamic library then you will have to find a copy
 * of that too.
 * 
 * To build the project the include files for gdal will need to be in
 * your include path, and you will need to link against the static library
 * gdal_i.lib.  The easiest way to handle that in VC6 is to add it to the
 * "Source Files" folder.  To run the built executable you will need to
 * have the gdal12.dll and proj.dll (both can be easily acquired from the
 * website mentioned above, in the mentined package for instance)
 * somewhere where the program can find them.  In the project directory 
 * is a simple solution.
 * 
 * Running the program is a whole 'nother story.  Because the requirements
 * have been changing so quickly my solution has been to write a specialized
 * driver for each run.  To do this you will need to do some reading up
 * on how the program works, and that's what this documentation is for.
 * Good Luck.
 * 
 *
 * \subsection linuxInstallation Installation in Linux
 * \ref buildingOnLinux Linux Installation Guide
 *
 * \section mainPageNotes Notes
 *
 * \subsection dependencyOnGDALOGR Dependency on GDAL/OGR
 * One thing that I think it is good to point out is why there is a 
 * dependency on the GDAL/OGR library.  The reason is that parsing 
 * geospatial data files, like Arc Coverages and Shapefiles, isn't a
 * simple task, and the GDAL/OGR library already does it.  Basically
 * I saved myself about 3 months of coding by using this open source
 * tool.  I have tried not to depend on it more than is necessary in 
 * the code, but if you ever want to get rid of it you will probably 
 * be in for some work.  Off the top of my head, the main place in the
 * code that uses GDAL/OGR is the driver, but a little of this stuff
 * snuck into my classes from time to time.  Any output function that
 * outputs to something other than text or generic binary will require
 * GDAL/OGR, and The IntersectionMap class also relies on the library
 * pretty heavily.  Also, I seem to remember that InMemRaster uses the
 * library's functions for reading in Tiff images.  
 *
 * I definitely should have encapsulated that library instead of using
 * it directly, but hindsight is 20/20 and I'm pretty sure I'm blind
 * when it comes to foresight.  
 *
 * \subsection transformationProblems Transformation Problems
 * The rubber sheeting transformation that the program currently uses
 * has some major problems that could be improved upon.  The transformation
 * works by creating a set of traingles (a triangulation) that includes
 * every control point as a triangle vertex.  Then it creates a set of
 * linear transformations, one for each triangle, that move the original
 * points onto the control points.  Once this transformation is defined
 * the program looks to see what triangle each point is in and then applies
 * the linear transformation for that triangle to the point.  
 *
 * This works fine so long as all points are contained in the triangulation,
 * and so long as there are as few small angles as possible in the 
 * triangles that are generated.  The few small angles problem is helped
 * by the basic properties of the Delauney Triangulation (it maximizes
 * the minimum angle in the set of triangles), but there still wind up
 * being some pretty small angles near the edges of the area that is
 * covered by the control points.  Knuth describes a method of performing
 * this triangulation which assumes that we have three imaginary control
 * points at infinity, and thus fixes the problem of small edges at 
 * the outside borders, but I have had trouble finding a paper describing
 * his method.  
 *
 * The other problem with the triangulation occurs when the control points
 * create a set of triangles that doesn't contain every original point.
 * This results in some lines where some of the control points are moved
 * and others are not, and so creates an ugly, wavy line.  Knuth's method
 * would also correct this problem, so a better triangulation would be
 * a major source of improvement.
 *
 * \subsection classification Classification
 * The classifier could be improved a great deal, but it will not be easy
 * to do.  The current classifier uses the three attributes, hue, saturation,
 * and value of a color to tell if it is a road.  One possible alternative
 * would be to use the three as a single attribute of the pixel, instead
 * of as three separate attributes.  Another way that the classifier might
 * be able to be improved would be to add more attributes to the list of
 * what is checked.  i.e. proximity to other road pixels, or road pixels 
 * are more likely to form linear features, etc.  I'm not sure how you
 * would implement those in such a way that the classifier could use the
 * information, but if you could I imagine that it would help.
 */
