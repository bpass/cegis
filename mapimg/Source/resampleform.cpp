// $Id: resampleform.cpp,v 1.5 2005/08/15 19:51:40 lwoodard Exp $


/****************************************************************************
** Form implementation generated from reading ui file 'resampleform.ui'
**
** Created: Thu Jan 20 10:21:37 2005
**      by: The User Interface Compiler (Id: qt/main.cpp   3.3.3   edited Nov 24 2003)
****************************************************************************/

#include "resampleform.h"

#include <QVariant>
#include <QPushButton>
//#include <q3groupbox.h>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <q3buttongroup.h>
#include <QRadioButton>
#include <QLineEdit>
//#include <q3listbox.h>
#include <QList>
#include <QListWidget>
#include <QLayout>
#include <QToolTip>
#include <QWhatsThis>
#include <QEvent>
#include <QValidator>
#include <QMessageBox>
#include <QSlider>
#include <q3hbox.h>
//Added by qt3to4:
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <Qt>
#include <Q3IconView>

#include "mapimgform.h"
#include "mapimgvalidator.h"
#include "mapimgpalette.h"
#include "rasterinfo.h"
#include "imgio.h"

/*
*  Constructs a ResampleForm as a child of 'parent', with the
*  name 'name' and widget flags set to 'f'.
*
*  The dialog will by default be modeless, unless you set 'modal' to
*  TRUE to construct a modal dialog.
*/
ResampleForm::ResampleForm( RasterInfo input, RasterInfo output, QWidget* parent, const char* name, bool modal, Qt::WFlags fl )
: QDialog( parent, name, modal, fl )
{
	if ( !name )
		setName( "ResampleForm" );

	setPalette( RESAMPLEFORM_COLOR );

	bytesPerRow = (input.cols() * (input.bitCount() / 8));

	ResampleFormLayout = new QVBoxLayout( this, 11, 6, "ResampleFormLayout");

	inputLayout = new QVBoxLayout( 0, 0, 6, "inputLayout"); 

	resampleBox = new QGroupBox( this, "resampleBox" );		/**3**/
	//   resampleBox->setColumnLayout(0, Qt::Vertical );
	resampleBox->layout()->setSpacing( 6 );
	resampleBox->layout()->setMargin( 11 );
	resampleBoxLayout = new QVBoxLayout( resampleBox->layout() );
	resampleBoxLayout->setAlignment( Qt::AlignTop );

	resampleCombo = new QComboBox( FALSE, resampleBox, "resampleCombo" );
	resampleCombo->setMinimumSize( QSize( 125, 0 ) );
	resampleCombo->installEventFilter( this );
	resampleBoxLayout->addWidget( resampleCombo );

	categoricalLayout = new QHBoxLayout( resampleBoxLayout );
	catconLabel = new QLabel( "", resampleBox, "catconLabel" );
	conRadio = new QRadioButton( "Continuous Data", resampleBox, "conRadio" );
	catRadio = new QRadioButton( "Categorical Data", resampleBox, "catRadio" );
	/*   catconButtonGroup = new Q3ButtonGroup( resampleBox, "catconButtonGroup" );*/
	catconButtonGroup = new QGroupBox( resampleBox, "catconButtonGroup" );
	catconButtonGroup->hide();
	/*catconButtonGroup->insert( catRadio );
	catconButtonGroup->insert( conRadio );*/
	QHBoxLayout *catconLayout = new QHBoxLayout( catconButtonGroup );
	catconLayout->addWidget( catRadio );
	catconLayout->addWidget( conRadio );
	//  catconButtonGroup->setLayout( catconLayout );
	categoricalLayout->addWidget( catconLabel );
	categoricalLayout->addWidget( conRadio );
	categoricalLayout->addWidget( catRadio );

	inputLayout->addWidget( resampleBox );

	ignoreBox = new QGroupBox( this, "ignoreBox" );		/**3**/
	//   ignoreBox->setColumnLayout(0, Qt::Vertical );
	ignoreBox->layout()->setSpacing( 6 );
	ignoreBox->layout()->setMargin( 11 );
	ignoreBoxLayout = new QHBoxLayout( ignoreBox->layout() );
	ignoreBoxLayout->setAlignment( Qt::AlignTop );

	ignoreLayout = new QVBoxLayout( 0, 0, 6, "ignoreLayout");

	ignoreLabel = new QLabel( "Ignore values cannot be used if an output \"No Data \nValue\" is not provided.", ignoreBox, "ignoreLabel" );
	ignoreLabel->hide();
	ignoreBoxLayout->addWidget( ignoreLabel );


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

	/*ignoreListBox = new Q3ListBox( ignoreBox, "ignoreListBox" );*/
	ignoreListBox = new QListWidget( ignoreBox );
	//   QListWidgetItem* ignoreListItem = new QListWidgetItem;

	ignoreListBox->setMinimumSize( QSize( 125, 0 ) );
	ignoreListBox->installEventFilter( this );
	ignoreBoxLayout->addWidget( ignoreListBox );
	inputLayout->addWidget( ignoreBox );

	if( !((mapimgForm*)parent)->allowIgnoreValues() )
	{
		ignoreLabel->show();

		ignoreEdit->hide();
		newButton->hide();
		delButton->hide();
		ignoreListBox->hide();
	}

	ResampleFormLayout->addLayout( inputLayout );

	memoryBox = new QGroupBox( this, "memoryBox" );		/**3**/
	//   memoryBox->setColumnLayout(0, Qt::Vertical );
	memoryBox->layout()->setSpacing( 6 );
	memoryBox->layout()->setMargin( 11 );
	memoryBoxLayout = new QVBoxLayout( memoryBox->layout() );
	memoryBoxLayout->setAlignment( Qt::AlignHCenter | Qt::AlignTop );

	memoryLabelResetLayout = new QHBoxLayout();

	memoryLabel = new QLabel( memoryBox, "memoryLabel" );
	memoryLabel->setAlignment( Qt::AlignCenter );

	memoryResetButton = new QPushButton( "Default", memoryBox, "memoryResetButton" );
	QFontMetrics metrics( memoryResetButton->font() );
	memoryResetButton->setMaximumWidth( metrics.width(memoryResetButton->text()) + (metrics.maxWidth()) );
	memoryResetButton->setMaximumHeight( (int)(1.5*metrics.height()) );

	memoryResetButton->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum ) );

	//DEFAULT_Max_Data_Element_Count is defined in imgio.h
	//minimum computed as the ratio of output to input plus 2% of the input
	memoryAllocation = new QSlider( (int)((output.pixelSize()/input.pixelSize()) + 0.02*(float)input.rows()), //min
		input.rows(),                           //max
		10,                                     //step size
		DEFAULT_Max_Data_Element_Count,         //default
		Qt::Horizontal,                         //orientation
		memoryBox,                              //parent
		"memoryAllocationSlider" );             //name
	defaultMemory = memoryAllocation->value();

	memoryLabelResetLayout->addWidget( memoryLabel );
	memoryLabelResetLayout->addWidget( memoryResetButton );
	memoryBoxLayout->addLayout( memoryLabelResetLayout );
	memoryBoxLayout->addWidget( memoryAllocation );
	inputLayout->addWidget( memoryBox );

	okLayout = new QHBoxLayout( 0, 0, 6, "okLayout");
	okSpacer = new QSpacerItem( 141, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
	okLayout->addItem( okSpacer );

	okButton = new QPushButton( this, "okButton" );
	okButton->setAutoDefault( false );
	okLayout->addWidget( okButton );

	cancelButton = new QPushButton( this, "cancelButton" );
	cancelButton->setAutoDefault( false );
	cancelButton->setAccel( Qt::Key_Escape );
	okLayout->addWidget( cancelButton );
	ResampleFormLayout->addLayout( okLayout );
	languageChange();
	resize( QSize(300, 218).expandedTo(minimumSizeHint()) );
	//   clearWState( WState_Polished );


	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( resampleCombo, SIGNAL( activated(const QString&) ), this, SLOT( rcodeChanged(const QString&) ) );
	connect( ignoreEdit, SIGNAL( returnPressed() ), this, SLOT( newVal() ) );
	connect( ignoreEdit, SIGNAL( returnPressed() ), newButton, SLOT( animateClick() ) );
	connect( newButton, SIGNAL( clicked() ), this, SLOT( newVal() ) );
	connect( delButton, SIGNAL( clicked() ), this, SLOT( delVal() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( memoryAllocation, SIGNAL( valueChanged( int ) ), this, SLOT( updateMemoryAllocation() ) );
	connect( memoryResetButton, SIGNAL( clicked() ), this, SLOT( resetMemory() ) );


	canceled = false;
	ilist.clear();
	rcodeChanged( resampleCombo->currentText() );
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
	resampleCombo->insertItem( tr( "Sum" ) );
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
	memoryBox->setTitle( tr( "Memory Allocation" ) );
	okButton->setText( tr( "Ok" ) );
	cancelButton->setText( tr( "Cancel" ) );

	updateMemoryAllocation();
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


void ResampleForm::rcodeChanged( const QString &rcode )
{
	bool showLabel;
	if( rcode.startsWith( "Nearest" ) )
	{
		catconLabel->setText( "Categorical and Continuous use same algorithm." );
		showLabel = true;
	}
	else if( rcode == "Sum" )
	{
		catconLabel->setText( "Sum only valid for Continuous data." );
		showLabel = true;
		conRadio->setChecked(true);
	}
	else if( rcode == "Mean" )
	{
		catconLabel->setText( "Mean only defined for Continuous data." );
		showLabel = true;
	}
	else if( rcode == "Median" )
	{
		catconLabel->setText( "Median only defined for Continuous data." );
		showLabel = true;
		conRadio->setChecked(true);
	}
	else if( rcode == "Mode" )
	{
		catconLabel->setText( "Categorical and Continuous use same algorithm." );
		showLabel = true;
	}
	else if( rcode == "Min" )
	{
		catconLabel->setText( "Categorical and Continuous use same algorithm." );
		showLabel = true;
	}
	else if( rcode == "Max" )
	{
		catconLabel->setText( "Categorical and Continuous use same algorithm." );
		showLabel = true;
	}
	else
	{
		catconLabel->setText( "Unknown resample method selected." );
		showLabel = true;
	}

	catconLabel->setShown( showLabel );
	catRadio->setShown( !showLabel );
	conRadio->setShown( !showLabel );
}


void ResampleForm::updateMemoryAllocation()
{
	QString labelText = QString("%1 lines / ").arg( memoryAllocation->value() );

	float bytes = memoryAllocation->value()*bytesPerRow;


	if( bytes >= 1000 && bytes < 1000000 )
		labelText += QString( "%2 KB" ).arg( bytes/1000 );
	else if( bytes >= 1000000 && bytes < 1000000000 )
		labelText += QString( "%2 MB" ).arg( bytes/1000000 );
	else if( bytes >= 1000000000 )
		labelText += QString( "%2 GB" ).arg( bytes/1000000000 );
	else
		labelText += QString( "%2 B" ).arg( bytes );


	memoryLabel->setText( tr( labelText ) );
	return;
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

		//      if(  ilist.contains( i ) < 1 )
		if( !ilist.contains( i ) )
		{
			ilist.append( i );
			/*         ignoreListBox->insertItem( QString::number( i, 'f', decimals ) );*/
			ignoreListBox->addItem( QString::number( i, 'f', decimals ) );
		}
		else
		{
			int k = 0;
			QList<QListWidgetItem*> ignoreListItem = ignoreListBox->findItems( QString::number( i, 'f', decimals ), Qt::MatchExactly );
			while( !ignoreListItem.at( k )!= NULL )
			{
				ignoreListBox->setItemSelected( ignoreListItem.at( k ), TRUE );
				k++;
			}
		}

		if( ignoreListBox->count() > 0 )
			delButton->setDisabled(false);
	}

	ignoreEdit->selectAll();
}


void ResampleForm::delVal()
{
	int i = ignoreListBox->currentRow();

	if( i != -1 )
	{
		ilist.removeAt( ignoreListBox->item( i )->text().toDouble() );
		QListWidgetItem *removed = ignoreListBox->takeItem( i );
		delete removed;
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
	{
		/****/		QListWidgetItem* ignoreItem = new QListWidgetItem;
		/****/		ignoreItem = ignoreListBox->item( i );
		ivals[i] =  ignoreItem->text().toDouble();
	}

	ResampleInfo r;
	r.setResampleCode( resampleCombo->currentText() );
	r.setIsCategorical( catRadio->isChecked() );
	r.setIgnoreList( sz, ivals );
	r.setCacheLineCount( memoryAllocation->value() );

	return r;
}

void ResampleForm::resetMemory()
{
	memoryAllocation->setValue( defaultMemory );
	return;
}
