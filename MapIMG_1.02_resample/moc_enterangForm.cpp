/****************************************************************************
** enterangForm meta object code from reading C++ file 'enterangForm.h'
**
** Created: Thu Oct 7 08:49:21 2004
**      by: The Qt MOC ($Id: moc_enterangForm.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "enterangForm.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *enterangForm::className() const
{
    return "enterangForm";
}

QMetaObject *enterangForm::metaObj = 0;
static QMetaObjectCleanUp cleanUp_enterangForm( "enterangForm", &enterangForm::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString enterangForm::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "enterangForm", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString enterangForm::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "enterangForm", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* enterangForm::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"checkSeconds", 0, 0 };
    static const QUMethod slot_1 = {"setAng", 0, 0 };
    static const QUMethod slot_2 = {"getAng", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "toTrim", &static_QUType_varptr, "\x03", QUParameter::In }
    };
    static const QUMethod slot_3 = {"trimZeros", 1, param_slot_3 };
    static const QUMethod slot_4 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "checkSeconds()", &slot_0, QMetaData::Public },
	{ "setAng()", &slot_1, QMetaData::Public },
	{ "getAng()", &slot_2, QMetaData::Public },
	{ "trimZeros(QString*)", &slot_3, QMetaData::Public },
	{ "languageChange()", &slot_4, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ "l", &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"changesMade", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod signal_1 = {"getAng", 1, param_signal_1 };
    static const QMetaData signal_tbl[] = {
	{ "changesMade(double)", &signal_0, QMetaData::Public },
	{ "getAng(double*)", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"enterangForm", parentObject,
	slot_tbl, 5,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_enterangForm.setMetaObject( metaObj );
    return metaObj;
}

void* enterangForm::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "enterangForm" ) )
	return this;
    return QDialog::qt_cast( clname );
}

// SIGNAL changesMade
void enterangForm::changesMade( double t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL getAng
void enterangForm::getAng( double* t0 )
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

bool enterangForm::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: checkSeconds(); break;
    case 1: setAng(); break;
    case 2: getAng(); break;
    case 3: trimZeros((QString*)static_QUType_varptr.get(_o+1)); break;
    case 4: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool enterangForm::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: changesMade((double)static_QUType_double.get(_o+1)); break;
    case 1: getAng((double*)static_QUType_varptr.get(_o+1)); break;
    default:
	return QDialog::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool enterangForm::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool enterangForm::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
