//onchange(board) - called on change with the newest board

function checkers_board_editor_t(div)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");
	this.div.appendChild(this.el);
	var myself=this;

	this.error_div=new error_div_t(this.el);

	this.board=new checkers_board_t(this.error_div.el);
	this.board.onerror=function(error){myself.error_div.add(error);};
	this.board.onclick=function(index){myself.onmouseclick_m(index);};
	this.board.onchange=function(board){if(myself.onchange)myself.onchange(board);};
	this.board.style.display="block";
	this.board.style.marginLeft="auto";
	this.board.style.marginRight="auto";

	this.error_div.el.appendChild(document.createElement("br"));

	this.input=document.createElement("input");
	this.error_div.el.appendChild(this.input);
	this.input.className="form-control";
	this.input.style.width="320px";
	this.input.style.textAlign="center";
	this.input.style.display="block";
	this.input.style.marginLeft="auto";
	this.input.style.marginRight="auto";
	this.input.style.marginBottom="10px";
	this.input.style.padding="10px";
	this.input.style.fontFamily="monospace";
	this.input.spellcheck=false;
	this.input.onchange=function(){myself.draw_board_m(this.value);};
	this.input.onkeyup=function(){myself.draw_board_m(this.value);};

	this.button_group=document.createElement("div");
	this.el.appendChild(this.button_group);
	this.button_group.className="text-center";
	this.button_group.style.marginBottom="14px";

	this.reset_button=make_button("Reset",function(){myself.reset();});
	this.button_group.appendChild(this.reset_button);
	this.reset_button.style.marginRight="5px";

	this.clear_button=make_button("Clear",function(){myself.clear();});
	this.button_group.appendChild(this.clear_button);
	this.clear_button.style.marginLeft="5px";

	this.url_box=document.createElement("div");
	this.el.appendChild(this.url_box);
	this.url_box.className="text-center";
	this.url_box.style.marginBottom="20px";

	this.url=document.createElement("a");
	this.url_box.appendChild(this.url);
	this.url.innerHTML="Link to This Board";

	this.reset();
	var options=parse_uri();

	if(options.board)
		this.set_value(options.board);
};

checkers_board_editor_t.prototype.reset=function()
{
	this.error_div.clear();
	this.board.reset();
	this.input.value=this.board.get_value();
	this.update_url_m(this.input.value);
}

checkers_board_editor_t.prototype.clear=function()
{
	this.error_div.clear();
	this.board.clear();
	this.input.value=this.board.get_value();
	this.update_url_m(this.input.value);
}

checkers_board_editor_t.prototype.set_value=function(value)
{
	this.input.value=value;
	this.draw_board_m(value);
}

checkers_board_editor_t.prototype.show_buttons=function(show)
{
	if(show)
		this.button_group.style.display="visible";
	else
		this.button_group.style.display="none";
}

checkers_board_editor_t.prototype.show_url=function(show)
{
	if(show)
		this.url.style.display="visible";
	else
		this.url.style.display="none";
}

checkers_board_editor_t.prototype.set_disabled=function(disabled)
{
	if(disabled)
	{
		this.board.onclick=null;
		this.input.disabled=true;
	}
	else
	{
		var myself=this;
		this.board.onclick=function(index){myself.onmouseclick_m(index);};
		this.input.disabled=false;
	}
}





checkers_board_editor_t.prototype.draw_board_m=function(board)
{
	this.error_div.clear();
	this.board.set_value(board);

	this.update_url_m(board);
}



checkers_board_editor_t.prototype.onmouseclick_m=function(index)
{
	if(this.input.value[index]=='r')
		this.input.value=this.input.value.replace_at(index,'R');
	else if(this.input.value[index]=='R')
		this.input.value=this.input.value.replace_at(index,'b');
	else if(this.input.value[index]=='b')
		this.input.value=this.input.value.replace_at(index,'B');
	else if(this.input.value[index]=='B')
		this.input.value=this.input.value.replace_at(index,'_');
	else
		this.input.value=this.input.value.replace_at(index,'r');

	this.set_value(this.input.value);
}

checkers_board_editor_t.prototype.update_url_m=function(value)
{
	this.url.href=window.location.href.split('?')[0]+"?board="+value;
}
