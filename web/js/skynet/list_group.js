function list_group_t(div)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");
	this.div.appendChild(this.el);
	var myself=this;

	this.list_group=document.createElement("div");
	this.el.appendChild(this.list_group);
	this.list_group.className="list-group";

	this.list=[];
}

list_group_t.prototype.create=function()
{
	var item=document.createElement("a");
	this.list_group.appendChild(item);
	item.className="list-group-item";
	item.style.cursor="pointer";
	this.list.push(item);
	return item;
}

list_group_t.prototype.remove=function(item)
{
	var index=this.list.indexOf(item);

	if(index>-1)
	{
		this.list_group.removeChild(item);
		this.list.splice(index,1);
	}
}

list_group_t.prototype.clear=function()
{
	for(key in this.list)
		this.list_group.removeChild(this.list[key]);

	this.list=[];
}