#include "header.h"
list <string>::iterator it;

int main()
{
	int _exit=0;
	string connect_ip, connect_port;
	string key, value;
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
			if(command.size()==3)
			{	
				it++;
				connect_ip=*it;		// taking 2 arguments
				it++;
				connect_port=*it;
				joining(connect_ip,connect_port);
			}
			else
				cout<<"Wrong number of arguments!!\n";
		}
		else if (choice == "put")
		{
			it++;
			key=*it;		// taking 2 arguments
			it++;
			value=*it;
			put_key(key,value);
		}
		else if (choice == "get")
		{
			it++;
			key=*it;		// taking 1 argument
			cout<<get_key(key)<<"\n";
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