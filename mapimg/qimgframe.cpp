// $Id: qimgframe.cpp,v 1.2 2005/01/27 18:15:16 jtrent Exp $


#include "qimgframe.h"
#include "qimgpainter.h"


const QColor IMGCOLOR = QColor( 148, 153, 186 );

QImgFrame::QImgFrame( QWidget* parent, const char* name)
   : QFrame( parent, name)
{
   setPaletteBackgroundColor( IMGCOLOR );

   pixmapLabel	= new QLabel( this, "pixmapLabel" );
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
   pixmapLabel->pixmap()->setOptimization(QPixmap::BestOptim);
   resizeEvent(NULL);
}


QPixmap *QImgFrame::pixmap() const
{
   return pixmapLabel->pixmap();
}


void QImgFrame::loadImg( const QString &fileName, bool reload )
{
   newFile = ( imgName != fileName ) || reload;
   if( newFile )
   {
      imgName = fileName;
      if( isShown() )
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
   newS.scale(contentsRect().size(), QSize::ScaleMin);
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