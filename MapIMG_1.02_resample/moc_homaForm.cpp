/****************************************************************************
** homaForm meta object code from reading C++ file 'homaForm.h'
**
** Created: Thu Oct 7 08:49:21 2004
**      by: The Qt MOC ($Id: moc_homaForm.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "homaForm.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *homaForm::className() const
{
    return "homaForm";
}

QMetaObject *homaForm::metaObj = 0;
static QMetaObjectCleanUp cleanUp_homaForm( "homaForm", &homaForm::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString homaForm::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "homaForm", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString homaForm::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "homaForm", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* homaForm::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"makeChanges", 0, 0 };
    static const QUMethod slot_1 = {"init", 0, 0 };
    static const QUMethod slot_2 = {"getGCTP", 0, 0 };
    static const QUMethod slot_3 = {"getlat", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "l", &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"setLat", 1, param_slot_4 };
    static const QUMethod slot_5 = {"getlon1", 0, 0 };
    static const QUParameter param_slot_6[] = {
	{ "l", &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"setLon1", 1, param_slot_6 };
    static const QUMethod slot_7 = {"getlon2", 0, 0 };
    static const QUParameter param_slot_8[] = {
	{ "l", &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"setLon2", 1, param_slot_8 };
    static const QUMethod slot_9 = {"getlat1", 0, 0 };
    static const QUParameter param_slot_10[] = {
	{ "l", &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"setLat1", 1, param_slot_10 };
    static const QUMethod slot_11 = {"getlat2", 0, 0 };
    static const QUParameter param_slot_12[] = {
	{ "l", &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"setLat2", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "check", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"smajCheck", 1, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ "check", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"sminCheck", 1, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ "check", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"scaleCheck", 1, param_slot_15 };
    static const QUParameter param_slot_16[] = {
	{ "check", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_16 = {"feCheck", 1, param_slot_16 };
    static const QUParameter param_slot_17[] = {
	{ "check", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_17 = {"fnCheck", 1, param_slot_17 };
    static const QUParameter param_slot_18[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_18 = {"valueCheck", 1, param_slot_18 };
    static const QUParameter param_slot_19[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod slot_19 = {"sendLon1st", 1, param_slot_19 };
    static const QUParameter param_slot_20[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod slot_20 = {"sendLon2n", 1, param_slot_20 };
    static const QUParameter param_slot_21[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod slot_21 = {"sendLat1st", 1, param_slot_21 };
    static const QUParameter param_slot_22[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod slot_22 = {"sendLat2nd", 1, param_slot_22 };
    static const QUParameter param_slot_23[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod slot_23 = {"sendLatOrig", 1, param_slot_23 };
    static const QUMethod slot_24 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "makeChanges()", &slot_0, QMetaData::Public },
	{ "init()", &slot_1, QMetaData::Public },
	{ "getGCTP()", &slot_2, QMetaData::Public },
	{ "getlat()", &slot_3, QMetaData::Public },
	{ "setLat(double)", &slot_4, QMetaData::Public },
	{ "getlon1()", &slot_5, QMetaData::Public },
	{ "setLon1(double)", &slot_6, QMetaData::Public },
	{ "getlon2()", &slot_7, QMetaData::Public },
	{ "setLon2(double)", &slot_8, QMetaData::Public },
	{ "getlat1()", &slot_9, QMetaData::Public },
	{ "setLat1(double)", &slot_10, QMetaData::Public },
	{ "getlat2()", &slot_11, QMetaData::Public },
	{ "setLat2(double)", &slot_12, QMetaData::Public },
	{ "smajCheck(const QString&)", &slot_13, QMetaData::Public },
	{ "sminCheck(const QString&)", &slot_14, QMetaData::Public },
	{ "scaleCheck(const QString&)", &slot_15, QMetaData::Public },
	{ "feCheck(const QString&)", &slot_16, QMetaData::Public },
	{ "fnCheck(const QString&)", &slot_17, QMetaData::Public },
	{ "valueCheck()", &slot_18, QMetaData::Public },
	{ "sendLon1st(double*)", &slot_19, QMetaData::Public },
	{ "sendLon2n(double*)", &slot_20, QMetaData::Public },
	{ "sendLat1st(double*)", &slot_21, QMetaData::Public },
	{ "sendLat2nd(double*)", &slot_22, QMetaData::Public },
	{ "sendLatOrig(double*)", &slot_23, QMetaData::Public },
	{ "languageChange()", &slot_24, QMetaData::Protected }
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
	"homaForm", parentObject,
	slot_tbl, 25,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_homaForm.setMetaObject( metaObj );
    return metaObj;
}

void* homaForm::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "homaForm" ) )
	return this;
    return QDialog::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL changesMade
void homaForm::changesMade( double* t0 )
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
void homaForm::getGCTP( double* t0 )
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

bool homaForm::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: makeChanges(); break;
    case 1: init(); break;
    case 2: getGCTP(); break;
    case 3: getlat(); break;
    case 4: setLat((double)static_QUType_double.get(_o+1)); break;
    case 5: getlon1(); break;
    case 6: setLon1((double)static_QUType_double.get(_o+1)); break;
    case 7: getlon2(); break;
    case 8: setLon2((double)static_QUType_double.get(_o+1)); break;
    case 9: getlat1(); break;
    case 10: setLat1((double)static_QUType_double.get(_o+1)); break;
    case 11: getlat2(); break;
    case 12: setLat2((double)static_QUType_double.get(_o+1)); break;
    case 13: smajCheck((const QString&)static_QUType_QString.get(_o+1)); break;
    case 14: sminCheck((const QString&)static_QUType_QString.get(_o+1)); break;
    case 15: scaleCheck((const QString&)static_QUType_QString.get(_o+1)); break;
    case 16: feCheck((const QString&)static_QUType_QString.get(_o+1)); break;
    case 17: fnCheck((const QString&)static_QUType_QString.get(_o+1)); break;
    case 18: static_QUType_bool.set(_o,valueCheck()); break;
    case 19: sendLon1st((double*)static_QUType_varptr.get(_o+1)); break;
    case 20: sendLon2n((double*)static_QUType_varptr.get(_o+1)); break;
    case 21: sendLat1st((double*)static_QUType_varptr.get(_o+1)); break;
    case 22: sendLat2nd((double*)static_QUType_varptr.get(_o+1)); break;
    case 23: sendLatOrig((double*)static_QUType_varptr.get(_o+1)); break;
    case 24: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool homaForm::qt_emit( int _id, QUObject* _o )
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

bool homaForm::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool homaForm::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
