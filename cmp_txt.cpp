#include<bits/stdc++.h>
using namespace std;
string trimLine(const string &line)
{
	size_t start=line.find_first_not_of(" \t");
	if(start==string::npos)
	{
		return "";
	}
	size_t end=line.find_last_not_of(" \t\n\r");
	return line.substr(start,end-start+1);
}
vector<string> readFile(const string &filename)
{
	ifstream file(filename);
	if(!file.is_open())
	{
		throw runtime_error("Can not open file:"+filename);
	}
	vector<string> lines;
	string line;
	while(getline(file,line))
	{
		lines.push_back(trimLine(line));
	}
	return lines;
}
bool compareFiles(const string &file1,const string &file2)
{
	try
	{
		vector<string> lines1=readFile(file1);
		vector<string> lines2=readFile(file2);
		if(lines1.size()!=lines2.size())
		{
			printf("Different number of file lines\n");
			return 0;
		}
		for(size_t i=0;i<lines1.size();++i)
		{
			if(lines1[i]!=lines2[i])
			{
				printf("Wrong on line %d :\nans:%s\nout:%s\n",(int)i+1,lines1[i].c_str(),lines2[i].c_str());
				return 0;
			}
		}
		return 1;
	}
	catch(const exception&e)
	{
		cerr<<"ERROR:"<<e.what()<<endl;
		return 0;
	}
}
int main(int argc,char*argv[])
{
	freopen("result.res","a",stdout);
	if(argc!=3)
	{
		cerr<<"ERROR. Enter in the following format: text_conparison <ansfile> <outfile>"<<endl;
		return 1;
	}
	string file1=argv[1];
	string file2=argv[2];
	if(compareFiles(file1,file2))
	{
		printf("Accepted.\n\n");
		cout<<flush;
		return 0;
	}
	else
	{
		printf("Wrong answer.\n\n");
		cout<<flush;
		return 1;
	}
	return -1;
}
