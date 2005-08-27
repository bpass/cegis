// $Id: gctpdiffform.h,v 1.2 2005/08/27 00:30:44 rbuehler Exp $


#include <QDialog>

class QVBoxLayout;
class QPushButton;
class RasterDropAcceptor;
class QLabel;

class GctpDiffForm : public QDialog
{
   Q_OBJECT

public:
   GctpDiffForm();

   QVBoxLayout *mainLayout;

   QPushButton *diffButton;

   RasterDropAcceptor *inputAcceptor;
   QLabel *inputLabel;

   RasterDropAcceptor *outputAcceptor;
   QLabel *outputLabel;
}