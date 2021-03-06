

//******************************************************************************
//*File: transferHeader.h
//*Description: Contains all includes and structs for transferHeader program.
//*Programmer: Michael Williams USGS 
//*Date: June, 2004
//******************************************************************************
#ifndef TRANS_H
#define TRANS_H
/*
** Toolkit Includes
*/
#include <esys.h>
#include <evue.h>
#include <eint.h>
#include <eerr.h>
#include <eprj.h>
#include <etxt.h>
#include <estr.h>
#include <eimg.h>
#include <emsc.h>
#include <efio.h>
#include <ehfa.h>

#include <eeml.h>
#include <eeml_menu.h>
#include <eeml_dialog.h>

//storage for band pass object
typedef struct {
	double centralWavelength; 
	double FWHM;
	char* spacer; //doesn't actually store anything,
				  //it's here to increase the struct size so that
				  //the ehfa_ObjectRead function puts all of the
				  //data in the right fields.  you have to have one
				  //before and after every char* or char** in your struct.
			      //I can see you rolling your eyes from here, hey, I didn't
				  //write the damn library, don't bitch at me!
	char* wavelengthUnits;
	char* spacer2;
	char* dnUnits;
	char* spacer3;
	double usefulness;
} bandPass;

//Storage for HDFBandAttributes object for each layer.
typedef struct {
	long IMAGEDATAINFORMATION[3];
    long MINANDMAX[2];
    double MEANANDSTD[2];
    long MODEANDMEDIAN[2];
    long NUMBEROFBADPIXELS[2];
	char* spacer;
    char* CORINTEL;
	char* spacer2;
    char* CORPARA;
	char* spacer3;
    char* RESMETHOD;
	char* spacer4;
    char* MPMETHOD;
    double PROJECTIONPARAMETERS[13];
    long UTMZONECODE;
    double INCL;
    double OFFSET;
	char* spacer5;
    char* CONUNIT;
}HDFBandAttributes;

//Storage for HDFGlobalAttributes object
//
//I'd explain all of the entries, but there's 
//over 800 and I'm not that motivated.
//Plus, I don't know what 99% of them are 
//anyway, and while we're on the subject,
//why should I have to explain this stuff to you anyway?
//Haven't I done enough for you??!?!?
//Get off my back!!
typedef struct {
    char* Attribute1;
	char* spacer;
    char* String;
	char* spacer2;
    char* Attribute2;
	char* spacer3;
    char* SWATHNAME;
	char* spacer4;
    char* DIMENSIONNAME;
	char* spacer5;
    long SIZE;
	char* spacer6;
    char* DIMENSIONNAME2;
	char* spacer7;
    long SIZE2;
	char* spacer8;
    char* DIMENSIONNAME3;
	char* spacer9;
    long SIZE3;
	char* spacer10;
    char* DIMENSIONNAME4;
	char* spacer11;
    long SIZE4;
	char* spacer12;
    char* DIMENSIONNAME5;
	char* spacer13;
    long SIZE5;
	char* spacer14;
    char* GEODIMENSION;
	char* spacer15;
    char* DATADIMENSION;
	char* spacer16;
    long OFFSET;
    long INCREMENT;
	char* spacer17;
    char* GEODIMENSION2;
	char* spacer18;
    char* DATADIMENSION2;
	char* spacer19;
    long OFFSET2;
    long INCREMENT2;
	char* spacer20;
    char* GEODIMENSION3;
	char* spacer21;
    char* DATADIMENSION3;
	char* spacer22;
    long OFFSET3;
    long INCREMENT3;
	char* spacer23;
    char* GEOFIELDNAME;
	char* spacer24;
    char* DATATYPE;
	char* spacer25;
    char* DIMLIST;
	char* spacer26;
    char* GEOFIELDNAME2;
	char* spacer27;
    char* DATATYPE2;
	char* spacer28;
    char* DIMLIST2;
	char* spacer29;
    char* DATAFIELDNAME;
	char* spacer30;
    char* DATATYPE3;
	char* spacer31;
    char* DIMLIST3;
	char* spacer32;
    char* DATAFIELDNAME2;
	char* spacer33;
    char* DATATYPE4;
	char* spacer34;
    char* DIMLIST4;
	char* spacer35;
    char* DATAFIELDNAME3;
	char* spacer36;
    char* DATATYPE5;
	char* spacer37;
    char** DIMLIST5;
	char* spacer38;
    char* DATAFIELDNAME4;
	char* spacer39;
    char* DATATYPE6;
	char* spacer40;
    char** DIMLIST6;
	char* spacer41;
    char* SWATHNAME2;
	char* spacer42;
    char* DIMENSIONNAME6;
	char* spacer43;
    long SIZE6;
	char* spacer44;
    char* DIMENSIONNAME7;
	char* spacer45;
    long SIZE7;
	char* spacer46;
    char* DIMENSIONNAME8;
	char* spacer47;
    long SIZE8;
	char* spacer48;
    char* DIMENSIONNAME9;
	char* spacer49;
    long SIZE9;
	char* spacer50;
    char* GEODIMENSION4;
	char* spacer51;
    char* DATADIMENSION4;
	char* spacer52;
    long OFFSET4;
    long INCREMENT4;
	char* spacer53;
    char* GEODIMENSION5;
	char* spacer54;
    char* DATADIMENSION5;
	char* spacer55;
    long OFFSET5;
    long INCREMENT5;
	char* spacer56;
    char* GEOFIELDNAME3;
	char* spacer57;
    char* DATATYPE7;
	char* spacer58;
    char** DIMLIST7;
	char* spacer59;
    char* GEOFIELDNAME4;
	char* spacer60;
    char* DATATYPE8;
	char* spacer61;
    char** DIMLIST8;
	char* spacer62;
    char* DATAFIELDNAME5;
	char* spacer63;
    char* DATATYPE9;
	char* spacer64;
    char** DIMLIST9;
	char* spacer65;
    char* DATAFIELDNAME6;
	char* spacer66;
    char* DATATYPE10;
	char* spacer67;
    char** DIMLIST10;
	char* spacer68;
    char* DATAFIELDNAME7;
	char* spacer69;
    char* DATATYPE11;
	char* spacer70;
    char** DIMLIST11;
	char* spacer71;
    char* DATAFIELDNAME8;
	char* spacer72;
    char* DATATYPE12;
	char* spacer73;
    char** DIMLIST12;
	char* spacer74;
    char* DATAFIELDNAME9;
	char* spacer75;
    char* DATATYPE13;
	char* spacer76;
    char** DIMLIST13;
	char* spacer77;
    char* DATAFIELDNAME10;
	char* spacer78;
    char* DATATYPE14;
	char* spacer79;
    char** DIMLIST14;
	char* spacer80;
    char* SWATHNAME3;
	char* spacer81;
    char* DIMENSIONNAME10;
	char* spacer82;
    long SIZE10;
	char* spacer83;
    char* DIMENSIONNAME11;
	char* spacer84;
    long SIZE11;
	char* spacer85;
    char* DIMENSIONNAME12;
	char* spacer86;
    long SIZE12;
	char* spacer87;
    char* DIMENSIONNAME13;
	char* spacer88;
    long SIZE13;
	char* spacer89;
    char* GEODIMENSION6;
	char* spacer90;
    char* DATADIMENSION6;
	char* spacer91;
    long OFFSET6;
    long INCREMENT6;
	char* spacer92;
    char* GEODIMENSION7;
	char* spacer93;
    char* DATADIMENSION7;
	char* spacer94;
    long OFFSET7;
    long INCREMENT7;
	char* spacer95;
    char* GEOFIELDNAME5;
	char* spacer96;
    char* DATATYPE15;
	char* spacer97;
    char** DIMLIST15;
	char* spacer98;
    char* GEOFIELDNAME6;
	char* spacer99;
    char* DATATYPE16;
	char* spacer100;
    char** DIMLIST16;
	char* spacer101;
    char* DATAFIELDNAME11;
	char* spacer102;
    char* DATATYPE17;
	char* spacer103;
    char** DIMLIST17;
	char* spacer104;
    char* DATAFIELDNAME12;
	char* spacer105;
    char* DATATYPE18;
	char* spacer106;
    char** DIMLIST18;
	char* spacer107;
    char* DATAFIELDNAME13;
	char* spacer108;
    char* DATATYPE19;
	char* spacer109;
    char** DIMLIST19;
	char* spacer110;
    char* DATAFIELDNAME14;
	char* spacer111;
    char* DATATYPE20;
	char* spacer112;
    char** DIMLIST20;
	char* spacer113;
    char* DATAFIELDNAME15;
	char* spacer114;
    char* DATATYPE21;
	char* spacer115;
    char** DIMLIST21;
	char* spacer116;
    char* Attribute3;
	char* spacer117;
    char* GROUPTYPE;
	char* spacer118;
    char* IDOFASTERGDSDATAGRANULE;
	char* spacer119;
    char* RECEIVINGCENTER;
	char* spacer120;
    char* PROCESSINGCENTER;
	char* spacer121;
    char** SOURCEDATAPRODUCT;
	char* spacer122;
    char* SENSORNAME1;
	char* spacer123;
    double POINTINGANGLE1;
	char* spacer124;
    char* SETTINGTIMEOFPOINTING1;
	char* spacer125;
    char* SENSORNAME2;
	char* spacer126;
    double POINTINGANGLE2;
	char* spacer127;
    char* SETTINGTIMEOFPOINTING2;
	char* spacer128;
    char* SENSORNAME3;
	char* spacer129;
    double POINTINGANGLE3;
	char* spacer130;
    char* SETTINGTIMEOFPOINTING3;
	char* spacer131;
    char** GAIN1;
	char* spacer132;
    char** GAIN2;
	char* spacer133;
    char** GAIN3;
	char* spacer134;
    char** GAIN4;
	char* spacer135;
    char** GAIN5;
	char* spacer136;
    char** GAIN6;
	char* spacer137;
    char** GAIN7;
	char* spacer138;
    char** GAIN8;
	char* spacer139;
    char** GAIN9;
	char* spacer140;
    char** GAIN10;
	char* spacer141;
    char** GEOMETRICDBVERSION;
	char* spacer142;
    char** RADIOMETRICDBVERSION;
	char* spacer143;
    char** COARSEDEMVERSION;
	char* spacer144;
    long SCENECLOUDCOVERAGE;
    long QUADRANTCLOUDCOVERAGE[4];
	char* spacer145;
    char* ASTEROPERATIONMODE;
	char* spacer146;
    char* PROCESSEDBANDS;
	char* spacer147;
    char** ASTEROBSERVATIONMODE1;
	char* spacer148;
    char** ASTEROBSERVATIONMODE2;
	char* spacer149;
    char** ASTEROBSERVATIONMODE3;
	char* spacer150;
    char** ASTEROBSERVATIONMODE4;
	char* spacer151;
    long ASTERSCENEID[3];
    long ORBITNUMBER;
    long RECURRENTCYCLENUMBER[2];
	char* spacer152;
    char* FLYINGDIRECTION;
	char* spacer153;
    double SOLARDIRECTION[2];
    long SPATIALRESOLUTION[3];
    double SCENECENTER[2];
    double MAPORIENTATIONANGLE;
    double UPPERLEFT[2];
    double UPPERRIGHT[2];
    double LOWERLEFT[2];
    double LOWERRIGHT[2];
	char* spacer154;
    char* Attribute4;
	char* spacer155;
    char* GROUPTYPE2;
	char* spacer156;
    char** SENSORSHORTNAME;
	char* spacer157;
    char* IDOFASTERGDSDATABROWSE;
	char* spacer158;
    char* Attribute5;
	char* spacer159;
    char* GROUPTYPE3;
	char* spacer160;
    long IMAGEDATAINFORMATION1[3];
    long MINANDMAX1[2];
    double MEANANDSTD1[2];
    long MODEANDMEDIAN1[2];
    long NUMBEROFBADPIXELS1[2];
	char* spacer161;
    char* CORINTEL1;
	char* spacer162;
    char* CORPARA1;
	char* spacer163;
    char* RESMETHOD1;
	char* spacer164;
    char* MPMETHOD1;
	char* spacer165;
    double PROJECTIONPARAMETERS1[13];
    long UTMZONECODE1;
    double INCL1;
    double OFFSET1;
	char* spacer166;
    char* CONUNIT1;
	char* spacer167;
    long IMAGEDATAINFORMATION2[3];
    long MINANDMAX2[2];
    double MEANANDSTD2[2];
    long MODEANDMEDIAN2[2];
    long NUMBEROFBADPIXELS2[2];
	char* spacer168;
    char* CORINTEL2;
	char* spacer169;
    char* CORPARA2;
	char* spacer170;
    char* RESMETHOD2;
	char* spacer171;
    char* MPMETHOD2;
	char* spacer172;
    double PROJECTIONPARAMETERS2[13];
    long UTMZONECODE2;
    double INCL2;
    double OFFSET22;
	char* spacer173;
    char* CONUNIT2;
	char* spacer174;
    long IMAGEDATAINFORMATION3N[3];
    long MINANDMAX3N[2];
    double MEANANDSTD3N[2];
    long MODEANDMEDIAN3N[2];
    long NUMBEROFBADPIXELS3N[2];
	char* spacer175;
    char* CORINTEL3N;
	char* spacer176;
    char* CORPARA3N;
	char* spacer177;
    char* RESMETHOD3N;
	char* spacer178;
    char* MPMETHOD3N;
	char* spacer179;
    double PROJECTIONPARAMETERS3N[13];
    long UTMZONECODE3N;
    double INCL3N;
    double OFFSET3N;
	char* spacer180;
    char* CONUNIT3N;
	char* spacer181;
    long IMAGEDATAINFORMATION3B[3];
    long MINANDMAX3B[2];
    double MEANANDSTD3B[2];
    long MODEANDMEDIAN3B[2];
    long NUMBEROFBADPIXELS3B[2];
	char* spacer182;
    char* CORINTEL3B;
	char* spacer183;
    char* CORPARA3B;
	char* spacer184;
    char* RESMETHOD3B;
	char* spacer185;
    char* MPMETHOD3B;
	char* spacer186;
    double PROJECTIONPARAMETERS3B[13];
    long UTMZONECODE3B;
    double INCL3B;
    double OFFSET3B;
	char* spacer187;
    char* CONUNIT3B;
	char* spacer188;
    char* Attribute6;
	char* spacer189;
    char* GROUPTYPE4;
	char* spacer190;
    long IMAGEDATAINFORMATION4[3];
    long MINANDMAX4[2];
    double MEANANDSTD4[2];
    long MODEANDMEDIAN4[2];
    long NUMBEROFBADPIXELS4[2];
    long PROCESSINGFLAG4;
    long NUMBEROFMEASUREMENTS4;
    long MEASUREMENTPOINTNUMBER4;
    double AVERAGEOFFSET4[2];
    double STANDARDDEVIATIONOFFSET4[2];
    double THRESHOLD4[4];
    long PCTIMAGEMATCH4;
    double AVGCORRELCOEF4;
    double CTHLD4;
	char* spacer191;
    char* CORINTEL4;
	char* spacer192;
    char* CORPARA4;
	char* spacer193;
    char* RESMETHOD4;
	char* spacer194;
    char* MPMETHOD4;
	char* spacer195;
    double PROJECTIONPARAMETERS4[13];
    long UTMZONECODE4;
    double INCL4;
    double OFFSET42;
	char* spacer196;
    char* CONUNIT4;
	char* spacer197;
    long IMAGEDATAINFORMATION5[3];
    long MINANDMAX5[2];
    double MEANANDSTD5[2];
    long MODEANDMEDIAN5[2];
    long NUMBEROFBADPIXELS5[2];
    long PROCESSINGFLAG5;
    long NUMBEROFMEASUREMENTS5;
    long MEASUREMENTPOINTNUMBER5;
    double AVERAGEOFFSET5[2];
    double STANDARDDEVIATIONOFFSET5[2];
    double THRESHOLD5[4];
    long PCTIMAGEMATCH5;
    double AVGCORRELCOEF5;
    double CTHLD5;
	char* spacer198;
    char* CORINTEL5;
	char* spacer199;
    char* CORPARA5;
	char* spacer200;
    char* RESMETHOD5[2];
	char* spacer201;
    char* MPMETHOD5[3];
	char* spacer202;
    double PROJECTIONPARAMETERS5[13];
    long UTMZONECODE5;
    double INCL5;
    double OFFSET52;
	char* spacer203;
    char* CONUNIT5;
	char* spacer204;
    long IMAGEDATAINFORMATION6[3];
    long MINANDMAX6[2];
    double MEANANDSTD6[2];
    long MODEANDMEDIAN6[2];
    long NUMBEROFBADPIXELS6[2];
    long PROCESSINGFLAG6;
    long NUMBEROFMEASUREMENTS6;
    long MEASUREMENTPOINTNUMBER6;
    double AVERAGEOFFSET6[2];
    double STANDARDDEVIATIONOFFSET6[2];
    double THRESHOLD6[4];
    long PCTIMAGEMATCH6;
    double AVGCORRELCOEF6;
    double CTHLD6;
	char* spacer205;
    char* CORINTEL6;
	char* spacer206;
    char* CORPARA6;
	char* spacer207;
    char* RESMETHOD6;
	char* spacer208;
    char* MPMETHOD6;
	char* spacer209;
    double PROJECTIONPARAMETERS6[13];
    long UTMZONECODE6;
    double INCL6;
    double OFFSET62;
	char* spacer210;
    char* CONUNIT6;
	char* spacer211;
    long IMAGEDATAINFORMATION7[3];
    long MINANDMAX7[2];
    double MEANANDSTD7[2];
    long MODEANDMEDIAN7[2];
    long NUMBEROFBADPIXELS7[2];
    long PROCESSINGFLAG7;
    long NUMBEROFMEASUREMENTS7;
    long MEASUREMENTPOINTNUMBER7;
    double AVERAGEOFFSET7[2];
    double STANDARDDEVIATIONOFFSET7[2];
    double THRESHOLD7[4];
    long PCTIMAGEMATCH7;
    double AVGCORRELCOEF7;
    double CTHLD7;
    char* spacer212;
    char* CORINTEL7;
	char* spacer213;
    char* CORPARA7;
	char* spacer214;
    char* RESMETHOD7;
	char* spacer215;
    char* MPMETHOD7;
	char* spacer216;
    double PROJECTIONPARAMETERS7[13];
    long UTMZONECODE7;
    double INCL7;
    double OFFSET72;
	char* spacer217;
    char* CONUNIT7;
	char* spacer218;
    long IMAGEDATAINFORMATION8[3];
    long MINANDMAX8[2];
    double MEANANDSTD8[2];
    long MODEANDMEDIAN8[2];
    long NUMBEROFBADPIXELS8[2];
    long PROCESSINGFLAG8;
    long NUMBEROFMEASUREMENTS8;
    long MEASUREMENTPOINTNUMBER8;
    double AVERAGEOFFSET8[2];
    double STANDARDDEVIATIONOFFSET8[2];
    double THRESHOLD8[4];
    long PCTIMAGEMATCH8;
    double AVGCORRELCOEF8;
    double CTHLD8;
	char* spacer219;
    char* CORINTEL8;
	char* spacer220;
    char* CORPARA8;
	char* spacer221;
    char* RESMETHOD8;
	char* spacer222;
    char* MPMETHOD8;
	char* spacer223;
	double PROJECTIONPARAMETERS8[13];
    long UTMZONECODE8;
    double INCL8;
    double OFFSET8;
	char* spacer224;
    char* CONUNIT8;
	char* spacer225;
    long IMAGEDATAINFORMATION9[3];
    long MINANDMAX9[2];
    double MEANANDSTD9[2];
    long MODEANDMEDIAN9[2];
    long NUMBEROFBADPIXELS9[2];
    long PROCESSINGFLAG9;
    long NUMBEROFMEASUREMENTS9;
    long MEASUREMENTPOINTNUMBER9;
    double AVERAGEOFFSET9[2];
    double STANDARDDEVIATIONOFFSET9[2];
    double THRESHOLD9[4];
    long PCTIMAGEMATCH9;
    double AVGCORRELCOEF9;
    double CTHLD9;
	char* spacer226;
    char* CORINTEL9;
	char* spacer227;
    char* CORPARA9;
	char* spacer228;
    char* RESMETHOD9;
	char* spacer229;
    char* MPMETHOD9;
	char* spacer230;
    double PROJECTIONPARAMETERS9[13];
    long UTMZONECODE9;
    double INCL9;
    double OFFSET9;
	char* spacer231;
    char* CONUNIT9;
	char* spacer232;
    char* Attribute7;
	char* spacer233;
    char* GROUPTYPE5;
	char* spacer234;
    long IMAGEDATAINFORMATION10[3];
    long MINANDMAX10[2];
    double MEANANDSTD10[2];
    long MODEANDMEDIAN10[2];
    long NUMBEROFBADPIXELS10[2];
    long PROCESSINGFLAG10;
    long NUMBEROFMEASUREMENTS10;
    long MEASUREMENTPOINTNUMBER10;
    double AVERAGEOFFSET10[2];
    double STANDARDDEVIATIONOFFSET10[2];
    double THRESHOLD10[4];
	char* spacer235;
    char* CORINTEL10;
	char* spacer236;
    char* CORPARA10;
	char* spacer237;
    char* RESMETHOD10;
	char* spacer238;
    char* MPMETHOD10;
	char* spacer239;
    double PROJECTIONPARAMETERS10[13];
    long UTMZONECODE10;
    double INCL10;
    double OFFSET10;
	char* spacer240;
    char* CONUNIT10;
	char* spacer241;
    long IMAGEDATAINFORMATION11[3];
    long MINANDMAX11[2];
    double MEANANDSTD11[2];
    long MODEANDMEDIAN11[2];
    long NUMBEROFBADPIXELS11[2];
    long PROCESSINGFLAG11;
    long NUMBEROFMEASUREMENTS11;
    long MEASUREMENTPOINTNUMBER11;
    double AVERAGEOFFSET11[2];
    double STANDARDDEVIATIONOFFSET11[2];
    double THRESHOLD11[4];
	char* spacer242;
    char* CORINTEL11;
	char* spacer243;
    char* CORPARA11;
	char* spacer244;
    char* RESMETHOD11;
	char* spacer245;
    char* MPMETHOD11;
	char* spacer246;
    double PROJECTIONPARAMETERS11[13];
    long UTMZONECODE11;
    double INCL11;
    double OFFSET11;
	char* spacer247;
    char* CONUNIT11;
	char* spacer248;
    long IMAGEDATAINFORMATION12[3];
    long MINANDMAX12[2];
    double MEANANDSTD12[2];
    long MODEANDMEDIAN12[2];
    long NUMBEROFBADPIXELS12[2];
    long PROCESSINGFLAG12;
    long NUMBEROFMEASUREMENTS12;
    long MEASUREMENTPOINTNUMBER12;
    double AVERAGEOFFSET12[2];
    double STANDARDDEVIATIONOFFSET12[2];
    double THRESHOLD12[4];
	char* spacer249;
    char* CORINTEL12;
	char* spacer250;
    char* CORPARA12;
	char* spacer251;
    char* RESMETHOD12;
	char* spacer252;
    char* MPMETHOD12;
	char* spacer253;
    double PROJECTIONPARAMETERS12[13];
    long UTMZONECODE12;
    double INCL12;
    double OFFSET12;
	char* spacer254;
    char* CONUNIT12;
	char* spacer255;
    long IMAGEDATAINFORMATION13[3];
    long MINANDMAX13[2];
    double MEANANDSTD13[2];
    long MODEANDMEDIAN13[2];
    long NUMBEROFBADPIXELS13[2];
    long PROCESSINGFLAG13;
    long NUMBEROFMEASUREMENTS13;
    long MEASUREMENTPOINTNUMBER13;
    double AVERAGEOFFSET13[2];
    double STANDARDDEVIATIONOFFSET13[2];
    double THRESHOLD13[4];
	char* spacer256;
    char* CORINTEL13;
	char* spacer257;
    char* CORPARA13;
	char* spacer258;
    char* RESMETHOD13;
	char* spacer259;
    char* MPMETHOD13;
	char* spacer260;
    double PROJECTIONPARAMETERS13[13];
    long UTMZONECODE13;
    double INCL13;
    double OFFSET13;
	char* spacer261;
    char* CONUNIT13[10];
	char* spacer262;
    long IMAGEDATAINFORMATION14[3];
    long MINANDMAX14[2];
    double MEANANDSTD14[2];
    long MODEANDMEDIAN14[2];
    long NUMBEROFBADPIXELS14[2];
    long PROCESSINGFLAG14;
    long NUMBEROFMEASUREMENTS14;
    long MEASUREMENTPOINTNUMBER14;
    double AVERAGEOFFSET14[2];
    double STANDARDDEVIATIONOFFSET14[2];
    double THRESHOLD14[4];
	char* spacer263;
    char* CORINTEL14;
	char* spacer264;
    char* CORPARA14;
	char* spacer265;
    char* RESMETHOD14;
	char* spacer266;
    char* MPMETHOD14;
	char* spacer267;
    double PROJECTIONPARAMETERS14[13];
    long UTMZONECODE14;
    double INCL14;
    double OFFSET14;
	char* spacer268;
    char* CONUNIT14;
	char* spacer269;
    char* Attribute8;
	char* spacer270;
    char* GROUPTYPE6;
	char* spacer271;
    char* SHORTNAME;
	char* spacer272;
    double SIZEMBDATAGRANULE;
	char* spacer273;
    char* PRODUCTIONDATETIME;
	char* spacer274;
    char* PLATFORMSHORTNAME;
	char* spacer275;
    char* INSTRUMENTSHORTNAME;
	char* spacer276;
    char* REPROCESSINGACTUAL;
	char* spacer278;
    char* PGEVERSION;
	char* spacer279;
    char* PROCESSINGLEVELID;
	char* spacer280;
    char* MAPPROJECTIONNAME;
	char* spacer281;
    double WESTBOUNDINGCOORDINATE;
    double NORTHBOUNDINGCOORDINATE;
    double EASTBOUNDINGCOORDINATE;
    double SOUTHBOUNDINGCOORDINATE;
	char* spacer282;
    char* TIMEOFDAY;
	char* spacer283;
    char* CALENDARDATE;
	char* spacer284;
    char* FUTUREREVIEWDATE;
	char* spacer285;
    char* SCIENCEREVIEWDATE;
	char* spacer286;
    double QAPERCENTMISSINGDATA;
    double QAPERCENTOUTOFBOUNDSDATA;
    double QAPERCENTINTERPOLATEDDATA;
	char* spacer287;
    char* Attribute9;
	char* spacer288;
	unsigned short Time_Tag0[29];
    unsigned short Time_Tag1[29];
	unsigned short Time_Tag2[29];
    unsigned short Time_Tag3[29];
    short Primary_Header0[29];
    short Primary_Header1[29];
    short Primary_Header2[29];
	short Primary_Header3[29];
    short Primary_Header4[29];
    short Primary_Header5[29];
    short Secondary_Header0[29];
    short Secondary_Header1[29];
    short Secondary_Header2[29];
    short Secondary_Header3[29];
    short Secondary_Header4[29];
    short Secondary_Header5[29];
    short Secondary_Header6[29];
    short Secondary_Header7[29];
    short Flag_Byte[29];
    short Time_Conversion0[29];
    short Time_Conversion1[29];
    short Time_Conversion2[29];
    long Position0[29];
    long Position1[29];
    long Position2[29];
    long Velocity0[29];
    long Velocity1[29];
    long Velocity2[29];
    short Attitude_Angle0[29];
    short Attitude_Angle1[29];
    short Attitude_Angle2[29];
    short Attitude_Rate0[29];
    short Attitude_Rate1[29];
    short Attitude_Rate2[29];
    short Magnetic_Coil0[29];
    short Magnetic_Coil1[29];
    short Magnetic_Coil2[29];
    short Solar_Array[29];
    short Solar_Position0[29];
    short Solar_Position1[29];
    short Solar_Position2[29];
    short Moon_Position0[29];
    short Moon_Position1[29];
    short Moon_Position2[29];
} HDFGlobalAttributes;

static char* removeChar(const char* input, char toRemove);

#endif