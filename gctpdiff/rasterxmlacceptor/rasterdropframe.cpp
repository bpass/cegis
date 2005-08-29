// $Id: rasterdropframe.cpp,v 1.1 2005/08/29 22:14:15 rbuehler Exp $

#include "rasterdropframe.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>

RasterDropFrame::RasterDropFrame( QWidget *parent, Qt::WFlags flags )
: QFrame(parent, flags)
{
   setAcceptDrops(true);
}

bool RasterDropFrame::hasRaster() const
{
   return info.xmlFileName().isEmpty();
}

void RasterDropFrame::dragEnterEvent( QDragEnterEvent *evt )
{
   if ( evt->mimeData()->hasUrls() )
		evt->accept();
}

void RasterDropFrame::dropEvent( QDropEvent *evt )
{
	if ( evt->mimeData()->hasUrls() )
      if( !info.load( evt->mimeData()->urls()[0].toLocalFile() ) )
         info.setFileName(QString());

   emit rasterAccepted(hasRaster());
}
