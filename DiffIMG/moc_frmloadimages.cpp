/****************************************************************************
** FrmLoadImages meta object code from reading C++ file 'frmloadimages.h'
**
** Created: Fri Oct 22 08:57:51 2004
**      by: The Qt MOC ($Id: moc_frmloadimages.cpp,v 1.2 2004/10/22 15:37:22 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "frmloadimages.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *FrmLoadImages::className() const
{
    return "FrmLoadImages";
}

QMetaObject *FrmLoadImages::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FrmLoadImages( "FrmLoadImages", &FrmLoadImages::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FrmLoadImages::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FrmLoadImages", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FrmLoadImages::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FrmLoadImages", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FrmLoadImages::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"loadOriginal", 0, 0 };
    static const QUMethod slot_1 = {"loadReprojected", 0, 0 };
    static const QUMethod slot_2 = {"showDifference", 0, 0 };
    static const QUMethod slot_3 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "loadOriginal()", &slot_0, QMetaData::Public },
	{ "loadReprojected()", &slot_1, QMetaData::Public },
	{ "showDifference()", &slot_2, QMetaData::Public },
	{ "languageChange()", &slot_3, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"FrmLoadImages", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FrmLoadImages.setMetaObject( metaObj );
    return metaObj;
}

void* FrmLoadImages::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FrmLoadImages" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool FrmLoadImages::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: loadOriginal(); break;
    case 1: loadReprojected(); break;
    case 2: showDifference(); break;
    case 3: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool FrmLoadImages::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FrmLoadImages::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool FrmLoadImages::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
