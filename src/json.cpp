#include "json.hpp"

#include <sstream>

std::string JSON_serialize(const json_t& json)
{
	Json::FastWriter writer;
	std::string stringified=writer.write(json);

	while(stringified.size()>0&&isspace(stringified[stringified.size()-1])!=0)
		stringified.pop_back();

	return stringified;
}

json_t JSON_parse(const std::string& stringified)
{
	std::istringstream istr(stringified);
	json_t json;
	istr>>json;
	return json;
}
