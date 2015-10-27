function error_t(div,message)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");
	this.div.appendChild(this.el);

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