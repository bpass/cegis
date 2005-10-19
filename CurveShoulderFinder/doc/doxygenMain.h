// $Id: doxygenMain.h,v 1.1 2005/10/19 22:09:10 ahartman Exp $

/**
 * @mainpage CurveShoulderFinder
 *
 * @section intro Introduction
 * This project was created to analyze the output of the USDA's Agricultural 
 * Nonpoint Source Pollution (AGNPS) model using different input resolutions
 * in order to determine the point where the model stops improving as input
 * resolution increases.  In order to do this, the model's output will be 
 * compared to ground-truth data at various georeferenced points.
 *
 * The main part of this project is a templated math library that creates 
 * curves to fit points on an accuracy vs. resolution graph.  These curves 
 * will then be analyzed to determine the "best" resolution as described 
 * above.  The library currently contains implementations to find three
 * different types of curves: polynomial, cubic splines, and logistic.  
 * There are also many other files which contain code used to implement 
 * finding these curves.
 *
 * @ref installation Instructions for how to build everything on both
 * Windows and Linux.
 */
