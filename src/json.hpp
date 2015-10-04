#ifndef JSON_HPP
#define JSON_HPP

#include <jsoncpp/json.h>
#include <string>
#include <vector>

typedef Json::Value json_t;

std::string JSON_serialize(const json_t& json);
json_t JSON_parse(const std::string& stringified);

std::vector<float> to_float_array(const json_t& json);
std::vector<size_t> to_size_array(const json_t& json);
std::vector<std::vector<float>> to_array_float_array(const json_t& json);

#endif
