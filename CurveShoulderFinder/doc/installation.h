// $Id: installation.h,v 1.1 2005/10/19 22:09:10 ahartman Exp $

/**
 * @page installation Installation
 * 
 * @section installationIntroduction Introduction
 * 
 * This project was developed using the GNU Autotools build system, which
 * works in both Windows and Linux and allows the use of the g++ compiler
 * on both platforms.
 *
 * @section installationPrerequisites Prerequisites
 * This section describes how to build the libraries that the program depends
 * upon in order to compile and run.  The instructions are separated for the
 * individual platforms.
 *
 * @subsection installationPrerequisitesCommon Common 
 * <a href="http://www.boost.org/">Boost</a> - This project uses Boost smart
 * pointers to vastly simplify the use shared dynamic memory.  It currently
 * works with Boost 1.32.0, but it will very likely work with any newer
 * version and probably most older versions as well. <BR>
 * <a href="http://www.remotesensing.org/gdal/">GDAL</a> - This project requires
 * the GDAL library in order to read common raster file
 * formats.  This project currently works with GDAL version 1.2.6, but it's
 * likely that both newer and older versions will work as well. <BR>
 * <a href="http://proj.maptools.org/">PROJ</a> - GDAL claims to require this
 * library when transforming coordinates between projections, and it gives
 * a runtime error if it can't find this library, so you'll need it as well.
 * This project currently works with PROJ 4.4.9.
 * <BR><BR>
 * Build instructions for these libraries will be discussed below in the 
 * sections for the individual platforms.
 *
 * @subsection installationPrerequisitesWindows Windows 
 * The first step is get MinGW and MSys from 
 * <a href="http://www.mingw.org">www.mingw.org</a> so that you can
 * use the build system.  First, download and install the package labeled 
 * as just MinGW, which will install the packages labeled MinGW Runtime, 
 * Windows API, binutils, gcc-*, and make.  You can also get those separately 
 * if you desire.  Second, download and install the MSys package, followed by 
 * autoconf, automake, and libtool from the MSys Developer Toolkit package. 
 * <BR><BR>
 * Now you're ready to build Boost using MSys and MinGW.  To do so, follow 
 * the instructions on the
 * <a href="http://www.boost.org/more/getting_started.html">Boost Getting
 * Started</a> page.  You should use the command 
 * <ul>
 *   <li> bjam "-sTOOLS=mingw" install </li>
 * </ul>
 * when you get to the Build and Install step. 
 *
 * Next you'll build PROJ and GDAL, again using MSys and MinGW.  PROJ needs
 * to be built first, since GDAL requires it.  After extracting PROJ, run the
 * following commands in MSys while in PROJ's top directory:
 * <ul>
 *   <li> configure </li>
 *   <li> make </li>
 *   <li> make install </li>
 * </ul>
 * Do almost the same thing for GDAL, except run the following commands which
 * pass some options to the configure script:
 * <ul>
 *   <li> configure --with-unix-stdio-64=no --with-static-proj4=yes 
 *        --enable-shared=no </li>
 *   <li> make </li>
 *   <li> make install </li>
 * </ul>
 * The options passed to configure will build GDAL as a static library, because
 * linking to it dynamically causes linker errors with the current version
 * of MinGW.  Possibly with future improvements to MinGW, it will work 
 * correctly.  The configure options also fix a problem with GDAL's configure
 * script that improperly detects MinGW as having a 64-bit cstdio header.
 * <BR>
 *
 * @subsection installationPrerequisitesLinux Linux 
 * Build Boost by following the instructions on the 
 * <a href="http://www.boost.org/more/getting_started.html">Boost Getting
 * Started</a> page.  You'll use gcc as your toolset and you'll want to
 * do the install step.  You might need to change the installation prefix
 * in case you don't have the ability to write to the system directories.
 *
 * Build both PROJ and GDAL (in that order) with the usual commands to build 
 * from source on Linux:
 * <ul>
 *   <li> configure </li>
 *   <li> make </li>
 *   <li> make install </li>
 * </ul>
 *
 * @section installationProject Building the Actual Project
 *
 * Now that the prerequisites have been taken care of, the actual project can
 * be built.
 *
 * @subsection installationProjectWindows Windows
 * Change to the top directory of the project and run the following commands 
 * from MSys:
 * <ul>
 *   <li> export AUTOMAKE='automake --foreign' </li>
 *   <li> autoreconf --force --install --verbose </li>
 *   <li> configure --enable-static --disable-shared </li>
 *   <li> make </li>
 *   <li> make install </li>
 * </ul>
 *
 * @subsection installationProjectLinux Linux
 * Run the following commands from the top directory of the project:
 * <ul>
 *   <li> export AUTOMAKE='automake --foreign' </li>
 *   <li> autoreconf --force --install --symlink --verbose </li>
 *   <li> configure </li>
 *   <li> make </li>
 *   <li> make install </li>
 * </ul>
 *
 * @section installationProjectFinal Wrapping Up
 * Now that everything has built, run
 * <ul>
 *   <li> make check </li>
 * </ul>
 * to verify that everything worked correctly.  You should get output
 * that says that all tests were passed.  You can now modify and use 
 * the contents of the project as you wish.
 */ 
