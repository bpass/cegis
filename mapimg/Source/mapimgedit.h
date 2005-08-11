// $Id: mapimgedit.h,v 1.1 2005/08/11 20:21:51 lwoodard Exp $


#ifndef QINFOFRAME_H
#define QINFOFRAME_H

// Qt classes used
#include <QTabWidget>
#include <QString>

#include <QWidget>
#include <QVBoxLayout>
//Added by qt3to4:
#include <QEvent>

// My classes used
class QDmsEdit;
class MapEdit;
class ProjectionEdit;
class RasterInfo;
class QSize;

/*
QInfoFrame is the main class here and it contains one MapEdit and one 
ProjectionEdit. It is a subclass of a QTabWidget for easy switching between the two
forms. The data fields in the QInfoFrame relate to all data entries found in a
".info" file.

The QInfoFrame has two modes, Input and Output. When set as Input the 
QInfoFrame takes on a green tint and displays a lock and a frame it button.
The lock button is used to show that the user isn't expected to edit the input
info. However, if the user knows what he/she is doing then they have the power
to unlock the QInfoFrame and edit all values. The frame it button is used for
assisting the user in finding all values that can be calculated. This button 
has the same functionality in output mode. When set as Output the QInfoFrame 
takes on a red tint and displays a copy button along with the frame it 
button. The copy button will copy all values from another QInfoFrame,
presumably one set as input. The purpose is if the user wants to merely tweak
the current values of a loaded map.
*/
class QInfoFrame : public QTabWidget
{
	Q_OBJECT

public:
	QInfoFrame( QWidget* parent = 0, const char* name = 0);
	~QInfoFrame();

	void reset();
	void fixWidth( uint w );

	void setReadOnly( bool ro );
	void setAsInput();
	void setAsOutput();
	void setPartner( QInfoFrame * i );

	void setInfo( RasterInfo &input ); //made a friend in mapedit and projectionedit
	RasterInfo info();  //made a friend in mapedit and projectionedit

public slots:
	//copy() copies all non-calculated info from another QInfoFrame
	//if (src) copy from src, else copy from partner
	void copy( QInfoFrame* src = 0 ); //made a friend in mapedit and projectionedit
	void partnerChanged();	//made friend in mapedit

	//Locks the info editor and emits the locked() signal if on == TRUE
	void lock( bool on, bool saveFile = true );
	bool frame();
	void getFill();

signals:
	void locked();
	void resetMapData();
	void resetProjectionData();
	void setMapTabWidth( uint w );
	void setGctpTabWidth( uint w );
	void setMapTabRO( bool ro );
	void setGctpTabRO( bool ro );
	void setMapTabsetAsInput();
	void setGctpTabsetAsInput();
	void setMapTabsetAsOutput();
	void setGctpTabsetAsOutput();
	void mapTabLock( bool on );
	void gctpTabLock( bool on );
	void mapTabFrame( int rows, int cols, QString ul_X, QString ul_Y );
	void gctpTabFrame( QString projName );
	void mapTabgetFill( QString fillString );



private:
	QString     xmlName;
	bool        locking;
	QInfoFrame  *partner;

	MapEdit     *mapTab;
	ProjectionEdit    *gctpTab;

};


#endif