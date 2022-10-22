#include<iostream>
#include<string_view>

void
test_1(std::string_view first, std::string_view last){
	std::cout<< " test_1 " << std::endl;
}

void 
test_1(std::string_view first, bool hasName){
	std::cout<< " test_2 " << std::endl;
}

int
main(){
	test_1("yc","lsm");

	return 0;
}
