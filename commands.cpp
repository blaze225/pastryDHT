#include "header.h"
pthread_t thread1, thread2;
char buffer[1000];

int portno;
pid_t pid;
string my_connect, my_port;
string ip_addr;
string my_hash;

struct cell RoutingTable[ROW][COL], leaf_set[L];
list <string> command;
int sockfd;

// Tokenizing given input and inserting into command
void Input()
{
	cout<<"pastry$ ";
	fgets(buffer,1000,stdin);
	string in(buffer);
	while(isspace(*in.begin()))
		in.erase(in.begin());
	strcpy(buffer,in.c_str());
	command.clear();
	char delim[5];
	strcpy(delim," \n");
	char *token = strtok(buffer,delim);
	while (token != NULL)
	{
		command.push_back(strdup(token));
		token = strtok(NULL, delim);
	}
}

// Update leaf set
void appendLS(vector<string> comm1)
{	
	string connect_hash;
	string connectip;
	string connectport;
	connectip=comm1[0];
	connectport=comm1[1];
	connect_hash=comm1[2];
	
	if(strdiff(my_hash, connect_hash) > 0)				// comparing new node and existing node to check which children to update
	{	
		// First 2 children
		if(strdiff((leaf_set[0]).hash, (leaf_set[1]).hash) > 0)		// Checking which of the 2 is greater and replacing that
		{
			if(strdiff((leaf_set[0]).hash, connect_hash) > 0)
			{
				(leaf_set[0]).ip = connectip;
				(leaf_set[0]).port = connectport;
				(leaf_set[0]).hash = connect_hash;
			}
		}
		else
		{
			if(strdiff((leaf_set[1]).hash, connect_hash) > 0)
			{
				(leaf_set[1]).ip = connectip;
				(leaf_set[1]).port = connectport;
				(leaf_set[1]).hash = connect_hash;
			}	
		}
	}
	else														
	{	
		if(strdiff((leaf_set[2]).hash, (leaf_set[3]).hash) > 0)		// Checking which of the 2 is greater and replacing that
		{
			if(strdiff((leaf_set[2]).hash, connect_hash) > 0)
			{
				(leaf_set[2]).ip = connectip;
				(leaf_set[2]).port = connectport;
				(leaf_set[2]).hash = connect_hash;
			}
		}
		else
		{
			if(strdiff((leaf_set[3]).hash, connect_hash) > 0)
			{
				(leaf_set[3]).ip = connectip;
				(leaf_set[3]).port = connectport;
				(leaf_set[3]).hash = connect_hash;
			}	
		}
	}
}

//void appendRT(list<string> command, int mode)			// Update routing table
void appendRT(string address, int mode)			// Update routing table
{	
	if(mode == 0)
	{
		vector<string> comm;
		int i=0;
		char buff1[1000];
		strcpy(buff1,address.c_str());
		comm.clear();
		char delim[5];
		strcpy(delim,":\n");
		char *token = strtok(buff1,delim);
		string temp_hash;
		while (token != NULL)
		{
			comm.push_back(strdup(token));
			token = strtok(NULL, delim);
		}

		// get common prefix to index by row into table
		string common=commonprefix(my_hash,comm[3]);
		int size=int(common.size());
		int col_no;

		col_no = convertHextoDec((comm[3])[size]); // col will be first non matching digit

		if(strdiff((RoutingTable[size][col_no]).hash, comm[3]) > 0)
		{
			(RoutingTable[size][col_no]).hash=comm[3];
			(RoutingTable[size][col_no]).ip=comm[1];
			(RoutingTable[size][col_no]).port=comm[2];
		}
	}
	else
	{
		char buff1[2000];
		strcpy(buff1,address.c_str());
		char delim[5];
		char *end2;
		strcpy(delim,"#\n");
		char *token = strtok_r(buff1,delim,&end2);
		string temp_add;
		int row_no=0;
		while (token != NULL)
		{
			temp_add = strdup(token);
			update_RT(temp_add,row_no);
			token = strtok_r(NULL, delim, &end2);
			row_no++;
		}
	}
}

void update_RT(string temp_row, int row_no)
{
		char buff1[2000];
		strcpy(buff1,temp_row.c_str());
		char delim[5];
		strcpy(delim,"@\n");
		char *end;
		char *token = strtok_r(buff1,delim,&end);
		string temp_add, tick;
		int temp_col = 0;
		while (token != NULL)
		{
			temp_add = token;
			Update_row(temp_add, temp_col, row_no);
			temp_col++;
			token = strtok_r(NULL, delim,&end);
		}

}
void Update_row(string temp_a, int temp_col, int row_no)
{
	char bufferr[1000];
	strcpy(bufferr,temp_a.c_str());
	char delim1[5];
	char *end1;
	strcpy(delim1,":\n");
	char *token1 = strtok_r(bufferr,delim1,&end1);
	vector<string> comm1;
	while(token1!=NULL)
	{
		comm1.push_back(token1);
		token1=strtok_r(NULL, delim1, &end1);
	}

	if(strdiff((RoutingTable[row_no][temp_col]).hash, comm1[2]) > 0 && comm1[1]!=my_port)
	{
		(RoutingTable[row_no][temp_col]).hash=comm1[2];
		(RoutingTable[row_no][temp_col]).ip=comm1[0];
		(RoutingTable[row_no][temp_col]).port=comm1[1];
	}
}

void exec_cmd(char *address, int sockfd)
{	
	list <string>::iterator it;
	vector <string> LS_set;
	string in(address);
	while(isspace(*in.begin()))
		in.erase(in.begin());
	char buff1[1000];
	strcpy(buff1,in.c_str());
	command.clear();
	char delim[5];
	strcpy(delim,":\n");
	char *token = strtok(buff1,delim);
	string temp_hash;
	int i=0;
	while (token != NULL)
	{
		if(i>=1)
			LS_set.push_back(token);
		command.push_back(strdup(token));
		temp_hash=token;
		token = strtok(NULL, delim);
		i++;
	}

	string temp_var="";
	string temp_common = commonprefix(my_hash,temp_hash);
	int size=int(temp_common.size());
	for(int j=0;j<=size;j++)
	{
		for(int i=0;i<16;i++)
			temp_var=temp_var+(RoutingTable[j][i]).ip+":"+(RoutingTable[j][i]).port+":"+(RoutingTable[j][i]).hash+"@";
		temp_var=temp_var+"#";
	}

	write(sockfd,temp_var.c_str(),strlen(temp_var.c_str()));

	it=command.begin();
	if(*it == "join")
	{
		appendRT(in,0);
		appendLS(LS_set);
	}
	
}

/* Converts given port as string into int */
void porting(string port)
{
	my_port=port;
	portno = atoi(port.c_str());
}

void *reading(void *ptr)
{	
	int *newsockfd=(int *)ptr;
    char buff[1000];
   	int n=read(*newsockfd,buff,999);
    buff[n]='\0';
    cout<<buff<<"\n";
	exec_cmd(buff,*newsockfd);
	close(*newsockfd);
	return 0;
}

void *listening(void *ptr)
{	
	struct sockaddr_in cli_addr;
	socklen_t clilen;
	int newsockfd;
    clilen = sizeof(cli_addr);
    int *sockfd=(int *)ptr;
	while(1)
		{
			newsockfd = accept(*sockfd, (struct sockaddr *) &cli_addr, &clilen);
			if (newsockfd < 0)
			{
				cout<<"ERROR on accept\n";
				exit(0);
			}
			pthread_create(&thread2, NULL, reading, &newsockfd);
		}

	close(newsockfd);
}

// Socket creation , RT & LS initialization , thread creation
void creating()
{
    struct sockaddr_in serv_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        cout<<"ERROR opening socket\n";
    	exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
    	cout<<"ERROR on binding\n";
    	exit(0);
	}

	struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;
    getifaddrs(&ifAddrStruct);
    int i=0;

    // Getting IP  
    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) 
    {
        if (ifa->ifa_addr->sa_family == AF_INET)
        { 
            i++;
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            if(i==2)
                ip_addr=(string)addressBuffer;
        }
    }
	cout<<"Node Created!! on "<<ip_addr<<" at port "<<portno<<"\n";
	
	my_connect="join:"+ip_addr+":"+my_port;
	string temp=ip_addr+":"+my_port;
   	string hash(md5(temp).substr(0,8));	// get hash value for given ip and port
   	my_connect=my_connect+":"+hash;
   	my_hash=hash;
   	
   	InitializeRT(ip_addr,my_port,my_hash);	// initialize routing table and leaf set
	InitializeLS();

	listen(sockfd,SOMAXCONN);	// Listening and creating a new thread for every connection
    pthread_create(&thread1, NULL, listening, &sockfd);
}

void joining(string connect_ip, string connect_port)
{
    int sockfd1;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd1 < 0) {
            printf("ERROR opening socket");
            exit(0);
        }
    server = gethostbyname(connect_ip.c_str());
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(atoi(connect_port.c_str()));
    connect(sockfd1,(struct sockaddr *) &serv_addr,sizeof(serv_addr));		
   	write(sockfd1,my_connect.c_str(),strlen(my_connect.c_str()));
  	vector <string> LS_set;
  	LS_set.push_back(connect_ip);
  	LS_set.push_back(connect_port);

  	string temp=connect_ip+":"+connect_port;
   	string hash(md5(temp).substr(0,8));	// get hash value for given ip and port
   	LS_set.push_back(hash);
  	appendLS(LS_set);

   	char buff1[2000];
   	int n=read(sockfd1,buff1,2000);
   	buff1[n]='\0';
   	string new_row(buff1); // returning matching number of rows
  	appendRT(new_row,1);
    close(sockfd1);
}