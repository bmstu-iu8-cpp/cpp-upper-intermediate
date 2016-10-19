#include <iostream>
#include <thread>

struct Object
{
	std::string name;
	void print() const
	{
		std::cout << name.c_str() << std::endl;
	}

	void printMessage(const std::string & message) const
	{
		std::cout << name.c_str() << ":" << message.c_str() << std::endl;
	}

	void getName(std::string & message) const
	{
		std::cout << "name = " << name.c_str() << std::endl;
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
	std::cout << message.c_str() << std::endl;

	std::thread(&Object::getName, o, std::ref(message)).join();
	std::cout << message.c_str() << std::endl;

	std::thread(&Object::change, o).join();
	std::cout << o.name.c_str() << std::endl;

	std::thread(&Object::change, std::ref(o)).join();
	std::cout << o.name.c_str() << std::endl;

	return 0;
}