/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

#include "version.h"



void aboutForm::init()
{
    QString version = "<i>MapIMG</i> v",
            major_version = "",
            minor_version = "",
            revision_version = "";

    major_version.setNum( MAJOR_VER );
    version += major_version;
    version += ".";
    minor_version.setNum( MINOR_VER );
    version += minor_version;
    version += ".";
    revision_version.setNum( REVISION_VER );
    version += revision_version;

    lblVersion->setText( version );
}


void aboutForm::mousePressEvent(QMouseEvent *e)
{
   QWidget::mousePressEvent(e);

   accept();
}
