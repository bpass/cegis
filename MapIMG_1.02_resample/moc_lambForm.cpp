/****************************************************************************
** lambForm meta object code from reading C++ file 'lambForm.h'
**
** Created: Thu Oct 7 08:49:21 2004
**      by: The Qt MOC ($Id: moc_lambForm.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "lambForm.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *lambForm::className() const
{
    return "lambForm";
}

QMetaObject *lambForm::metaObj = 0;
static QMetaObjectCleanUp cleanUp_lambForm( "lambForm", &lambForm::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString lambForm::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "lambForm", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString lambForm::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "lambForm", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* lambForm::staticMetaObject()
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
    static const QUMethod slot_5 = {"getlat1", 0, 0 };
    static const QUParameter param_slot_6[] = {
	{ "l", &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"setLat1", 1, param_slot_6 };
    static const QUMethod slot_7 = {"getlat2", 0, 0 };
    static const QUParameter param_slot_8[] = {
	{ "l", &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"setLat2", 1, param_slot_8 };
    static const QUMethod slot_9 = {"getlat3", 0, 0 };
    static const QUParameter param_slot_10[] = {
	{ "l", &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"setLat3", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ "check", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"smajCheck", 1, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ "check", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"sminCheck", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "check", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"feCheck", 1, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ "check", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"fnCheck", 1, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_15 = {"valueCheck", 1, param_slot_15 };
    static const QUParameter param_slot_16[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod slot_16 = {"sendLat1st", 1, param_slot_16 };
    static const QUParameter param_slot_17[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod slot_17 = {"sendLat2nd", 1, param_slot_17 };
    static const QUParameter param_slot_18[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod slot_18 = {"sendLon", 1, param_slot_18 };
    static const QUParameter param_slot_19[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod slot_19 = {"sendLatOrig", 1, param_slot_19 };
    static const QUMethod slot_20 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "makeChanges()", &slot_0, QMetaData::Public },
	{ "init()", &slot_1, QMetaData::Public },
	{ "getGCTP()", &slot_2, QMetaData::Public },
	{ "getlon()", &slot_3, QMetaData::Public },
	{ "setLon(double)", &slot_4, QMetaData::Public },
	{ "getlat1()", &slot_5, QMetaData::Public },
	{ "setLat1(double)", &slot_6, QMetaData::Public },
	{ "getlat2()", &slot_7, QMetaData::Public },
	{ "setLat2(double)", &slot_8, QMetaData::Public },
	{ "getlat3()", &slot_9, QMetaData::Public },
	{ "setLat3(double)", &slot_10, QMetaData::Public },
	{ "smajCheck(const QString&)", &slot_11, QMetaData::Public },
	{ "sminCheck(const QString&)", &slot_12, QMetaData::Public },
	{ "feCheck(const QString&)", &slot_13, QMetaData::Public },
	{ "fnCheck(const QString&)", &slot_14, QMetaData::Public },
	{ "valueCheck()", &slot_15, QMetaData::Public },
	{ "sendLat1st(double*)", &slot_16, QMetaData::Public },
	{ "sendLat2nd(double*)", &slot_17, QMetaData::Public },
	{ "sendLon(double*)", &slot_18, QMetaData::Public },
	{ "sendLatOrig(double*)", &slot_19, QMetaData::Public },
	{ "languageChange()", &slot_20, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod signal_0 = {"changesMade", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod signal_1 = {"getGCTP", 1, param_signal_1 };
    static const QMetaData signal_tbl[] = {
	{ "changesMade(double*)", &signal_0, QMetaData::Public },
	{ "getGCTP(double*)", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"lambForm", parentObject,
	slot_tbl, 21,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_lambForm.setMetaObject( metaObj );
    return metaObj;
}

void* lambForm::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "lambForm" ) )
	return this;
    return QDialog::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL changesMade
void lambForm::changesMade( double* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_varptr.set(o+1,t0);
    activate_signal( clist, o );
}

// SIGNAL getGCTP
void lambForm::getGCTP( double* t0 )
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

bool lambForm::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: makeChanges(); break;
    case 1: init(); break;
    case 2: getGCTP(); break;
    case 3: getlon(); break;
    case 4: setLon((double)static_QUType_double.get(_o+1)); break;
    case 5: getlat1(); break;
    case 6: setLat1((double)static_QUType_double.get(_o+1)); break;
    case 7: getlat2(); break;
    case 8: setLat2((double)static_QUType_double.get(_o+1)); break;
    case 9: getlat3(); break;
    case 10: setLat3((double)static_QUType_double.get(_o+1)); break;
    case 11: smajCheck((const QString&)static_QUType_QString.get(_o+1)); break;
    case 12: sminCheck((const QString&)static_QUType_QString.get(_o+1)); break;
    case 13: feCheck((const QString&)static_QUType_QString.get(_o+1)); break;
    case 14: fnCheck((const QString&)static_QUType_QString.get(_o+1)); break;
    case 15: static_QUType_bool.set(_o,valueCheck()); break;
    case 16: sendLat1st((double*)static_QUType_varptr.get(_o+1)); break;
    case 17: sendLat2nd((double*)static_QUType_varptr.get(_o+1)); break;
    case 18: sendLon((double*)static_QUType_varptr.get(_o+1)); break;
    case 19: sendLatOrig((double*)static_QUType_varptr.get(_o+1)); break;
    case 20: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool lambForm::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: changesMade((double*)static_QUType_varptr.get(_o+1)); break;
    case 1: getGCTP((double*)static_QUType_varptr.get(_o+1)); break;
    default:
	return QDialog::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool lambForm::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool lambForm::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
