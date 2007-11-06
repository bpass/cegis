/* Programmer: David Mattli */

#ifndef TRANS_TRANSFORMATION_HH
#define TRANS_TRANSFORMATION_HH

namespace trans {

  enum Datum {EARTH};
    
  template <typename C>
  struct coord
  {
    C x;
    C y;
  };

  template <typename C>
  class transformation
  {
  public: 
    transformation();
    virtual ~transformation();
    
    virtual coord<C> forward (coord<C> geoCoord)=0;
    virtual coord<C> inverse (coord<C> rectProjCoord)=0;

  protected:
    unsigned int scale;
  };

  template <typename C>
  transformation<C>::transformation()
  {
    return;
  }

  template <typename C>
  transformation<C>::~transformation()
  {
    return;
  }
    
}


#endif
