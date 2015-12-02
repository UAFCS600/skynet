function checkers_game_list_t(div)
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
	this.ui.column.className="col-md-6 col-md-offset-3";

	this.ui.group=document.createElement("div");
	this.ui.column.appendChild(this.ui.group);
	this.ui.group.className="input-group";
	this.ui.group.style.marginBottom="32px";

	this.ui.input=make_input("","Game Name");
	this.ui.group.appendChild(this.ui.input);
	this.ui.input.onkeyup=function(event)
	{
		if(event.keyCode==13)
			myself.ui.button.click();
		return false;
	};

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
	this.ui.button.onkeyup=function(event)
	{
		if(event.keyCode==13)
			this.click();
		return false;
	};

	this.ui.list={};

	this.ui.list.div=document.createElement("div");
	this.ui.column.appendChild(this.ui.list.div);

	this.ui.list.obj=new list_group_t(this.ui.list.div);

	this.get_list();
	setInterval(function(){myself.get_list();},1000);
}

checkers_game_list_t.prototype.get_list=function()
{
	var myself=this;

	checkers_query
	(
		this.el,
		"list_games",
		null,
		function(json)
		{
			myself.ui.list.obj.clear();

			myself.make_list_item_m(["Name","Status","Moves"],true);

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

checkers_game_list_t.prototype.create_game=function(name)
{
	var myself=this;

	checkers_query
	(
		this.el,
		"create_game",
		{name:name},
		function()
		{
			myself.ui.input.value="";
		}
	);
}







checkers_game_list_t.prototype.make_list_item_m=function(data_cols,title,onclick,href)
{
	var item=this.ui.list.obj.create();
	var table=document.createElement("table");
	//table.border="1";
	table.style.width="100%";
	var row=table.insertRow();
	var cols=[];
	item.appendChild(table);
	item.onclick=onclick;
	item.href=href;

	for(var ii=0;ii<3;++ii)
	{
		var col=row.insertCell();
		col.style.textAlign="left";
		cols.push(col);
		cols[ii].innerHTML=data_cols[ii];
	}

	cols[0].width="55%";
	cols[1].width="30%";
	cols[2].width="15%";
	cols[2].style.textAlign="right";

	if(title)
	{
		item.readonly=true;
		item.className+=" active";
		item.style.cursor="default";
	}
}

checkers_game_list_t.prototype.make_game_list_item_m=function(game)
{

	this.make_list_item_m
	(
		[
			game.name,
			checkers_state_to_str(game.info.status),
			game.info.boards.length-1
		],
		false,
		null,
		"view/?name="+game.name
	);
}
