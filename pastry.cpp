#include "header.h"
list <string>::iterator it;

int main()
{
	int _exit=0;
	string connect_ip, connect_port;
	string key, value;
	int seq=0;
	while(_exit==0)
	{
		Input();
		it=command.begin();
		string choice = *it;
		if (choice == "port")
		{
			if((int)command.size()!=2)
				cout<<"Enter the correct format: port <port no>"<<endl;
			else
			{
				it++;
				porting(*it);
				seq=1;
			}
		}
		else if (choice == "create")
		{
			if(seq!=1)
				cout<<"No port to create"<<endl;
			else
			{
				creating();
				seq=2;
			}
		}
		else if (choice == "join")
		{
			if(seq<2)
				cout<<"Create the node first"<<endl;
			else
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
				seq++;
			}	
		}
		else if (choice == "put")
		{
			if(seq<2)
				cout<<"First create the node"<<endl;
			else
			{
				it++;
				key=*it;		// taking 2 arguments
				it++;
				value=*it;
				put_key(key,value);
			}
		}
		else if (choice == "get")
		{
			if(seq<2)
				cout<<"First create the node"<<endl;
			else
			{
				it++;
				key=*it;		// taking 1 argument
				cout<<get_key(key)<<"\n";
			}
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