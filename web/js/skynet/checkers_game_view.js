function checkers_game_view_t(div)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");
	this.div.appendChild(this.el);

	var myself=this;

	this.default_speed=1000;
	this.max_divisor=16;

	this.status={};
	this.status.center=document.createElement("center");
	this.el.appendChild(this.status.center);

	this.status.text="";
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
	for(var ii=0;ii<3;++ii)
	{
		var col=this.follow.row.insertCell();
		this.follow.cols.push(col);
	}
	this.follow.cols[0].style.textAlign="right";
	this.follow.cols[1].style.textAlign="left";
	this.follow.cols[2].style.textAlign="left";
	this.follow.cols[0].style.width="45%";
	this.follow.cols[1].style.width="24px";
	this.follow.cols[2].style.width="55%";

	this.follow.checkbox=document.createElement("input");
	this.follow.cols[0].appendChild(this.follow.checkbox);
	this.follow.checkbox.type="checkbox";
	this.follow.checkbox.style.width="24px";
	this.follow.checkbox.style.height="24px";
	this.follow.checkbox.style.margin="0px";
	this.follow.checkbox.style.paddingRight="5px";
	this.follow.checkbox.style.float="right";
	this.follow.checkbox.onchange=function(){myself.update_disables_m();};

	this.follow.label=document.createElement("label");
	this.follow.cols[1].appendChild(this.follow.label);
	this.follow.label.innerHTML="Follow";
	this.follow.label.className="";

	this.follow.speed=document.createElement("select");
	this.follow.cols[2].appendChild(this.follow.speed);
	this.follow.speed.className="form-control";
	this.follow.speed.style.width="96px";
	this.follow.speed.style.marginLeft="10px";
	var speeds=[1,2,4,8,16];
	for(var ii=0;ii<speeds.length;++ii)
	{
		var option=document.createElement("option");
		option.text=speeds[ii]+"x Speed";
		option.speed=speeds[ii];
		this.follow.speed.add(option);
	}
	this.follow.speed.disabled=true;
	this.follow.speed.onchange=function(){myself.update_redraw_interval();};

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
	this.download_interval=setInterval(function(){myself.get_info();},this.default_speed/this.max_divisor);
	this.redraw_interval=null;
}

checkers_game_view_t.prototype.update_redraw_interval=function()
{
	var myself=this;

	if(this.redraw_interval)
	{
		clearInterval(this.redraw_interval);
		this.redraw_interval=null;
	}

	this.redraw_interval=setInterval(function(){myself.redraw();},this.default_speed/
		this.follow.speed.options[this.follow.speed.selectedIndex].speed);
}

checkers_game_view_t.prototype.get_info=function()
{
	if(this.name)
	{
		var myself=this;

		checkers_query
		(
			this.el,
			"info_game",
			{name:this.name},
			function(json)
			{
				var str="";

				for(key in json.boards)
					str+=json.boards[key]+"\n";

				myself.viewer.set_list(str);
				myself.set_header_m(json.status);

				if(json.status=="red_won"||json.status=="black_won")
				{
					clearInterval(myself.download_interval);
					myself.download_interval=null;
				}
			},
			function(error_title,error_message)
			{
				myself.set_header_m(error_title+":  "+error_message);
				console.log(error_title+":  "+error_message);
			}
		);
	}
}

checkers_game_view_t.prototype.redraw=function()
{
	if(this.follow.checkbox.checked)
	{
		this.viewer.set_pointer(this.viewer.get_pointer());

		if(this.viewer.get_pointer()+1<this.viewer.get_boards().length)
		{
			this.viewer.set_pointer(this.viewer.get_pointer()+1);
		}
		else if(this.status.text=="red_won"||this.status.text=="black_won")
		{
			this.follow.checkbox.checked=false;
			this.update_disables_m();
		}
	}
	else
	{
		this.viewer.set_pointer(this.viewer.get_pointer());
	}
}





checkers_game_view_t.prototype.set_header_m=function(status)
{
	this.status.text=status;
	this.status.header.innerHTML=this.name+" - "+checkers_state_to_str(status);
}

checkers_game_view_t.prototype.update_disables_m=function()
{
	var myself=this;
	this.viewer.set_buttons_disabled(this.follow.checkbox.checked);
	this.follow.speed.disabled=!this.follow.checkbox.checked;

	if(this.follow.checkbox.checked&&!this.redraw_interval)
	{
		this.update_redraw_interval();
	}
	else
	{
		clearInterval(this.redraw_interval);
		this.redraw_interval=null;
	}
}