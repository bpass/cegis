// $Id: gctpdiffform.h,v 1.4 2005/08/29 22:13:47 rbuehler Exp $

//////////////////////////////////////////////
////
////  CLASS GctpDiffForm
////
////  Description: The GctpDiffForm is the main window displayed.
////     It displays two drop acceptors to allow for file dropping.
////
//////////////////////////////////////////////

#include <QDialog>

class RasterDropFrame;
class QGridLayout;
class QLabel;
class QPushButton;

class GctpDiffForm : public QDialog
{
   Q_OBJECT

public:
   GctpDiffForm( QWidget *parent=0, Qt::WFlags flags=0 );

protected slots:
   //void inputAccepted(bool);
   //void outputAccepted(bool);

private:
   RasterDropFrame *inputAcceptor;
   QLabel *inputLabel;
   QPushButton *inputButton;

   RasterDropFrame *outputAcceptor;
   QLabel *outputLabel;
   QPushButton *outputButton;

   QPushButton *diffButton;

   QGridLayout *mainLayout;
};
