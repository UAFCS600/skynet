function checkers_multi_viewer_t(div)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");
	this.div.appendChild(this.el);
	var myself=this;

	this.boards=[];
	this.boards_ptr=0;

	this.side_by_side=document.createElement("div");
	this.el.appendChild(this.side_by_side);
	this.side_by_side.className="row";
	this.side_by_side.style.display="block";
	this.side_by_side.style.marginLeft="auto";
	this.side_by_side.style.marginRight="auto";

	this.board_col=document.createElement("div");
	this.side_by_side.appendChild(this.board_col);
	this.board_col.className="col-md-4";
	this.board_col.style.padding="0px";
	this.board_col.style.margin="0px";
	this.board_editor=new checkers_board_editor_t(this.board_col);
	this.board_editor.input.readOnly=true;
	this.board_editor.show_buttons(false);
	this.board_editor.show_url(false);
	this.board_editor.error_div.show(false);
	this.board=this.board_editor.board;
	this.board.onclick=null;
	this.board.style.display="block";
	this.board.style.marginLeft="auto";
	this.board.style.marginRight="auto";

	this.list_col=document.createElement("div");
	this.side_by_side.appendChild(this.list_col);
	this.list_col.className="col-md-4";
	this.list_col.style.padding="0px";
	this.list_col.style.margin="0px";

	this.list=new row_editor_t(this.list_col);
	this.list.validator=function(row){myself.board.validate_board(row);};
	this.list.onchange=function(rows){myself.boards=rows;myself.update_boards_m();};

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

	this.board.reset();
	this.update_boards_m();

	this.list.set_value
	(
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
		"rrrrrr__rr_r__b_____b__bb__bbbbb"
	);
};

checkers_multi_viewer_t.prototype.prev_move=function()
{
	this.boards_ptr-=1;
	this.board_editor.set_value(this.boards[this.boards_ptr]);
	this.update_disables_m();
}

checkers_multi_viewer_t.prototype.next_move=function()
{
	this.boards_ptr+=1;
	this.board_editor.set_value(this.boards[this.boards_ptr]);
	this.update_disables_m();
}







checkers_multi_viewer_t.prototype.update_boards_m=function()
{
	this.list.error_div.clear();
	this.boards_ptr=0;

	if(!this.boards)
		this.boards=[];

	if(this.boards.length>0)
		this.board_editor.set_value(this.boards[this.boards_ptr]);

	this.update_disables_m();
}

checkers_multi_viewer_t.prototype.update_disables_m=function()
{
	this.next_button.disabled=(this.boards_ptr+1>=this.boards.length);
	this.prev_button.disabled=(this.boards_ptr-1<0||this.boards.length<=1);

	if(this.boards.length>0)
		this.index_indicator.value=""+(this.boards_ptr+1)+"";
	else
		this.index_indicator.value="-";

	var width=16+16*this.index_indicator.value.length;

	if(width<48)
		width=48;

	this.index_indicator.style.width=width+"px";
}
