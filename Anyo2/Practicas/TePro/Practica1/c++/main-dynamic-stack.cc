#include "dynamic-stack.h"
#include <iostream>

int main(int argc,char* argv[])
{
	dynamic_stack<int> stk;

	stk.push(42);
	stk.push(15);
	stk.pop();
	for (int i = 0; i < 100; i+=5)
		stk.push(i);

	dynamic_stack<int>::iterator iter = stk.begin();
	while(iter != stk.end())
	{
		std::cout << *iter << " ";
		++iter;
	}
	std::cout << std::endl;

	// Opcionalmente, con esta nomenclatura, puede hacerse asi (bucle foreach)
	for (int i : stk)
		std::cout<< i <<" ";
	std::cout << std::endl;

	return 0;
}
