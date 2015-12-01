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
	this.ui.column.style.width="360px";
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

			myself.make_list_item_m(["Name","Status","Moves"]);

			var sorted_array=[];

			for(key in json)
				sorted_array.push({name:key,info:json[key]});

			sorted_array.sort(function(lhs,rhs)
			{
				if(lhs.name.toLowerCase()<rhs.name.toLowerCase())
					return -1;
				if(lhs.name.toLowerCase()>rhs.name.toLowerCase())
					return 1;
				return 0;
			});

			for(key in sorted_array)
				myself.make_game_list_item_m(sorted_array[key]);
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

checkers_games_t.prototype.make_list_item_m=function(data_cols)
{
	var item=this.ui.list.obj.create();
	var table=document.createElement("table");
	//table.border="1";
	table.style.width="100%";
	var row=table.insertRow();
	var cols=[];
	item.appendChild(table);
	item.onclick=function(){console.log(game.name);};

	for(var ii=0;ii<3;++ii)
	{
		var col=row.insertCell();
		col.style.textAlign="left";
		cols.push(col);
		cols[ii].innerHTML=data_cols[ii];
	}

	cols[0].width="45%";
	cols[1].width="30%";
	cols[2].width="25%";
	cols[2].style.textAlign="right";
}

checkers_games_t.prototype.make_game_list_item_m=function(game)
{
	this.make_list_item_m
	([
		game.name,
		this.state_pretty_str_m(game.info.status),
		game.info.boards.length-1
	]);
}

checkers_games_t.prototype.state_pretty_str_m=function(status)
{
	if(status=="red_turn")
		return "Red Turn";
	if(status=="black_turn")
		return "Black Turn";
	if(status=="red_won")
		return "Red Won";
	if(status=="black_won")
		return "Black Won";

	return status;
}