// $Id: resampleform.h,v 1.10 2007/10/04 12:19:59 dmattli Exp $


/****************************************************************************
** Form interface generated from reading ui file 'resampleform.ui'
**
** Created: Thu Jan 20 10:21:37 2005
**      by: The User Interface Compiler (Id: qt/main.cpp   3.3.3   edited Nov 24 2003)
****************************************************************************/

#ifndef RESAMPLEFORM_H
#define RESAMPLEFORM_H

#include <QVariant>
#include <QDialog>

//Added by qt3to4:
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QListWidget>
#include <QSlider>
#include <QEvent>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include "resampleinfo.h"
#include "rasterinfo.h"

/*
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;	
class QComboBox;
class QLabel;
class QRadioButton;
class QLineEdit;
class QPushButton;
class QEvent;
class QSlider;
class QListWidget;
class QList;
*/

class ResampleForm : public QDialog
{
	Q_OBJECT

public:
	ResampleForm( RasterInfo input, RasterInfo output, QWidget* parent = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
	~ResampleForm();

	ResampleInfo info();
	IgnoreList &ignoreList(){return ilist;}
	bool wasCanceled(){return canceled;}


protected:
	QGroupBox* resampleBox;		
	QComboBox* resampleCombo;
	QLabel* catconLabel;
	QGroupBox* catconButtonGroup;
	QRadioButton* catRadio;
	QRadioButton* conRadio;

	QGroupBox* ignoreBox;		
	QLineEdit* ignoreEdit;
	QPushButton* newButton;
	QPushButton* delButton;
	QListWidget* ignoreListBox;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QLabel* ignoreLabel;

	QVBoxLayout* ResampleFormLayout;
	QHBoxLayout* categoricalLayout;
	QVBoxLayout* inputLayout;
	QVBoxLayout* resampleBoxLayout;
	QHBoxLayout* ignoreBoxLayout;
	QVBoxLayout* ignoreLayout;
	QSpacerItem* ingoreSpacer;
	QHBoxLayout* okLayout;
	QSpacerItem* okSpacer;

	QGroupBox* memoryBox;		
	QSlider* memoryAllocation;
	QPushButton* memoryResetButton;
	QHBoxLayout* memoryLabelResetLayout;
	QVBoxLayout* memoryBoxLayout;
	QLabel* memoryLabel;

	bool eventFilter( QObject* object, QEvent* event );

protected slots:
	virtual void languageChange();
	void rcodeChanged( const QString &rcode );
	void newVal();
	void delVal();
	void cancel();
	void reject();
	void ignoreListKeyPress( QKeyEvent* e );
	void updateMemoryAllocation();
	void resetMemory();

private:
	IgnoreList ilist;
	bool canceled;
	int defaultMemory;

	float bytesPerRow;
};

#endif // RESAMPLEFORM_H
