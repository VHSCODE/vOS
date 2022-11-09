#include "machine.h"

int main()
{

	init_machine(1); //init our global machine and threads

	while (g_machine.is_running)
	{
		//Draw thread ?
	}

	deinit_machine();
	return 0;
}
