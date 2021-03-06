/******************************************************************************
 * $Id: dbfcreate.cpp,v 1.1 2005/08/18 01:00:41 ahartman Exp $
 *
 * Project:  Shapelib
 * Purpose:  Sample application for creating a new .dbf file.
 * Author:   Frank Warmerdam, warmerda@home.com
 *
 ******************************************************************************
 * Copyright (c) 1999, Frank Warmerdam
 *
 * This software is available under the following "MIT Style" license,
 * or at the option of the licensee under the LGPL (see LICENSE.LGPL).  This
 * option is discussed in more detail in shapelib.html.
 *
 * --
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 *
 * $Log: dbfcreate.cpp,v $
 * Revision 1.1  2005/08/18 01:00:41  ahartman
 * Initial commit of the source code from the Cartographic Research web page.
 *
 * Revision 1.4  1999/11/05 14:12:04  warmerda
 * updated license terms
 *
 * Revision 1.3  1999/04/01 18:47:44  warmerda
 * Fixed DBFAddField() call convention.
 *
 * Revision 1.2  1995/08/04 03:17:11  warmerda
 * Added header.
 *
 */

static char rcsid[] = 
  "$Id: dbfcreate.cpp,v 1.1 2005/08/18 01:00:41 ahartman Exp $";

#include "shapefil.h"

