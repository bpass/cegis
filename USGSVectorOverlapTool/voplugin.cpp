/**
 * @file voplugin.cpp
 * $Id
 */

// QGIS headers
#include <qgisapp.h>
#include <qgisplugin.h>
#include <qstring.h>

// xpm for creating the toolbar icon
#include "icon.xpm"

#include "voplugin.h"
#include "voplugingui.h"

// Variables for the information needed by QGIS for every plugin
namespace
{
    const char * ident_ = "$Id";
    const char * name_ = "USGS Vector Overlap Tool Plug-in";
    const char * description_ = "Analyze two vector layers to find "
                                "their points of intersection";
    const char * version_ = "0.01";
    const QgisPlugin::PLUGINTYPE type_ = QgisPlugin::UI;
}

/** Implementation struct for VOPlugin */
struct VOPlugin::Imp
{
    /** Pointer to the QGIS main application object */
    QgisApp * qgisMainWindow;

    /** Pointer to the QGIS interface object */
    QgisIface * qgisInterface;

    /** Pointer to the QAction for the toolbar */
    QAction * invokeVOPlugin;    

    /** Pointer to the dialog */
    VOPluginGui * dialog;
}; // VOPlugin::Imp


VOPlugin::VOPlugin(QgisApp * qgis, QgisIface * qi)
    : QgisPlugin(name_, description_, version_, type_),
      imp(new VOPlugin::Imp)
{
    imp->qgisMainWindow = qgis;
    imp->qgisInterface = qi;
} // VOPlugin::VOPlugin(QGisApp * qgis, QgisIface * qi)


void VOPlugin::initGui()
{
    // add a button on the plugins toolbar to invoke our dialog
    imp->invokeVOPlugin =
        new QAction(QIconSet(icon),
                    "USGS Vector Overlap Tool",
                    QString::null,
                    this);
    Q_CHECK_PTR(imp->invokeVOPlugin);
    imp->qgisInterface->addToolBarIcon(imp->invokeVOPlugin);
    connect(imp->invokeVOPlugin, SIGNAL(activated()), this, SLOT(run()));

    // add the dialog
    imp->dialog = new VOPluginGui(imp->qgisMainWindow, 
                                  "USGSVectorOverlapTool");
} // VOPlugin::initGui()


void VOPlugin::run()
{
    imp->dialog->show();
} // VOPlugin::run()


/**
 * Required extern functions needed  for every plugin
 * These functions can be called prior to creating an instance
 * of the plugin class
 */
/*@{*/
/**
 * Class factory to return a new instance of the plugin class
 * @return a pointer to our newly created plugin object
 */
extern "C" QgisPlugin * classFactory(QgisApp * qgis, QgisIface * qI)
{
    return new VOPlugin(qgis, qI);
}

/** @return the name of the plugin */
extern "C" QString name()
{
    return name_;
}

/** @return the description */
extern "C" QString description()
{
    return description_;
}

/** return the type (a UI plugin) */
extern "C" int type()
{
    return type_;
}

/** @return the version number for the plugin */
extern "C" QString version()
{
    return version_;
}

/** Delete ourself */
extern "C" void unload(QgisPlugin * p)
{
    delete p;
}
/*@}*/

