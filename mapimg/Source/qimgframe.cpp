// $Id: qimgframe.cpp,v 1.5 2005/09/28 20:24:28 lwoodard Exp $


#include "qimgframe.h"
#include "qimgpainter.h"
//Added by qt3to4:
#include <QPixmap>
#include <QLabel>
#include <QShowEvent>
#include <QFrame>
#include <QResizeEvent>
#include "mapimgpalette.h"

QImgFrame::QImgFrame( QWidget* parent )
: QFrame( parent)
{
	QPalette palette;
	palette.setColor( backgroundRole(), IMGCOLOR );
	setPalette( palette );

   pixmapLabel	= new QLabel( this );
   pixmapLabel->setPixmap( QPixmap(1,1) );
   pixmapLabel->move( contentsRect().topLeft() );
   pixmapLabel->setScaledContents( TRUE );

   imgPainter = new QImgPainter;
   imgName = "";
   newFile = false;
}


QImgFrame::~QImgFrame() 
{
   delete imgPainter;
}


void QImgFrame::setPixmap( const QPixmap &pixmap)
{
   imgName = "";
   pixmapLabel->setPixmap(pixmap);
//   pixmapLabel->pixmap()->setOptimization(QPixmap::BestOptim);
   resizeEvent(NULL);
}


const QPixmap *QImgFrame::pixmap() const
{
   return pixmapLabel->pixmap();
}


void QImgFrame::loadImg( const QString &fileName, bool reload )
{
   newFile = ( imgName != fileName ) || reload;
   if( newFile )
   {
      imgName = fileName;
      if( !isHidden() )
      {
         imgPainter->setFile(imgName);
         resample();
         resizeEvent(NULL);
         newFile = false;
      }
   }
}


void QImgFrame::resample()
{
   if( imgName != "" )
      pixmapLabel->setPixmap( imgPainter->getSample(contentsRect().size() ) );
}


void QImgFrame::resizeEvent( QResizeEvent * )
{
   QSize newS(pixmapLabel->pixmap()->size());
   newS.scale(contentsRect().size(), Qt::KeepAspectRatio);
   QRect newG(QPoint(0,0), newS);
   newG.moveCenter(contentsRect().center());
   pixmapLabel->setGeometry(newG);
}


void QImgFrame::showEvent( QShowEvent * )
{
   if( newFile )
   {
      imgPainter->setFile(imgName);
      resample();
      resizeEvent(NULL);
      newFile = false;
   }
}

