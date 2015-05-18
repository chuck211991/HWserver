/********************************************************************************************************************
* This script is brought to you by Vasplus Programming Blog by whom all copyrights are reserved.
* Website: www.vasplus.info
* Email: info@vasplus.info
* Please, do not remove this information from the top of this page.
*********************************************************************************************************************/

//Users Login Function
function vpb_users_login() 
{
	var vpb_username = $("#username").val();
	var vpb_passwd = $("#passwd").val();
	
	if(vpb_username == "")
	{
		$("#login_status").html('<div class="info">Please enter your account username to proceed.</div>');
		$("#username").focus();
	}
	else if(vpb_passwd == "")
	{
		$("#login_status").html('<div class="info">Please enter your account password to go.</div>');
		$("#passwd").focus();
	}
	else
	{
		var dataString = 'vpb_username=' + vpb_username + '&vpb_passwd=' + vpb_passwd + '&page=login';
		$.ajax({
			type: "POST",
			url: "vpb_save_details.php",
			data: dataString,
			cache: false,
			beforeSend: function() 
			{
				$("#login_status").html('<br clear="all"><br clear="all"><div align="left"><font style="font-family:Verdana, Geneva, sans-serif; font-size:12px; color:black;">Please wait</font> <img src="images/loadings.gif" alt="Loading...." align="absmiddle" title="Loading...."/></div><br clear="all">');
			},
			success: function(response)
			{
				var vpb_result_broght = response.indexOf('completed');
				if (vpb_result_broght != -1 ) 
				{
					$("#login_status").html('');
					$("#username").val('');
					$("#passwd").val('');
					window.location.replace("../index.php?course=csci1200");
					//window.location.replace(QueryURL);
					
				}
				else
				{
					$("#login_status").hide().fadeIn('slow').html(response);
				}
				
			}
		});
	}
}