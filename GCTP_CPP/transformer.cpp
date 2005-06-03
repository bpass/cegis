#include "transformer.h"

#include "sinusoidal.h"
#include "equirectangular.h"
#include "mollweide.h"
#include "mercator.h"
#include "albersconeqarea.h"

Transformer::Transformer()
: m_inProj(NULL), m_outProj(NULL)
{
}

Transformer::~Transformer()
{
   if( m_inProj )
      delete m_inProj;

   if( m_outProj )
      delete m_outProj;
}

bool Transformer::setInput( int projectionCode )
{
   if( m_inProj )
      delete m_inProj;

   m_inProj = convertProjection( projectionCode );

   return (m_inProj != NULL );
}

bool Transformer::setInput( int projectionCode, double gctpParameters[15], int units, long datum, long spheroid )
{
   if( setInput( projectionCode ) )
   {
      m_inProj->setParams( gctpParameters );
      m_inProj->setUnits( units );
      m_inProj->setDatum( datum );
      m_inProj->setSpheroid( spheroid );
      return true;
   }
   else
      return false;
}

bool Transformer::setInput( Projection &in )
{
   return setInput( in.number(), in.params(), in.units(), in.datum(), in.spheroid() );
}

Projection* Transformer::input()
{
   return m_inProj;
}

bool Transformer::setOutput( int projectionCode )
{
   if( m_outProj )
      delete m_outProj;

   m_outProj = convertProjection( projectionCode );

   return (m_outProj != NULL );
}

bool Transformer::setOutput( int projectionCode, double gctpParameters[15], int units, long datum, long spheroid )
{
   if( setOutput( projectionCode ) )
   {
      m_outProj->setParams( gctpParameters );
      m_outProj->setUnits( units );
      m_outProj->setDatum( datum );
      m_outProj->setSpheroid( spheroid );
      return true;
   }
   else
      return false;
}

bool Transformer::setOutput( Projection &out )
{
   return setOutput( out.number(), out.params(), out.units(), out.datum(), out.spheroid() );
}

Projection* Transformer::output()
{
   return m_outProj;
}

void Transformer::transform( Coordinate* io_coord )
{
   if( m_inProj == NULL || m_outProj == NULL || io_coord->units != m_inProj->units() )
   {
      io_coord->x = 0;
      io_coord->y = 0;
      io_coord->units = 0;
      return;
   }

   Coordinate tmp;
   m_inProj->inverse( io_coord->x, io_coord->y, &(tmp.x), &(tmp.y) );
   m_outProj->forward( tmp.x, tmp.y, &(io_coord->x), &(io_coord->y) );
   io_coord->units = m_outProj->units();
}

void Transformer::transformInverse( Coordinate* io_coord )
{
   if( m_inProj == NULL || io_coord->units != m_inProj->units() )
   {
      io_coord->x = 0;
      io_coord->y = 0;
      io_coord->units = 0;
      return;
   }

   Coordinate tmp(*io_coord);
   m_inProj->inverse( tmp.x, tmp.y, &(io_coord->x), &(io_coord->y) );
   io_coord->units = DEGREE;
}

void Transformer::transformForward( Coordinate* io_coord )
{
   if( m_outProj == NULL || io_coord->units != DEGREE )
   {
      io_coord->x = 0;
      io_coord->y = 0;
      io_coord->units = 0;
      return;
   }

   Coordinate tmp(*io_coord);
   m_outProj->forward( tmp.x, tmp.y, &(io_coord->x), &(io_coord->y) );
   io_coord->units = m_outProj->units();
}

Projection* Transformer::convertProjection( int projectionCode )
{
   Projection* proj = NULL;

   switch( projectionCode )
   {
   case ALBERS:
      proj = new AlbersConEqArea();
      break;
   case MERCAT:
      proj = new Mercator();
      break;
   case SNSOID:
      proj = new Sinusoidal();
      break;
   case EQRECT:
      proj = new Equirectangular();
      break;
   case MOLL:
      proj = new Mollweide();
      break;
   }

   return proj;
}

