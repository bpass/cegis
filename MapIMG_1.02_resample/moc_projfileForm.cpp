/****************************************************************************
** projfileForm meta object code from reading C++ file 'projfileForm.h'
**
** Created: Thu Oct 7 08:49:22 2004
**      by: The Qt MOC ($Id: moc_projfileForm.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "projfileForm.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *projfileForm::className() const
{
    return "projfileForm";
}

QMetaObject *projfileForm::metaObj = 0;
static QMetaObjectCleanUp cleanUp_projfileForm( "projfileForm", &projfileForm::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString projfileForm::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "projfileForm", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString projfileForm::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "projfileForm", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* projfileForm::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"makeChanges", 0, 0 };
    static const QUMethod slot_1 = {"pixEditCheck", 0, 0 };
    static const QUMethod slot_2 = {"pixelCheck", 0, 0 };
    static const QUMethod slot_3 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "makeChanges()", &slot_0, QMetaData::Public },
	{ "pixEditCheck()", &slot_1, QMetaData::Public },
	{ "pixelCheck()", &slot_2, QMetaData::Public },
	{ "languageChange()", &slot_3, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ "pixel", &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"changesMade", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "changesMade(double)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"projfileForm", parentObject,
	slot_tbl, 4,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_projfileForm.setMetaObject( metaObj );
    return metaObj;
}

void* projfileForm::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "projfileForm" ) )
	return this;
    return QDialog::qt_cast( clname );
}

// SIGNAL changesMade
void projfileForm::changesMade( double t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

bool projfileForm::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: makeChanges(); break;
    case 1: pixEditCheck(); break;
    case 2: pixelCheck(); break;
    case 3: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool projfileForm::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: changesMade((double)static_QUType_double.get(_o+1)); break;
    default:
	return QDialog::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool projfileForm::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool projfileForm::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
