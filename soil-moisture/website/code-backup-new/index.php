<?php

//get everything I need
require('/opt/oracle/product/mid904/Apache/Apache/htdocs/sso_php/authenticate.php');
$file_loc="/opt/oracle/product/mid904/Apache/Apache/htdocs/SMP/ACL.php";	
require('common.php');
$mode = "";
$index = "";
$id = "";
//what am I doing?
if(isset($_GET['mode']))
	$mode = $_GET['mode'];

if(isset($_GET['index']))
	$index = $_GET['index'];

if($mode=="search"  || $mode=="showImages" || $mode=="doSearch" || $mode=="viewImage")
	$auth_type="read";
else if($mode=="addImage" || $mode=="editImage" || $mode=="news" || $mode=="doImgAdd" || $mode=="editImage")
	$auth_type="write";
else
	$auth_type="read";
checkAuthenticatedProxy($file_loc, $auth_type);

/*-------------------------------------------------
----If you get this far, you are authenticated.----
-------------------------------------------------*/

include("./head.php");
require_once("/opt/oracle/product/mid904/Apache/Apache/htdocs/sso_php/authenticate.php");
echo ssoLogoutLinkProxy("http://www.usgs.gov")," </td></tr></table></div><div id='content'><p><tr><td>";

switch($mode) {
      case 'addImage':
	      showImageForm();
	      break;
      case 'doImgAdd':
	      doImgOp("add");
	      break;
      case 'editImage':
	    
      	  if(isset($_GET['id'])) {      
	     		 $id = $_GET['id'];
		  	}	 
	      if(is_numeric($id)) {
		      editImage($id, "", "");
	      }
	      break;
      case 'doEditImage':
	    
      	  if(isset($_GET['id'])) {      
	     		 $id = $_GET['id'];
				 }
	      if(is_numeric($id)) {
		      doImgOp("edit", $id);
	      }
	      break;
      case 'delete':
	     
      	  if(isset($_GET['id'])) {      
	     		 $id = $_GET['id'];
				 }
	      if(is_numeric($id)) {
		      doDeleteRequest($id);
	      }
	      break;
      case 'showImages':
	      if($index == "") {
		      showImages();
	      }

	      else {
		      showImages($index);
	      }
	      break;
      case 'viewImage':
	       if(isset($_GET['id'])) {      
	     		 $id = $_GET['id'];
				 }
	      if(is_numeric($id)) {
		      doViewImage($id) ;
	      }
	      break;
      case 'search':
	      printSearchForm();
	      break;
      case 'doSearch';
	      doSearch();
	      break;
	      
      //ok this part's a little weird, 
      //just bare with me
      case 'news':
	  	  $thing = "";
		  $id = "";
	      if(isset($_GET['thing'])) {
		      $thing = $_GET['thing'];
		  }
		  
	      //Do we want to print the news form,
	      //or do some operations on a news article?
	      if($thing == "printForm") {
		      printNewsForm();
	      }
	      
	      else if($thing == "editExisting") {
	      		if(isset($_GET['id'])) {
				$id = $_GET['id'];
			}
			
			//pull story from DB
			$results = getNewsStory($id);
			editNews($results[1], $results[0], $thing, $id);
		}
				
	      //else we are doing an operation
	      else {
		      $action = $_POST['submit'];
		      $subject = $_POST['subject'];

		      $article = $_POST['article'];
		      
		     //if the user clicked preview from the edit or the
		     //add new story forms.
		     if($action == "Preview") {
		     	      $previewMode = "";
			
			      
			      //check if we are previewing an edit or a 
			      //new image
		              if(isset($_POST["hidden"])) {
			      	   $previewMode = $_POST["hidden"];
				       $id = $_POST["id"];
			       }
			       
			       else {
			            $previewMode = "";
			       }
			       
			      $subject = strip_tags($subject);
			      $article = html_entity_decode($article, ENT_QUOTES);
			      $article = htmlentities($article, ENT_QUOTES);
			      doNewsPreview($article, $subject, $previewMode, $id);
		      }
		      
		       //else if we are editing a preview of a new story we are
		      //adding.
		      else if($action == "Edit") {
		      		$op = "";
				
				
		      	      if(isset($_POST['hidden'])) {
			      	$op = $_POST['hidden'];
				}
			      
			      if(isset($_POST['id'])) {
			        $id = $_POST['id'];
				}
				
			      $subject = str_replace("~q~", '\"', $subject);
			
			      $article = str_replace("~q~", '"', $article);
			      $article = html_entity_decode($article, ENT_QUOTES);
			      $article = htmlentities($article, ENT_QUOTES);
			      
			     
			      
			      //if op is set to edit, we are editing a preview of
			      //a change to an existing article, so we must pass
			      //"preview" to the editNews function
			      if($op == "edit" || $op == "") {
			      	editNews($article, $subject, "preview", $id);
			      }
			      
			      else {
			      	editNews($article, $subject, $op, $id);
			      }
		      }
		      
		     //else if we are submiting an edit of an existing story
		     else if($_POST["hidden"] == "edit" && $action == "Submit") {
			    
			    if(isset($_POST['id'])) { 
			        $id = $_POST["id"];
			    }
				
			    if(is_numeric($id)) {
				   $subject = str_replace("~q~", '\"', $subject);
			           $article = str_replace("~q~", '\"', $article);
				   $article = html_entity_decode($article, ENT_QUOTES);
				   $article = htmlentities($article, ENT_QUOTES);
			
				   doEditNews($id, $article, $subject);
			    }
			   
		     }
		     
		     //else if we are submitting a new story
		      else if($action == "Submit") {
			      $subject = str_replace("~q~", '\"', $subject);
			      $article = str_replace("~q~", '\"', $article);
			      $article = html_entity_decode($article, ENT_QUOTES);
			      $article = htmlentities($article, ENT_QUOTES);
			      doAddNews($article, $subject);
		      }

		     
	      }
	      break;
	      
      
      		
      case 'viewArticle':
	      if(isset($_GET['id'])) {      
	     	$id = $_GET['id'];
		 }
	      if(is_numeric($id)) {
		      doViewArticle($id);
	      }
	      break;
      default:
	      //$index = $_SERVER['PHP_SELF'];
	      doMain($index);

}
	
include("tail.php");
?>
