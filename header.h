#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <string.h>
#include <stdlib.h>
#include <iterator>
#include <error.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <utility>
#include <map>
#include <bitset>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <pthread.h>
#include <signal.h>
#include "md5.h"

using namespace std;

#define b 2
#define ROW 8
#define COL 16
#define L 4

void Input();
void joining(string str1, string str2);
void creating();
void *listening(void *ptr);
void *reading(void *ptr);
void porting(string port);
void exec_cmd(char *address);
void appendRT(list<string> command);
void appendLS(list<string> command);
string commonprefix(string str1, string str2);
void printRT();
void InitializeRT(string ip_addr, string my_port, string my_hash);
int convertHextoDec(char c);
void InitializeLS();
long long strdiff(string s1, string s2);
void printLS();

struct cell
{
	string ip;
	string port;
	string hash;
};
extern struct cell RoutingTable[ROW][COL], leaf_set[L];

extern list <string> command;
extern int sockfd;