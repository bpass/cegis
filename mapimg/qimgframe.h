// $Id: qimgframe.h,v 1.1 2005/01/14 16:18:50 rbuehler Exp $


#ifndef QIMGFRAME_H
#define QIMGFRAME_H

#include <qframe.h>
#include <qlabel.h>
#include <qpixmap.h>

class QImgPainter;

/*
  The QImgFrame is a reimplimentation of the QFrame containing
  \	a QLabel. It is useful for loading  images that you want to maintain
  \   their aspect ratio.  The QImgFrame will resize to its layout while
  \   stretching and centering the internal QLabel to fill as much of the
  \   QImgFrame as possible.
  
  To use the QImgFrame in the Qt Designer create a new Custom Widget.
  \	Name the class "QImgFrame", set the header file to the qimgframe.h
  \	and, in the Properties tab, create a new property called "pixmap",
  \   with type Pixmap.
*/


class QImgFrame : public QFrame
{
   Q_OBJECT
   Q_PROPERTY( QPixmap pixmap READ pixmap WRITE setPixmap )
   
public:
   QImgFrame( QWidget* parent = 0, const char* name = 0);
   ~QImgFrame();

   QPixmap *pixmap() const;
   void setPixmap( const QPixmap &);
   void loadImg( QString &fileName, bool reload = false );
   
public slots:
   void resample();

protected:
   void resizeEvent ( QResizeEvent *);
   void showEvent ( QShowEvent *);
   
private:
   QImgPainter *imgPainter;
   QLabel      *pixmapLabel;
   QString     imgName;
   bool        newFile;
};



#endif
