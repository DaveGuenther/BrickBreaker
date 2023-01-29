//Core includes
#include <iostream>
#include <vector>
#include <map>

//SDL related
#include <SDL2/SDL.h>




int main(int argv, char** args)
{

	std::cout<<"Hello SDL World!" << std::endl;
	int x=5;
    std::vector<int> my_ints;
    my_ints.push_back(5);
    my_ints.push_back(10);
    my_ints.push_back(6);
    my_ints.push_back(7);
    my_ints.push_back(8);

    std::map<std::string,int> my_map;
    my_map.insert({"ASCII_dec",5});   
    my_map.insert({"ASCII_chr",4});
    my_map.insert({"Width",3});

    std::cout << my_map.at("Width") << std::endl;
    std::cout << my_map.at("ASCII_dec") << std::endl;
    std::cout << my_map.at("ASCII_chr") << std::endl;

	std::cout << "Goodbye!" << std::endl;

	return 0;
}