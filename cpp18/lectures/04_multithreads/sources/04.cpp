#include <iostream>
#include <string>
#include <thread>

struct Object
{
	std::string name;
	void print() const
	{
		std::cout << name << std::endl;
	}

	void printMessage(const std::string & message) const
	{
		std::cout << name << ":" << message << std::endl;
	}

	void getName(std::string & message) const
	{
		std::cout << "name = " << name << std::endl;
		message = name;
	}

	void change()
	{
		name += name;
	}
};

int main()
{
	Object o;
	o.name = "student";

	std::thread(&Object::print, o).join();

	std::string message = "some message";
	std::thread(&Object::printMessage, o, message).join();

	std::thread(&Object::getName, o, message).join();
	std::cout << message << std::endl;

	std::thread(&Object::getName, o, std::ref(message)).join();
	std::cout << message << std::endl;

	std::thread(&Object::change, o).join();
	std::cout << o.name << std::endl;

	std::thread(&Object::change, std::ref(o)).join();
	std::cout << o.name << std::endl;

	return 0;
}