/****************************************************************************
** FrmDifference meta object code from reading C++ file 'frmdifference.h'
**
** Created: Thu Sep 16 10:45:01 2004
**      by: The Qt MOC ($Id: moc_frmdifference.cpp,v 1.1 2004/10/22 13:30:14 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "frmdifference.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *FrmDifference::className() const
{
    return "FrmDifference";
}

QMetaObject *FrmDifference::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FrmDifference( "FrmDifference", &FrmDifference::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FrmDifference::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FrmDifference", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FrmDifference::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FrmDifference", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FrmDifference::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"init", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "w", &static_QUType_int, 0, QUParameter::In },
	{ "h", &static_QUType_int, 0, QUParameter::In },
	{ "dataType", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"paint", 3, param_slot_1 };
    static const QUMethod slot_2 = {"saveHisto", 0, 0 };
    static const QUMethod slot_3 = {"languageChange", 0, 0 };
    static const QUMethod slot_4 = {"saveImage", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "init()", &slot_0, QMetaData::Public },
	{ "paint(int,int,QString)", &slot_1, QMetaData::Public },
	{ "saveHisto()", &slot_2, QMetaData::Public },
	{ "languageChange()", &slot_3, QMetaData::Protected },
	{ "saveImage()", &slot_4, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"FrmDifference", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FrmDifference.setMetaObject( metaObj );
    return metaObj;
}

void* FrmDifference::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FrmDifference" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool FrmDifference::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: init(); break;
    case 1: paint((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(QString)static_QUType_QString.get(_o+3)); break;
    case 2: saveHisto(); break;
    case 3: languageChange(); break;
    case 4: saveImage(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool FrmDifference::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FrmDifference::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool FrmDifference::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
