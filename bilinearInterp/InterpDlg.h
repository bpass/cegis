
#ifndef INTERP_DLG_H
#define INTERP_DLG_H
#include <qdialog.h>
class QLineEdit;
class QPushButton;
class QLabel;
class QGroupBox;
class QString;
class InterpDlg : public QDialog {
	Q_OBJECT
public:
	InterpDlg(QWidget* parent = NULL, const char* name = 0);
	~InterpDlg();
private slots:
	void run(); //perform the interpolation
	void browseImage(); //select input image
	void browsePoints(); //select points file
	
private:
	QLineEdit* imageFileEdit;
	QLineEdit* pointsFileEdit;
	QLineEdit* imageWidthEdit;
	QLineEdit* imageHeightEdit;
	QLineEdit* pixelWidthEdit;
	QLineEdit* pixelHeightEdit;
	QLineEdit* numLayersEdit;
	QLineEdit* ulxEdit;
	QLineEdit* ulyEdit;
	QPushButton* imageFileButton;
	QPushButton* pointsFileButton;
	QPushButton* cancelButton;
	QPushButton* runButton;

	QLabel* imageFileLabel;
	QLabel* pointsFileLabel;
	QLabel* imageWidthLabel;
	QLabel* imageHeightLabel;
	QLabel* pixelWidthLabel;
	QLabel* pixelHeightLabel;
	QLabel* numLayersLabel;
	QLabel* ulxLabel;
	QLabel* ulyLabel;

	QGroupBox* fileGroup;
	QGroupBox* imageGroup;
	QGroupBox* pixelGroup;

	QString* imageFile;
	QString* pointsFile;

};

#endif
