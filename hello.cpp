#include <stdio.h>
#include <iostream>
#include "orandom.hpp"

int main(int argc, char** argv) {

	for (size_t index = 1; index <= 10; index++) {

		COrandom<int> iorand;
		iorand.push_data(1111);
		iorand.push_data(2222);
		iorand.push_data(3333);
		iorand.push_data(4444);
		iorand.push_data(5555);

		std::cout << "data type: 'int'" << "   " << "rand num: " << index << std::endl;
		for (size_t i = 0; i < 10; i++) {
			std::vector<int> rdata = iorand.get_rdata(index);
		}

		COrandom<float> forand;
		forand.push_data(1.111f);
		forand.push_data(2.222f);
		forand.push_data(3.333f);
		forand.push_data(4.444f);
		forand.push_data(5.555f);

		std::cout << "data type: 'float'" << "   " << "rand num: " << index << std::endl;
		for (size_t i = 0; i < 10; i++) {
			std::vector<float> rdata = forand.get_rdata(index);
		}

		COrandom<std::string> sorand;
		sorand.push_data("c++   ");
		sorand.push_data("golang");
		sorand.push_data("python");
		sorand.push_data("js    ");
		sorand.push_data("java  ");

		std::cout << "data type: 'string'" << "   " << "rand num: " << index << std::endl;
		for (size_t i = 0; i < 10; i++) {
			std::vector<std::string> rdata = sorand.get_rdata(index);
		}
	}

	return 0;
}