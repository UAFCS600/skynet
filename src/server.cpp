#include <iostream>
#include <mongoose/mongoose.h>
#include <stdexcept>
#include <string>

#include "handler.hpp"

int main(int argc,char* argv[])
{
	try
	{
		std::string ip="0.0.0.0";
		std::string port="8080";
		std::string web_root="web";

		mg_server* server=mg_create_server(nullptr,client_handler);

		if(server==nullptr)
		{
			std::cout<<"Server failed to start (Out of ram? Out of disk space?)."<<std::endl;
			return 0;
		}

		std::string address=ip+":"+port;

		if(mg_set_option(server,"listening_port",address.c_str())!=0)
		{
			std::cout<<"Failed to open port "<<mg_get_option(server,"listening_port")<<
				" (Is something running on this port? Do you have permissions?)."<<std::endl;
			return 0;
		}

		mg_set_option(server,"document_root",web_root.c_str());

		std::cout<<"Server started on "<<mg_get_option(server,"listening_port")<<" with web root \""<<
			web_root<<"\"."<<std::endl;

		while(mg_poll_server(server,1000))
		{}

		mg_destroy_server(&server);
	}
	catch(std::exception& exception)
	{
		std::cout<<exception.what()<<std::endl;
		return 1;
	}

	return 0;
}
