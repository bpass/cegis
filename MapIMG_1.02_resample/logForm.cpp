/****************************************************************************
** Form implementation generated from reading ui file 'logForm.ui'
**
** Created: Thu Oct 7 08:49:15 2004
**      by: The User Interface Compiler ($Id: logForm.cpp,v 1.1 2004/10/20 16:36:40 jtrent Exp $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "logForm.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtextedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "logForm.ui.h"
static const char* const image0_data[] = { 
"16 16 86 2",
".Q c #006f41",
".v c #006f42",
".z c #007041",
".# c #007042",
".f c #017042",
".L c #027043",
".U c #027143",
".9 c #027144",
".P c #047144",
".F c #067245",
".n c #0a7348",
"#k c #0a7449",
"#e c #0c7147",
"#b c #0c754a",
".u c #0d764b",
".I c #0e754b",
".a c #13734c",
".y c #13784f",
"#j c #157950",
".b c #1a714d",
".r c #267e5a",
".E c #2b8661",
".h c #308763",
".5 c #368a67",
".3 c #368a68",
".1 c #398d6a",
"#o c #408f6f",
"#s c #409270",
".m c #449272",
".K c #469373",
".Y c #4b9477",
"#p c #4b9778",
".d c #4b9878",
".c c #4e997a",
"#h c #509a7c",
"#d c #53967b",
".s c #55987d",
"Qt c #559d80",
".e c #559e80",
".B c #5f9f86",
"#r c #62a087",
".2 c #66a68b",
".w c #68a68d",
".6 c #6ea88f",
".p c #73ac95",
"#a c #73ad95",
".A c #86b7a2",
".H c #8cbba8",
".M c #9dc4b4",
".X c #a5c7b9",
"#l c #a6c6b9",
".S c #a7c9bb",
".D c #a8c6ba",
".8 c #b8d2c7",
".R c #b9d4c9",
".g c #b9d5c9",
".V c #bed7cd",
".O c #c5dad2",
".T c #c5dbd2",
".q c #c5dcd2",
"#. c #c6dbd2",
"#m c #d0e0d9",
".t c #d2e1db",
"#t c #dae5e1",
"#q c #dae7e1",
".G c #dbe8e2",
"#g c #dee9e5",
"#f c #e0ebe7",
"#c c #e4ede9",
".o c #e4eeea",
".l c #e5eeeb",
"#i c #eaf1ef",
".x c #eaf2ef",
".J c #ecf2ef",
".W c #edf0ef",
".7 c #eef2f1",
"#n c #f2f4f3",
".N c #f9f9f9",
".C c #fbfbfb",
".4 c #fbfcfc",
".0 c #fcfdfc",
".Z c #fdfdfd",
"## c #fdfdfe",
".k c #fefdfd",
".i c #fefefe",
".j c #ffffff",
"Qt.#.#.#.#.#.#.#.#.#.#.#.#.a.b.c",
"Qt.#.#.#.#.#.#.#.#.#.#.#.#.#.#.d",
".e.#.#.#.#.#.#.#.#.#.#.#.#.#.#.d",
".e.#.#.#.#.#.#.#.#.#.#.#.#.#.f.g",
".e.#.#.#.#.#.#.#.#.#.#.#.#.h.i.j",
".k.l.m.#.#.#.#.#.#.#.#.n.o.j.p.d",
".q.r.s.t.u.#.#.#.#.v.w.i.x.y.z.A",
"Qt.B.j.C.D.E.#.#.F.G.i.H.#.I.J.j",
".j.i.K.v.L.M.N.j.j.O.P.Q.R.j.S.d",
".T.U.L.V.j.j.W.X.Q.v.Y.Z.0.1.#.2",
"Qt.3.i.4.5.#.#.6.7.i.j.8.Q.9#..j",
"##.i#a.Q.#.#.#.##b#c#d#e#f.i#g#h",
"#i#j.#.#.#.#.#.#.##k#l#m#n#o.##p",
"Qt.#.#.#.#.#.#.#.#.#.Q#q#r#s#t.j",
"Qt.#.#.#.#.#.#.#.#.#.#.#.#.a.b.c",
"Qt.#.#.#.#.#.#.#.#.#.#.#.#.#.#.d"};


/*
 *  Constructs a logForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
logForm::logForm( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl ),
      image0( (const char **) image0_data )
{
    if ( !name )
	setName( "logForm" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 300, 300 ) );
    setMaximumSize( QSize( 600, 600 ) );
    setBaseSize( QSize( 400, 375 ) );
    setIcon( image0 );
    logFormLayout = new QGridLayout( this, 1, 1, 11, 6, "logFormLayout"); 

    layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 
    QSpacerItem* spacer = new QSpacerItem( 210, 21, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer );

    saveButton = new QPushButton( this, "saveButton" );
    layout3->addWidget( saveButton );

    okButton = new QPushButton( this, "okButton" );
    layout3->addWidget( okButton );

    logFormLayout->addLayout( layout3, 1, 0 );

    logViewer = new QTextEdit( this, "logViewer" );

    logFormLayout->addWidget( logViewer, 0, 0 );
    languageChange();
    resize( QSize(361, 365).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( saveButton, SIGNAL( clicked() ), this, SLOT( saveLog() ) );

    // tab order
    setTabOrder( okButton, saveButton );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
logForm::~logForm()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void logForm::languageChange()
{
    setCaption( tr( "MapIMG Log..." ) );
    setIconText( QString::null );
    saveButton->setText( tr( "&Save..." ) );
    okButton->setText( tr( "&OK" ) );
}

