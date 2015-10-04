function request(request,json,onreply,onerror)
{
	var data=null;

	try
	{
		data=JSON.stringify(json);

		var xmlhttp=new XMLHttpRequest();

		xmlhttp.onreadystatechange=function()
		{
			if(xmlhttp.readyState==4)
			{
				if(xmlhttp.status==200)
				{
					try
					{
						var json=JSON.parse(xmlhttp.responseText);

						if(onreply)
							onreply(json);
					}
					catch(error)
					{
						onerror(error);
					}
				}
				else if(onerror)
				{
					onerror(xmlhttp.status);
				}
			}
		};

		xmlhttp.open("POST",request,true);
		xmlhttp.setRequestHeader("Content-Type","application/json");
		xmlhttp.send(data);
	}
	catch(error)
	{
		if(onerror)
			onerror(error);
	}
}