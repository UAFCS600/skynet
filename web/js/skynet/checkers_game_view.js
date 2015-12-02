function checkers_game_view_t(div)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");
	this.div.appendChild(this.el);

	var myself=this;

	this.status={};
	this.status.center=document.createElement("center");
	this.el.appendChild(this.status.center);

	this.status.header=document.createElement("h3");
	this.status.center.appendChild(this.status.header);
	this.status.header.style.marginBottom="24px";

	this.viewer=new checkers_multi_viewer_t(this.el);
	this.viewer.set_disabled(true);
	this.viewer.reset_pointer=false;

	this.follow={};

	this.follow.table=document.createElement("table");
	this.follow.table.style.marginTop="10px";
	this.viewer.get_button_group().appendChild(this.follow.table);
	this.follow.table.style.width="100%";

	this.follow.row=this.follow.table.insertRow();

	this.follow.cols=[];
	for(var ii=0;ii<2;++ii)
	{
		var col=this.follow.row.insertCell();
		col.style.width="50%";
		this.follow.cols.push(col);
	}
	this.follow.cols[0].style.textAlign="right";
	this.follow.cols[1].style.textAlign="left";

	this.follow.checkbox=document.createElement("input");
	this.follow.cols[0].appendChild(this.follow.checkbox);
	this.follow.checkbox.type="checkbox";
	this.follow.checkbox.className="form-control";
	this.follow.checkbox.style.width="24px";
	this.follow.checkbox.style.height="24px";
	this.follow.checkbox.style.margin="0px";
	this.follow.checkbox.style.marginRight="5px";
	this.follow.checkbox.style.padding="0px";
	this.follow.checkbox.style.float="right";
	this.follow.checkbox.checked=true;
	this.follow.checkbox.onchange=function(){myself.update_disables_m();};

	this.follow.label=document.createElement("label");
	this.follow.cols[1].appendChild(this.follow.label);
	this.follow.label.innerHTML="Follow";
	this.follow.label.className="";

	var options=parse_uri();

	this.name="Invalid";
	this.set_header_m("Connecting");

	if(options.name)
	{
		this.name=options.name;
		this.set_header_m("");
	}

	this.update_disables_m();

	this.get_info();
	setInterval(function(){myself.get_info();},1000);
}

checkers_game_view_t.prototype.get_info=function()
{
	if(this.name)
	{
		var myself=this;

		checkers_query
		(
			"info_game",
			{name:this.name},
			function(json)
			{
				var str="";

				for(key in json.boards)
					str+=json.boards[key]+"\n";

				myself.viewer.set_list(str);
				myself.set_header_m(json.status);

				if(myself.follow.checkbox.checked)
					myself.viewer.set_pointer(myself.viewer.get_boards().length-1);
				else
					myself.viewer.set_pointer(myself.viewer.get_pointer());
			},
			function(error_title,error_message)
			{
				console.log(error_title+":  "+error_message);
			}
		);
	}
}







checkers_game_view_t.prototype.set_header_m=function(status)
{
	this.status.header.innerHTML=this.name+" - "+checkers_state_to_str(status);
}

checkers_game_view_t.prototype.update_disables_m=function()
{
	this.viewer.set_buttons_disabled(this.follow.checkbox.checked);

	if(this.follow.checkbox.checked)
		this.viewer.set_pointer(this.viewer.get_boards().length-1);
}