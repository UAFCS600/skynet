function checkers_query(command,args,success,fail)
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
					var modal=new modal_ok_t(myself.el,"Server Error",json.error);
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
				var modal=new modal_ok_t(myself.el,"Send Error",error);
				modal.show();
			}
			else
			{
				fail("Send Error",error);
			}
		}
	);
}