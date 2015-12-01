function checkers_game_view_t(div)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");
	this.div.appendChild(this.el);

	var myself=this;

	this.viewer=new checkers_multi_viewer_t(this.el);
	this.viewer.set_disabled(true);
	this.viewer.reset_pointer=false;

	var options=parse_uri();

	if(options.name)
		this.name=options.name;

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
				myself.viewer.set_pointer(myself.viewer.get_pointer());
			},
			function(error_title,error_message)
			{
				console.log(error_title+":  "+error_message);
			}
		);
	}
}