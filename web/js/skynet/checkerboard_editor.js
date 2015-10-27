function checkerboard_editor_t(div)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");
	this.div.appendChild(this.el);
	var myself=this;

	this.error_boxes=[];

	this.canvas=document.createElement("canvas");
	this.el.appendChild(this.canvas);
	this.canvas.width=320;
	this.canvas.height=320;
	this.canvas.style.display="block";
	this.canvas.style.marginLeft="auto";
	this.canvas.style.marginRight="auto";
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

	this.input=document.createElement("input");
	this.el.appendChild(this.input);
	this.input.style.width="320px";
	this.input.style.textAlign="center";
	this.input.style.display="block";
	this.input.style.marginLeft="auto";
	this.input.style.marginRight="auto";
	this.input.className="form-control";
	this.input.value="Rrrrrrrrrrrr________Bbbbbbbbbbbb";
	this.input.onchange=function(){myself.draw_board(this.value);};
	this.input.onkeydown=function(){myself.draw_board(this.value);};
	this.input.onkeyup=function(){myself.draw_board(this.value);};

	this.draw_board(this.input.value);

	this.crown_sprite_loaded=false;
	this.crown_sprite=new Image();
	this.crown_sprite.onload=function(){myself.crown_sprite_loaded=true;};
	this.crown_sprite.src="/images/crown.png";
};

checkerboard_editor_t.prototype.draw_board=function(board)
{
	this.el.className="form-group has-feedback";

	for(var key in this.error_boxes)
		this.error_boxes[key].destroy();

	this.error_boxes=[];

	try
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
	catch(error)
	{
		this.el.className="form-group has-feedback has-error";
		this.error_boxes.push(this.create_error_m(error));
	}
}

checkerboard_editor_t.prototype.validate_board=function(board)
{
	if(board.length!=32)
		throw "Invalid board length (expected 32 got "+board.length+").";

	for(var ii=0;ii<32;++ii)
		if((board[ii]!='r'&&board[ii]!='R'&&board[ii]!='b'&&board[ii]!='B'&&board[ii]!='_'))
			throw "Invalid piece at position "+ii+" '"+board[ii]+"'.";
}

checkerboard_editor_t.prototype.draw_piece_m=function(xx,yy,piece)
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

checkerboard_editor_t.prototype.create_error_m=function(error)
{
	var error_box=new error_t(this.el,error);
	error_box.div.style.width="320px";
	error_box.div.style.textAlign="center";
	error_box.div.style.display="block";
	error_box.div.style.marginLeft="auto";
	error_box.div.style.marginRight="auto";
	return error_box;
}

checkerboard_editor_t.prototype.onmousemove_m=function(xx,yy)
{
	var x_coord=Math.floor(xx/this.canvas.width*8);
	var y_coord=Math.floor(yy/this.canvas.height*8);

	if((x_coord+y_coord)%2!=0)
	{
		this.canvas.style.cursor="pointer";
	}
	else
	{
		this.canvas.style.cursor="default";
	}
}

checkerboard_editor_t.prototype.onmouseclick_m=function(xx,yy)
{
	var x_coord=Math.floor(xx/this.canvas.width*8);
	var y_coord=Math.floor(yy/this.canvas.height*8);

	if((x_coord+y_coord)%2!=0)
	{
		if(x_coord%2!=0)
			x_coord-=1;

		var index=y_coord*4+x_coord/2;

		console.log(this.input.value[index]);

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

		this.draw_board(this.input.value);
	}
}

checkerboard_editor_t.prototype.draw_crown_sprite_m=function(draw_x,draw_y,radius)
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