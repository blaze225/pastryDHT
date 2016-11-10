#include "header.h"
map <string,string> key_value;
void printRT()
{
	for(int i=0;i<ROW;i++)
	{
		for(int j=0;j<COL;j++)
		{
			cout<< (RoutingTable[i][j]).ip <<"-"<<(RoutingTable[i][j]).port<<"-"<<(RoutingTable[i][j]).hash;
			cout<<" ";
		}
		cout<<endl;
	}
}

void printLS()
{
	for(int i=0;i<L;i++)
	{
		cout<< (leaf_set[i]).ip <<"-"<<(leaf_set[i]).port<<"-"<<(leaf_set[i]).hash;
		cout<<endl;
		if(i==1)
			cout<<"--------------------------\n";
	}
}

void InitializeLS()
{
	for(int i=0;i<L;i++)
	{
		(leaf_set[i]).ip="-";
		(leaf_set[i]).port="-";
		(leaf_set[i]).hash="ffffffff";
	}
}

void InitializeRT(string ip_addr, string my_port, string my_hash)
{
	for(int i=0;i<ROW;i++)
		for(int j=0;j<COL;j++)
		{
			(RoutingTable[i][j]).ip="-";
			(RoutingTable[i][j]).port="-";
			(RoutingTable[i][j]).hash="ffffffff";
		}
	
	int col_no;
	for(int i=0;i<8;i++)
		{
			col_no = convertHextoDec(my_hash[i]);
			(RoutingTable[i][col_no]).ip=ip_addr;
			(RoutingTable[i][col_no]).port=my_port;
			(RoutingTable[i][col_no]).hash=my_hash;
		}
}

string commonprefix(string str1, string str2)
{
    string result;
    int n1 = str1.length(), n2 = str2.length();
    
    for (int i=0, j=0; i<=n1-1 && j<=n2-1; i++,j++)
    {
        if (str1[i] != str2[j])
            break;
        result.push_back(str1[i]);
    }
    return result;
}

// Get diff of hex values after converting them into ints
long long strdiff(string s1, string s2)
{
	long long x, y;
	stringstream ss,tt;
	ss << hex << s1.c_str();
	ss >> x;
	tt << hex << s2.c_str();
	tt >> y;
	return x-y;
}

int convertHextoDec(char c)
{
	int col_no;

	if (c >= 48 && c <= 57)
	{
	    col_no = (((int)c) - 48);
	}
	else if ((c >= 65 && c <= 70))
	{
	    col_no = (((int)c) - 55);
	}
	else if (c >= 97 && c <= 102)
	{
	    col_no = (((int)c) - 87);
	}

return col_no;
}

void put_key(string key, string value)
{
	string hash(md5(key).substr(0,8));
	string min_hash="ffffffff";
	int nodeid=-1;
	for(int i=0;i<L;i++)
	{
		if(strdiff((leaf_set[i]).hash, min_hash) < 0)
			{
				min_hash=(leaf_set[i]).hash;
				nodeid=i;
			}
	}

	if(min_hash=="ffffffff")
	{
		key_value[key]=value;
	}
	else
	{
		string connect_ip=leaf_set[nodeid].ip;
		string connect_port=leaf_set[nodeid].port;
		int sockfd1;
	    struct sockaddr_in serv_addr;
	    struct hostent *server;
	    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
	    if (sockfd1 < 0)
	    {
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
	    string put_str="put:"+key+":"+value;		
	   	write(sockfd1,put_str.c_str(),strlen(put_str.c_str()));
	}
}

string get_key(string key)
{
	string min_hash="ffffffff";
	int nodeid=-1;
	for(int i=0;i<L;i++)
	{
		if(strdiff((leaf_set[i]).hash, min_hash) < 0)
			{
				min_hash=(leaf_set[i]).hash;
				nodeid=i;
			}
	}
	if (key_value.find(key) != key_value.end())
		return key_value[key];

	if(min_hash=="ffffffff")
	{
		return key_value[key];
	}
	else
	{
		string connect_ip=leaf_set[nodeid].ip;
		string connect_port=leaf_set[nodeid].port;
		int sockfd1;
	    struct sockaddr_in serv_addr;
	    struct hostent *server;
	    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
	    if (sockfd1 < 0)
	    {
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
	    string put_str="get:"+key;
	   	write(sockfd1,put_str.c_str(),strlen(put_str.c_str()));
	   	char buff[1000];
	   	int n=read(sockfd1,buff,999);
	   	buff[n]='\0';
	   	string res(buff);
	   	return res;
	}
	return NULL;
}