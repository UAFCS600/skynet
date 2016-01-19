#include <iostream>
#include <skynet/neuralnet.hpp>
#include <stdexcept>

double sigmoid(const double value)
{
	return value*1.0;
}

int main()
{
	try
	{
		skynet::neuralnet_t neuralnet({3,2,1},{1,1,1,1,1,1,1,1});
		std::cout<<"Evaluation: "<<neuralnet.evaluate({1,1,1},sigmoid)<<std::endl;
	}
	catch(std::exception& error)
	{
		std::cout<<"Error - "<<error.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"Error - Unknown."<<std::endl;
	}

	return 0;
}