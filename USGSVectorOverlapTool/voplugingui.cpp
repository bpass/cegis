/**
 * @file voplugingui.cpp
 * $Id: voplugingui.cpp,v 1.2 2004/12/08 16:18:02 ahartman Exp $
 */

// Qt headers
#include <qlabel.h>
#include <qlayout.h>

#include "voplugingui.h"

VOPluginGui::VOPluginGui(QWidget* parent, const char* name, 
                         bool modal, WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    QLabel* label = new QLabel("Hello World", this);
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(label);    
}

