######################################################################
# Automatically generated by qmake (1.07a) Tue Jan 11 11:44:20 2005
# Manully Edited by $Id: mapimg.pro,v 1.10 2005/02/17 18:52:59 jtrent Exp $ 
######################################################################

TARGET = mapimg2
#for each platform check that TIFFDIR environment variable is set.
# If it IS set, then set up the library and include paths
# If it is NOT set, report an error and exit

#bring environment variables into qmake variables
TIFFDIR = $$(TIFFDIR)
TIFFINCLUDE = $$(TIFFINCLUDE)
TIFFLIB = $$(TIFFLIB)

#make sure that TIFFDIR and/or TIFFLIB and TIFFINCLUDE are set
isEmpty( TIFFDIR ) {
    isEmpty( TIFFLIB ) {
        error( "TIFFDIR environment variable must be set to the directory containing the TIFF library before qmaking and compiling. You may set TIFFINCLUDE and TIFFLIB instead if the headers and libraries are not in the same directory" )
    }
    isEmpty( TIFFINCLUDE ) {
        error( "TIFFDIR environment variable must be set to the directory containing the TIFF library before qmaking and compiling. You may set TIFFINCLUDE and TIFFLIB instead if the headers and libraries are not in the same directory" )
    }
}

#else
LIBPATH += $(TIFFDIR) $(TIFFDIR)/lib
INCLUDEPATH += . $(TIFFDIR) $(TIFFDIR)/include

!isEmpty( TIFFLIB ) {
     LIBPATH += $(TIFFLIB)
}
!isEmpty( TIFFINCLUDE ) {
     INCLUDEPATH += $(TIFFINCLUDE)

}

win32:LIBS += libtiff.lib
unix:LIBS += -ltiff

TEMPLATE = app
INCLUDEPATH += .

CONFIG += qt warn_on

OBJECTS_DIR	= .tmp\obj
MOC_DIR		= .tmp\moc
UI_DIR		= .tmp\ui

# Input
HEADERS += authorform.h \
           aboutform.h \
           cproj.h \
           gctpnames.h \
           getprojinfo.h \
           imgio.h \
           jt_time.h \
           logform.h \
           mapimg.h \
           mapimgform.h \
           mapimgimages.h \
           mapimgvalidator.h \
           mapimgversion.h \
           proj.h \
           qdmsedit.h \
           qembed_images.h \
           qgctpbox.h \
           qimgframe.h \
           qimgpainter.h \
           qinfoframe.h \
           rasterinfo.h \
           rasterxml.h \
           resampleform.h \
           resampleinfo.h \
           tiff2img.h \
           tinystr.h \
           tinyxml.h \
           window_flags.h
SOURCES += authorform.cpp \
           aboutform.cpp \
           alberfor.c \
           alberinv.c \
           alconfor.c \
           alconinv.c \
           azimfor.c \
           aziminv.c \
           cproj.c \
           eqconfor.c \
           eqconinv.c \
           equifor.c \
           equiinv.c \
           for_init.c \
           gctp.c \
           gnomfor.c \
           gnominv.c \
           goodfor.c \
           goodinv.c \
           gvnspfor.c \
           gvnspinv.c \
           hamfor.c \
           haminv.c \
           imgio.cpp \
           imolwfor.c \
           imolwinv.c \
           inv_init.c \
           lamazfor.c \
           lamazinv.c \
           lamccfor.c \
           lamccinv.c \
           logform.cpp \
           main.cpp \
           mapimg.cpp \
           mapimgform.cpp \
           mapimgvalidator.cpp \
           merfor.c \
           merinv.c \
           millfor.c \
           millinv.c \
           molwfor.c \
           molwinv.c \
           obleqfor.c \
           obleqinv.c \
           omerfor.c \
           omerinv.c \
           orthfor.c \
           orthinv.c \
           paksz.c \
           polyfor.c \
           polyinv.c \
           psfor.c \
           psinv.c \
           qdmsedit.cpp \
           qgctpbox.cpp \
           qimgframe.cpp \
           qinfoframe.cpp \
           rasterinfo.cpp \
           rasterxml.cpp \
           resampleform.cpp \
           resampleinfo.cpp \
           report.c \
           robfor.c \
           robinv.c \
           sinfor.c \
           sininv.c \
           somfor.c \
           sominv.c \
           sphdz.c \
           sterfor.c \
           sterinv.c \
           stplnfor.c \
           stplninv.c \
           tinystr.cpp \
           tinyxml.cpp \
           tinyxmlerror.cpp \
           tinyxmlparser.cpp \
           tmfor.c \
           tminv.c \
           untfz.c \
           utmfor.c \
           utminv.c \
           vandgfor.c \
           vandginv.c \
           wivfor.c \
           wivinv.c \
           wviifor.c \
           wviiinv.c
RC_FILE += icon.rc
