<?php
require_once("auth_lib.php");
require_once("HTMLSyntaxChecker.php");

//This function is used to do proper oracle quote escaping
function oracleEscape(&$query, $searching) {
	     //replace the automatic escape style for quotes
	     //for the style used by oracle.
	     $query = str_replace("\'", "''", $query);
	     $query = str_replace('\"', '"', $query);
	     
	   
     }

	//This function prints the web form used to enter
	//image data into the database	
	function showImageForm() {
		if(!($GLOBALS['AUTH_ADMIN'] || $GLOBALS['AUTH_WRITE'])) {
			print "<p><b>You do not have permission to access this resource.</b><p>\n";
			return(0);
		} 		
		echo "<div class=\"spacer\">Add New Image Information</div>\n<form method=\"POST\" action=\"index.php?mode=doImgAdd\" onLoad=\"listSensors(document.forms[0].satellite);\">
				<p>
						<b>Date</b>&nbsp;<select name=\"month\">
						<option value=\"jan\" SELECTED>January
						<option value=\"feb\">February
						<option value=\"mar\">March
						<option value=\"apr\">April
						<option value=\"may\">May
						<option value=\"jun\">June
						<option value=\"jul\">July
						<option value=\"aug\">August
						<option value=\"sep\">September
						<option value=\"oct\">October
						<option value=\"nov\">November
						<option value=\"dec\">December
					</select>&nbsp;
					<input type=\"text\" name=\"day\" maxlength=\"2\" size=\"2\" value=\"dd\">&nbsp;
					<input type=\"text\" name=\"year\" maxlength=\"2\" size=\"2\" value=\"yy\">&nbsp;&nbsp; 
					<b>Satellite</b>&nbsp;<select name=\"satellite\" maxlength=\"50\" onChange=\"listSensors(document.forms[0].satellite);\">
							    <option value=\"-1\" SELECTED>
							    <option value=\"aster\">ASTER
							    <option value=\"ls5\">Landsat 5
							    <option value=\"ls7\">Landsat 7
							    <option value=\"eo1\">EO-1
							    <option value=\"amser-e\">AMSER-E
							    <option value=\"doq\">DOQ
							    </select>&nbsp;&nbsp;
							    <b>Sensor</b><select name=\"sensor\" onChange=\"setSpatialResolution(this.options[this.selectedIndex].value)\">
							    		  <option value=\"\">&nbsp;&nbsp;&nbsp;&nbsp;</select>
					<p>
					<b>Path</b>&nbsp;<input type=\"text\" name=\"path\" maxlength=\"3\" size=\"4\">&nbsp;
					<b>Row</b>&nbsp;<input type=\"text\" name=\"row\" maxlength=\"3\" size=\"4\">&nbsp;&nbsp;&nbsp;
					<b>Lat(N)</b>&nbsp;<input type=\"text\" name=\"lat\" maxlength=\"10\" size=\"4\">&nbsp;
					<b>Lon(W)</b>&nbsp;<input type=\"text\" name=\"lon\" maxlength=\"10\" size=\"4\">&nbsp;&nbsp;
					<b>Spatial Res (m)</b>&nbsp;<input type=\"text\" name=\"spat_r\" maxlength=\"10\" size=\"4\"><p>
					<b>Projection</b>&nbsp;<input type=\"text\" name=\"projection\" maxlength=\"100\">&nbsp;&nbsp;
						<b>Spheroid</b>&nbsp;<input type=\"text\" name=\"spheroid\" maxlength=\"100\">&nbsp;&nbsp;
						<b>Datum</b>&nbsp;<input type=\"text\" name=\"datum\" maxlength=\"100\"></td><p><p>
						<b>Comments</b><br>
						<textarea rows=\"5\" cols=\"60\" name=\"comments\"></textarea><p>
						<input type=\"submit\"></form>";
	}

	//This is the function responsible for both adding new image information
	//to the database, and for editing previously entered information.
	function doImgOp($operation, $id="") {
		if(!($GLOBALS['AUTH_ADMIN'] || $GLOBALS['AUTH_WRITE'])) {
			print "<p><b>You do not have permission to access this resource.</b><p>\n";
			return(0);
		}
		
		if(($id != "") && (!is_numeric($id) || ($id < 0))) {
			print "<p>Error: invalid ID</p>\n";
			return(0);
		}
		//pull out form values.
		$month = $_POST['month'];
		$day = $_POST['day'];
		$year = $_POST['year'];
		$sat = 	$_POST['satellite'];
		$sensor = $_POST['sensor'];
		$lat = $_POST['lat'];
		$lon = $_POST['lon'];
		$path = $_POST['path'];
		$row = $_POST['row'];
		$spat_r = $_POST['spat_r'];
		$proj = $_POST['projection'];
		$spheroid = $_POST['spheroid'];
		$datum = $_POST['datum'];
		$comments = $_POST['comments'];
		$errorList = array();
		$error = false;
		$errorCount = 0;
		
		//the formValues array is created in order to pass to
		//the editImage function in order to repopulate the form.
		$formValues = array();
		$formValues[0] = $sat;
		$formValues[1] = $sensor;
		$formValues[2] = $lat;
		$formValues[3] = $lon;
		$formValues[4] = $path;
		$formValues[5] = $row;
		$formValues[6] = $day;
		$formValues[7] = $month;
		$formValues[8] = $year;
			
		$formValues[9] = $spat_r;
		$formValues[10] = $proj;
		$formValues[11] = $datum;
		$formValues[12] = $spheroid;
		$formValues[13] = $comments;
		
		//Store the length of each month
		//and the names of each month in associative arrays
		//in order to check whether or not the day entered
		//was valid for a particular month.
		$month_lengths = array('jan' => 31,
							   'feb' => 28,
							   'mar' => 31,
							   'apr' => 30,
							   'may' => 31,
							   'jun' => 30,
							   'jul' => 31,
							   'aug' => 31,
							   'sep' => 30,
							   'oct' => 31,
							   'nov' => 30,
							   'dec' => 31);
							   
		$month_names = array('jan' => 'January',
							   'feb' => 'February',
							   'mar' => 'March',
							   'apr' => 'April',
							   'may' => 'May',
							   'jun' => 'June',
							   'jul' => 'July',
							   'aug' => 'August',
							   'sep' => 'September',
							   'oct' => 'October',
							   'nov' => 'November',
							   'dec' => 'December');
		$leapYear = false;
		
		//convert all strings to upper-case for database storage
		//this makes searching case-insensitive
		$sat = strtoupper($sat);
		$sensor = strtoupper($sensor);
		$proj = strtoupper($proj);
		$spheroid = strtoupper($spheroid);
		$datum = strtoupper($datum);
		
		//force month, day, and year to be 2 digit numbers			
		if(!preg_match('/[a-z]{2,2}/', $month)) {
			$errorList[$errorCount] = "<p>Invalid month entered</p>";
			$error = true;
			$errorCount++;
		}
		
		if(!preg_match('/\d{2,2}/', $day)) {
			$errorList[$errorCount] = "<p>Invalid day</p>";
			$error = true;
			$errorCount++;
			
		}
		
		if(!preg_match('/\d{2,2}/', $year)) {
			$errorList[$errorCount] = "<p>Invalid year</p>";
			$error = true;
			$errorCount++;
			
		}
		
		if(($year%4) == 0) {
			$leapYear = true;
		}
		
		//if we are in feb, then we need to check for a leap year
		//so that the user can enter day 29
		if($month == 'feb') {
		    
			
		    if(!$leapYear && $day > $month_lengths[$month]) {
			 	$errorList[$errorCount] = "<p>There are not that many days in {$month_names[$month]}.</p>";
				$error = true;
				$errorCount++;
				
		    }
			
		    else if($leapYear && $day > 29) {
			 	$errorList[$errorCount] = "<p>There are not that many days in {$month_names[$month]}.</p>";
				$error = true;
				$errorCount++;
				
		    }
		     	
		}
			 
		else {
			
			  //if the current day is out of range for the current month 
			 if($day > $month_lengths[$month]) {
			 	$errorList[$errorCount] = "<p>There are not that many days in {$month_names[$month]}.</p>";
				$error = true;
				$errorCount++;
				
			}
		}
		
		//format the date for storage in the database
		$date = "$day-$month-$year";
		
		
		//Check legality of all remaining fields, 
		//populate errorList array with any encountered errors.
		
		if(!empty($sat) && !preg_match('/^\s+$/s', $sat)) {
			if(!preg_match('/^[\w\s-]+$/', $sat)) {
				$errorList[$errorCount] = "<p>Only alphanumeric characters are allowed for the satellite name.</p>";
				$error = true;
				$errorCount++;
				
			}
		}
		
		else {
			$errorList[$errorCount] = "<p>Satellite cannot be blank</p>";
			$error = true;
			$errorCount++;
		}
		
		
		if(!(empty($lat) && empty($lon)) && (preg_match('/^\s+$/s', $lat) || preg_match('/^\s+$/s', $lon))) {
			$errorList[$errorCount] = "<p>You must either provide both latitude and longitude, or leave them both blank</p>";
			$error = true;
			$errorCount++;
			
		}
		
		if((!is_numeric($lat) || !is_numeric($lon)) && !(empty($lat) && empty($lon))) {
			$errorList[$errorCount] = "<p>Latitude and longitude must be entered in decimal degrees</p>";
			$error = true;
			$errorCount++;
			
		}
		
		if(!(empty($path) && empty($row)) && (preg_match('/^\s+$/s', $path) || preg_match('/^\s+$/s', $row))) {
			$errorList[$errorCount] = "<p>You must either provide both path and row, or leave them both blank</p>";
			$error = true;
			$errorCount++;
			
		}
		
		if((!is_numeric($path) || !is_numeric($row)) && !(empty($path) && empty($row))) {
			$errorList[$errorCount] = "<p>Path and row must be numeric</p>";
			$error = true;
			$errorCount++;
			
		}
		
		if($path < 0 || $row < 0 || $path > 999 || $row > 999) {
			$errorList[$errorCount] = "<p>Path and row must be positive 3-digit numbers</p>";
			$error = true;
			$errorCount++;
		}
		
		if((abs($lat) > 90) || (abs($lon) > 180)) {
			$errorList[$errorCount] = "<p>Coordinates out of range</p>";
			$error = true;
			$errorCount++;
			
		}
		
		if(!empty($spat_r) && !preg_match('/^\s+$/s', $spat_r)) {
			if(!is_numeric($spat_r)) {
				$errorList[$errorCount] = "<p>Spatial resolution must be numeric</p>";
				$error = true;
				$errorCount++;
				
			}
		}
		
		else if(!empty($spat_r) && preg_match('/^\s+$/s', $spat_r)){
			$errorList[$errorCount] = "<p>Spatial resolution cannot be blank</p>";
			$error = true;
			$errorCount++;
		}
			
		if(!empty($proj) && !preg_match('/^\s+$/s', $proj)) {
			if(!preg_match('/^[\w\s]+$/', $proj)) {
				$errorList[$errorCount] = "Projection must contain only alphanumeric characters</p>";
				$error = true;
				$errorCount++;
				
			}
		}
		if(!empty($spheroid) && !preg_match('/^\s+$/s', $spheroid)) {
			if(!preg_match('/^[\w\s]+$/', $spheroid)) {
				$errorList[$errorCount] = "Spheroid must contain only alphanumeric characters</p>";
				$error = true;
				$errorCount++;
				
			}	
		}
		
		if(!empty($datum) && !preg_match('/^\s+$/s', $datum)) {
			if(!preg_match('/^[\w\s]+$/', $datum)) {
				$errorList[$errorCount] = "Datum must contain only alphanumeric characters</p>";
				$error = true;
				$errorCount++;
				
			}
		}
		
		if(!empty($comments) ) {
			oracleEscape($comments, false);
			$comments = html_entity_decode($comments, ENT_QUOTES);
			$comments = htmlentities($comments, ENT_QUOTES);
			
		}
		
		//if the user chose to leave out the lat and lon, force them to 0
		if((empty($lat) && empty($lon)) || ((preg_match('/^\s+$/s', $lat) && preg_match('/^\s+$/s', $lon)))) {
				$lat = 0;
				$lon = 0;
		}
		
		//if the user chose to leave out the lat and lon, force them to 0
		if((empty($path) && empty($row)) || ((preg_match('/^\s+$/s', $path) && preg_match('/^\s+$/s', $row)))) {
				$path = 0;
				$row = 0;
		}
		
		//If there were problems 
		if($error) {
			foreach($errorList as $err) {
				print $err;
			}
			
			print "<div class=\"spacer\">&nbsp;</div>";
			
			//if there were problems when adding a new image,
			//call editImage to repopulate the form with the
			//values the user tried to enter, but don't pass
			//an image id since we don't have one yet.
			if($operation == "add") {
			
				editImage("", $formValues, "add");
				return(0);
			}
			
			//same as above, but pass our current id
			else if($operation == "edit") {
				editImage($id, $formValues, "edit");
				return(0);
			}
		}
		
	
		
		//build appropriate query string based on whether
		//we are adding or editing.
		
		if($operation == "add") {
					
			$query = "INSERT INTO mwilliams.image_info VALUES (mwilliams.id_seq.nextval, '$sat', '$sensor', $lat, $lon, $path, $row, '$date', $spat_r, '$proj', '$datum', '$spheroid', '$comments')";
			$confirmation = "<p>Image information added to database.</p>";
		}
		
		else if($operation == "edit") {
			$query = "UPDATE mwilliams.image_info 
			  SET SATELLITE='$sat',
			  SENSOR='$sensor',
			  LAT=$lat,
			  LON=$lon,
			  LS_PATH=$path,
			  LS_ROW=$row,
			  CAPTURE_DATE='$date',
			  SPAT_R=$spat_r,
			  PROJECTION='$proj',
			  DATUM='$datum',
			  SPHEROID='$spheroid',
			  COMMENTS='$comments'
			  WHERE ID=$id";
			  
			  $confirmation = "<p>Image values changed. Click <a href=\"./index.php?mode=viewImage&id=$id\">here</a> to view your changes.</p>";
		}

		//Get credentials, logon to database, and execute query.
		
		$cred=getCred();	
		$conn = OCILogon($cred['user'], $cred['passhash'], 'prod85');
		if(!$conn) {
			print "<p>Error connecting to database</p>";
			return(0);
		}
		
		
		$QO = OCIParse($conn, $query);
		$result = OCIExecute($QO);
		if(!$result) {
			print "<p>Error on image $operation</p>";
			return(0);
		}
		OCICommit($conn);
		OCILogoff($conn);
		
		print $confirmation;
		
				
	}
	
	//This function prints the list of images currently
	//stored in the database.		 	
	function showImages($index = 0) {
		$cred=getCred();		
		$conn = OCILogon($cred['user'], $cred['passhash'], 'prod85');
		if(!$conn) {
			print "<p>Error connecting to database</p>";
			return(0);
		}
		
		$countQ = "SELECT COUNT(ID) from mwilliams.image_info";
		$imageQ = "SELECT ID, capture_date, satellite, sensor, lat, lon, LS_PATH, LS_ROW from mwilliams.image_info ORDER BY ID DESC";
		
		$QO1 = OCIParse($conn, $countQ);
		$QO2 = OCIParse($conn, $imageQ);
		$result1 = OCIExecute($QO1);
		$result2 = OCIExecute($QO2);
		
		if(!$result1 || !$result2) {
			print "<p>Error in query execution</p>";
			return(0);
		}
		OCIFetchInto($QO1, $count);
		
		//How many results were returned?
		$max = $count[0];
		
		//how many rows did we get?
		$nrows = OCIFetchStatement($QO2, $result, $index, 8);
		
		print "<div class=\"spacer\"><h3>Currently Available Images</h3></div>\n";
		print "<br>\n";
		print "<div align=\"center\">\n";
		print "<table>\n";
		$id = $result['ID'];
		$date = $result['CAPTURE_DATE'];
		$sat = $result['SATELLITE'];
		$sens = $result['SENSOR'];
		$lat = $result['LAT'];
		$lon = $result['LON'];
		$path = $result['LS_PATH'];
		$row = $result['LS_ROW'];
		
		for($i = 0; $i < $nrows; $i++) {
			print "<tr><td><b>Capture Date</b></td><td><b>Satellite</b></td><td><b>Sensor</b></td><td><b>Path</b></td><td><b>Row</b></td><td><b>Latitude(N)</b></td><td><b>Longitude(W)</b></td></tr>\n";
			print "<tr><td>$date[$i]</td><td>$sat[$i]</td><td>$sens[$i]</td><td>$path[$i]</td><td>$row[$i]</td><td>$lat[$i]</td><td>$lon[$i]</td><td><a href=\"./index.php?mode=viewImage&id=$id[$i]\">Details</a></td>";
			if($GLOBALS['AUTH_ADMIN'] || $GLOBALS['AUTH_WRITE']) {
				print "<td><a href=\"./index.php?mode=editImage&id=$id[$i]\">Edit</a></td>";
			}
			
			print "</tr>\n";
			
		}
		
		print "</table>\n";
		
		//if we have more than 8 entries to display
		if(($max > 8)) {
			
			//get indices of next page, current page,
			//and previous page of entries.
			$next = $index+8;
			$current = $index;
			$prev = $index-8;
			
			//if this is not the last page of data
			if($next < $max) {
			
				//if we are on the first page, and we have more than 8 entries, print a next link
				if($current < 8) {
					print "<div align=\"center\"><a href=\"./index.php?mode=showImages&index=$next\">Next</a></div>\n";
				}
			
				//else we need both a next and prev link
				else {
				    
					print "<div align=\"center\"><a href=\"./index.php?mode=showImages&index=$prev\">Prev</a> | <a href=\"./index.php?mode=showImages&index=$next\">Next</a></div>\n";
				}
			}
			
			//else we need just a prev link
			else {
				print "<div align=\"center\"><a href=\"./index.php?mode=showImages&index=$prev\">Prev</a></div>\n";
				}
			
		}
		
		print "</div>";	
		
		OCILogoff($conn);
	}
		
		
	//This function pulls the image information out
	//of the database based on the image ID, and 
	//displays the information	
	function doViewImage($ID) {
	
		if(!is_numeric($ID) || ($ID < 0)) {
			print "<p>ID must be numeric and positive</p>";
			return(0);
		}
		
		$cred=getCred();
		$conn = OCILogon($cred['user'], $cred['passhash'], 'prod85');
		if(!$conn) {
			print "<p>Error connecting to database</p>";
			return(0);
		}
		
		$imageQ = "SELECT * from mwilliams.image_info where id=$ID";
		
		$QO = OCIParse($conn, $imageQ);
		$result = OCIExecute($QO);
		
		if(!$result) {
			print "<p>Error in query execution</p>";
			return(0);
		}
		OCIFetchStatement($QO, $result, 0, 1);
		$date = $result['CAPTURE_DATE'];
		$sat = $result['SATELLITE'];
		$sens = $result['SENSOR'];
		$spat_r = $result['SPAT_R'];
		$lat = $result['LAT'];
		$lon = $result['LON'];
		$path = $result['LS_PATH'];
		$row = $result['LS_ROW'];
		$proj = $result['PROJECTION'];
		$sph = $result['SPHEROID'];
		$dat = $result['DATUM'];
		$comments = $result['COMMENTS'];
		
		//Print information
		print "<div class=\"spacer\">Image Information&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
		if($GLOBALS['AUTH_ADMIN'] || $GLOBALS['AUTH_WRITE']) {
			print "<a href=\"./index.php?mode=editImage&id=$ID\">Edit</a>";
		}
		
		print "</div>\n";
		print "<br>";
		print "<div align=\"center\" width=\"100%\">\n";
		print "<table width=\"100%\" cellspacing=\"20\">";
		print "<tr><td><b>ID</b></td><td><b>Capture Date</b></td><td><b>Satellite</b></td><td><b>Sensor</b></td><td><b>Path</b></td><td><b>Row</b></td><td><b>Latitude(N)</b></td><td><b>Longitude(W)</b></td>";
		print "<td><b>Spatial Resolution</b></td></tr>\n";
		print "<tr><td>$ID</td><td>$date[0]</td><td>$sat[0]</td><td>$sens[0]</td><td>$path[0]</td><td>$row[0]</td><td>$lat[0]</td><td>$lon[0]</td><td>$spat_r[0]</td></tr></table>\n";
	
		print "<br>";
		print "<table>\n";
		print "<tr><td><b>Projection</b></td><td><b>Spheroid</b></td><td><b>Datum</b></td></tr>\n";
		print "<tr><td>$proj[0]</td><td>$sph[0]</td><td>$dat[0]</td></tr></table>\n";
	
		print "<br>";
		print "<div class=\"spacer\">Comments</div>\n";
		print "<br>";
		print "<pre>";
		print "$comments[0]</pre></div>\n";
		
		OCILogoff($conn);
	}
	
	//This function repopulates the image entry form,
	//so that the user can edit the values.
	
	//$id is the id of the image
	
	//$formValues is an array of values to populate the
	//form with, if this is null, then id is used to pull
	//the information from the database.
	
	//$operation controls which action string gets printed
	//for the form, if it is set to "add", then we send
	//the form data to doImgOp("add"), if it is set to "edit"
	//then we send the form data to doImgOp("edit", $id)
	function editImage($id, $formValues, $operation) {
		if(!($GLOBALS['AUTH_ADMIN'] || $GLOBALS['AUTH_WRITE'])) {
			print "<p><b>You do not have permission to access this resource.</b><p>\n";
			return(0);
		} 
		$sat = "";
		$sensor ="";
		$lat = "";
		$lon = "";
		$path = "";
		$row_val = "";
		$date = array();
		$spat_r = "";
		$proj = "";
		$datum = "";
		$spheroid = "";
		$comments = "";
		$action = "";
		$caption = "";
		if(!empty($id) && (!is_numeric($id) || ($id < 0))) {
			print "<p>Image ID must be numeric and positive</p>\n";
			return(0);
		}
		
		//if the user did not pass in predetermined form values
		//then pull them out of the database
		if(empty($formValues)) {
			$action = "./index.php?mode=doEditImage";
			$caption = "Edit Image Information";
			$query = "SELECT * from mwilliams.image_info where id=$id";
		
			$cred=getCred();
			$conn = OCILogon($cred['user'], $cred['passhash'], "prod85");
		
			if(!$conn) {
				print "<p>Error connecting to database</p>";
				return(0);
			}
		
			$QO = OCIParse($conn, $query);
		
			$result = OCIExecute($QO);
		
			if(!$result) {
				print "<p>Error retrieving image</p>";
				return(0);
			}
		
			$row = array();
			OCIFetchInto($QO, $row);
			
			
			//fill in required values
			$sat = $row[1];
			$sat = strtolower($sat);
			$sensor =$row[2];
			$sensor = strtolower($sensor);
			$lat = $row[3];
			$lon = $row[4];
			$path = $row[5];
			$row_val = $row[6];
			$date = explode("-",$row[7]);
			$date[1] = strtolower($date[1]);
			$spat_r = $row[8];
			
			//check optional values
			if(isset($row[9]))
			   $proj = $row[9];
			else
			   $proj = "";
			if(isset($row[10]))
			   $datum = $row[10];
			else
			   $datum="";
			if(isset($row[11]))
			   $spheroid = $row[11];
			else
			   $spheroid="";
			if(isset($row[12])) {
			   $comments = $row[12];
			   $comments = str_replace("<br>", "\n", $comments);
			 }
			else
			   $comments="";
			OCILogoff($conn);
		}
		
		//else we were given canned form values
		else {
		
			$sat = $formValues[0];
			oracleEscape($sat);
			$sensor = $formValues[1];
			oracleEscape($sensor);
			$lat = $formValues[2];
			oracleEscape($lat);
			$lon = $formValues[3];
			oracleEscape($lon);
			$path = $formValues[4];
			oracleEscape($path);
			$row_val = $formValues[5];
			oracleEscape($row);
			$date[0] = $formValues[6];
			$date[1] = $formValues[7];
			$date[2] = $formValues[8];
			oracleEscape($date);
			$spat_r = $formValues[9];
			oracleEscape($spat_r);
			$proj = $formValues[10];
			oracleEscape($proj);
			$datum = $formValues[11];
			oracleEscape($datum);
			$spheroid = $formValues[12];
			oracleEscape($spheroid);
			$comments = $formValues[13];
			oracleEscape($comments);
			
			
			if($operation == "add") {
				$action = "./index.php?mode=doImgAdd";
				$caption = "Add New Image Information";
			}
			
			else if($operation == "edit") {
				$action = "./index.php?mode=doEditImage";
				$caption = "Edit Image Information";
			}
		}
		
		//*************************************************************************
		//*The way the form is repopulated is by placing tags like %tag% wherever
		//*a value needs to go, we then use str_replace to put the proper values
		//*where they need to be.
		//*************************************************************************
		$formCode = "<div class=\"spacer\">$caption</div>
			     <form method=\"POST\" action=\"$action&id=$id\" onLoad=\"listSensors(document.forms[0].satellite);\">
				<p>
						<b>Date</b>&nbsp;<select name=\"month\">
						<option value=\"jan\" %jan%>January
						<option value=\"feb\" %feb%>February
						<option value=\"mar\" %mar%>March
						<option value=\"apr\" %apr%>April
						<option value=\"may\" %may%>May
						<option value=\"jun\" %jun%>June
						<option value=\"jul\" %jul%>July
						<option value=\"aug\" %aug%>August
						<option value=\"sep\" %sep%>September
						<option value=\"oct\" %oct%>October
						<option value=\"nov\" %nov%>November
						<option value=\"dec\" %dec%>December
					</select>&nbsp;
					<input type=\"text\" name=\"day\" maxlength=\"2\" size=\"2\" value=\"%day%\">&nbsp;
					<input type=\"text\" name=\"year\" maxlength=\"2\" size=\"2\" value=\"%year%\">&nbsp;&nbsp; 
					<b>Satellite</b>&nbsp;<select name=\"satellite\" maxlength=\"50\" onChange=\"listSensors(document.forms[0].satellite, false);\">
							    <option value=\"-1\">
							    <option value=\"aster\" %aster%>ASTER
							    <option value=\"ls5\" %ls5%>Landsat 5
							    <option value=\"ls7\" %ls7%>Landsat 7
							    <option value=\"eo1\" %eo1%>EO-1
							    <option value=\"amser-e\" %amser-e%>AMSER-E
							    <option value=\"doq\" %doq%>DOQ
							    </select>&nbsp;&nbsp;";
		$sensorListCode = " <b>Sensor</b><select name=\"sensor\" onChange=\"setSpatialResolution(this.options[this.selectedIndex].value)\">\n";
		
		//all of the $xOptions variables are used to properly populate the 
		//dropdown menus, based on what values were selected.
		//we have to do it this way as it was done via javascript before.
		$asterSensorOptions = "<option value=\"all\" %all%>All
					<option value=\"vnir\" %vnir%>VNIR
				       <option value=\"swir\" %swir%>SWIR
				       <option value=\"TIR\" %tir%>TIR\n";
		
		$ls7SensorOptions = "<option value =\"all\" %all%>All
				     <option value =\"etm\" %etm%>ETM
				     <option value =\"bw\" %bw%>BW\n";
				     
		$eo1SensorOptions = "<option value =\"hyperion\" %hyperion%>Hyperion
		                     <option value =\"ali\" %ali%>ALI\n";
		
		$doqSensorOptions = " <option value =\"all\" %all%>All
				     <option value =\"cir\" %cir%>CIR
				     <option value =\"bw\" %bw%>BW\n";
		
		$amserESensorOptions = "<option value =\"25km\" %25km%>25km
				        <option value =\"50km\" %50km%>50km\n";
		 
		 //populate sensor values based on what sat we have
		switch($sat) {
			case "aster":
				$sensorListCode .= $asterSensorOptions . "</select>";
				break;
			case "ls7":
				$sensorListCode .= $ls7SensorOptions . "</select>";
				break;
			case "eo1":
				$sensorListCode .= $eo1SensorOptions . "</select>";
				break;
			case "amser-e":
				$sensorListCode .= $amserESensorOptions . "</select>";
				break;
			case "doq":
				$sensorListCode .= $doqSensorOptions . "</select>";
				break;
			default:
				$sensorListCode .= "<option value=\"0\">&nbsp;</select>\n";
		}
		
		$formCode .= $sensorListCode;
		
		//finish form template
		$formCode .= "<p><b>Path</b>&nbsp;<input type=\"text\" name=\"path\" maxlength=\"3\" size=\"4\" value=\"%path%\">&nbsp;
					<b>Row</b>&nbsp;<input type=\"text\" name=\"row\" maxlength=\"3\" size=\"4\" value=\"%row%\">&nbsp;&nbsp;&nbsp;
				<b>Lat(N)</b>&nbsp;<input type=\"text\" name=\"lat\" maxlength=\"10\" size=\"4\" value=\"%lat%\">&nbsp;
				<b>Lon(W)</b>&nbsp;<input type=\"text\" name=\"lon\" maxlength=\"10\" size=\"4\" value=\"%lon%\">&nbsp;&nbsp;
				<b>Spatial Res (m)</b>&nbsp;<input type=\"text\" name=\"spat_r\" maxlength=\"10\" size=\"4\" value=\"%spat_r%\"><p>
				<b>Projection</b>&nbsp;<input type=\"text\" name=\"projection\" maxlength=\"100\" value=\"%projection%\">&nbsp;&nbsp;
					<b>Spheroid</b>&nbsp;<input type=\"text\" name=\"spheroid\" maxlength=\"100\" value=\"%spheroid%\">&nbsp;&nbsp;
					<b>Datum</b>&nbsp;<input type=\"text\" name=\"datum\" maxlength=\"100\" value=\"%datum%\"></td><p><p>
					<b>Comments</b><br>
					<textarea rows=\"5\" cols=\"60\" name=\"comments\">%comments%</textarea><p>
					<input type=\"submit\"></form>";
					
		
		
		//fill in date
		$formCode = str_replace("%$date[1]%", "SELECTED", $formCode);
		$formCode = str_replace("%day%", "$date[0]", $formCode);
		$formCode = str_replace("%year%", "$date[2]", $formCode);
		
		//fill in sat / sens
		$formCode = str_replace("%$sat%", "SELECTED", $formCode);
		$formCode = str_replace("%$sensor%", "SELECTED", $formCode);
		
		//fill in lat / lon and path / row
		$formCode = str_replace("%path%", "$path", $formCode);
		$formCode = str_replace("%row%", "$row_val", $formCode);
		$formCode = str_replace("%lat%", "$lat", $formCode);
		$formCode = str_replace("%lon%", "$lon", $formCode);
		
		//spat_r, proj, sphere, datum
		$formCode = str_replace("%spat_r%", $spat_r, $formCode);
		$formCode = str_replace("%projection%", "$proj", $formCode);
		$formCode = str_replace("%spheroid%", "$spheroid", $formCode);
		$formCode = str_replace("%datum%", "$datum", $formCode);
		$formCode = str_replace("%comments%", "$comments", $formCode);
		
		//strip out unused % % tags
		$formCode = preg_replace("/%.*%/", " ", $formCode);
		
		print "$formCode\n";
		
	}
	
	//This function simply prints out the form for performing 
	//searches	
	function printSearchForm() {
		print "<div class=\"spacer\">Search Images</div>";
		print "<br>";
		print "<form method=\"POST\" action=\"./index.php?mode=doSearch\">
			<table>
			<tr><td><h3>Month</h3></td><td><h3>Year</h3></td><td><h3>Satellite</h3></td><td><h3>Sensor</h3></td></tr>
			<tr><td><select name=\"month\">
						<option value=\"\" SELECTED>
						<option value=\"jan\">January
						<option value=\"feb\">February
						<option value=\"mar\">March
						<option value=\"apr\">April
						<option value=\"may\">May
						<option value=\"jun\">June
						<option value=\"jul\">July
						<option value=\"aug\">August
						<option value=\"sep\">September
						<option value=\"oct\">October
						<option value=\"nov\">November
						<option value=\"dec\">December
					</select></td>
				<td><input type=\"text\" name=\"year\" maxlength=\"2\" size=\"2\"></td>
				<td><select name=\"satellite\" maxlength=\"50\" onChange=\"listSensors(document.forms[0].satellite, true);\">
							    <option value=\"\" SELECTED>
							    <option value=\"aster\">ASTER
							    <option value=\"ls5\">Landsat 5
							    <option value=\"ls7\">Landsat 7
							    <option value=\"eo1\">EO-1
							    <option value=\"amser-e\">AMSER-E
							    <option value=\"doq\">DOQ
							    </select></td>
				<td><select name=\"sensor\" onChange=\"setSpatialResolution(this.options[this.selectedIndex].value)\">
							    		  <option value=\"\">&nbsp;&nbsp;&nbsp;&nbsp;</select>
				<td><input type=\"submit\" value=\"Search\"></td></table></form>";
	}
	
	//This function builds and executes a search query
	//based on what search options the user chose to fill in.
	function doSearch() {
		$month = $_POST['month'];
		$year = $_POST['year'];
		$sat = $_POST['satellite'];
		$sensor = $_POST['sensor'];
		$imageQ = "";
		$countQ = "";
		
		if(!empty($year) && (!is_numeric($year) || ($year < 0))) {
			print "<p>Year must be numeric</p>";
			return(0);
		}
		
		if(!($year >= 0) && (year <= 99)) {
			print "<p>Year must be between 0 and 99</p>";
			return(0);
		}
		
		//force everything to upper case to
		//make searches case-insensitive
		$month = strtoupper($month);
		$sat = strtoupper($sat);
		$sensor = strtoupper($sensor);
		
		
		//each if statement represents one of the possible input combinations,
		//the image query searches for all images matching the search criteria
		//the count query gets the number of rows returned.
		if((empty($month))  && (empty($year)) && (empty($sat)) && (empty($sensor))) {
		       $imageQ = "SELECT ID, capture_date, satellite, sensor, lat, lon from mwilliams.image_info ORDER BY ID DESC";
		       $countQ = "SELECT COUNT(ID) from mwilliams.image_info";
		}
		
		else if((empty($month))  && (empty($year)) && (empty($sat)) && !(empty($sensor))) {
		      $imageQ = "SELECT ID, capture_date, satellite, sensor, lat, lon from mwilliams.image_info WHERE SENSOR='$sensor' ORDER BY ID DESC";
		      $countQ = "SELECT COUNT(ID) from mwilliams.image_info WHERE SENSOR='$sensor'";
		
		}
		else if((empty($month))  && (empty($year)) && !(empty($sat)) && (empty($sensor))) {
			$imageQ = "SELECT ID, capture_date, satellite, sensor, lat, lon from mwilliams.image_info WHERE SATELLITE='$sat' ORDER BY ID DESC";
			 $countQ = "SELECT COUNT(ID) from mwilliams.image_info WHERE SATELLITE='$sat'";
		}
		else if((empty($month))  && (empty($year)) && !(empty($sat)) && !(empty($sensor))) {
			$imageQ = "SELECT ID, capture_date, satellite, sensor, lat, lon from mwilliams.image_info WHERE SATELLITE='$sat' AND SENSOR='$sensor' ORDER BY ID DESC";
			$countQ = "SELECT COUNT(ID) from mwilliams.image_info WHERE SATELLITE='$sat' AND SENSOR='$sensor'";
		}
		else if((empty($month))  && !(empty($year)) && (empty($sat)) && (empty($sensor))) {
			$imageQ = "SELECT ID, capture_date, satellite, sensor, lat, lon from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-%-$year' ORDER BY ID DESC";
			$countQ = "SELECT COUNT(ID) from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-%-$year'";
		}
		else if((empty($month))  && !(empty($year)) && (empty($sat)) && !(empty($sensor))) {
			$imageQ = "SELECT ID, capture_date, satellite, sensor, lat, lon from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-%-$year' AND SENSOR='$sensor' ORDER BY ID DESC";
			$countQ = "SELECT COUNT(ID) from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-%-$year' AND SENSOR='$sensor' ";
		}
		else if((empty($month))  && !(empty($year)) && !(empty($sat)) && (empty($sensor))) {
			$imageQ = "SELECT ID, capture_date, satellite, sensor, lat, lon from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-%-$year' AND SATELLITE='$sat' ORDER BY ID DESC";
			$countQ = "SELECT COUNT(ID) from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-%-$year' AND SATELLITE='$sat'";
		}
		else if((empty($month))  && !(empty($year)) && !(empty($sat)) && !(empty($sensor))) {
			$imageQ = "SELECT ID, capture_date, satellite, sensor, lat, lon from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-%-$year' AND SATELLITE='$sat' AND SENSOR='$sensor' ORDER BY ID DESC";
			$countQ = "SELECT COUNT(ID) from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-%-$year' AND SATELLITE='$sat' AND SENSOR='$sensor'";
		}
		else if(!(empty($month))  && (empty($year)) && (empty($sat)) && (empty($sensor))) {
			$imageQ = "SELECT ID, capture_date, satellite, sensor, lat, lon from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-$month-%' ORDER BY ID DESC";
			$countQ = "SELECT COUNT(ID) from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-$month-%'";
		}
		else if(!(empty($month))  && (empty($year)) && (empty($sat)) && !(empty($sensor))) {
			$imageQ = "SELECT ID, capture_date, satellite, sensor, lat, lon from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-$month-%' AND SENSOR='$sensor' ORDER BY ID DESC";
			$countQ = "SELECT COUNT(ID) from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-$month-%' AND SENSOR='$sensor'";
		}
		else if(!(empty($month))  && (empty($year)) && !(empty($sat)) && (empty($sensor))) {
			$imageQ = "SELECT ID, capture_date, satellite, sensor, lat, lon from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-$month-%' AND SATELLITE='$sat' ORDER BY ID DESC";
			$countQ = "SELECT COUNT(ID) from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-$month-%' AND SATELLITE='$sat'";
		}
		else if(!(empty($month))  && (empty($year)) && !(empty($sat)) && !(empty($sensor))) {
			$imageQ = "SELECT ID, capture_date, satellite, sensor, lat, lon from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-$month-%' AND SATELLITE='$sat' AND SENSOR='$sensor' ORDER BY ID DESC";
			$countQ = "SELECT COUNT(ID) from mwilliams.image_info  WHERE CAPTURE_DATE LIKE '%-$month-%' AND SATELLITE='$sat' AND SENSOR='$sensor'";
		}
		else if(!(empty($month))  && !(empty($year)) && (empty($sat)) && (empty($sensor))) {
			$imageQ = "SELECT ID, capture_date, satellite, sensor, lat, lon from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-$month-$year' ORDER BY ID DESC";
			$countQ = "SELECT COUNT(ID) from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-$month-$year'";
		}
		else if(!(empty($month))  && !(empty($year)) && (empty($sat)) && !(empty($sensor))) {
			$imageQ = "SELECT ID, capture_date, satellite, sensor, lat, lon from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-$month-$year' AND SENSOR='$sensor' ORDER BY ID DESC";
			$countQ = "SELECT COUNT(ID) from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-$month-$year' AND SENSOR='$sensor'";
		}
		else if(!(empty($month))  && !(empty($year)) && !(empty($sat)) && (empty($sensor))) {
			$imageQ = "SELECT ID, capture_date, satellite, sensor, lat, lon from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-$month-$year' AND SATELLITE='$sat' ORDER BY ID DESC";
			$countQ = "SELECT COUNT(ID) from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-$month-$year' AND SATELLITE='$sat'";
		}
		else if(!(empty($month))  && !(empty($year)) && !(empty($sat)) && !(empty($sensor))) {
			$imageQ = "SELECT ID, capture_date, satellite, sensor, lat, lon from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-$month-$year' AND SATELLITE='$sat' AND SENSOR='$sensor' ORDER BY ID DESC";
			$countQ = "SELECT COUNT(ID) from mwilliams.image_info WHERE CAPTURE_DATE LIKE '%-$month-$year' AND SATELLITE='$sat' AND SENSOR='$sensor'";
		}
		
		$cred=getCred();		
		$conn = OCILogon($cred['user'], $cred['passhash'], 'prod85');
		if(!$conn) {
			print "<p>Error connecting to database</p>";
			return(0);
		}
		
		
		
		$QO1 = OCIParse($conn, $countQ);
		$QO2 = OCIParse($conn, $imageQ);
		$result1 = OCIExecute($QO1);
		$result2 = OCIExecute($QO2);
		
		if(!$result1 || !$result2) {
			print "<p>Error in query execution</p>";
			return(0);
		}
		OCIFetchInto($QO1, $count);
		
		$max = $count[0];
		
		if(!isset($index))
		   $index=0;
		$nrows = OCIFetchStatement($QO2, $result, $index, 8);
		
		//Display images same as showImages, but using the results
		//of our search query
		print "<div class=\"spacer\"><h3>Currently Available Images</h3></div>\n";
		print "<br>\n";
		print "<div align=\"center\">\n";
		print "<table>\n";
		$id = $result['ID'];
		$date = $result['CAPTURE_DATE'];
		$sat = $result['SATELLITE'];
		$sens = $result['SENSOR'];
		$lat = $result['LAT'];
		$lon = $result['LON'];
		for($i = 0; $i < $nrows; $i++) {
			print "<tr><td><b>ID</b></td><td><b>Capture Date</b></td><td><b>Satellite</b></td><td><b>Sensor</b></td><td><b>Latitude</b></td><td><b>Longitude</b></td></tr>\n";
			print "<tr><td>$id[$i]</td><td>$date[$i]</td><td>$sat[$i]</td><td>$sens[$i]</td><td>$lat[$i]</td><td>$lon[$i]</td><td><a href=\"./index.php?mode=viewImage&id=$id[$i]\">Details</a></td>";
			if($GLOBALS['AUTH_WRITE'] || $GLOBALS['AUTH_ADMIN']) {
				print "<td><a href=\"./index.php?mode=editImage&id=$id[$i]\">Edit</a></td></tr>";
			}
		}
		
		print "</table>\n";
		
		
		if(($max > 8)) {
			$next = $index+8;
			$current = $index;
			$prev = $index-8;
			if($next < $max) {
			
				if($current < 8) {
					print "<div align=\"center\"><a href=\"./index.php?mode=showImages&index=$next\">Next</a></div>\n";
				}
			
				else {
				    
					print "<div align=\"center\"><a href=\"./index.php?mode=showImages&index=$prev\">Prev</a> | <a href=\"./index.php?mode=showImages&index=$next\">Next</a></div>\n";
				}
			}
			
			else {
				print "<div align=\"center\"><a href=\"./index.php?mode=showImages&index=$prev\">Prev</a></div>\n";
				}
			
		}
		
		print "</div>";	
		
		OCILogoff($conn);
	}	
		
	
	//print the form for entering site news
	function printNewsForm() {
		if(!($GLOBALS['AUTH_ADMIN'] || $GLOBALS['AUTH_WRITE'])) {
			print "<p><b>You do not have permission to access this resource.</b><p>\n";
			return(0);
		} 
		print "<div class=\"spacer\">Add Project News</div>
			<br>
			<form method=\"POST\" action=\"./index.php?mode=news\">
			<div align=\"center\">
			<table>
			<tr><td>&nbsp;</td></tr>
			<tr><td><input type=\"text\" name=\"subject\" value=\"Subject\" size=\"100\"></td></tr>
			<tr><td><textarea name=\"article\" rows=\"20\" cols=\"90\">Article</textarea></td></tr>
			<tr><td>&nbsp;</td></tr>
			<tr><td><input type=\"submit\" name=\"submit\" value=\"Submit\" ></td></tr>
			<tr><td><input type=\"submit\" name=\"submit\" value=\"Preview\" ></table></form></div>";
	}
	
	//Adds a news article to the db
	function doAddNews($article, $subject) {
		if(!($GLOBALS['AUTH_ADMIN'] || $GLOBALS['AUTH_WRITE'])) {
			print "<p><b>You do not have permission to access this resource.</b><p>\n";
			return(0);
		} 
		//strip all html tags from subject
		$subject = strip_tags($subject);
		if(preg_match('/^\s+$/s', $subject) || empty($subject)) {
			$subject = "No Subject";
		}
		
		//strip all tags from the article
		$article = strip_tags($article);
		
		$temp = removeEscapedQuotes($article);
		
		
		
		//use proper quotes for Oracle
		oracleEscape($article, false);
		oracleEscape($subject, false);
		$date = date('d') . '-' . date('M') . '-' . date('y');
		$time = date('h') . ':' . date('i') . ":" . date('s');
		$dateTime = $time . "/$date";
		
		

		$cred=getCred();		
		$conn = OCILogon($cred['user'], $cred['passhash'], "prod85");
		
		if(!$conn) {
			print "<p>Error connecting to database</p>";
			return(0);
		}
		
		$query = "INSERT INTO mwilliams.project_news VALUES (mwilliams.news_seq.nextval, to_date('$dateTime','hh:mi:ss/dd-mon-yyAM'), '$article', '$subject')";
		
		$QO = OCIParse($conn, $query);
		$result = OCIExecute($QO);
		
		if(!$result) {
			print "<p>Error on news insert</p>\n";
			return(0);
		}
		
		OCICommit($conn);
		OCILogoff($conn);
		print "<p>Story added, click <a href=\"./index.php\">here</a> to return to the main page</p>";
		
	}
	
	//Preview the news article before we submit it
	function doNewsPreview($article, $subject, $mode="", $id="") {
		if(!($GLOBALS['AUTH_ADMIN'] || $GLOBALS['AUTH_WRITE'])) {
			print "<p><b>You do not have permission to access this resource.</b><p>\n";
			return(0);
		} 
		if(!empty($id) && (!is_numeric($id) || ($id < 0))) {
			print "<p>Error, ID must be numeric and positive</p>\n";
			return(0);
		}
		$date = date('d') . '-' . date('m') . '-' . date('y');
		$subject = strip_tags($subject);
		$article = strip_tags($article);
		
		//take care of magic quotes
		$subject = removeEscapedQuotes($subject);
		$article = removeEscapedQuotes($article);
			
		
		print "<table style=\"width: 100%;\">
			<tr><td><div class=\"spacer\">$subject&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Posted: $date</div></td></tr>
			<tr><td><pre>$article</pre></td></tr></table>";
		
			
	        //replace regular quotes with these special ones to 
		//avoid the browser thinking we are trying to define tag attributes
		//we will change them back later
		$article = str_replace('"', "~q~", $article);
		$subject = str_replace('"', "~q~", $subject);	
		print "<form method=\"POST\" action=\"./index.php?mode=news\">
			<input type=\"hidden\" name=\"subject\" value=\"$subject\">
			<input type=\"hidden\" name=\"article\" value=\"$article\">
			<input type=\"hidden\" name=\"hidden\" value=\"$mode\">";
			if($mode == "edit" && isset($id)) {
				print "<input type=\"hidden\" name=\"id\" value=\"$id\">";
			}
			
			print "<input type=\"submit\" value=\"Submit\" name=\"submit\">
			<input type=\"submit\" value=\"Edit\" name=\"submit\"></form>\n";
		
		
	}
	
	//Edit a news article	
	function editNews($article, $subject, $op="preview", $id="") {
		if(!($GLOBALS['AUTH_ADMIN'] || $GLOBALS['AUTH_WRITE'])) {
			print "<p><b>You do not have permission to access this resource.</b></p>\n";
			return(0);
		}
		
		if(!empty($id) && (!is_numeric($id) || ($id < 0))) {
		
			return(0);
		} 
		$subject = removeEscapedQuotes($subject);
		$article = removeEscapedQuotes($article);
		
	
		
		$formCode = "";
		//If we are editing an article that has not been submitted yet
		//Use the passed values for $article and $subject
		if($op == "preview") {
			$formCode= "<div class=\"spacer\">Add Project News</div>
			 	   <br>
			 	   <form method=\"POST\" action=\"./index.php?mode=news\">
				    <div align=\"center\">
				    <table>
				    <tr><td>&nbsp;</td></tr>
				    <tr><td><input type=\"text\" name=\"subject\" value=\"$subject\" size=\"100\"></td></tr>
				    <tr><td><textarea name=\"article\" rows=\"20\" cols=\"90\">$article</textarea></td></tr>
				    <tr><td>&nbsp;</td></tr>
				    <tr><td><input type=\"submit\" name=\"submit\" value=\"Submit\" ></td></tr>
				    <tr><td><input type=\"submit\" name=\"submit\" value=\"Preview\" ></table>";
				    
				    //if id is set, then we are previewing an edit of an existing article,
				    //so pass along 'edit' and 'id' so that a new article is not added
				    if($id != "") {
				    	$formCode .= "<input type=\"hidden\" name=\"hidden\" value=\"edit\">
				    		      <input type=\"hidden\" name=\"id\" value=\"$id\">";
				    }
				    
				    $formCode.= "</div></form>";
		}
		
		//else if we are editing an allready existing article
		//used the passed ID to pull the needed info from the db
		else if($op == "editExisting") {
			
			$formCode= "<div class=\"spacer\">Edit Project News</div>
			 	   <br>
			 	   <form method=\"POST\" action=\"./index.php?mode=news\">
				    <div align=\"center\">
				    <table>
				    <tr><td>&nbsp;</td></tr>
				    <tr><td><input type=\"text\" name=\"subject\" value=\"$subject\" size=\"100\"></td></tr>
				    <tr><td><textarea name=\"article\" rows=\"20\" cols=\"90\">$article</textarea></td></tr>
				    <tr><td>&nbsp;</td></tr>
				    <tr><td><input type=\"submit\" name=\"submit\" value=\"Submit\" ></td></tr>
				    <tr><td><input type=\"submit\" name=\"submit\" value=\"Preview\" ></table>
				    <input type=\"hidden\" name=\"hidden\" value=\"edit\">
				    <input type=\"hidden\" name=\"id\" value=\"$id\"></form></div>";
		}
		print $formCode;
		
	}
	
	function getNewsStory($id) {
	
		if(!empty($id) && (!is_numeric($id) || ($id < 0))) {
			print "<p>Error, ID must be numeric and positive</p>\n";
			return(0);
		}
		$cred=getCred();
		$conn = OCILogon($cred['user'], $cred['passhash'], 'prod85');
		if(!$conn) {
			print "<p>Error connecting to database</p>";
			return(0);
		}
			
		$query = "SELECT article, subject from mwilliams.project_news WHERE id=$id";
			
		$QO = OCIParse($conn, $query);
		$result = OCIExecute($QO);
		
		if(!$result) {
			print "<p>Error getting article</p>";
			return(0);
		}
		
		if(!OCIFetchInto($QO, $row)) {
			print "<p>Article not found</p>";
			return(0);
		}
		
		$article = $row[0];
		$subject = $row[1];
		$return = array($subject, $article);
		OCILogoff($conn);
		return $return;
	}
			
	//Submit the edited article to the db
	function doEditNews($id, $article, $subject) {
		if(!($GLOBALS['AUTH_ADMIN'] || $GLOBALS['AUTH_WRITE']) ){
			print "<p><b>You do not have permission to access this resource.</b><p>\n";
			return(0);
		} 
		
		if(!empty($id) && (!is_numeric($id) || ($id < 0))) {
			print "<p>Error, ID must be numeric and positive</p>\n";
			return(0);
		}
		//strip all html tags from subject
		$subject = strip_tags($subject);
		if(preg_match('/^\s+$/s', $subject) || empty($subject)) {
			$subject = "No Subject";
		}
		
		//strip all tags but those in the list from article and replace newlines with <br>
		$article = strip_tags($article);
		
		//use proper quotes for Oracle
		oracleEscape($article, false);
		oracleEscape($subject, false);
		

		$cred=getCred();		
		$conn = OCILogon($cred['user'], $cred['passhash'], "prod85");
		
		if(!$conn) {
			print "<p>Error connecting to database</p>";
			return(0);
		}
		
		$query = "UPDATE mwilliams.project_news 
			  SET article = '$article', 
			  subject = '$subject'
			  WHERE id = $id";
		
		$QO = OCIParse($conn, $query);
		$result = OCIExecute($QO);
		
		if(!$result) {
			print "<p>Error on news edit</p>\n";
			return(0);
		}
		
		OCICommit($conn);
		OCILogoff($conn);
		print "<p>Story changed, click <a href=\"./index.php?mode=viewArticle&id=$id\">here</a> to view.</p>";
	 
	 }	
	 
	//Displays the most recent news articles on the main page		
	function doMain($index) {
		if(!empty($index) && (!is_numeric($index) || ($index < 0))) {
			print "<p>Error, index must be numeric and positive\n";
			return(0);
		}
		$displayMax = 11;
		
		$cred=getCred();
		$conn = OCILogon($cred['user'], $cred['passhash'], 'prod85');
		if(!$conn) {
			print "<p>Error connecting to database</p>";
			return(0);
		}
		
		$countQ = "select count(id) from mwilliams.project_news";
		$newsQ = "SELECT id, post_date, subject, article FROM mwilliams.project_news ORDER BY id DESC";
		
		$QO1 = OCIParse($conn, $countQ);
		$QO2 = OCIParse($conn, $newsQ);
		$result1 = OCIExecute($QO1);
		$result2 = OCIExecute($QO2);
		
		//if(!$result1 || !$result2) {
		//	print "<p>Error in query execution</p>";
		//	return(0);
		//}
		OCIFetchInto($QO1, $count);
		
		$max = $count[0];
		
		
		$nrows = OCIFetchStatement($QO2, $result, $index,$displayMax);
		
		print "<div class=\"spacer\"><h3>Project News</h3></div>\n";
		print "<br>\n";
		print "<div align=\"center\">\n";
		print "<table>\n";
		$id = $result['ID'];
		$date = $result['POST_DATE'];
		$subject = $result['SUBJECT'];
		$article = $result['ARTICLE'];
		
		for($i = 0; $i < $nrows; $i++) {
			
			print "<table>
			<tr><td><div class=\"spacer\"><a href=\"./index.php?mode=viewArticle&id=$id[$i]\">$subject[$i]</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Posted: $date[$i]</div></td></tr>";
			
			print "<br>";
		}
		
		print "</table>\n";
		
		//Same method used in showImages to display only a certain number 
		//if items on one page.
		if(($max > $displayMax)) {
			$next = $index+$displayMax;
			$current = $index;
			$prev = $index-$displayMax;
			if($next < $max) {
			
				if($current < $displayMax) {
					print "<div align=\"center\"><a href=\"./index.php?index=$next\">Next</a></div>\n";
				}
			
				else {
				    
					print "<div align=\"center\"><a href=\"./index.php?index=$prev\">Prev</a> | <a href=\"./index.php?index=$next\">Next</a></div>\n";
				}
			}
			
			else {
				print "<div align=\"center\"><a href=\"./index.php?index=$prev\">Prev</a></div>\n";
				}
			
		}	
		print "</div>";			
		OCILogoff($conn);	
	}
	
	//View the whole news article.
	function doViewArticle($id) {
		if(!empty($id) && (!is_numeric($id) || ($id < 0))) {
			print "<p>Id must be numeric</p>";
			return(0);
		}
		
		$cred=getCred();		
		$conn = OCILogon($cred['user'], $cred['passhash'], 'prod85');
		if(!$conn) {
			print "<p>Error connecting to database</p>";
			return(0);
		}
		
		$query = "select article, subject, post_date from mwilliams.project_news where id=$id";
		
		$QO = OCIParse($conn, $query);
		$result = OCIExecute($QO);
		
		if(!$result) {
			print "<p>Error getting article</p>";
			return(0);
		}
		
		if(!OCIFetchInto($QO, $row)) {
			print "<p>Article not found</p>";
			return(0);
		}
		
		$article = $row[0];
		$subject = $row[1];
		$date = $row[2];
		
		print "<div class=\"spacer\">$subject&nbsp;&nbsp;&nbsp;Post Date: $date&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
		
		if($GLOBALS['AUTH_ADMIN'] || $GLOBALS['AUTH_WRITE']) {
			print "<a href=\"./index.php?mode=news&id=$id&thing=editExisting\">Edit</a>";
		}
		
		print "</div>\n";
		
		print "<pre>";
		print "<p>$article</p>\n";
		print "</pre";
		OCILogoff($conn);
	}
	
	function removeEscapedQuotes($text) {
		$text = str_replace("\'", "'", $text);
		$text = str_replace('\"', '"', $text);
		return($text);
	}	
?>
