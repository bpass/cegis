/* File: image.hh
   Programmer: David Mattli

   Copyright 2007 U.S. Geological Survey

   This program is free software; you can redistribute it and/or
   modify if under the terms of the GNU General Public License version
   2 as published by the Free Software Foundation.
*/

#ifndef IMAGE_HH
#define IMAGE_HH

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using std::string;

/* Class: image
 * Description: Container for generic binary image files
 *
 * Constructors: image() Description: Default constructor,
 * 'initialize' must be run before image can be used.
 *
 * PUBLIC MEMBERS: 
 * bool init(string filename, C cols, C rows)
 * Precondition: Parameter filename corresponds to a valid file or place for
 *               file, cols and rows > 0 
 * Postcondition: A file at filename is opened and if it has fewer than the
 *                specified rows/columns then the image is zero filled to 
 *                the appropriate size.
 *

 * 
 * 
 *                
 * 
 */

namespace image {
    /* T is pixel value type and C is coordinate type */
    template <typename T, typename C = unsigned int>
    class image
    {
    public:
	image ();
	image (string filename, C cols, C rows);
	~image();
    
	bool shut();
	bool ready();

	T pixel (C x, C y);
	bool setPixel (C x, C y, T value);

	C numberColumns();
	C numberRows();

    private:
	/* private data */
	string name;
	int imgfile;
	C ncols;
	C nrows;
	int badbit;

    };


    /*               
     * Implementation 
     */             

    template <typename T, typename C>
    image<T, C>::image ()
    {
	imgfile = -1;
	badbit = 0;
	return;
    }
  
    template <typename T, typename C>
    image<T, C>::image (string filename, C cols, C rows)
    {
	imgfile = -1;
	badbit = 0;
	initialize(filename, cols, rows);
    
	return;
    }
        
    template <typename T, typename C>
    image<T, C>::~image()
    {
	shut();
	return;
    }

    template <typename T, typename C>
    bool image<T, C>::init (string filename, C cols, C rows)
    {
	int fsize = 0;
	void *buf = calloc(2, 1);

	ncols = cols;
	nrows = rows;
	name = filename;
      
	imgfile = open(filename.c_str(), _O_BINARY|_O_RDWR|_O_CREAT, 00700);
	fsize = lseek(imgfile, 0, SEEK_END)/sizeof(T);

	if (imgfile == -1) {
	    badbit = -1;
	    return false;
	}
	else if (fsize < ncols * nrows) {
	    fsize = lseek(imgfile, ncols*nrows, SEEK_SET);
	    write(imgfile, buf, 1);
	}

	return true;
    }

    template <typename T, typename C>
    bool image<T, C>::shut()
    {
	if (close(imgfile) == 0)
	    return true;
	else 
	    return false;
    }
  
    template <typename T, typename C>
    bool image<T, C>::ready()
    {
	if ((imgfile != -1) && (badbit == 0))
	    return true;
	else
	    return false;
    }
  
    template <typename T, typename C>
    T image<T, C>::pixel (C x, C y)
    {
	T temp;
	C position = x + (ncols * y); 
	unsigned int nr = 0;
    
	lseek(imgfile, position, SEEK_SET);
	if ((nr = read(imgfile, *temp, sizeof(T))) != -1 && nr  != 0)
	    return temp;
	else /* ERROR ! */
	    return temp;
    }

    template <typename T, typename C>
    bool image<T, C>::setPixel (C x, C y, T value)
    {
	int nr = 0;
	C position = x + (ncols * y);
	T buf[1] = {value};
	
	lseek(imgfile, position, SEEK_SET);
	if ((nr = write(imgfile, buf, sizeof(T))) != -1 && nr  != 0)
	    return true;
	else
	    return false;
    }

    template <typename T, typename C>
    C image<T, C>::numberColumns()
    {
	return ncols;
    }

    template <typename T, typename C>
    C image<T, C>::numberRows()
    {
	return nrows;
    }
	

} // namespace image

#endif //IMAGE_HH
