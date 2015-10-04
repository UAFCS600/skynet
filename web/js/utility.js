function to_int(str)
{
	if(str.length<=0)
		throw "Invalid digit.";

	for(var ii=0;ii<str.length;++ii)
		if(str[ii]<'0'||str[ii]>'9')
			throw "Invalid digit.";

	return parseInt(str);
}