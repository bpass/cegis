/****************************************************************************
** Form interface generated from reading ui file 'mapimgform.ui'
**
** Created: Thu Oct 7 08:49:09 2004
**      by: The User Interface Compiler ($Id: mapimgform.h,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef MAPIMGFORM_H
#define MAPIMGFORM_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLineEdit;
class QPushButton;
class QComboBox;
class QSpinBox;
class QLabel;
class QFrame;
class QButtonGroup;
class QRadioButton;

class mapimgForm : public QDialog
{
    Q_OBJECT

public:
    mapimgForm( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~mapimgForm();

    QLineEdit* inFilenameEdit;
    QPushButton* infileButton;
    QComboBox* inputDataTypeComboBox;
    QComboBox* inputProjectionComboBox;
    QSpinBox* numRowsSpin;
    QSpinBox* numColsSpin;
    QLabel* inInfoText;
    QLabel* inFilenameLabel;
    QLabel* inputBitageText;
    QLabel* inputProjectionText;
    QLabel* numRowsText;
    QLabel* numColsText;
    QFrame* line7;
    QLabel* outInfoText;
    QLabel* outFilenameLabel;
    QLabel* outputProjectionText;
    QLabel* pixelText;
    QLineEdit* outFilenameEdit;
    QPushButton* outfileButton;
    QComboBox* projectionComboBox;
    QComboBox* pixelComboBox;
    QLineEdit* pixelEdit;
    QLabel* pixsizeLabel;
    QFrame* line5;
    QLabel* fillvalText;
    QSpinBox* fillvalSpin;
    QFrame* line6;
    QButtonGroup* buttonGroup1;
    QRadioButton* clearRadio;
    QRadioButton* keepInputRadio;
    QRadioButton* moveOutputRadio;
    QPushButton* okayButton;
    QPushButton* exitButton;

public slots:
    virtual void executemapimg();
    virtual void comboCheck();
    virtual void pixEditCheck();
    virtual void getInfile();
    virtual void loadInputInfo( QString & s );
    virtual void getOutfile();
    virtual void pixelCheck();
    virtual bool infilenameExist();
    virtual bool outfilenameExist();
    virtual void init();
    virtual void setgctpArray( double * ptr );
    virtual void comboCheckIn();
    virtual void setgctpArrayOut( double * ptr );
    virtual void setInPixel( double pixel );
    virtual void setUTMout( double * ptr, int val );
    virtual void setUTMin( double * ptr, int val );
    virtual void setSPout( int val );
    virtual void infileValid( const QString & infileName );
    virtual void outfileValid( const QString & outfileName );
    virtual void calcDataType();
    virtual void comboCheckData();
    virtual void showAllDataTypes();
    virtual void show8BitDataTypes();
    virtual void show16BitDataTypes();
    virtual void show32BitDataTypes();
    virtual void show64BitDataTypes();
    virtual void getOutputGCTP( double * ptr );
    virtual void getInputGCTP( double * ptr );
    virtual void timerDone();
    virtual void showAboutMapIMG();
    virtual void showAboutQt();

protected:
    int outspunit;
    int oututmzone;
    double inPixelSize;
    double * input_gctparray;
    QString infilepath;
    QString defaultInPath;
    QString defaultOutPath;
    double pixelSize;
    QString outfilepath;
    double * output_gctparray;
    int inutmzone;
    int inspunit;

    virtual void mousePressEvent( QMouseEvent * e );

    QVBoxLayout* mapimgFormLayout;
    QGridLayout* layout19;
    QVBoxLayout* layout16;
    QHBoxLayout* layout42;
    QVBoxLayout* layout15;
    QVBoxLayout* layout29;
    QGridLayout* layout17;
    QVBoxLayout* layout56;
    QVBoxLayout* layout51;
    QVBoxLayout* layout16_2;
    QHBoxLayout* layout46;
    QHBoxLayout* layout15_2;
    QHBoxLayout* layout63;
    QVBoxLayout* buttonGroup1Layout;
    QHBoxLayout* layout15_3;

protected slots:
    virtual void languageChange();

};

#endif // MAPIMGFORM_H
