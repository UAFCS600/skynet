String.prototype.replace_at=function(index,value)
{
	return this.substr(0,index)+value+this.substr(index+value.length);
}
