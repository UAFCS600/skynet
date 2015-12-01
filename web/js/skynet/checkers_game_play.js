function checkers_game_play_t(div)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");
	this.div.appendChild(this.el);

	var myself=this;

	var editor=new checkers_board_editor_t(this.el);
	editor.show_url(false);
	editor.show_buttons(false);
	editor.error_div.show(false);
	editor.set_disabled(true);
}