//onchange(rows) - called on change with an array containing each row
//validator(row) - validation callback checking a single row and throwing on invalid input

function row_editor_t(div)
{
	if(!div)
		return null;

	this.div=div;
	this.el=document.createElement("div");
	this.div.appendChild(this.el);
	var myself=this;

	this.error_div=new error_div_t(this.el);

	this.rows=[];

	this.list=document.createElement("textarea");
	this.error_div.el.appendChild(this.list);
	this.list.className="form-control input-normal";
	this.list.style.resize="none";
	this.list.style.width="320px";
	this.list.style.height="320px";
	this.list.style.display="block";
	this.list.style.paddingRight="0px";
	this.list.style.marginLeft="auto";
	this.list.style.marginRight="auto";
	this.list.style.fontFamily="monospace";
	this.list.style.overflowX="hidden";
	this.list.style.overflowY="scroll";
	this.list.spellcheck=false;
	this.list.onchange=function(){myself.onchange_m();};
};

row_editor_t.prototype.set_value=function(str)
{
	this.list.value=str;

	if(this.onchange)
		this.onchange_m();
}

row_editor_t.prototype.onchange_m=function()
{
	this.error_div.clear();
	this.rows=[];
	this.rows=this.parse_rows_m(this.list.value,true);

	if(this.onchange)
		this.onchange(this.rows);

	this.rows=this.parse_rows_m(this.list.value);
}

row_editor_t.prototype.parse_rows_m=function(str,hide_errors)
{
	while(str.length>0&&str[str.length-1]=='\n')
		str=str.substring(0,str.length-1);

	str=str.replace(/\n/g,"\",\"");
	str="{\"rows\":[\""+str+"\"]}";
	var json=JSON.parse(str);

	for(var ii=0;ii<json.rows.length;++ii)
	{
		try
		{
			if(!hide_errors&&this.validator)
				this.validator(json.rows[ii]);
		}
		catch(error)
		{
			this.error_div.add("Line "+(ii+1)+": "+error);
		}
	}

	return json.rows;
}
