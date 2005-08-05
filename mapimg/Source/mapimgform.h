// $Id: mapimgform.h,v 1.2 2005/08/05 16:01:59 lwoodard Exp $


/****************************************************************************
** Form interface generated from reading ui file 'mapimgform.ui'
**
** Created: Thu Jul 29 12:27:35 2004
**      by: The User Interface Compiler (Id: qt/main.cpp   3.3.3   edited Nov 24 2003)
****************************************************************************/

#ifndef mapimgFORM_H
#define mapimgFORM_H

#include <qvariant.h>
#include <q3mainwindow.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <Q3ActionGroup>
#include <QGridLayout>
#include <QEvent>
#include <QHBoxLayout>
#include <QDropEvent>
#include <Q3PopupMenu>
#include <QDragEnterEvent>

class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class Q3ActionGroup;
class Q3PopupMenu;
class Q3ToolBar;
class QToolButton;
class Q3WidgetStack;
class QImgFrame;
class QInfoFrame;
class RasterInfo;

class mapimgForm : public Q3MainWindow
{
   Q_OBJECT

public:
   mapimgForm( QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = Qt::WType_TopLevel );
   ~mapimgForm();

   void dragEnterEvent( QDragEnterEvent *evt );
   void dropEvent( QDropEvent *evt );
   bool openFile( QString inFile );
   QString dataType() const;
   bool allowIgnoreValues() const;

protected:
   QHBoxLayout    *formLayout;
   bool eventFilter( QObject* object, QEvent* event );

protected slots:
   void inOpenClicked();
   void inSaveClicked();
   void previewClicked( bool on = true );
   void previewInput( bool on = true );
   void previewOutput( bool on = true );
   bool previewProjection();
   void outSaveClicked();
   void editAuthor();
   void aboutClicked();
   void showAboutQt(){QMessageBox::aboutQt( this );}

   //Tools
   void launchWebTool( const QString& url );
   void webDSSClicked();

private:
   QMenuBar       *menuBar;
   Q3ToolBar       *toolBar;

   Q3PopupMenu     *File;
   QAction        *exitAction;
   QAction        *bigAction;

   Q3PopupMenu     *Options;
   QAction        *authAction;

   QAction        *inInfoAction;
   QAction        *inOpenAction;
   QAction        *inSaveAction;
   QInfoFrame     *inInfoFrame;

   Q3PopupMenu     *Preview;
   QAction        *viewShowAction;
   QAction        *viewResampleAction;
   QAction        *previewProjAction;
   QImgFrame      *imgFrame;

   QAction        *outInfoAction;
   QAction        *outSaveAction;
   QInfoFrame     *outInfoFrame;

   QToolButton    *viewShowButton;
   Q3PopupMenu     *viewShowPopup;
   Q3ActionGroup   *prevGroup;
   QAction        *prevInput;
   QAction        *prevOutput;

   Q3PopupMenu     *Tools;
   QAction        *webDSS;

   Q3PopupMenu     *Help;
   QAction        *aboutAction;
   QAction        *aboutQtAction;

   QString inPath;
   QString outPath;
   QString authName;
   QString authCompany;
   QString authEmail;

   QString imgName;
   bool imgSet; //Has the user opened a .img file?
   //Used to determine wether or not to save .info files when the user says to
   bool newInfo; //Is the user generating a new .xml file for the .img?

   bool ignorePreviewSignals; //Are actions being automated on/off?
   QAction *prevLast; //What was the user looking at when imgFrame->hide()
};

#endif // mapimgFORM_H
