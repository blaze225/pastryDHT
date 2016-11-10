#include "header.h"
list <string>::iterator it;

int main()
{
	int _exit=0;
	string connect_ip, connect_port;
	while(_exit==0)
	{
		Input();
		it=command.begin();
		string choice = *it;
		if (choice == "port")
		{
			it++;
			porting(*it);
		}
		else if (choice == "create")
		{
			creating();
		}
		else if (choice == "join")
		{
			it++;
			connect_ip=*it;
			it++;
			connect_port=*it;
			joining(connect_ip,connect_port);
		}
		else if (choice == "put")
		{

		}
		else if (choice == "get")
		{

		}
		else if (choice == "lset")
		{
			printLS();
		}
		else if (choice == "routetable")
		{
			printRT();
		}
		else if(choice == "exit")
		{
			_exit=1;
			close(sockfd);
			//kill(pid);
		}
		else
		{
			cout<<"Invalid command\n";
		}
	}
}