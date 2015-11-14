function checkers_board_t(div)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");
	this.div.appendChild(this.el);

	var myself=this;

	this.value="";

	this.canvas=document.createElement("canvas");
	this.el.appendChild(this.canvas);
	this.canvas.width=320;
	this.canvas.height=320;
	this.canvas.style.display="block";
	this.canvas.style.marginLeft="auto";
	this.canvas.style.marginRight="auto";
	this.canvas.style.marginBottom="20px";
	this.canvas.addEventListener('mousemove',function(event)
	{
		var box=myself.canvas.getBoundingClientRect();
		myself.onmousemove_m(event.clientX-box.left,event.clientY-box.top);
	});
	this.canvas.addEventListener('mouseup',function(event)
	{
		var box=myself.canvas.getBoundingClientRect();
		myself.onmouseclick_m(event.clientX-box.left,event.clientY-box.top);
	});

	this.crown_sprite_loaded=false;
	this.crown_sprite=new Image();
	this.crown_sprite.onload=function(){myself.crown_sprite_loaded=true;};

	this.crown_sprite.src="/images/crown.png";

	this.reset();
};

checkers_board_t.prototype.validate_board=function(board)
{
	if(board.length!=32)
		throw "Invalid board length (expected 32 got "+board.length+").";

	for(var ii=0;ii<32;++ii)
		if((board[ii]!='r'&&board[ii]!='R'&&board[ii]!='b'&&board[ii]!='B'&&board[ii]!='_'))
			throw "Invalid piece at position "+ii+" '"+board[ii]+"'.";
}

checkers_board_t.prototype.reset=function()
{
	var value="rrrrrrrrrrrr________bbbbbbbbbbbb";
	this.set_value(value);

	if(this.onreset)
		this.onreset(value);
}

checkers_board_t.prototype.clear=function()
{
	var value="________________________________";
	this.set_value(value);

	if(this.onclear)
		this.onclear(value);
}

checkers_board_t.prototype.set_value=function(board)
{
	try
	{
		this.draw_board_m(board);
		this.value=board;
	}
	catch(error)
	{
		if(this.onerror)
			this.onerror(error);
	}
}

checkers_board_t.prototype.get_value=function()
{
	return this.value;
}










checkers_board_t.prototype.draw_board_m=function(board)
{
	this.validate_board(board);

	var light="#FCCCA1";
	var dark="#D38944";

	this.ctx=this.canvas.getContext("2d");
	this.ctx.fillStyle="#FF0000";
	this.ctx.fillRect(0,0,this.canvas.width,this.canvas.height);

	var width=this.canvas.width/8;
	var height=this.canvas.height/8;

	var counter=0;

	for(var yy=0;yy<8;++yy)
	{
		for(var xx=0;xx<8;++xx)
		{
			if((xx+yy)%2==0)
				this.ctx.fillStyle=light;
			else
				this.ctx.fillStyle=dark;

			this.ctx.fillRect(xx*width,yy*height,(xx+1)*width,(yy+1)*height);

			if((xx+yy)%2!=0)
				this.draw_piece_m(xx,yy,board[counter++]);
		}
	}
}

checkers_board_t.prototype.draw_piece_m=function(xx,yy,piece)
{
	if(piece!="_")
	{
		var width=this.canvas.width/8.0;
		var height=this.canvas.height/8.0;
		var radius=width/2.0*0.8;
		var dot_radius=width/2.0*0.2;

		if(piece=="r"||piece=="R")
			this.ctx.fillStyle="red";
		else if(piece=="b"||piece=="B")
			this.ctx.fillStyle="#555555";

		var start_x=0;

		if((xx+yy)%2==0)
			start_x+=width;

		var draw_x=xx*width+width/2+start_x;
		var draw_y=yy*height+height/2;

		this.ctx.beginPath();
		this.ctx.arc(draw_x,draw_y,radius,0,2*Math.PI,false);
		this.ctx.fill();
		this.ctx.lineWidth=3;
		this.ctx.strokeStyle="#000000";
		this.ctx.stroke();

		if(piece=="R"||piece=="B")
			this.draw_crown_sprite_m(draw_x,draw_y,radius);
	}
}

checkers_board_t.prototype.onmousemove_m=function(xx,yy)
{
	var x_coord=Math.floor(xx/this.canvas.width*8);
	var y_coord=Math.floor(yy/this.canvas.height*8);

	if((x_coord+y_coord)%2!=0)
		this.canvas.style.cursor="pointer";
	else
		this.canvas.style.cursor="default";
}

checkers_board_t.prototype.onmouseclick_m=function(xx,yy)
{
	var x_coord=Math.floor(xx/this.canvas.width*8);
	var y_coord=Math.floor(yy/this.canvas.height*8);

	if((x_coord+y_coord)%2!=0)
	{
		if(x_coord%2!=0)
			x_coord-=1;

		var index=y_coord*4+x_coord/2;

		if(this.onclick)
			this.onclick(index);
	}
}

checkers_board_t.prototype.draw_crown_sprite_m=function(draw_x,draw_y,radius)
{
	if(this.crown_sprite_loaded)
	{
		var img_w=radius*1.2;
		var img_h=this.crown_sprite.height/this.crown_sprite.width*img_w;
		this.ctx.drawImage(this.crown_sprite,draw_x-img_w/2.0,draw_y-img_h/2.0,img_w,img_h);
	}
	else
	{
		var myself=this;
		setTimeout(function(){myself.draw_crown_sprite_m(draw_x,draw_y,radius);},100);
	}
}
