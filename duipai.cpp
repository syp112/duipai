#include <bits/stdc++.h>
#ifdef _WIN32
	#include <windows.h>
#endif
using namespace std;
string s,ex,checker,test_list;
vector<string>test_pt;
string to_string(string s)
{
	return s;
}
string to_string(const char *s)
{
	return (string)s;
}
template<typename T1,typename ...T2>
string to_string(T1 s1,T2 ...s2)
{
	return to_string(s1)+to_string(s2...);
}
namespace color_print
{
	#ifdef _WIN32
		#include <windows.h>
		const int RED=FOREGROUND_RED;
	    const int GREEN=FOREGROUND_GREEN;
	    const int YELLOW=FOREGROUND_RED|FOREGROUND_GREEN;
	    const int BLUE=FOREGROUND_BLUE;
	    const int PURPLE=FOREGROUND_RED|FOREGROUND_BLUE;
	    const int CYAN=FOREGROUND_GREEN|FOREGROUND_BLUE;
	    const int WHITE=FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE;
	#else
		const int RED=1;
	    const int GREEN=2;
	    const int YELLOW=3;
	    const int BLUE=4;
	    const int PURPLE=5;
	    const int CYAN=6;
	    const int WHITE=7;
	#endif
    void print(string s,int c)
    {
    	#ifdef _WIN32
			freopen("CON","a",stderr);
		    SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE),c);
	    	cerr<<s<<endl;
		    SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE),WHITE);
		#elif __linux__
			freopen("dev\cty","a",stderr);
			string t="\033[1;"+to_string(30+c)+"m";
			cerr<<t<<s<<"\033[0m"<<endl;
		#endif
		freopen("result.res","a",stderr);
	    return;
	}
}
using namespace color_print;
double time()
{
	return 1.0*clock()/CLOCKS_PER_SEC;
}
bool fileExists(const string& filename)
{
    ifstream file(filename);
    return file.good();
}
void judge(string t,string inform)
{
	print(inform,WHITE);
	printf(to_string(inform+"\n").c_str());
	if(!fileExists(to_string(t,".in"))||!fileExists(to_string(t,".",ex)))
	{
		print("File Error.\n",YELLOW);
		printf("File Error.\n\n");
		return;
	}
	#ifdef _WIN32
		freopen("___TMP___","a",stdout);
		system(to_string("copy ",t,".in ",s,".in").c_str());
		system(to_string("copy ",t,".",ex," ",s,".ans").c_str());
		freopen("result.res","a",stdout);
		double start=time();
		int res=system((s+".exe").c_str());
		if(res==0)
		{
			printf("RUNTIME : %.3lfs\n",time()-start);
			cout<<flush;
			if(system(to_string(checker,".exe ",s,".ans ",s,".out").c_str()))
			{
				print("Wrong Answer.\n",RED);
			}
			else
			{
				print("Accepted.\n",GREEN);
			}
		}
		else
		{
			print("Runtime Error.\n",PURPLE);
			printf("Runtime Error.\n\n");
		}
	#elif __linux__
		freopen("___TMP___","a",stdout);
		system(to_string("cp ",t,".in ",s,".in").c_str());
		system(to_string("cp ",t,".",ex," ",s,".ans").c_str());
		freopen("result.res","a",stdout);
		double start=time();
		int res=system((s+".exe").c_str());
		if(res==0)
		{
			printf("RUNTIME : %.3lfs\n",time()-start);
			cout<<flush;
			if(system(to_string("./",checker," ",s,".ans ",s,".out").c_str()))
			{
				print("Wrong Answer.\n",RED);
			}
			else
			{
				print("Accepted.\n",GREEN);
			}
		}
		else
		{
			print("Runtime Error.\n",PURPLE);
			printf("Runtime Error.\n\n");
		}
	#else
		throw runtime_error("Unknown operation.");
	#endif
	return;
}
pair<int,int> calc_test(string s)
{
	if(s.empty())
	{
		print("Illegal test point configuration.",RED);
		throw runtime_error("Illegal test point configuration.");
	}
	int p=-1;
	for(int i=0;i<(int)s.size();i++)
	{
		if(s[i]=='-')
		{
			if(~p)
			{
				print("Illegal test point configuration.",RED);
				throw runtime_error("Illegal test point configuration.");
			}
			p=i;
		}
	}
	if(~p)
	{
		return {stoi(s.substr(0,p)),stoi(s.substr(p+1,s.size()-p-1))};
	}
	else
	{
		return {stoi(s),-1};
	}
}
vector<string> calc_part(string s)
{
	if(s.empty())
	{
		print("Illegal test point configuration.",RED);
		throw runtime_error("Illegal test point configuration.");
	}
	int p=-1;
	for(int i=0;i<(int)s.size();i++)
	{
		if(s[i]=='~')
		{
			if(~p)
			{
				print("Illegal test point configuration.",RED);
				throw runtime_error("Illegal test point configuration.");
			}
			p=i;
		}
	}
	vector<string>r;
	if(~p)
	{
		auto [l1,l2]=calc_test(s.substr(0,p));
		auto [r1,r2]=calc_test(s.substr(p+1,s.size()-p-1));
		if(l2==-1&&r2==-1)
		{
			for(int i=l1;i<=r1;i++)
			{
				r.push_back(to_string(i));
			}
		}
		else
		{
			if(l2==-1||r2==-1)
			{
				print("Illegal test point configuration.",RED);
				throw runtime_error("Illegal test point configuration.");
			}
			for(int i=l1;i<=r1;i++)
			{
				for(int j=l2;j<=r2;j++)
				{
					r.push_back(to_string(i,"-",j));
				}
			}
		}
	}
	else
	{
		if(p==0||p==(int)s.size()-1)
		{
			print("Illegal test point configuration.",RED);
			throw runtime_error("Illegal test point configuration.");
		}
		r.push_back(s);
	}
	return r;
}
vector<string> calc_list(string s)
{
	vector<string>r;
	for(int i=0,j=0;i<(int)s.size();i=j+1)
	{
		for(j=i;s[j]!=','&&j<(int)s.size();j++)
		{
			if(!isdigit(s[j])&&s[j]!='-'&&s[j]!='~')
			{
				print("Illegal test point configuration.",RED);
				throw runtime_error("Illegal test point configuration.");
			}
		}
		vector<string>t=calc_part(s.substr(i,j-i));
		for(auto x:t)
		{
			r.push_back(x);
		}
	}
	return r;
}
signed main()
{
	print("Welcome to use syp's cross-checking.\nVisit https://www.luogu.com.cn/article/bavfmtbl to obtain the latest version.\nIf you encounter any problems, send an email to 3336088317@qq.com.\n",WHITE);
	#ifdef _WIN32
		SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
	#endif
	freopen("___TMP___","a",stderr);
	system("del result.res");
	if(fileExists("duipai.json"))
	{
		freopen("duipai.json","r",stdin);
		print("The configuration file already exists.\n",WHITE);
	}
	else
	{
		print("Please scan duipai.json\n",WHITE);
	}
	cin>>s>>ex>>test_list>>checker;
	if(!fileExists("duipai.json"))
	{
		freopen("duipai.json","w",stdout);
		printf("%s %s\n%s\n%s\n",s.c_str(),ex.c_str(),test_list.c_str(),checker.c_str());
	}
	freopen("result.res","a",stdout);
	freopen("result.res","a",stderr);
	print("Compiling...",WHITE);
	if(system(to_string("g++ -o2 -std=c++14 -Wall -Wextra -o ",s," ",s,".cpp").c_str()))
	{
		print("Compilation error.",YELLOW);
		printf("Compilation error.");
		exit(0);
	}
	test_pt=calc_list(test_list);
	judge(s+test_pt[0],to_string("Running on Pretest : "));
	for(auto x:test_pt)
	{
		string t=s+x;
		judge(t,to_string("Running on Task : ",t));
	}
	#ifdef __WIN32
		system(to_string("del ",s,".in").c_str());
		system(to_string("del ",s,".ans").c_str());
		system(to_string("del ",s,".out").c_str());
		system("del ___TMP___");
	#elif __linux__
		system(to_string("rm ",s,".in").c_str());
		system(to_string("rm ",s,".ans").c_str());
		system(to_string("rm ",s,".out").c_str());
		system("rm ___TMP___");
	#endif
	return 0;
}
