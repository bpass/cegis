<?php
function getCred($file_loc="./ouser.php")
{
	$parsed=file($file_loc);
	$cred['user']=trim($parsed[0]);
	$cred['passhash']=trim($parsed[1]);
	$cred['passbirth']=trim($parsed[2]);
	$cred['db']=trim($parsed[3]);
	if($cred['passbirth']<=time()-2160)
 	{
		 $cred=ora_change_auth($cred, $file_loc);
	}
 	return $cred;
}

function ora_change_auth($cred, $file_loc)
{
	$pass_length=17;
	$sha_length=40;
	$error="";
	$new=mt_rand();
	$new=sha1($new);
	$start=mt_rand(0,$sha_length-$pass_length);
	$new=substr($new, $start, $start+$pass_length);
	$con=ocilogon($cred['user'],$cred['passhash'], $cred['db']);
	if($con && is_writable($file_loc))
	{
		if(ocipasswordchange($con, $cred['user'], 'ghgh456654AA', $new))
		{
			$cred['passhash']=$new;
			$cred['passbirth']=time();
			$newcred=implode("\n", $cred);
			if($handle=fopen($file_loc, 'w'))
			{
				if(fwrite($handle, $newcred))
				{
					fclose($handle);
				}
				else
					$error="Couldn't write to proper file location";
			}
			else
				$error="Couldn't open proper file.";
		}
		else
			$error="Security failure.";// change password on $cred['db'] for user $cred['user']
	}
	else
		$error="Couldn't allocate proper local resources."; //verify file $file_loc is writeable or couldn't connect to database $cred['user']
	print $error;
	return $cred;
}
?>
