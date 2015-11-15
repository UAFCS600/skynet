function error_t(div,message)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");
	this.div.appendChild(this.el);

	this.error_boxes=[];

	this.glyph=document.createElement("span");
	this.el.appendChild(this.glyph);
	this.glyph.className="glyphicon glyphicon-remove"
	this.glyph.style.marginRight="5px";
	this.glyph.style.color="#a94442";

	this.label=document.createElement("label");
	this.el.appendChild(this.label);
	this.label.className="control-label has-feedback";
	this.label.innerHTML=message;
}

error_t.prototype.destroy=function()
{
	if(this.div)
		this.div.removeChild(this.el);

	this.div=this.el=null;
}

function error_div_t(div,max_errors)
{
	if(!div)
		return null;

	this.div=div;
	this.container=document.createElement("div");
	this.div.appendChild(this.container);

	this.el=document.createElement("div");
	this.container.appendChild(this.el);
	this.el.className="form-group has-feedback";

	this.error_boxes=[];
	this.max_errors=1;

	if(max_errors)
		this.max_errors=max_errors

	this.error_region=document.createElement("div");
	this.container.appendChild(this.error_region);
	this.error_region.style.height="20px";
}

error_div_t.prototype.size=function()
{
	return this.error_boxes.length;
}

error_div_t.prototype.add=function(error)
{
	if(this.error_boxes.length<this.max_errors)
	{
		this.container.className="form-group has-feedback has-error";

		var error_box=new error_t(this.error_region,error);
		error_box.div.style.textAlign="center";
		if(this.error_region.style.display=="none")
			error_box.div.style.display="none";
		else
			error_box.div.style.display="block";
		error_box.div.style.marginLeft="auto";
		error_box.div.style.marginRight="auto";

		this.error_boxes.push(error_box);
	}
}

error_div_t.prototype.clear=function()
{
	this.container.className="form-group has-feedback";

	for(var key in this.error_boxes)
		this.error_boxes[key].destroy();

	this.error_boxes=[];
}

error_div_t.prototype.show=function(show)
{
	if(show)
		this.error_region.style.display="visible";
	else
		this.error_region.style.display="none";
}