// $Id: resampleform.cpp,v 1.11 2005/02/25 18:37:49 jtrent Exp $


/****************************************************************************
** Form implementation generated from reading ui file 'resampleform.ui'
**
** Created: Thu Jan 20 10:21:37 2005
**      by: The User Interface Compiler (Id: qt/main.cpp   3.3.3   edited Nov 24 2003)
****************************************************************************/

#include "resampleform.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qevent.h>
#include <qvalidator.h>
#include <qmessagebox.h>

#include "mapimgform.h"
#include "mapimgvalidator.h"         
#include "mapimgpalette.h"

/*
*  Constructs a ResampleForm as a child of 'parent', with the
*  name 'name' and widget flags set to 'f'.
*
*  The dialog will by default be modeless, unless you set 'modal' to
*  TRUE to construct a modal dialog.
*/
ResampleForm::ResampleForm( QWidget* parent, const char* name, bool modal, WFlags fl )
: QDialog( parent, name, modal, fl )
{
   if ( !name )
      setName( "ResampleForm" );

   setPalette( RESAMPLEFORM_COLOR );

   ResampleFormLayout = new QVBoxLayout( this, 11, 6, "ResampleFormLayout");

   inputLayout = new QVBoxLayout( 0, 0, 6, "inputLayout"); 

   resampleBox = new QGroupBox( this, "resampleBox" );
   resampleBox->setColumnLayout(0, Qt::Vertical );
   resampleBox->layout()->setSpacing( 6 );
   resampleBox->layout()->setMargin( 11 );
   resampleBoxLayout = new QVBoxLayout( resampleBox->layout() );
   resampleBoxLayout->setAlignment( Qt::AlignTop );

   resampleCombo = new QComboBox( FALSE, resampleBox, "resampleCombo" );
   resampleCombo->setMinimumSize( QSize( 125, 0 ) );
   resampleCombo->installEventFilter( this );
   resampleBoxLayout->addWidget( resampleCombo );
   inputLayout->addWidget( resampleBox );

   ignoreBox = new QGroupBox( this, "ignoreBox" );
   ignoreBox->setColumnLayout(0, Qt::Vertical );
   ignoreBox->layout()->setSpacing( 6 );
   ignoreBox->layout()->setMargin( 11 );
   ignoreBoxLayout = new QHBoxLayout( ignoreBox->layout() );
   ignoreBoxLayout->setAlignment( Qt::AlignTop );

   ignoreLayout = new QVBoxLayout( 0, 0, 6, "ignoreLayout");

   ignoreEdit = new QLineEdit( ignoreBox, "ignoreEdit" );
   ignoreEdit->setMinimumSize( QSize( 125, 0 ) );
   ignoreEdit->setValidator( new MapimgValidator( ((mapimgForm*)parent)->dataType(), ignoreEdit ) );
   ignoreLayout->addWidget( ignoreEdit );

   newButton = new QPushButton( ignoreBox, "newButton" );
   newButton->setAutoDefault( false );
   ignoreLayout->addWidget( newButton );
   delButton = new QPushButton( ignoreBox, "delButton" );
   delButton->setEnabled( false );
   delButton->setAutoDefault( false );
   ignoreLayout->addWidget( delButton );
   ingoreSpacer = new QSpacerItem( 31, 91, QSizePolicy::Minimum, QSizePolicy::Expanding );
   ignoreLayout->addItem( ingoreSpacer );
   ignoreBoxLayout->addLayout( ignoreLayout );

   ignoreListBox = new QListBox( ignoreBox, "ignoreListBox" );
   ignoreListBox->setMinimumSize( QSize( 125, 0 ) );
   ignoreListBox->installEventFilter( this );
   ignoreBoxLayout->addWidget( ignoreListBox );
   inputLayout->addWidget( ignoreBox );
   ResampleFormLayout->addLayout( inputLayout );

   okLayout = new QHBoxLayout( 0, 0, 6, "okLayout");
   okSpacer = new QSpacerItem( 141, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
   okLayout->addItem( okSpacer );

   okButton = new QPushButton( this, "okButton" );
   okButton->setAutoDefault( false );
   okLayout->addWidget( okButton );

   cancelButton = new QPushButton( this, "cancelButton" );
   cancelButton->setAutoDefault( false );
   cancelButton->setAccel( Key_Escape );
   okLayout->addWidget( cancelButton );
   ResampleFormLayout->addLayout( okLayout );
   languageChange();
   resize( QSize(300, 218).expandedTo(minimumSizeHint()) );
   clearWState( WState_Polished );


   connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
   connect( ignoreEdit, SIGNAL( returnPressed() ), this, SLOT( newVal() ) );
   connect( ignoreEdit, SIGNAL( returnPressed() ), newButton, SLOT( animateClick() ) );
   connect( newButton, SIGNAL( clicked() ), this, SLOT( newVal() ) );
   connect( delButton, SIGNAL( clicked() ), this, SLOT( delVal() ) );
   connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );


   canceled = false;
   ilist.clear();
   rcode = ResampleInfo::NearestNeighbor;
}

/*
*  Destroys the object and frees any allocated resources
*/
ResampleForm::~ResampleForm()
{
   // no need to delete child widgets, Qt does it all for us
}

/*
*  Sets the strings of the subwidgets using the current
*  language.
*/
void ResampleForm::languageChange()
{
   setCaption( tr( "Resample Options" ) );
   resampleBox->setTitle( tr( "Resample Method" ) );
   resampleCombo->clear();
   resampleCombo->insertItem( tr( "Nearest Neighbor" ) );
   resampleCombo->insertItem( tr( "Add" ) );
   resampleCombo->insertItem( tr( "Mean" ) );
   resampleCombo->insertItem( tr( "Median" ) );
   resampleCombo->insertItem( tr( "Mode" ) );
   resampleCombo->insertItem( tr( "Min" ) );
   resampleCombo->insertItem( tr( "Max" ) );
   ignoreBox->setTitle( tr( "Ignore Values" ) );
   ignoreEdit->setText( tr( "0" ) );
   newButton->setText( tr( "New Value -->" ) );
   delButton->setText( tr( "Delete Value <--" ) );
   ignoreListBox->clear();
   okButton->setText( tr( "Ok" ) );
   cancelButton->setText( tr( "Cancel" ) );
}

bool ResampleForm::eventFilter( QObject* object, QEvent* event )
{
   if( (object == ignoreListBox) && (event->type() == QEvent::KeyPress) )
   {
      QKeyEvent* keyEvent = (QKeyEvent*)event;
      if( keyEvent->key() == Qt::Key_Delete )
      {
         delButton->animateClick();
         return true;
      }
      else
      {
         return QDialog::eventFilter( object, event );
      }
   }
   else if( (object == resampleCombo) && (event->type() == QEvent::KeyPress) )
   {
      QKeyEvent* keyEvent = (QKeyEvent*)event;
      if( keyEvent->key() == Qt::Key_Return )
      {
         okButton->animateClick();
         return true;
      }
      else
      {
         return QDialog::eventFilter( object, event );
      }
   }

   return QDialog::eventFilter( object, event );
}


void ResampleForm::ignoreListKeyPress( QKeyEvent* )
{

   delButton->animateClick();

   return;
}

void ResampleForm::newVal()
{
   if( ignoreEdit->hasAcceptableInput() )
   {
      IgnoreValue i = (IgnoreValue)ignoreEdit->text().toDouble();

      int decimals = 6;

      if( ignoreEdit->validator() != NULL )
         decimals = ((MapimgValidator*)ignoreEdit->validator())->decimals();

      if(  ilist.contains( i ) < 1 )
      {
         ilist.append( i );
         ignoreListBox->insertItem( QString::number( i, 'f', decimals ) );
      }
      else
      {
         QListBoxItem* ignoreListItem = ignoreListBox->findItem( QString::number( i, 'f', decimals ), Qt::ExactMatch );
         ignoreListBox->setSelected( ignoreListItem, TRUE );
      }

      if( ignoreListBox->count() > 0 )
         delButton->setDisabled(false);
   }

   ignoreEdit->selectAll();
}


void ResampleForm::delVal()
{
   int i = ignoreListBox->currentItem();

   if( i != -1 )
   {
      ilist.remove( ignoreListBox->text(i).toDouble() );
      ignoreListBox->removeItem( i );
   }

   if( ignoreListBox->count() == 0 )
      delButton->setDisabled(true);
}


void ResampleForm::cancel()
{
   canceled = true;
   close();
}

void ResampleForm::reject()
{
   canceled = true;
   QDialog::reject();
}

ResampleInfo ResampleForm::info()
{
   unsigned int sz = ignoreListBox->count();
   double *ivals = new double [sz];
   for( unsigned int i = 0; i < sz; ++i )
      ivals[i] = ignoreListBox->text(i).toDouble();

   ResampleInfo r;
   r.setResampleCode( resampleCombo->currentText() );
   r.setIgnoreList( sz, ivals );      

   return r;
}
