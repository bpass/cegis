#include "InterpDlg.h"
#include "BilInterpolator.h"
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qstring.h>
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qlayout.h>
#include <qvalidator.h>
#include <qprogressdialog.h>
InterpDlg::InterpDlg(QWidget* parent, const char* name) 
	: QDialog(parent, name)
{
	setCaption(tr("Bilinear Interpolation Program"));
	
	imageFile = NULL;
	pointsFile = NULL;
	//create widget groupings
	fileGroup = new QGroupBox(2, Qt::Horizontal, "Input Files", this, NULL);
	imageGroup = new QGroupBox(3, Qt::Horizontal, "Image Information", this, NULL);
	pixelGroup = new QGroupBox(2, Qt::Horizontal, "Pixel Information", this, NULL);

	//file group
	imageFileLabel = new QLabel("Image File", fileGroup);
	fileGroup->addSpace(0);
	imageFileEdit = new QLineEdit(fileGroup);
	imageFileButton = new QPushButton("Browse", fileGroup);
	pointsFileLabel = new QLabel("Points File", fileGroup);
	fileGroup->addSpace(0);
	pointsFileEdit = new QLineEdit(fileGroup);
	pointsFileButton = new QPushButton("Browse", fileGroup);

	//image info group
	imageWidthLabel = new QLabel("Columns", imageGroup);
	imageHeightLabel = new QLabel("Rows", imageGroup);
	numLayersLabel = new QLabel("# Layers", imageGroup);
	imageWidthEdit = new QLineEdit(imageGroup);
	imageWidthEdit->setValidator(new QIntValidator(this));
	imageHeightEdit = new QLineEdit(imageGroup);
	imageHeightEdit->setValidator(new QIntValidator(this));
	numLayersEdit = new QLineEdit(imageGroup);
	numLayersEdit->setValidator(new QIntValidator(this));
	ulxLabel = new QLabel("Ulx", imageGroup);
	ulyLabel = new QLabel("Uly", imageGroup);
	imageGroup->addSpace(0);
	ulxEdit = new QLineEdit(imageGroup);
	ulxEdit->setValidator(new QDoubleValidator(this));
	ulyEdit = new QLineEdit(imageGroup);
	ulyEdit->setValidator(new QDoubleValidator(this));

	//pixel info group
	pixelWidthLabel = new QLabel("Width", pixelGroup);
	pixelHeightLabel = new QLabel("Height", pixelGroup);
	pixelWidthEdit = new QLineEdit(pixelGroup);
	pixelWidthEdit->setValidator(new QDoubleValidator(this));
	pixelHeightEdit = new QLineEdit(pixelGroup);
	pixelHeightEdit->setValidator(new QDoubleValidator(this));

	cancelButton = new QPushButton("Cancel", this);
	runButton = new QPushButton("Run", this);
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	QHBoxLayout* bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(cancelButton);
	bottomLayout->addItem(new QSpacerItem(20,1));
	bottomLayout->addWidget(runButton);
	mainLayout->addWidget(fileGroup);
	mainLayout->addWidget(imageGroup);
	mainLayout->addWidget(pixelGroup);
	mainLayout->addLayout(bottomLayout);
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(runButton, SIGNAL(clicked()), this, SLOT(run()));
	connect(imageFileButton, SIGNAL(clicked()), this, SLOT(browseImage()));
	connect(pointsFileButton, SIGNAL(clicked()), this, SLOT(browsePoints()));
}

InterpDlg::~InterpDlg() {
	if(pointsFile)
		delete pointsFile;
	if(imageFile)
		delete imageFile;
}
void InterpDlg::browseImage() {

	QString tempFile = QFileDialog::getOpenFileName(QString::null, "BSQ Images (*.bsq)", this);
	if(!tempFile.isEmpty()) 
		imageFileEdit->setText(tempFile);
	
}

void InterpDlg::browsePoints() {
	QString tempFile = QFileDialog::getOpenFileName(QString::null, "Point Files (*.dat)", this);

	if(!tempFile.isEmpty()) 
		pointsFileEdit->setText(tempFile);
	
}

void InterpDlg::run() {
	ImageParameters params;
	if(imageFile) 
		delete imageFile;

	if(pointsFile)
		delete pointsFile;
	
	if(imageFileEdit->text().isEmpty()) {
		QMessageBox::critical(this, "Error", "You must specify an image file.");
		return;
	}

	if(pointsFileEdit->text().isEmpty()) {
		QMessageBox::critical(this, "Error", "You must specify a points file.");
		return;
	}

	if(imageWidthEdit->text().isEmpty()) {
		QMessageBox::critical(this, "Error", "You must specify the number of columns.");
		return;
	}

	if(imageHeightEdit->text().isEmpty()) {
		QMessageBox::critical(this, "Error", "You must specify the number of rows.");
		return;
	}

	if(numLayersEdit->text().isEmpty()) {
		QMessageBox::critical(this, "Error", "You must specify the number of layers in the image.");
		return;
	}

	if(ulxEdit->text().isEmpty()) {
		QMessageBox::critical(this, "Error", "You must specify the upper-left X coordinate of the image.");
		return;
	}

	if(ulyEdit->text().isEmpty()) {
		QMessageBox::critical(this, "Error", "You must specify the upper-left Y coordinate of the image.");
		return;
	}

	if(pixelWidthEdit->text().isEmpty()) {
		QMessageBox::critical(this, "Error", "You must specify the width of the image pixels (in meters).");
		return;
	}

	if(pixelHeightEdit->text().isEmpty()) {
		QMessageBox::critical(this, "Error", "You must specify the height of the image pixels (in meters).");
		return;
	}

	imageFile = new QString(imageFileEdit->text());
	pointsFile = new QString(pointsFileEdit->text());
	params.width = imageWidthEdit->text().toLong();
	params.height = imageHeightEdit->text().toLong();
	params.numLayers = numLayersEdit->text().toUInt();
	params.ulx = ulxEdit->text().toDouble();
	params.uly = ulyEdit->text().toDouble();
	params.pixelWidth = pixelWidthEdit->text().toFloat();
	params.pixelHeight = pixelHeightEdit->text().toFloat();

	try {
	//	QProgressDialog pd(this, "Progress", TRUE);
		BilInterpolator* b = NULL;
	//	connect(b, SIGNAL(updateProgress(int progress, int totalSteps)), pd, SLOT(setProgress(int progress, int totalSteps)));
		
		b = new BilInterpolator(imageFile->ascii(), pointsFile->ascii(), params);
		b->saveResults(imageFile->ascii());
		delete b;
	}

	catch(GeneralException e) {
		QMessageBox::critical(this, "Error", QString(e.getMessage()));
		return;
	}
	QMessageBox::information(this, "", "Interpolation Complete!");
}
