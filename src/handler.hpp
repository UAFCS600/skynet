#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <string>
#include <mongoose/mongoose.h>

std::string get_query(const mg_connection* connection,const std::string& var);

void mg_send(mg_connection* connection,const std::string& data,const std::string& mime);
void mg_send_status(mg_connection* connection,const std::string& status);

int eval_handler(mg_connection* connection,mg_event event,const std::string& post_data);
int client_handler(mg_connection* connection,mg_event event);

#endif
