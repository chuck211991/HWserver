<?php
/***************************************************************************
* This script is brought to you by Vasplus Programming Blog by whom all 
	copyrights are reserved.
* Website: www.vasplus.info
* Email: info@vasplus.info
* Please, do not remove this information from the top of this page.
****************************************************************************/

session_start();

//set 30min time-out
$inactive = 1800; // 30 minutes
if(isset($_SESSION['timeout'])) 
{
	$session_life = time() - $_SESSION['timeout'];
	if($session_life > $inactive)
	{
		session_unset();
		session_destroy(); 
	}
}
$_SESSION['timeout'] = time();
ini_set('error_reporting', E_NONE);
$server_or_host = $_SERVER['HTTP_HOST'];

//Please leave this field alone. Do not touch it otherwise you will have 
//problems with Error Message Displays on your system
$vpb_error = ''; 
			  
//database
include "vpb_database.php"; 
$vpb_server_path = getcwd();

//Check to see that the file name to save and retrieve user details is set 
//and that the file exist otherwise, set and create the file to avoid errors
if(name_of_file_to_save_user_details == "")
{
	if(file_exists($vpb_server_path."/vpb_database.txt"))
	{
		$vpb_array_line = array();
		array_unshift( $vpb_array_line, 
		"<?php \ndefine('name_of_file_to_save_user_details', 'vpb_database.txt');\n?>" );
		$vpb_file_ip = implode("\n",$vpb_array_line);

		$vpb_create_write_file = @fopen("vpb_database.php", "w");
		if ($vpb_create_write_file) 
		{
		  fwrite($vpb_create_write_file, $vpb_file_ip);
		  @fclose($vpb_create_write_file);
		}
	}
	else
	{
		$vpb_array_line = array();
		array_unshift( $vpb_array_line, "<?php \ndefine('name_of_file_to_save_user_details', 
														'vpb_database.txt');?>" );
		$vpb_file_ip = implode("\n",$vpb_array_line);

		@fopen("vpb_database.txt", "w");
		$vpb_create_write_file = @fopen("vpb_database.php", "w");
		if ($vpb_create_write_file) 
		{
		  fwrite($vpb_create_write_file, $vpb_file_ip);
		  @fclose($vpb_create_write_file);
		}
	}
}
elseif(!file_exists($vpb_server_path."/".name_of_file_to_save_user_details))
{	
	@fopen(name_of_file_to_save_user_details, "w");
}

//This function converts the fullnames of users to Uppercase every 
//first letter of their names
function vpb_format_fullnames($name=NULL) 
{
	if (empty($name))
		return false;
	$name = strtolower($name);
	$names_array = explode('-',$name);
	for ($i = 0; $i < count($names_array); $i++) {	
		if (strncmp($names_array[$i],'mc',2) == 0 || 
			ereg('^[oO]\'[a-zA-Z]',$names_array[$i])) 
		{
			$names_array[$i][2] = strtoupper($names_array[$i][2]);
		}
		$names_array[$i] = ucfirst($names_array[$i]);
	}
	$name = implode('-',$names_array);
	return ucwords($name);
}

if(isset($_POST['page']) && !empty($_POST['page']))
{
	//========The login process starts from here=======
	if($_POST['page'] == "login")
	{
		if(isset($_POST['vpb_username']) && isset($_POST['vpb_passwd']) && 
		   !empty($_POST['vpb_username']) && !empty($_POST['vpb_passwd']))
		{
			$username = trim(strip_tags($_POST['vpb_username']));
			$passwd = trim(strip_tags($_POST['vpb_passwd']));
			$encrypted_user_password = md5($passwd);
			
			if($username == "")
			{
				echo '<div class="info">Please enter your account username to proceed.</div>';
			}
			elseif($passwd == "")
			{
				echo '<div class="info">Please enter your account password to proceed.</div>';
			}
			elseif(!@fopen(name_of_file_to_save_user_details,"r"))
			{
				echo '<div class="info">Sorry, we could not open the required 
					 database file to log you into your account. Please try 
					 again or contact this website admin to report this error 
					 message if the problem persist. Thanks.</div>';
			}
			else
			{
				$vpb_databases = fopen(name_of_file_to_save_user_details,"r");
				rewind($vpb_databases);
			
				while (!feof($vpb_databases)) 
				{
					$vpb_get_db_line = fgets($vpb_databases);
					$vpb_fetch_details = explode('::::::::::', $vpb_get_db_line);
					
					//User Information are shown below:
					//$vpb_fetch_detail[0] = Fullname
					//$vpb_fetch_detail[1] = Username
					//$vpb_fetch_detail[2] = Email Address
					//$vpb_fetch_detail[3] = Password
					
					$vpb_account_username = base64_decode(trim($vpb_fetch_details[1]));
					$vpb_account_password = base64_decode(trim($vpb_fetch_details[3]));
					
					// Validate Username
					if (!empty($vpb_account_username) && !empty($vpb_account_password) 
						&& $vpb_account_username == $username) 
					{
						// Username is valid therefore, validate user password
						if ($vpb_account_password == $encrypted_user_password)
						{
							// User details are validated therefore, create required
							// sessions for the logged in user
							//Don't forget that the user information were encrypted 
							//before they were saved to a file hence the decoding below
							$_SESSION['validfullname'] = base64_decode(
										   trim(strip_tags($vpb_fetch_details[0])));
							$_SESSION['validusername'] = base64_decode(
										   trim(strip_tags($vpb_fetch_details[1])));
							$_SESSION['validemail'] = base64_decode(
										   trim(strip_tags($vpb_fetch_details[2])));
							$_SESSION['validpassword'] = base64_decode(
										   trim(strip_tags($vpb_fetch_details[3])));
						}
						else
						{
							$vpb_error = '<div class="info">Sorry, the information you have provided are incorrect. Please enter your valid information to access this system. Thanks.</div>';
							break;
						}
					}
				}
				
				//If the user is validated and every thing is alright, then 
				//grant the user access to the secured account page 
				//otherwise, display error message to the user
				if ($vpb_error == '')
				{
					if(isset($_SESSION['validfullname']) && 
					   isset($_SESSION['validusername']) && 
					   isset($_SESSION['validemail']) && 
					   isset($_SESSION['validpassword']))
					{
						echo '<font style="font-size:0px;">completed</font>';
					}
					else
					{
						echo '<div class="info">Sorry, it seems your information 
						are incorrect and as a result, we are unable to create the 
						required sessions to log you into your account. Please enter 
						your valid account information to proceed. Thanks.</div>';
					}
				}
				else
				{
					echo $vpb_error;
				}
				fclose($vpb_databases);
			}
		}
		else
		{
			echo '<div class="info">Sorry, we could not get your valid username 
			and password to process your login. Please try again or contact this 
			website admin to report this error message if the problem persist. 
			Thanks.</div>';
		}
	}
	//========The login process ends here=======

	else
	{
		echo '<div class="info">Sorry, we could not identify the page you were 
		trying to access. Please try again or contact this website admin to 
		report this error message if the problem persist. Thanks.</div>';
	}
}
?>