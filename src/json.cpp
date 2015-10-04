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

std::vector<float> to_float_array(const json_t& json)
{
	if(!json.isArray())
		throw std::runtime_error("Not a JSON array.");

	std::vector<float> array;

	for(auto ii:json)
		array.push_back(ii.asFloat());

	return array;
}

std::vector<size_t> to_size_array(const json_t& json)
{
	if(!json.isArray())
		throw std::runtime_error("Not a JSON array.");

	std::vector<size_t> array;

	for(auto ii:json)
		array.push_back(ii.asUInt());

	return array;
}

std::vector<std::vector<float>> to_array_float_array(const json_t& json)
{
	if(!json.isArray())
		throw std::runtime_error("Not a JSON array.");

	std::vector<std::vector<float>> array;

	for(auto ii:json)
		array.push_back(to_float_array(ii));

	return array;
}