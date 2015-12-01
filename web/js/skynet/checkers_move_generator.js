function checkers_move_generator_t(div)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");
	this.div.appendChild(this.el);
	var myself=this;

	this.board_col_new=document.createElement("div");
	this.board_col_new.className="col-md-4";
	this.board_col_new.style.padding="0px";
	this.board_col_new.style.margin="0px";
	this.board_editor_new=new checkers_board_editor_t(this.board_col_new);
	this.board_editor_new.onchange=function(board){myself.get_moves_m(board);};
	this.board_editor_new.show_buttons(false);
	this.board_editor_new.show_url(false);
	this.board_new=this.board_editor_new.board;

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
	this.player.appendChild(red);

	var black=document.createElement("option");
	black.innerHTML="black";
	this.player.appendChild(black);

	this.multi_viewer=new checkers_multi_viewer_t(this.el);
	this.multi_viewer.get_side_by_side().appendChild(this.board_col_new);
	this.multi_viewer.set_disabled(true);
	this.multi_viewer.set_centered(false);
	this.get_moves_m(this.board_new.get_value());
};








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

				myself.multi_viewer.set_list(str);
			}
		},
		function(error)
		{
			var modal=new modal_ok_t(myself.el,"Send Error",error);
			modal.show();
		});
}