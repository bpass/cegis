######################################################################
# Automatically generated by qmake (1.07a) Wed Jul 28 10:08:23 2004
# Manually editted by $Author: jtrent $ on $Date: 2005/01/07 16:51:53 $
######################################################################

TARGET = mapimg
TEMPLATE = app


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


# Input
HEADERS += aboutform.ui.h \
           acForm.ui.h \
           cproj.h \
           ecaForm.ui.h \
           enterangForm.ui.h \
           enterlatForm.ui.h \
           enterlonForm.ui.h \
           equirectForm.ui.h \
           errorchecks.h \
           genVertForm.ui.h \
           getprojinfo.h \
           globals.h \
           homaForm.ui.h \
           hombForm.ui.h \
           imgio.h \
           interruptForm.ui.h \
           intsineForm.ui.h \
           jt_time.h \
           lambForm.ui.h \
           logForm.ui.h \
           mapimgform.ui.h \
           mercForm.ui.h \
           oblatedForm.ui.h \
           pcForm.ui.h \
           proj.h \
           proj15.h \
           projDatum.h \
           projfileForm.ui.h \
           projInfo.h \
           projName.h \
           projRowCol.h \
           projUnit.h \
           psForm.ui.h \
           qembed_images.h \
           resource.h \
           sineForm.ui.h \
           somaForm.ui.h \
           sombForm.ui.h \
           stereoForm.ui.h \
           tiff2img.h \
           tmercForm.ui.h \
           utmForm.ui.h \
           uvcode.h \
           vanderForm.ui.h \
           version.h \
           window_flags.h
INTERFACES += aboutform.ui \
              acForm.ui \
              ecaForm.ui \
              enterangForm.ui \
              enterlatForm.ui \
              enterlonForm.ui \
              equirectForm.ui \
              genVertForm.ui \
              homaForm.ui \
              hombForm.ui \
              interruptForm.ui \
              intsineForm.ui \
              lambForm.ui \
              logForm.ui \
              mapimgform.ui \
              mercForm.ui \
              oblatedForm.ui \
              pcForm.ui \
              projfileForm.ui \
              psForm.ui \
              sineForm.ui \
              somaForm.ui \
              sombForm.ui \
              spForm.ui \
              stereoForm.ui \
              tmercForm.ui \
              utmForm.ui \
              vanderForm.ui
SOURCES += alberfor.c \
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
           errorchecks.cpp \
           for_init.c \
           gctp.c \
           geo2eqr.cpp \
           getprojinfo.cpp \
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
           main.cpp \
           mapframeit.cpp \
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
           proj15.cpp \
           projDatum.cpp \
           projInfo.cpp \
           projName.cpp \
           projUnit.cpp \
           psfor.c \
           psinv.c \
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
RC_FILE += app.rc
