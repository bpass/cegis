/**
 * @file voplugin.h
 * $Id: voplugin.h,v 1.3 2004/12/08 18:41:17 ahartman Exp $
 */

#ifndef VOPLUGIN
#define VOPLUGIN

// Qt headers
#include <qobject.h>

// QGIS headers
#include <qgisplugin.h>

// Boost headers
#include <boost/smart_ptr.hpp>

// Forward declarations
class QgisApp;
class QgisIface;

/**
 * Vector Overlap Tool plug-in for QGIS.
 *
 * Takes two vector layers and finds their points of intersection
 */
class VOPlugin : public QObject, public QgisPlugin
{
    Q_OBJECT ;

public:
    /** 
     * @param qgis Pointer to the QgisApp object
     * @param qi Pointer to the QgisIface object
     */
    VOPlugin(QgisApp * qgis , QgisIface * qi);
private:
    struct Imp;
public:
    /** The destructor */
    virtual ~VOPlugin() { delete imp; }

public slots:
    /** Initialize the GUI */
    virtual void initGui();

    /** Unload the plug-in and clean up the GUI */
    virtual void unload() {}

    /** Show the dialog box */
    void run();

private:

    /**
     * A smart pointer to the object that will contain most of
     * the private data
     */
    //boost::scoped_ptr<Imp> imp;
    Imp* imp;
}; // class VOPlugin

#endif

