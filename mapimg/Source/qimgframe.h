// $Id: qimgframe.h,v 1.5 2005/09/28 20:24:28 lwoodard Exp $


#ifndef QIMGFRAME_H
#define QIMGFRAME_H

#include <QFrame>
#include <QLabel>
#include <QPixmap>
//Added by qt3to4:
#include <QShowEvent>
#include <QResizeEvent>

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
   QImgFrame( QWidget* parent = 0 );
   ~QImgFrame();

   const QPixmap *pixmap() const;
   void setPixmap( const QPixmap &);
   void loadImg( const QString &fileName, bool reload = false );

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
