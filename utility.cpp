#include "header.h"

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