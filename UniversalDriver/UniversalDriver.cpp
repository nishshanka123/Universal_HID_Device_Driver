// UniversalDriver.cpp : Defines the entry point for the application.
//

#include "UniversalDriver.h"

using namespace std;

void appSelfhelpMenu()
{
	int choice = 1;
	while (choice != 0)
	{
		std::cout << "=========================================" << std::endl;
		std::cout << "== Platform Independent USB HID Driver ==" << std::endl;
		std::cout << "=========================================" << std::endl;
		std::cout << "== 1 - Refresh the device list" << std::endl;
		std::cout << "== 2 - Show device information" << std::endl;
		std::cout << "== 0 - Quit...:)." << std::endl;
		std::cout << "== Enter menu ID : ";

		std::cin >> choice;

		switch (choice)
		{
		case 1: {
			std::cout << "menu 1" << std::endl;
			break;
		}
		case 2: {
			std::cout << "menu 2" << std::endl;			
			break;
		}
		case 0:
			std::cout << "Quiting.." << std::endl;
			break;
		defult:
			break;
		}

		std::cout << "=========================================" << std::endl;
	}
}

int main()
{
	appSelfhelpMenu();

	return 0;
}
