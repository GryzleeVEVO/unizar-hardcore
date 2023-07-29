#include "static-stack-struct.h"
#include <iostream>

int main(int argc,char* argv[])
{
	static_stack<int> stk;

	init(stk);
	push(stk, 42);
	push(stk, 15);
	pop(stk);
	for (int i=0; i<100; i+=5)
		push(stk, i);

	startIterator(stk);
	bool error = false;
	while(hasNext(stk) && (!error))
	{
		std::cout << next(stk, error) << " ";
	}
	std::cout << std::endl;

	return 0;
}
