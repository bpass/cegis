/**
 * @file voplugingui.h
 * $Id: voplugingui.h,v 1.2 2004/12/08 16:18:02 ahartman Exp $
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

