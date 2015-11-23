#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <map>
#include <string>
#include <mongoose/mongoose.h>

std::map<std::string,std::string> get_headers(const http_message& message);
std::string get_query(const mg_connection* connection,const std::string& var);

void mg_send(mg_connection* connection,const std::string& data,const std::string& mime);
void mg_send_status(mg_connection* connection,const std::string& status);

void eval_handler(mg_connection* connection,int event,const std::string& post_data);
void client_handler(mg_connection* connection,int event,void* event_data);

#endif
