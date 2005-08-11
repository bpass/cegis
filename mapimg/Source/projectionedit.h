#ifndef PROJECTIONEDIT_H
#define PROJECTIONEDIT_H

#include <QScrollArea>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QEvent>

#include "mapimgedit.h"

class QGctpBox;
class QInfoFrame;
class QRasterInfo;
/*
ProjectionEdit is the second of two tabs made for use in the QInfoFrame. It uses
QLineEdits, QSpinBoxes, and QComboBoxes to provide the user with methods to
enter the values that would be found in a mapimg ".info" file. The ProjectionEdit
only contains the projection parameters. (ie. Projection Name, 15 GCTP Params)

The ProjectionEdit also has two buttons which are connected to by the QInfoFrame.
See the QInfoFrame class for a description of their purpose.

By subclassing the QScrollARea the user is able to have greater freedom in
resizing the application and it leaves unlimited room for adding parameters.
*/
class ProjectionEdit : public QScrollArea
{
	Q_OBJECT

public:
	ProjectionEdit( QWidget* parent = 0 );
	~ProjectionEdit();

	QSize sizeHint() const;

	friend void QInfoFrame::copy( QInfoFrame *src );
	friend void QInfoFrame::setInfo( RasterInfo &input );
	friend RasterInfo QInfoFrame::info();

signals:
	void copyButtonClicked();
	void lockButtonToggled();

public slots:
	void projChange();
	void reset();
	/*void fixWidth( uint w );*/
	void setRO( bool ro );
	void setAsInput();
	void setAsOutput();
	void lock( bool on );
	void setFrameInfo( QString projName );

protected:
	void appearanceSetup();
	void generateWidgets();
	void polishWidgets();

private:
	
	//appearanceSetup()
	QWidget *contents;	

	//generateWidgets()
	QVBoxLayout *mainLayout;

	//generateWidgets() - titleBox
	QHBoxLayout *titleBox;
	QPushButton *copyButton;
	QPushButton *lockButton;
	QLabel		*gctpLabel;

	//generateWidgets() - projBox
	QVBoxLayout	*projBox;	
	QLabel		*projectionLabel;
	QComboBox   *projCombo;
	QWidget     *badProjBlank;
	QLabel      *badProjLabel;

	//generateWidgets() - zoneBox
	QWidget		*zoneBox;	//Must be widget so it can be hidden
	QVBoxLayout *zoneBoxLayout;
	QLabel		*zoneLabel;
	QSpinBox    *zoneSpin;

	//generateWidgets()
	QGctpBox   **gctpBoxes;
};

#endif PROJECTIONEDIT_H