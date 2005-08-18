#ifndef	MAPEDIT_H
#define MAPEDIT_H

#include <QCheckBox>
#include <QComboBox>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

#include "mapimgedit.h"	//QInfoFrame class

class RasterInfo;
class mapimgForm;

/*
MapEdit is the first of two tabs made for use in the QInfoFrame. It uses
QLineEdits, QSpinBoxes, and QComboBoxes to provide the user with methods to
enter the values that would be found in a mapimg ".info" file. The MapEdit
only contains those parameters that are descriptive of the map and not the
projection. (ie. Size, Location, Resoultion, Scale)

The MapEdit also has three buttons which are connected to by the QInfoFrame.
See the QInfoFrame class for a description of their purpose.

By subclassing the QScrollARea the user is able to have greater freedom in
resizing the application and it leaves unlimited room for adding parameters.
*/
class MapEdit : public QScrollArea	
{
	Q_OBJECT

public:
	MapEdit( QWidget* parent = 0 );
	~MapEdit();
	
		//Let be public due to conflicts
	QPushButton *lockButton;

	friend void QInfoFrame::copy( QInfoFrame *src );
	friend void QInfoFrame::lock( bool on, bool saveFile );
	friend void QInfoFrame::partnerChanged();
	friend void QInfoFrame::setInfo( RasterInfo &input );
	friend RasterInfo QInfoFrame::info();

	void reset();
	void setRO( bool ro );
	void setAsInput();
	void setAsOutput();
	void setFrameInfo( int rows, int cols, QString ul_X, QString ul_Y );
	void getFill( QString fillString );

signals:
	void copyButtonClicked();
	void lockButtonToggled();
	void frameButtonClicked();
	void fillButtonClicked();
	void dataComboActivated( int );
	void fillEditChanged( const QString & );
	void noDataEditChanged( const QString & );

public slots:
	void pixelChange(int);
	void dataChange(const QString&);
	void fillCheckToggled( bool state );
	void noDataCheckToggled( bool state );

protected:
	void appearanceSetup();
	void generateWidgets();
	void polishWidgets();

private:
	bool eventFilter( QObject* object, QEvent* event );

	//appearanceSetup() objects
	QWidget	*contents;	
	QVBoxLayout *mainLayout;

	//generateWidgets() titleBox objects
	QHBoxLayout *titleBox;
	QPushButton *copyButton;
	QLabel		*mapLabel;
	QPushButton *frameButton;

	//generateWidgets() fileBox objects
	QVBoxLayout *fileBox;	
	QLabel		*fileNameLabel;
	QLineEdit   *fileEdit;

	//generateWidgets() rowcolBox objects
	QVBoxLayout	*rowcolBox;	
	QLabel		*rowsOfPixelsLabel;
	QSpinBox    *rowSpin;
	QLabel		*colsOfPixelsLabel;
	QSpinBox    *colSpin;

	//generateWidgets() comboBox objects
	QVBoxLayout *comboBox;
	QLabel		*unitLabel;
	QComboBox   *unitCombo;
	QLabel		*spheroidLabel;
	QComboBox   *spheroidCombo;
	QLabel		*pixelSizeLabel;
	QComboBox   *pixelCombo;
	QLineEdit   *pixelEdit;

	//generateWidgets() ulBox layout
	QVBoxLayout	*ulBox;	
	QLabel		*ulLonLabel;
	QLineEdit   *ulLonEdit;
	QLabel		*ulLatLabel;
	QLineEdit   *ulLatEdit;

	//generateWidgets() dataBox layout
	QVBoxLayout	*dataBox;	
	QLabel		*pixelDataTypeLabel;
	QComboBox   *dataCombo;
	QLabel		*fillValueLabel;
	QLabel		*noDataValueLabel;	//Seen after fillBox

	//generateWidgets() fillBoxLayout -> dataBoxLayout
	QHBoxLayout *fillBox;
	QLineEdit   *fillEdit;
	QPushButton *fillButton;
	QLineEdit   *noDataEdit;

	//generateWidgets() noDataBox ->dataBoxLayout
	QHBoxLayout	*noDataBox;
	QCheckBox   *hasFillCheck;
	QCheckBox   *hasNoDataCheck;

	//Initialized in constructor
	QString     lastFillValue;
	QString     lastNoDataValue;

	QString     authName;
	QString     authCompany;
	QString     authEmail;

};

#endif MAPEDIT_H