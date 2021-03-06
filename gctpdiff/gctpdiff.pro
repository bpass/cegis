######################################################################
# Automatically generated by qmake (2.00a) Mon Aug 29 14:59:28 2005
# Manually edited by $Id: gctpdiff.pro,v 1.3 2005/08/29 22:13:47 rbuehler Exp $
######################################################################

TEMPLATE	= app

DEPENDPATH	+= . gctp gctpcpp rasterdropframe rasterdropframe\tinyxml
INCLUDEPATH	+= . gctp gctpcpp rasterdropframe rasterdropframe\tinyxml

OBJECTS_DIR	= .tmp\obj
MOC_DIR	= .tmp\moc
UI_DIR	= .tmp\ui
RCC_DIR	= .tmp\rcc

# Input
HEADERS += gctpdiffform.h \
           gctp/cproj.h \
           gctp/proj.h \
           gctpcpp/alaskaconformal.h \
           gctpcpp/albersconeqarea.h \
           gctpcpp/azequidistant.h \
           gctpcpp/constants.h \
           gctpcpp/coordinate.h \
           gctpcpp/equidistantc.h \
           gctpcpp/equirectangular.h \
           gctpcpp/genvertnsp.h \
           gctpcpp/gnomonic.h \
           gctpcpp/goodeh.h \
           gctpcpp/hammer.h \
           gctpcpp/hotinobmerc.h \
           gctpcpp/intmollweide.h \
           gctpcpp/lambertazimuth.h \
           gctpcpp/lambertcc.h \
           gctpcpp/mercator.h \
           gctpcpp/miller.h \
           gctpcpp/mollweide.h \
           gctpcpp/oblatedeqarea.h \
           gctpcpp/orthographic.h \
           gctpcpp/polarstereo.h \
           gctpcpp/polyconic.h \
           gctpcpp/projection.h \
           gctpcpp/robinson.h \
           gctpcpp/sinusoidal.h \
           gctpcpp/spaceobmerc.h \
           gctpcpp/stereo.h \
           gctpcpp/transformer.h \
           gctpcpp/transversemercator.h \
           gctpcpp/util.h \
           gctpcpp/utm.h \
           gctpcpp/vandergrinten.h \
           gctpcpp/wagneriv.h \
           gctpcpp/wagnervii.h \
           rasterdropframe/rasterdropframe.h \
           rasterdropframe/rasterinfo.h \
           rasterdropframe/rasterxml.h \
           rasterdropframe/tinyxml/tinystr.h \
           rasterdropframe/tinyxml/tinyxml.h \
           rasterdropframe\rasterdropframe.h \
           rasterdropframe\rasterinfo.h \
           gctp\cproj.h \
           gctp\proj.h \
           gctpcpp\alaskaconformal.h \
           gctpcpp\projection.h \
           gctpcpp\util.h \
           gctpcpp\constants.h \
           gctpcpp\albersConEqArea.h \
           gctpcpp\azequidistant.h \
           gctpcpp\coordinate.h \
           gctpcpp\equidistantc.h \
           gctpcpp\equirectangular.h \
           gctpcpp\genvertnsp.h \
           gctpcpp\gnomonic.h \
           gctpcpp\goodeh.h \
           gctpcpp\hammer.h \
           gctpcpp\hotinobmerc.h \
           gctpcpp\intmollweide.h \
           gctpcpp\lambertazimuth.h \
           gctpcpp\lambertcc.h \
           gctpcpp\mercator.h \
           gctpcpp\miller.h \
           gctpcpp\mollweide.h \
           gctpcpp\oblatedeqarea.h \
           gctpcpp\orthographic.h \
           gctpcpp\polarstereo.h \
           gctpcpp\polyconic.h \
           gctpcpp\robinson.h \
           gctpcpp\sinusoidal.h \
           gctpcpp\spaceobmerc.h \
           gctpcpp\stereo.h \
           gctpcpp\transformer.h \
           gctpcpp\albersconeqarea.h \
           gctpcpp\transversemercator.h \
           gctpcpp\vandergrinten.h \
           gctpcpp\wagneriv.h \
           gctpcpp\wagnervii.h \
           gctpcpp\utm.h \
           rasterdropframe\rasterxml.h \
           rasterdropframe\tinyxml\tinyxml.h \
           rasterdropframe\tinyxml\tinystr.h
SOURCES += gctpdiffform.cpp \
           main.cpp \
           gctp/alberfor.c \
           gctp/alberinv.c \
           gctp/alconfor.c \
           gctp/alconinv.c \
           gctp/azimfor.c \
           gctp/aziminv.c \
           gctp/cproj.c \
           gctp/eqconfor.c \
           gctp/eqconinv.c \
           gctp/equifor.c \
           gctp/equiinv.c \
           gctp/for_init.c \
           gctp/gctp.c \
           gctp/gnomfor.c \
           gctp/gnominv.c \
           gctp/goodfor.c \
           gctp/goodinv.c \
           gctp/gvnspfor.c \
           gctp/gvnspinv.c \
           gctp/hamfor.c \
           gctp/haminv.c \
           gctp/imolwfor.c \
           gctp/imolwinv.c \
           gctp/inv_init.c \
           gctp/lamazfor.c \
           gctp/lamazinv.c \
           gctp/lamccfor.c \
           gctp/lamccinv.c \
           gctp/merfor.c \
           gctp/merinv.c \
           gctp/millfor.c \
           gctp/millinv.c \
           gctp/molwfor.c \
           gctp/molwinv.c \
           gctp/obleqfor.c \
           gctp/obleqinv.c \
           gctp/omerfor.c \
           gctp/omerinv.c \
           gctp/orthfor.c \
           gctp/orthinv.c \
           gctp/paksz.c \
           gctp/polyfor.c \
           gctp/polyinv.c \
           gctp/psfor.c \
           gctp/psinv.c \
           gctp/report.c \
           gctp/robfor.c \
           gctp/robinv.c \
           gctp/sinfor.c \
           gctp/sininv.c \
           gctp/somfor.c \
           gctp/sominv.c \
           gctp/sphdz.c \
           gctp/sterfor.c \
           gctp/sterinv.c \
           gctp/stplnfor.c \
           gctp/stplninv.c \
           gctp/tmfor.c \
           gctp/tminv.c \
           gctp/untfz.c \
           gctp/utmfor.c \
           gctp/utminv.c \
           gctp/vandgfor.c \
           gctp/vandginv.c \
           gctp/wivfor.c \
           gctp/wivinv.c \
           gctp/wviifor.c \
           gctp/wviiinv.c \
           gctpcpp/alaskaconformal.cpp \
           gctpcpp/albersconeqqrea.cpp \
           gctpcpp/azequidistant.cpp \
           gctpcpp/coordinate.cpp \
           gctpcpp/equidistantc.cpp \
           gctpcpp/equirectangular.cpp \
           gctpcpp/genvertnsp.cpp \
           gctpcpp/gnomonic.cpp \
           gctpcpp/goodeh.cpp \
           gctpcpp/hammer.cpp \
           gctpcpp/hotinobmerc.cpp \
           gctpcpp/intmollweide.cpp \
           gctpcpp/lambertazimuth.cpp \
           gctpcpp/lambertcc.cpp \
           gctpcpp/mercator.cpp \
           gctpcpp/miller.cpp \
           gctpcpp/mollweide.cpp \
           gctpcpp/oblatedeqarea.cpp \
           gctpcpp/orthographic.cpp \
           gctpcpp/polarstereo.cpp \
           gctpcpp/polyconic.cpp \
           gctpcpp/projection.cpp \
           gctpcpp/robinson.cpp \
           gctpcpp/sinusoidal.cpp \
           gctpcpp/spaceobmerc.cpp \
           gctpcpp/stereo.cpp \
           gctpcpp/transformer.cpp \
           gctpcpp/transversemercator.cpp \
           gctpcpp/util.cpp \
           gctpcpp/utm.cpp \
           gctpcpp/vandergrinten.cpp \
           gctpcpp/wagneriv.cpp \
           gctpcpp/wagnervii.cpp \
           rasterdropframe/rasterdropframe.cpp \
           rasterdropframe/rasterinfo.cpp \
           rasterdropframe/rasterxml.cpp \
           rasterdropframe/tinyxml/tinystr.cpp \
           rasterdropframe/tinyxml/tinyxml.cpp \
           rasterdropframe/tinyxml/tinyxmlerror.cpp \
           rasterdropframe/tinyxml/tinyxmlparser.cpp \
           rasterdropframe\rasterdropframe.cpp \
           rasterdropframe\rasterinfo.cpp \
           gctpcpp\alaskaconformal.cpp \
           gctpcpp\projection.cpp \
           gctpcpp\util.cpp \
           gctpcpp\azequidistant.cpp \
           gctpcpp\coordinate.cpp \
           gctpcpp\equidistantc.cpp \
           gctpcpp\equirectangular.cpp \
           gctpcpp\genvertnsp.cpp \
           gctpcpp\gnomonic.cpp \
           gctpcpp\goodeh.cpp \
           gctpcpp\hammer.cpp \
           gctpcpp\hotinobmerc.cpp \
           gctpcpp\intmollweide.cpp \
           gctpcpp\lambertazimuth.cpp \
           gctpcpp\lambertcc.cpp \
           gctpcpp\mercator.cpp \
           gctpcpp\miller.cpp \
           gctpcpp\mollweide.cpp \
           gctpcpp\oblatedeqarea.cpp \
           gctpcpp\orthographic.cpp \
           gctpcpp\polarstereo.cpp \
           gctpcpp\polyconic.cpp \
           gctpcpp\robinson.cpp \
           gctpcpp\sinusoidal.cpp \
           gctpcpp\spaceobmerc.cpp \
           gctpcpp\stereo.cpp \
           gctpcpp\transformer.cpp \
           gctpcpp\transversemercator.cpp \
           gctpcpp\vandergrinten.cpp \
           gctpcpp\wagneriv.cpp \
           gctpcpp\wagnervii.cpp \
           gctpcpp\utm.cpp \
           rasterdropframe\rasterxml.cpp \
           rasterdropframe\tinyxml\tinyxml.cpp \
           rasterdropframe\tinyxml\tinystr.cpp
