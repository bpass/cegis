// $Id: gctpdiffform.cpp,v 1.3 2005/08/29 22:13:47 rbuehler Exp $

#include "gctpdiffform.h"

#include "rasterdropframe.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

GctpDiffForm::GctpDiffForm( QWidget *parent, Qt::WFlags flags )
: QDialog( parent, flags )
{
   inputAcceptor = new RasterDropFrame();
   inputAcceptor->setFrameStyle( QFrame::Sunken | QFrame::WinPanel );
   
   inputLabel = new QLabel;
   
   QVBoxLayout *inputLayout = new QVBoxLayout;
   inputAcceptor->setLayout( inputLayout );
   
   inputButton = new QPushButton( "Browse...", this );

   outputAcceptor = new RasterDropFrame();
   outputAcceptor->setFrameStyle( QFrame::Sunken | QFrame::WinPanel );

   outputLabel = new QLabel;

   QVBoxLayout *outputLayout = new QVBoxLayout;
   outputAcceptor->setLayout( outputLayout );
   
   outputButton = new QPushButton( "Browse...", this );

   mainLayout = new QGridLayout;
   // addWidget(QWidget, row, column, height, width)
   mainLayout->addWidget( inputAcceptor, 0, 0, 1, 1 );
   mainLayout->addWidget( inputButton, 1, 0, 1, 1 );
   mainLayout->addWidget( outputAcceptor, 0, 2, 1, 1 );
   mainLayout->addWidget( inputButton, 1, 2, 1, 1 );
   mainLayout->addWidget( inputButton, 3, 0, 1, 3 );

   setLayout(mainLayout);
}
