/**
 * @file voplugingui.cpp
 * $Id
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

