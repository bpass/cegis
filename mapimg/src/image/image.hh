/* Programmer: David Mattli */

#ifndef IMAGE_HH
#define IMAGE_HH

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* Class: image
 * Description: Abstract image container, representing a grid of values.
 *
 * Constructors: image() Description: Default constructor,
 * 'initialize' must be run before image can be used.
 *
 */

using std::string;

namespace image {

   /* T is pixel value type and C is coordinate type */
   template <typename T, typename C = unsigned int>
   class image
   {
      public:
	 image ();
	 image (string filename, C cols, C rows);
	 ~image();
    
	 bool initialize(string filename, C cols, C rows);
	 bool zero();

	 bool shut();
  
	 bool ready();

	 T getPoint (C x, C y);
	 bool setPoint (C x, C y, T value);

	 C num_cols();
	 C num_rows();

      private:
	 /* private data */
	 string name;
	 int imgfile;
	 C ncols;
	 C nrows;
	 bool readOnly;
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
   bool image<T, C>::initialize (string filename, C cols, C rows)
   {
      ncols = cols;
      nrows = rows;
      name = filename;
      
      
      imgfile = open(filename.c_str(), _O_BINARY|_O_RDWR|_O_CREAT, 00700);
      
      if (imgfile == -1)
	 return false;
      else 
	 return true;
    
   }

  template <typename T, typename C>
  bool image<T, C>::zero()
  {
    int nr = 0;
    void *buf = calloc(ncols, sizeof(C));

    if (buf != 0) {
      for(unsigned int i=0; i<nrows; ++i)
	nr = write(imgfile, buf, sizeof(buf));
      free(buf);
      return true;
      }
    else
      return false;
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
   T image<T, C>::getPoint (C x, C y)
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
   bool image<T, C>::setPoint (C x, C y, T value)
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
   C image<T, C>::num_cols()
   {
      return ncols;
   }

   template <typename T, typename C>
   C image<T, C>::num_rows()
   {
      return nrows;
   }
	

} //namespace image

#endif //IMAGE_H
