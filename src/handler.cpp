#include "handler.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>

#include "json.hpp"

std::string get_query(const mg_connection* connection,const std::string& var)
{
	std::string temp;
	temp.resize(32767);
	int size=mg_get_var(connection,var.c_str(),(char*)temp.data(),temp.size());
	temp.resize(std::max(0,size));
	return temp;
}

void mg_send(mg_connection* connection,const std::string& data,const std::string& mime)
{
	mg_printf(connection,"HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n%s",
		mime.c_str(),data.size(),data.c_str());
}

void mg_send_status(mg_connection* connection,const std::string& status)
{
	mg_printf(connection,"HTTP/1.1 %s\r\nContent-Length: 0\r\n\r\n\r\n\r\n",status.c_str());
}

int client_handler(mg_connection* connection,mg_event event)
{
	if(event==MG_AUTH)
		return MG_TRUE;

	if(event==MG_REQUEST)
	{
		std::string client(connection->remote_ip);
		for(int ii=0;ii<connection->num_headers&&client=="127.0.0.1";++ii)
			if(std::string(connection->http_headers[ii].name)=="X-Forwarded-For")
				client=connection->http_headers[ii].value;

		std::string method=(connection->request_method);
		std::string request(connection->uri);
		std::string post_data;

		if(method=="POST")
			post_data=std::string(connection->content,connection->content_len);

		std::string query;
		if(connection->query_string!=nullptr)
			query=connection->query_string;

		std::cout<<client;
		if(client==std::string(connection->remote_ip))
			std::cout<<" NOLOOKUP";
		else
			std::cout<<" LOOKUP  ";
		std::cout<<" "<<method<<" "<<request<<"."<<std::endl;
		std::cout<<"\tQuery:  \""<<query<<"\""<<std::endl;

		if(method=="POST")
			std::cout<<"\tPost:  \""<<post_data<<"\""<<std::endl;

		std::string is_eval=get_query(connection,"eval");

		if(is_eval!=""&&is_eval!="false")
		{
			try
			{
				json_t json=JSON_parse(post_data);
				mg_send(connection,"{\"output\":42,\"time\":200}","application/json");
			}
			catch(std::exception& error)
			{
				mg_send(connection,"{error:\""+std::string(error.what())+"\"}","application/json");
			}
			catch(...)
			{
				mg_send(connection,"{error:\"Unknown error.\"}","application/json");
			}

			return MG_TRUE;
		}
	}

	return MG_FALSE;
}
