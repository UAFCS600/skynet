function checkers_query(el,command,args,success,fail)
{
	var myself=this;
	var full_command="?"+command+"=true";

	request
	(
		full_command,
		args,
		function(json)
		{
			if(json.error)
			{
				if(!fail)
				{
					var modal=new modal_ok_t(el,"Server Error",json.error);
					modal.show();
				}
				else
				{
					fail("Server Error",json.error);
				}
			}
			else
			{
				if(success)
					success(json);
			}
		},
		function(error)
		{
			if(!fail)
			{
				var modal=new modal_ok_t(el,"Send Error",error);
				modal.show();
			}
			else
			{
				fail("Send Error",error);
			}
		}
	);
}

function checkers_state_to_str(status)
{
	if(status=="red_turn")
		return "Red's Turn";
	if(status=="black_turn")
		return "Black's Turn";
	if(status=="red_won")
		return "Red Won";
	if(status=="black_won")
		return "Black Won";

	return status;
}