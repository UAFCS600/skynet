function to_unsigned_int(str)
{
	if(str.length<=0)
		throw "invalid string length of 0";

	for(var ii=0;ii<str.length;++ii)
		if(str[ii]<'0'||str[ii]>'9')
			throw "invalid digit \""+str[ii]+"\"";

	return parseInt(str);
}