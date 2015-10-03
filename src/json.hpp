#ifndef JSON_HPP
#define JSON_HPP

#include <jsoncpp/json.h>
#include <string>

typedef Json::Value json_t;

std::string JSON_serialize(const json_t& json);
json_t JSON_parse(const std::string& stringified);

#endif
