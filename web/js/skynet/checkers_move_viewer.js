function checkers_move_viewer_t(div)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");
	this.div.appendChild(this.el);
	var myself=this;

	this.error_boxes=[];

	this.boards=[];
	this.boards_ptr=0;

	this.side_by_side=document.createElement("div");
	this.el.appendChild(this.side_by_side);
	this.side_by_side.className="row";
	this.side_by_side.style.display="block";
	this.side_by_side.style.marginLeft="auto";
	this.side_by_side.style.marginRight="auto";
	this.side_by_side.style.marginBottom="20px";

	this.left_margin=document.createElement("div");
	this.side_by_side.appendChild(this.left_margin);
	this.left_margin.className="col-md-2";

	this.board_col=document.createElement("div");
	this.side_by_side.appendChild(this.board_col);
	this.board_col.className="col-md-4";
	this.board_editor=new checkers_board_editor_t(this.board_col);
	this.board_editor.input.readOnly=true;
	this.board_editor.show_buttons(false);
	this.board_editor.show_url(false);
	this.board_editor.show_errors(false);
	this.board=this.board_editor.board;
	this.board.onclick=null;
	this.board.style.display="block";
	this.board.style.marginLeft="auto";
	this.board.style.marginRight="auto";

	this.list_col=document.createElement("div");
	this.side_by_side.appendChild(this.list_col);
	this.list_col.className="col-md-5";

	this.error_region=document.createElement("div");
	this.list_col.appendChild(this.error_region);
	this.error_region.style.padding="0px";
	this.error_region.style.margin="0px";

	this.list=document.createElement("textarea");
	this.error_region.appendChild(this.list);
	this.list.className="form-control input-normal";
	this.list.style.resize="none";
	this.list.style.width="320px";
	this.list.style.height="320px";
	this.list.style.display="block";
	this.list.style.paddingRight="0px";
	this.list.style.marginLeft="auto";
	this.list.style.marginRight="auto";
	this.list.style.fontFamily="monospace";
	this.list.style.overflowX="hidden";
	this.list.style.overflowY="scroll";
	this.list.spellcheck=false;
	this.list.innerHTML=
		"rrrrrrrrrr_r___r__b_bb_bbbbbbbbb\n"+
		"rrrrrrrrrr_r________bbrbbbbbbbbb\n"+
		"rrrrrrrrrr_r______b_bb_bb_bbbbbb\n"+
		"rrrrrrr_rrrr______b_bb_bb_bbbbbb\n"+
		"rrrrrrr_rrrr__b_____bb_bb_bbbbbb\n"+
		"rrrrrrr_rr_r_____r__bb_bb_bbbbbb\n"+
		"rrrrrrr_rr_r__b_____b__bb_bbbbbb\n"+
		"rrrrrrr_r__r______r_b__bb_bbbbbb\n"+
		"rrrrrrr_r__r__b_____b___b_bbbbbb\n"+
		"rrrrrr__rr_r__b_____b___b_bbbbbb\n"+
		"rrrrrr__rr_r__b_____b__bb__bbbbb";
	this.list.onchange=function(){myself.update_boards_m();};

	this.error_region.appendChild(document.createElement("br"));

	this.input_div=document.createElement("div");
	this.error_region.appendChild(this.input_div);
	this.input_div.style.width="100%";
	this.input_div.style.height="12px";
	this.input_div.style.marginTop="-4px";

	this.list_col.appendChild(document.createElement("br"));

	this.button_group=document.createElement("div");
	this.list_col.appendChild(this.button_group);
	this.button_group.className="text-center";

	this.prev_button=make_button("Prev",function(){myself.prev_move();});
	this.button_group.appendChild(this.prev_button);
	this.prev_button.style.marginRight="5px";

	this.index_indicator=document.createElement("input");
	this.index_indicator.type="text";
	this.button_group.appendChild(this.index_indicator);
	this.index_indicator.className="form-control";
	this.index_indicator.style.width="32px";
	this.index_indicator.style.display="inline";
	this.index_indicator.style.textAlign="center";
	this.index_indicator.style.paddingLeft="0px";
	this.index_indicator.style.paddingRight="0px";
	this.index_indicator.value="";
	this.index_indicator.readOnly=true;

	this.next_button=make_button("Next",function(){myself.next_move();});
	this.button_group.appendChild(this.next_button);
	this.next_button.style.marginLeft="5px";

	this.right_margin=document.createElement("div");
	this.side_by_side.appendChild(this.right_margin);
	this.right_margin.className="col-md-1";

	this.board.reset();
	this.update_boards_m();
};

checkers_move_viewer_t.prototype.prev_move=function()
{
	this.boards_ptr-=1;
	this.board_editor.set_value(this.boards[this.boards_ptr]);
	this.update_disables_m();
}

checkers_move_viewer_t.prototype.next_move=function()
{
	this.boards_ptr+=1;
	this.board_editor.set_value(this.boards[this.boards_ptr]);
	this.update_disables_m();
}









checkers_move_viewer_t.prototype.update_boards_m=function()
{
	this.clear_errors_m();
	this.boards=[];
	this.boards_ptr=0;
	this.boards=this.parse_line_boards_m(this.list.value);

	if(this.boards.length>0)
		this.board_editor.set_value(this.boards[this.boards_ptr]);

	this.update_disables_m();
}

checkers_move_viewer_t.prototype.update_disables_m=function()
{
	this.next_button.disabled=(this.boards_ptr+1>=this.boards.length);
	this.prev_button.disabled=(this.boards_ptr-1<0||this.boards.length<=1);

	if(this.boards.length>0)
		this.index_indicator.value=""+this.boards_ptr+"";
	else
		this.index_indicator.value="-";

	var width=16+16*this.index_indicator.value.length;

	if(width<48)
		width=48;

	this.index_indicator.style.width=width+"px";
}

checkers_move_viewer_t.prototype.parse_line_boards_m=function(str)
{
	while(str.length>0&&str[str.length-1]=='\n')
		str=str.substring(0,str.length-1);

	str=str.replace(/\n/g,"\",\"");
	str="{\"boards\":[\""+str+"\"]}";
	var json=JSON.parse(str);

	for(var ii=0;ii<json.boards.length;++ii)
	{
		try
		{
			this.board.validate_board(json.boards[ii]);
		}
		catch(error)
		{
			this.append_error_m("Line "+(ii+1)+": "+error);
		}
	}

	return json.boards;
	return [];
}

checkers_move_viewer_t.prototype.create_error_m=function(error)
{
	var error_box=new error_t(this.input_div,error);
	error_box.div.style.textAlign="center";
	error_box.div.style.display="block";
	error_box.div.style.marginLeft="auto";
	error_box.div.style.marginRight="auto";
	return error_box;
}

checkers_move_viewer_t.prototype.append_error_m=function(error)
{
	this.error_region.className="form-group has-feedback has-error";

	if(this.error_boxes.length==0)
		this.error_boxes.push(this.create_error_m(error));
}

checkers_move_viewer_t.prototype.clear_errors_m=function()
{
	this.error_region.className="form-group has-feedback";

	for(var key in this.error_boxes)
		this.error_boxes[key].destroy();

	this.error_boxes=[];
}
