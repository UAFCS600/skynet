function checkers_games_t(div)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");
	this.div.appendChild(this.el);
	var myself=this;

	this.ui={};

	this.ui.column=document.createElement("div");
	this.el.appendChild(this.ui.column);
	this.ui.column.style.width="320px";
	this.ui.column.style.textAlign="center";
	this.ui.column.style.display="block";
	this.ui.column.style.marginLeft="auto";
	this.ui.column.style.marginRight="auto";

	this.ui.group=document.createElement("div");
	this.ui.column.appendChild(this.ui.group);
	this.ui.group.className="input-group";
	this.ui.group.style.marginBottom="32px";

	this.ui.input=make_input("","Game Name");
	this.ui.group.appendChild(this.ui.input);

	this.ui.button_group=document.createElement("div");
	this.ui.group.appendChild(this.ui.button_group);
	this.ui.button_group.className="input-group-btn";

	this.ui.button=make_button
	(
		"Create",
		function()
		{
			myself.create_game(myself.ui.input.value);
		}
	);
	this.ui.button_group.appendChild(this.ui.button);

	this.ui.list={};

	this.ui.list.div=document.createElement("div");
	this.ui.column.appendChild(this.ui.list.div);

	this.ui.list.obj=new list_group_t(this.ui.list.div);

	setInterval(function(){myself.get_list();},1000);
}

checkers_games_t.prototype.get_list=function()
{
	var myself=this;

	this.query
	(
		"list_game",
		{},
		function(json)
		{
			myself.ui.list.obj.clear();

			for(key in json)
				myself.make_list_item_m(key);
		},
		function(error_title,error_message)
		{
			console.log(error_title+":  "+error_message);
		}
	);
}

checkers_games_t.prototype.create_game=function(name)
{
	this.query("create_game",{name:name});
}

checkers_games_t.prototype.info_game=function(name)
{
}

checkers_games_t.prototype.query=function(command,args,success,fail)
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

checkers_games_t.prototype.make_list_item_m=function(name)
{
	var item=this.ui.list.obj.create();
	item.innerHTML=name;
	item.onclick=function(){console.log(name);};
}