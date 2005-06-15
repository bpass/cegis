#include "transformer.h"

#include "albersconeqarea.h"
#include "lambertcc.h"
#include "mercator.h"
#include "polyconic.h"
#include "equidistantc.h"
#include "sinusoidal.h"
#include "equirectangular.h"
#include "mollweide.h"
#include "intmollweide.h"
#include "transversemercator.h"
#include "miller.h"
#include "polarstereo.h"

Transformer::Transformer()
: m_inProj(NULL), m_outProj(NULL), m_errored(false)
{
}

Transformer::~Transformer()
{
   if( m_inProj )
      delete m_inProj;

   if( m_outProj )
      delete m_outProj;
}

bool Transformer::setInput( ProjCode projectionCode )
{
   if( m_inProj )
      delete m_inProj;

   m_inProj = convertProjection( projectionCode );

   return (m_inProj != NULL );
}

bool Transformer::setInput( ProjCode projectionCode, double gctpParameters[15], ProjUnit units, ProjDatum datum )
{
   if( setInput( projectionCode ) )
   {
      m_inProj->setParams( gctpParameters );
      m_inProj->setUnits( units );
      m_inProj->setDatum( datum );
      return true;
   }
   else
      return false;
}

bool Transformer::setInput( Projection &in )
{
   return setInput( in.number(), in.params(), in.units(), in.datum() );
}

Projection* Transformer::input()
{
   return m_inProj;
}

bool Transformer::setOutput( ProjCode projectionCode )
{
   if( m_outProj )
      delete m_outProj;

   m_outProj = convertProjection( projectionCode );

   return (m_outProj != NULL );
}

bool Transformer::setOutput( ProjCode projectionCode, double gctpParameters[15], ProjUnit units, ProjDatum datum )
{
   if( setOutput( projectionCode ) )
   {
      m_outProj->setParams( gctpParameters );
      m_outProj->setUnits( units );
      m_outProj->setDatum( datum );
      return true;
   }
   else
      return false;
}

bool Transformer::setOutput( Projection &out )
{
   return setOutput( out.number(), out.params(), out.units(), out.datum() );
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
      io_coord->units = (ProjUnit)0;
      m_errored = true;
      return;
   }

   Coordinate tmp;
   m_inProj->inverse( io_coord->x, io_coord->y, &(tmp.x), &(tmp.y) );
   m_outProj->forward( tmp.x, tmp.y, &(io_coord->x), &(io_coord->y) );
   io_coord->units = m_outProj->units();

   m_errored = m_inProj->errorOccured() || m_outProj->errorOccured();
}

void Transformer::transformInverse( Coordinate* io_coord )
{
   if( m_inProj == NULL || io_coord->units != m_inProj->units() )
   {
      io_coord->x = 0;
      io_coord->y = 0;
      io_coord->units = (ProjUnit)0;
      m_errored = true;
      return;
   }

   Coordinate tmp(*io_coord);
   m_inProj->inverse( tmp.x, tmp.y, &(io_coord->x), &(io_coord->y) );
   io_coord->units = DEGREE;

   m_errored = m_inProj->errorOccured();
}

void Transformer::transformForward( Coordinate* io_coord )
{
   if( m_outProj == NULL || io_coord->units != DEGREE )
   {
      io_coord->x = 0;
      io_coord->y = 0;
      io_coord->units = (ProjUnit)0;
      m_errored = true;
      return;
   }

   Coordinate tmp(*io_coord);
   m_outProj->forward( tmp.x, tmp.y, &(io_coord->x), &(io_coord->y) );
   io_coord->units = m_outProj->units();

   m_errored = m_outProj->errorOccured();
}

bool Transformer::errored()
{
   return m_errored;
}

Projection* Transformer::convertProjection( ProjCode projectionCode )
{
   Projection* proj = NULL;

   switch( projectionCode )
   {
   case ALBERS:
      proj = new AlbersConEqArea();
      break;
   case LAMCC:
      proj = new LambertCC();
      break;
   case TM:
      proj = new TransverseMercator();
      break;
   case MERCAT:
      proj = new Mercator();
      break;
   case POLYC:
      proj = new Polyconic();
      break;
   case EQUIDC:
      proj = new EquidistantC();
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
   case IMOLL:
      proj = new IntMollweide();
      break;
   case MILLER:
      proj = new Miller();
      break;
   case PS:
      proj = new PolarStereo();
      break;
   }

   return proj;
}

