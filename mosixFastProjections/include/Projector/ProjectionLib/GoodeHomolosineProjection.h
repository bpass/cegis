//Class def for the Goodes Homolosine projection by Chris Bilderback

#ifndef _GOODEHOMOLOSINEPROJECTION_H_
#define _GOODEHOMOLOSINEPROJECTION_H_


#include <new>
#include "PseudocylindricalProjection.h"


namespace ProjLib
{


class GoodeHomolosineProjection : public PseudocylindricalProjection  
{
public:
	//Constructors
	GoodeHomolosineProjection(double sphereRadius, DATUM d, UNIT u,
			                  DATUM geoDatum = DEFAULT_DATUM,
							  UNIT geoUnit = ARC_DEGREES);
	GoodeHomolosineProjection(const GoodeHomolosineProjection & rhs);

	//Identification
	PROJSYS getProjectionSystem() const throw();
	
	//Carbon copy (remember to delete the memory returned)
	Projection * clone() const throw(std::bad_alloc);

	// String override
    std::string toString() const throw();
};

// ***************************************************************************
inline PROJSYS GoodeHomolosineProjection::getProjectionSystem() const throw()
{
  return GOOD; //Code for Goode's Homolosine Projection
}

} // namespace ProjLib

#endif
	
		

