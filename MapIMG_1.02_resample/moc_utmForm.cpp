/****************************************************************************
** utmForm meta object code from reading C++ file 'utmForm.h'
**
** Created: Thu Oct 7 08:49:22 2004
**      by: The Qt MOC ($Id: moc_utmForm.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "utmForm.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *utmForm::className() const
{
    return "utmForm";
}

QMetaObject *utmForm::metaObj = 0;
static QMetaObjectCleanUp cleanUp_utmForm( "utmForm", &utmForm::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString utmForm::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "utmForm", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString utmForm::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "utmForm", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* utmForm::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"makeChanges", 0, 0 };
    static const QUMethod slot_1 = {"init", 0, 0 };
    static const QUMethod slot_2 = {"getGCTP", 0, 0 };
    static const QUMethod slot_3 = {"getlon", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "l", &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"setLon", 1, param_slot_4 };
    static const QUMethod slot_5 = {"getlat", 0, 0 };
    static const QUParameter param_slot_6[] = {
	{ "l", &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"setLat", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod slot_7 = {"sendLat", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod slot_8 = {"sendLon", 1, param_slot_8 };
    static const QUMethod slot_9 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "makeChanges()", &slot_0, QMetaData::Public },
	{ "init()", &slot_1, QMetaData::Public },
	{ "getGCTP()", &slot_2, QMetaData::Public },
	{ "getlon()", &slot_3, QMetaData::Public },
	{ "setLon(double)", &slot_4, QMetaData::Public },
	{ "getlat()", &slot_5, QMetaData::Public },
	{ "setLat(double)", &slot_6, QMetaData::Public },
	{ "sendLat(double*)", &slot_7, QMetaData::Public },
	{ "sendLon(double*)", &slot_8, QMetaData::Public },
	{ "languageChange()", &slot_9, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In },
	{ "val", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"changesMade", 2, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod signal_1 = {"getGCTP", 1, param_signal_1 };
    static const QMetaData signal_tbl[] = {
	{ "changesMade(double*,int)", &signal_0, QMetaData::Public },
	{ "getGCTP(double*)", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"utmForm", parentObject,
	slot_tbl, 10,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_utmForm.setMetaObject( metaObj );
    return metaObj;
}

void* utmForm::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "utmForm" ) )
	return this;
    return QDialog::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL changesMade
void utmForm::changesMade( double* t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_varptr.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL getGCTP
void utmForm::getGCTP( double* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_varptr.set(o+1,t0);
    activate_signal( clist, o );
}

bool utmForm::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: makeChanges(); break;
    case 1: init(); break;
    case 2: getGCTP(); break;
    case 3: getlon(); break;
    case 4: setLon((double)static_QUType_double.get(_o+1)); break;
    case 5: getlat(); break;
    case 6: setLat((double)static_QUType_double.get(_o+1)); break;
    case 7: sendLat((double*)static_QUType_varptr.get(_o+1)); break;
    case 8: sendLon((double*)static_QUType_varptr.get(_o+1)); break;
    case 9: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool utmForm::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: changesMade((double*)static_QUType_varptr.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 1: getGCTP((double*)static_QUType_varptr.get(_o+1)); break;
    default:
	return QDialog::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool utmForm::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool utmForm::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
