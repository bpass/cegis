// $Id: qinfoframe.h,v 1.10 2005/04/07 17:22:05 rbuehler Exp $


#ifndef QINFOFRAME_H
#define QINFOFRAME_H

// Qt classes used
#include <qscrollview.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qtabwidget.h>
#include <qpushbutton.h>
#include <qpopupmenu.h>
#include <qcheckbox.h>

// My classes used
class QDmsEdit;
class QGctpBox;
class RasterInfo;


/*
   QMapTab is the first of two tabs made for use in the QInfoFrame. It uses
QLineEdits, QSpinBoxes, and QComboBoxes to provide the user with methods to
enter the values that would be found in a mapimg ".info" file. The QMapTab
only contains those parameters that are descriptive of the map and not the
projection. (ie. Size, Location, Resoultion, Scale)

   The QMapTab also has three buttons which are connected to by the QInfoFrame.
See the QInfoFrame class for a description of their purpose.

   By subclassing the QScrollView the user is able to have greater freedom in
resizing the application and it leaves unlimited room for adding parameters.
*/
class QMapTab : public QScrollView
{
   Q_OBJECT

public:
   QMapTab( QWidget* parent = 0, const char* name = 0);
   ~QMapTab();

   bool eventFilter( QObject* object, QEvent* event );

   QVBox       *contents;

   QPushButton *copyButton;
   QPushButton *lockButton;
   QPushButton *frameButton;

   QVBox       *fileBox;
   QLineEdit   *fileEdit;

   QVBox       *rowcolBox;
   QSpinBox    *rowSpin;
   QSpinBox    *colSpin;

   QVBox       *comboBox;
   QComboBox   *unitCombo;
   QComboBox   *spheroidCombo;
   QComboBox   *pixelCombo;
   QLineEdit   *pixelEdit;

   QVBox       *ulBox;
   QLineEdit   *ulLatEdit;
   QLineEdit   *ulLonEdit;

   QVBox       *dataBox;
   QComboBox   *dataCombo;
   QLineEdit   *fillEdit;
   QPushButton *fillButton;
   QLineEdit   *noDataEdit;

   QCheckBox   *hasFillCheck;
   QString     lastFillValue;
   QCheckBox   *hasNoDataCheck;
   QString     lastNoDataValue;

   QString     authName;
   QString     authCompany;
   QString     authEmail;

public slots:
   void pixelChange(int);
   void dataChange(const QString&);
   void fillCheckToggled( bool state );
   void noDataCheckToggled( bool state );
};


/*
   QGctpTab is the second of two tabs made for use in the QInfoFrame. It uses
QLineEdits, QSpinBoxes, and QComboBoxes to provide the user with methods to
enter the values that would be found in a mapimg ".info" file. The QGctpTab
only contains the projection parameters. (ie. Projection Name, 15 GCTP Params)

   The QGctpTab also has two buttons which are connected to by the QInfoFrame.
See the QInfoFrame class for a description of their purpose.

   By subclassing the QScrollView the user is able to have greater freedom in
resizing the application and it leaves unlimited room for adding parameters.
*/
class QGctpTab : public QScrollView
{
   Q_OBJECT
   
public:
   QGctpTab( QWidget* parent = 0, const char* name = 0);
   ~QGctpTab();

   QVBox       *contents;

   QPushButton *copyButton;
   QPushButton *lockButton;

   QVBox       *projBox;
   QComboBox   *projCombo;

   QVBox       *zoneBox;
   QSpinBox    *zoneSpin;

   QGctpBox   **gctpBoxes;

public slots:
   void projChange();
};


/*
   QInfoFrame is the main class here and it contains one QMapTab and one 
QGctpTab. It is a subclass of a QTabWidget for easy switching between the two
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

   void setInfo( RasterInfo &input );
   RasterInfo info();
//   static void cleanUp( QLineEdit *lEdit );

   QMapTab     *mapTab;
   QGctpTab    *gctpTab;

public slots:
   //copy() copies all non-calculated info from another QInfoFrame
   //if (src) copy from src, else copy from partner
   void copy( QInfoFrame* src = 0 );
   void partnerChanged();

   //Locks the info editor and emits the locked() signal if on == TRUE
   void lock( bool on, bool saveFile = true );
   bool frame();
   void getFill();

signals:
   void locked();

private:
   QString     xmlName;
   bool        locking;
   QInfoFrame  *partner;
};


#endif


