// ArgTest.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include "Arg.h"
int main(int argc, char* argv[]) 
{
	Arg arg(argc, argv);
	arg.addArgItem(ArgItem({ "help", "h", "?" }, 0));
	arg.addArgItem(ArgItem({ "opt1", "o1" }, 2));
	arg.addArgItem(ArgItem({ "opt2", "o2" }, 1));
	arg.parse();
	if (arg.items().size() == 0 || arg.items(0) == "help" || arg.items(0) == "h" || arg.items(0) == "?") {
		std::cout << "Usage: ArgTest [options] [items]" << std::endl;
		std::cout << "Options:" << std::endl;
		std::cout << "  -help, -h, -?          Show this help message." << std::endl;
		std::cout << "  -opt1, -o1 <val1> <val2>   Option 1 with two values." << std::endl;
		std::cout << "  -opt2, -o2 <val>       Option 2 with one value." << std::endl;
		std::cout << "Items:" << std::endl;
		std::cout << "  List of items." << std::endl;
		return 0;
	}
	for (const ArgItem& item : arg.args()) {
		if (item.isSet()) {
			std::cout << "Option: ";
			for (const std::string& op : item.opts()) {
				std::cout << "-" << op << " ";
			}
			std::cout << "\nValues: ";
			for (const std::string& val : item.values()) {
				std::cout << val << " ";
			}
			std::cout << std::endl;
		}
	}
	if (arg.argItem().size() > 0) {
		std::cout << "ArgItems: ";
		for (const std::string& s : arg.argItem()) {
			std::cout << s << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "opt count " << arg.optCount() << std::endl;

	return 0;
}
