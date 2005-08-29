// $Id: rasterdropframe.h,v 1.1 2005/08/29 22:14:15 rbuehler Exp $

#include "rasterinfo.h"

#include <QFrame>

class QDragEnterEvent;
class QDropEvent;

class RasterDropFrame : public QFrame
{
   Q_OBJECT

public:
   RasterDropFrame( QWidget *parent=0, Qt::WFlags flags=0);

   bool hasRaster() const;
   RasterInfo raster() const {return info;}

signals:
   void rasterAccepted(bool);

protected:
	void dragEnterEvent( QDragEnterEvent *evt );
	void dropEvent( QDropEvent *evt );

private:
   RasterInfo info;
};
