function to_unsigned_int(str)
{
	if(str.length<=0)
		throw "invalid string length of 0";

	for(var ii=0;ii<str.length;++ii)
		if(str[ii]<'0'||str[ii]>'9')
			throw "invalid digit \""+str[ii]+"\"";

	return parseInt(str);
}

function is_float(str)
{
	return (str.length>0&&!isNaN(str)&&str.indexOf('.')!=-1);
}

function is_int(str)
{
	return (str.length>0&&!isNaN(str));
}

function make_button(text,onclick)
{
	var button=document.createElement("input");
	button.className="btn btn-primary";
	button.disabled=false;
	button.type="button";
	button.value=text;
	button.onclick=onclick;
	return button;
}

function make_input(text,placeholder,onchange)
{
	var input=document.createElement("input");
	input.className="form-control";
	input.type="text";
	input.value=text;
	input.placeholder=placeholder;
	input.onchange=onchange;
	input.onkeyup=onchange;
	return input;
}