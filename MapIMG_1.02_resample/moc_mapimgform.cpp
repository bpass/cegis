/****************************************************************************
** mapimgForm meta object code from reading C++ file 'mapimgform.h'
**
** Created: Thu Oct 7 08:49:21 2004
**      by: The Qt MOC ($Id: moc_mapimgform.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "mapimgform.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *mapimgForm::className() const
{
    return "mapimgForm";
}

QMetaObject *mapimgForm::metaObj = 0;
static QMetaObjectCleanUp cleanUp_mapimgForm( "mapimgForm", &mapimgForm::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString mapimgForm::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "mapimgForm", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString mapimgForm::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "mapimgForm", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* mapimgForm::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"executemapimg", 0, 0 };
    static const QUMethod slot_1 = {"comboCheck", 0, 0 };
    static const QUMethod slot_2 = {"pixEditCheck", 0, 0 };
    static const QUMethod slot_3 = {"getInfile", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "s", &static_QUType_QString, 0, QUParameter::InOut }
    };
    static const QUMethod slot_4 = {"loadInputInfo", 1, param_slot_4 };
    static const QUMethod slot_5 = {"getOutfile", 0, 0 };
    static const QUMethod slot_6 = {"pixelCheck", 0, 0 };
    static const QUParameter param_slot_7[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_7 = {"infilenameExist", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_8 = {"outfilenameExist", 1, param_slot_8 };
    static const QUMethod slot_9 = {"init", 0, 0 };
    static const QUParameter param_slot_10[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod slot_10 = {"setgctpArray", 1, param_slot_10 };
    static const QUMethod slot_11 = {"comboCheckIn", 0, 0 };
    static const QUParameter param_slot_12[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod slot_12 = {"setgctpArrayOut", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ "pixel", &static_QUType_double, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"setInPixel", 1, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In },
	{ "val", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"setUTMout", 2, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In },
	{ "val", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"setUTMin", 2, param_slot_15 };
    static const QUParameter param_slot_16[] = {
	{ "val", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_16 = {"setSPout", 1, param_slot_16 };
    static const QUParameter param_slot_17[] = {
	{ "infileName", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_17 = {"infileValid", 1, param_slot_17 };
    static const QUParameter param_slot_18[] = {
	{ "outfileName", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_18 = {"outfileValid", 1, param_slot_18 };
    static const QUMethod slot_19 = {"calcDataType", 0, 0 };
    static const QUMethod slot_20 = {"comboCheckData", 0, 0 };
    static const QUMethod slot_21 = {"showAllDataTypes", 0, 0 };
    static const QUMethod slot_22 = {"show8BitDataTypes", 0, 0 };
    static const QUMethod slot_23 = {"show16BitDataTypes", 0, 0 };
    static const QUMethod slot_24 = {"show32BitDataTypes", 0, 0 };
    static const QUMethod slot_25 = {"show64BitDataTypes", 0, 0 };
    static const QUParameter param_slot_26[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod slot_26 = {"getOutputGCTP", 1, param_slot_26 };
    static const QUParameter param_slot_27[] = {
	{ "ptr", &static_QUType_varptr, "\x13", QUParameter::In }
    };
    static const QUMethod slot_27 = {"getInputGCTP", 1, param_slot_27 };
    static const QUMethod slot_28 = {"timerDone", 0, 0 };
    static const QUMethod slot_29 = {"showAboutMapIMG", 0, 0 };
    static const QUMethod slot_30 = {"showAboutQt", 0, 0 };
    static const QUMethod slot_31 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "executemapimg()", &slot_0, QMetaData::Public },
	{ "comboCheck()", &slot_1, QMetaData::Public },
	{ "pixEditCheck()", &slot_2, QMetaData::Public },
	{ "getInfile()", &slot_3, QMetaData::Public },
	{ "loadInputInfo(QString&)", &slot_4, QMetaData::Public },
	{ "getOutfile()", &slot_5, QMetaData::Public },
	{ "pixelCheck()", &slot_6, QMetaData::Public },
	{ "infilenameExist()", &slot_7, QMetaData::Public },
	{ "outfilenameExist()", &slot_8, QMetaData::Public },
	{ "init()", &slot_9, QMetaData::Public },
	{ "setgctpArray(double*)", &slot_10, QMetaData::Public },
	{ "comboCheckIn()", &slot_11, QMetaData::Public },
	{ "setgctpArrayOut(double*)", &slot_12, QMetaData::Public },
	{ "setInPixel(double)", &slot_13, QMetaData::Public },
	{ "setUTMout(double*,int)", &slot_14, QMetaData::Public },
	{ "setUTMin(double*,int)", &slot_15, QMetaData::Public },
	{ "setSPout(int)", &slot_16, QMetaData::Public },
	{ "infileValid(const QString&)", &slot_17, QMetaData::Public },
	{ "outfileValid(const QString&)", &slot_18, QMetaData::Public },
	{ "calcDataType()", &slot_19, QMetaData::Public },
	{ "comboCheckData()", &slot_20, QMetaData::Public },
	{ "showAllDataTypes()", &slot_21, QMetaData::Public },
	{ "show8BitDataTypes()", &slot_22, QMetaData::Public },
	{ "show16BitDataTypes()", &slot_23, QMetaData::Public },
	{ "show32BitDataTypes()", &slot_24, QMetaData::Public },
	{ "show64BitDataTypes()", &slot_25, QMetaData::Public },
	{ "getOutputGCTP(double*)", &slot_26, QMetaData::Public },
	{ "getInputGCTP(double*)", &slot_27, QMetaData::Public },
	{ "timerDone()", &slot_28, QMetaData::Public },
	{ "showAboutMapIMG()", &slot_29, QMetaData::Public },
	{ "showAboutQt()", &slot_30, QMetaData::Public },
	{ "languageChange()", &slot_31, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"mapimgForm", parentObject,
	slot_tbl, 32,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_mapimgForm.setMetaObject( metaObj );
    return metaObj;
}

void* mapimgForm::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "mapimgForm" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool mapimgForm::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: executemapimg(); break;
    case 1: comboCheck(); break;
    case 2: pixEditCheck(); break;
    case 3: getInfile(); break;
    case 4: loadInputInfo((QString&)static_QUType_QString.get(_o+1)); break;
    case 5: getOutfile(); break;
    case 6: pixelCheck(); break;
    case 7: static_QUType_bool.set(_o,infilenameExist()); break;
    case 8: static_QUType_bool.set(_o,outfilenameExist()); break;
    case 9: init(); break;
    case 10: setgctpArray((double*)static_QUType_varptr.get(_o+1)); break;
    case 11: comboCheckIn(); break;
    case 12: setgctpArrayOut((double*)static_QUType_varptr.get(_o+1)); break;
    case 13: setInPixel((double)static_QUType_double.get(_o+1)); break;
    case 14: setUTMout((double*)static_QUType_varptr.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 15: setUTMin((double*)static_QUType_varptr.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 16: setSPout((int)static_QUType_int.get(_o+1)); break;
    case 17: infileValid((const QString&)static_QUType_QString.get(_o+1)); break;
    case 18: outfileValid((const QString&)static_QUType_QString.get(_o+1)); break;
    case 19: calcDataType(); break;
    case 20: comboCheckData(); break;
    case 21: showAllDataTypes(); break;
    case 22: show8BitDataTypes(); break;
    case 23: show16BitDataTypes(); break;
    case 24: show32BitDataTypes(); break;
    case 25: show64BitDataTypes(); break;
    case 26: getOutputGCTP((double*)static_QUType_varptr.get(_o+1)); break;
    case 27: getInputGCTP((double*)static_QUType_varptr.get(_o+1)); break;
    case 28: timerDone(); break;
    case 29: showAboutMapIMG(); break;
    case 30: showAboutQt(); break;
    case 31: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool mapimgForm::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool mapimgForm::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool mapimgForm::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
