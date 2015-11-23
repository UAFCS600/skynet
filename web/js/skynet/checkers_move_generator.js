function checkers_move_generator_t(div)
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

	this.board_col_new=document.createElement("div");
	this.side_by_side.appendChild(this.board_col_new);
	this.board_col_new.className="col-md-4";
	this.board_col_new.style.padding="0px";
	this.board_col_new.style.margin="0px";
	this.board_editor_new=new checkers_board_editor_t(this.board_col_new);
	this.board_editor_new.onchange=function(board){myself.get_moves_m(board);};
	this.board_editor_new.show_buttons(false);
	this.board_editor_new.show_url(false);
	this.board_new=this.board_editor_new.board;
	this.board_new.style.display="block";
	this.board_new.style.marginLeft="auto";
	this.board_new.style.marginRight="auto";

	this.player=document.createElement("select");
	this.player.onchange=function(){myself.get_moves_m(myself.board_new.get_value());};
	this.board_editor_new.error_div.el.appendChild(this.player);
	this.player.className="form-control";
	this.player.style.width="320px";
	this.player.style.display="block";
	this.player.style.marginLeft="auto";
	this.player.style.marginRight="auto";
	this.player.style.marginBottom="32px";
	var red=document.createElement("option");
	red.innerHTML="red";
	var black=document.createElement("option");
	black.innerHTML="black";
	this.player.appendChild(red);
	this.player.appendChild(black);

	this.list_col=document.createElement("div");
	this.side_by_side.appendChild(this.list_col);
	this.list_col.className="col-md-4";
	this.list_col.style.padding="0px";
	this.list_col.style.margin="0px";

	this.board_col=document.createElement("div");
	this.side_by_side.appendChild(this.board_col);
	this.board_col.style.padding="0px";
	this.board_col.style.margin="0px";
	this.board_col.className="col-md-4";
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

	this.list=new row_editor_t(this.list_col);
	this.list.validator=function(row){myself.board.validate_board(row);};
	this.list.onchange=function(rows){myself.boards=rows;myself.update_boards_m();};
	this.list.set_disabled(true);

	this.button_group=document.createElement("div");
	this.list_col.appendChild(this.button_group);
	this.button_group.className="text-center";
	this.button_group.style.marginBottom="32px";

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
	this.get_moves_m(this.board_new.get_value());
};

checkers_move_generator_t.prototype.prev_move=function()
{
	this.boards_ptr-=1;
	this.board_editor.set_value(this.boards[this.boards_ptr]);
	this.update_disables_m();
}

checkers_move_generator_t.prototype.next_move=function()
{
	this.boards_ptr+=1;
	this.board_editor.set_value(this.boards[this.boards_ptr]);
	this.update_disables_m();
}





checkers_move_generator_t.prototype.get_moves_m=function(board)
{
	var myself=this;

	request("?move_generator=true",{board:board,player:this.player.options[this.player.selectedIndex].innerHTML},
		function(json)
		{
			if(json.error)
			{
				var modal=new modal_ok_t(myself.el,"Server Error",json.error);
				modal.show();
			}
			else
			{
				var str="";

				for(var key in json.moves)
					str+=json.moves[key]+"\n";

				myself.list.set_value(str);
			}
		},
		function(error)
		{
			var modal=new modal_ok_t(myself.el,"Send Error",error);
			modal.show();
		});
}

checkers_move_generator_t.prototype.update_boards_m=function()
{
	this.list.error_div.clear();
	this.boards_ptr=0;

	if(!this.boards)
		this.boards=[];

	if(this.boards.length>0)
		this.board_editor.set_value(this.boards[this.boards_ptr]);
	else
		this.board_editor.set_value("________________________________");

	this.update_disables_m();
}

checkers_move_generator_t.prototype.update_disables_m=function()
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
