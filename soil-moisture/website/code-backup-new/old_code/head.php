<html>
<head><title>Soil Moisture Project</title>
<link rel="stylesheet" href="./style.css">

<script language="JavaScript">
<!-- Hide
var landsat7Sensors = new Array(2);
var landsat7SensorValues = new Array(2);
var asterSensors = new Array(3);
var asterSensorValues = new Array(3);
var eo1Sensors = new Array(2);
var eo1SensorValues = new Array(2);
var doqSensors = new Array(2);
var doqSensorValues = new Array(2);
var amserESensors = new Array(2);


var sensorResolutions = new Array();

landsat7Sensors[0] = "ETM";
landsat7Sensors[1] = "BW";
landsat7SensorValues[0] = "etm";
landsat7SensorValues[1] = "bw";

asterSensors[0] = "VNIR";
asterSensors[1] = "SWIR";
asterSensors[2] = "TIR";
asterSensorValues[0] = "vnir";
asterSensorValues[1] = "swir";
asterSensorValues[2] = "tir";


eo1Sensors[0] = "Hyperion";
eo1Sensors[1] = "ALI";
eo1SensorValues[0] = "hyperion";
eo1SensorValues[1] = "ali";

doqSensors[0] = "CIR";
doqSensors[1] = "BW";
doqSensorValues[0] = "cir";
doqSensorValues[1] = "bw";

amserESensors[0] = "25km";
amserESensors[1] = "50km";

sensorResolutions[landsat7SensorValues[0]] = 28.5;
sensorResolutions[landsat7SensorValues[1]] = 15;
sensorResolutions["ls5"] = 28.5;
sensorResolutions[asterSensorValues[0]] = 15;
sensorResolutions[asterSensorValues[1]] = 30;
sensorResolutions[asterSensorValues[2]] = 90;
sensorResolutions[eo1SensorValues[0]] = 30;
sensorResolutions[eo1SensorValues[1]] = 30;
sensorResolutions[doqSensorValues[0]] = 1;
sensorResolutions[doqSensorValues[1]] = 1;
sensorResolutions[amserESensors[0]] = 25000;
sensorResolutions[amserESensors[1]] = 50000;

function setSpatialResolution(sensorValue) {
	
	if((sensorValue == "bw") && (document.forms[0].satellite.options[document.forms[0].satellite.selectedIndex].value == "ls7")) {
		document.forms[0].spat_r.value = 15;
	}
	
	else if(sensorValue != "0") {
		document.forms[0].spat_r.value = sensorResolutions[sensorValue];
	}
	else {
		document.forms[0].spat_r.value = "";
	}
}

function listSensors(currentList, searching) {
	var currentSat = currentList.options[currentList.selectedIndex].value;
	var sensorList = document.forms[0].sensor;
	var i = 0;
	for(i = 0; i <= sensorList.length; i++) {
		sensorList.options[i] = null;
	}
	
	sensorList.selectedIndex = -1;
	sensorList.options[0] = new Option(" ");
	sensorList.options[0].value = " ";
	if(currentSat == "0") {
		setSpatialResolution("0");
		return(0);
	}
	
	else if(currentSat == "ls5") {
		setSpatialResolution("ls5");
		return(0);
	}
	
	else if(currentSat == "aster") {
		
		for(i = 0; i < 3; i++) {
			sensorList.options[i] = new Option(asterSensors[i] );
			sensorList.options[i].value = asterSensorValues[i];
		}
		
		if(searching) {
			sensorList.options[i] = new Option("All");
			sensorList.options[i].value = "";
		}	
		sensorList.selectedIndex = 0;
		
	}
	
	else if(currentSat == "ls7") {
	
		for(i = 0; i < 2; i++) {
			sensorList.options[i] = new Option(landsat7Sensors[i]);
			sensorList.options[i].value = landsat7SensorValues[i];
		}
		if(searching) {
			sensorList.options[i] = new Option("All");
			sensorList.options[i].value = "";
		}
		sensorList.selectedIndex = 0;
	}
	
	else if(currentSat == "eo1") {
	
		for(i = 0; i < 2; i++) {
			sensorList.options[i] = new Option(eo1Sensors[i]);
			sensorList.options[i].value = eo1SensorValues[i];
		}
		if(searching) {
			sensorList.options[i] = new Option("All");
			sensorList.options[i].value = "";
		}
		sensorList.selectedIndex = 0;
	}
	
	else if(currentSat == "doq") {
	
		for(i = 0; i < 2; i++) {
			sensorList.options[i] = new Option(doqSensors[i]);
			sensorList.options[i].value = doqSensorValues[i];
		}
		if(searching) {
			sensorList.options[i] = new Option("All");
			sensorList.options[i].value = "";
		}
		sensorList.selectedIndex = 0;
	}
	
	else if(currentSat == "amser-e") {
		for(i = 0; i < 2; i++) {
			sensorList.options[i] = new Option(amserESensors[i]);
			sensorList.options[i].value = amserESensors[i];
		}
		if(searching) {
			sensorList.options[i] = new Option("All");
			sensorList.options[i].value = "";
		}
		sensorList.selectedIndex = 0;
	}
	
	setSpatialResolution(sensorList.options[0].value);	
}


	

//-->
</script>
</head>	
<body>
<a href="http://www.usgs.gov/"><img alt="Link to the USGS Home Page" src="./images/idbksm.gif" border=0></a><br>
<div id="logo"></div>
<div id="nav">
<div class="spacer">Navigation</div>
<table>
<?php 
  if($GLOBALS['AUTH_ADMIN'] || $GLOBALS['AUTH_WRITE']) {
  	print " <tr><td>
		<a href=\"./index.php?mode=addImage\">Add New Image</a></tr></td>
	  	<tr><td><a href=\"./index.php?mode=news&thing=printForm\">Add News</a></tr></td>";
   }
?>
<tr><td><a href="./index.php?mode=search">Search Images</a></tr></td>
<tr><td><a href="./index.php?mode=showImages">List Images</a></tr></td>
 <tr><td><a href="./index.php">Home</a></tr></td>
<tr><td>

