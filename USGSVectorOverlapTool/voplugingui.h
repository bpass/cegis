/**
 * @file voplugingui.h
 * $Id
 */

#include <qdialog.h>

class VOPluginGui : public QDialog
{
    Q_OBJECT ;
public:
    VOPluginGui(QWidget* parent = 0, const char* name = 0, 
                bool modal = false, WFlags fl = 0);
    ~VOPluginGui()
    {}
}; // class VOPluginGui

