// $Id: mapimgform.h,v 1.7 2005/08/29 16:48:14 lwoodard Exp $


/****************************************************************************
** Form interface generated from reading ui file 'mapimgform.ui'
**
** Created: Thu Jul 29 12:27:35 2004
**      by: The User Interface Compiler (Id: qt/main.cpp   3.3.3   edited Nov 24 2003)
****************************************************************************/

#ifndef mapimgFORM_H
#define mapimgFORM_H

#include <QActionGroup>
#include <QVariant>
#include <QMainWindow>

#include <QMessageBox>
//Added by qt3to4:
#include <QGridLayout>
#include <QEvent>
#include <QHBoxLayout>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QToolBar>

class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class QToolBar;	
class QToolButton;
class QImgFrame;
class QInfoFrame;
class RasterInfo;

class mapimgForm : public QMainWindow
{
	Q_OBJECT

public:
	mapimgForm( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = Qt::WType_TopLevel );
   void setupContents();
   void createActions();
   void setupMenubar();
   void setupToolbar();
   void makeConnections();
   void loadSettings();
	~mapimgForm();

	bool openFile( QString inFile );


protected:
	bool eventFilter( QObject* object, QEvent* event );

	void dragEnterEvent( QDragEnterEvent *evt );
	void dropEvent( QDropEvent *evt );

	void launchWebTool( const QString& url );

protected slots:
	void editAuthor();

	void inOpenClicked();
	void inSaveClicked();

	void showPreview( bool on = true );
	void showInputPreview( bool on = true );
	void showOutputPreview( bool on = true );
	bool previewProjection();

	void outSaveClicked();

	void aboutClicked();
	void showAboutQt(){QMessageBox::aboutQt( this );}

	void webDSSClicked();

private:

   // Contents
   QHBoxLayout *contentLayout;
	QInfoFrame  *inInfoFrame;
	QImgFrame   *imgFrame;
	QInfoFrame  *outInfoFrame;

	QAction        *exitAction;

	QAction        *authAction;

	QAction        *inInfoAction;
	QAction        *inOpenAction;
	QAction        *inSaveAction;


	QAction        *viewShowAction;
	QAction        *viewResampleAction;
	QAction        *previewProjAction;

	QActionGroup   *prevGroup;
	QAction        *prevInput;
	QAction        *prevOutput;

	QAction        *outInfoAction;
	QAction        *outSaveAction;

	QAction        *webDSSAction;

	QAction        *aboutAction;
	QAction        *aboutQtAction;

	QMenu			*fileMenu;
	QMenu			*optionsMenu;
	QMenu			*previewMenu;
	QMenu       *viewShowPopup;
	QMenu			*toolsMenu;
	QMenu			*helpMenu;
	
	QToolBar       *toolBar;	
	QToolButton    *viewShowButton;


	QString inPath;
	QString outPath;
	QString authName;
	QString authCompany;
	QString authEmail;

   QSize _iconSize;

	QString imgName;
	bool imgSet; //Has the user opened a .img file?
	//Used to determine wether or not to save .info files when the user says to
	bool newInfo; //Is the user generating a new .xml file for the .img?

	bool ignorePreviewSignals; //Are actions being automated on/off?
	QAction *prevLast; //What was the user looking at when imgFrame->hide()
};

#endif // mapimgFORM_H
