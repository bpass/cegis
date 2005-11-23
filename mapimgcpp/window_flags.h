// $Id: window_flags.h,v 1.1 2005/11/23 00:22:38 mswilliams Exp $


//Copyright 2003 United States Geological Survey
//Released under GPL with mapimg copyright 2003 USGS

///////////////////////////////////////////////////////////
// WINDOW_FLAGS constructs all windows to have a common  //
// window structure with only the reqired objects        //
// Customize tells the program to override class defaults//
// Title tells the program to add a title bar            //
// SysMenu tells the program to add a System Menu        //
// Minimize tells the program to add the ability to      //
//          minimize the window                          //
// fl allows custom flags per class to be sent from      //
//           the calling program                         //
///////////////////////////////////////////////////////////

#ifndef WINDOW_FLAGS_H
#define WINDOW_FLAGS_H

#define WINDOW_FLAGS Qt::WStyle_Customize | Qt::WStyle_Title | Qt::WStyle_SysMenu

#endif
