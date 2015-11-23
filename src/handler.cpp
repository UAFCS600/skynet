#include "handler.hpp"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "json.hpp"
#include "skynet/neuralnet.hpp"

std::map<std::string,std::string> get_headers(const http_message& message)
{
	std::map<std::string,std::string> headers;

	for(int ii=0;ii<MG_MAX_HTTP_HEADERS;++ii)
	{
		std::string key(message.header_names[ii].p,message.header_names[ii].len);
		std::string value(message.header_values[ii].p,message.header_values[ii].len);

		if(key=="")
			break;

		headers[key]=value;
	}

	return headers;
}

std::string get_query(const mg_str* query_string,const std::string& var)
{
	std::string temp;
	temp.resize(100);
	int size=mg_get_http_var(query_string,var.c_str(),(char*)temp.c_str(),temp.size());

	if(size<0)
		size=0;

	temp.resize(size);
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

void eval_handler(mg_connection* connection,int event,const std::string& post_data)
{
	try
	{
		json_t json=JSON_parse(post_data);

		if(!json.isObject())
			throw std::runtime_error("Not a JSON object.");

		auto inputs=to_double_array(json["inputs"]);
		auto layers=to_size_array(json["layers"]);
		auto weights=to_double_array(json["weights"]);
		size_t sigmoid_index=json["sigmoid_index"].asUInt();

		skynet::neuralnet_t neuralnet(layers,weights);

		if(inputs.size()!=layers[0])
			throw std::runtime_error("Invalid number of inputs(expected "+std::to_string(layers[0])+" got "+std::to_string(inputs.size())+").");

		int times=100000;
		auto start=std::chrono::system_clock::now();
		double output=0;

		for(auto ii=0;ii<times;++ii)
			output=neuralnet.evaluate(inputs,sigmoid_index);

		auto end=std::chrono::system_clock::now();
		auto time=std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count()/(double)times;

		mg_send(connection,"{\"output\":"+std::to_string(output)+",\"ns\":"+std::to_string(time)+"}","application/json");
	}
	catch(std::exception& error)
	{
		mg_send(connection,"{\"error\":\""+std::string(error.what())+"\"}","application/json");
	}
	catch(...)
	{
		mg_send(connection,"{\"error\":\"Could not parse JSON object.\"}","application/json");
	}
}

void client_handler(mg_connection* connection,int event,void* event_data)
{
	if(event==MG_EV_HTTP_REQUEST)
	{
		mg_serve_http_opts* test=(mg_serve_http_opts*)(connection->mgr->user_data);
		http_message message=*(http_message*)event_data;

		char client_raw[200];
		mg_sock_to_str(connection->sock,client_raw,200,MG_SOCK_STRINGIFY_IP);
		std::string client(client_raw);

		auto headers=get_headers(message);

		if(client=="127.0.0.1")
			for(auto header:headers)
				if(header.first=="X-Forwarded-For")
					client=header.second;

		std::string method(message.method.p,message.method.len);
		std::string request(message.uri.p,message.uri.len);
		std::string query(message.query_string.p,message.query_string.len);
		std::string post_data(message.body.p,message.body.len);

		std::cout<<client<<std::flush;
		if(client==std::string(client_raw))
			std::cout<<" NOLOOKUP";
		else
			std::cout<<" LOOKUP  ";
		std::cout<<" "<<method<<" "<<request<<"."<<std::endl;
		std::cout<<"\tQuery:  \""<<query<<"\""<<std::endl;

		if(method=="POST")
			std::cout<<"\tPost:  \""<<post_data<<"\""<<std::endl;

		std::string is_eval=get_query(&message.query_string,"eval");

		if(is_eval!=""&&is_eval!="false")
			eval_handler(connection,event,post_data);
		else
			mg_serve_http(connection,&message,*test);
	}
}
