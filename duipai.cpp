#include <bits/stdc++.h>
#ifdef _WIN32
	#include <windows.h>
#endif
using namespace std;
string s,ex,checker;
int st,ed;
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
bool fileExists(const std::string& filename)
{
    ifstream file(filename);
    return file.good();
}
signed main()
{
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
	cin>>s>>ex>>st>>ed>>checker;
	if(!fileExists("duipai.json"))
	{
		freopen("duipai.json","w",stdout);
		printf("%s %s\n%d %d\n%s\n",s.c_str(),ex.c_str(),st,ed,checker.c_str());
	}
	freopen("result.res","a",stdout);
	freopen("result.res","a",stderr);
	print("Compiling...",WHITE);
	if(system(("g++ -o2 -std=c++14 -Wall -Wextra -o "+s+" "+s+".cpp").c_str()))
	{
		print("Compilation error.",YELLOW);
		printf("Compilation error.");
		exit(0);
	}
	for(int i=st;i<=ed;i++)
	{
		string t=s+to_string(i);
		print("Running on Task : "+t,WHITE);
		printf("task #%d:\n",i);
		#ifdef _WIN32
			freopen("___TMP___","a",stdout);
			system(("copy "+t+".in "+s+".in").c_str());
			system(("copy "+t+"."+ex+" "+s+".ans").c_str());
			freopen("result.res","a",stdout);
			double start=time();
			int res=system((s+".exe").c_str());
			if(res==0)
			{
				printf("RUNTIME : %.3lfs\n",time()-start);
				cout<<flush;
				if(system((checker+".exe "+s+".ans "+s+".out").c_str()))
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
				print("Runtime ERROR.\n",PURPLE);
				printf("Runtime Error.\n\n");
			}
		#elif __linux__
			freopen("___TMP___","a",stdout);
			system(("cp "+t+".in "+s+".in").c_str());
			system(("cp "+t+"."+ex+" "+s+".ans").c_str());
			freopen("result.res","a",stdout);
			double start=time();
			int res=system((s+".exe").c_str());
			if(res==0)
			{
				printf("RUNTIME : %.3lfs\n",time()-start);
				cout<<flush;
				if(system(("./"+checker+" "+s+".ans "+s+".out").c_str()))
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
				print("Runtime ERROR.\n",PURPLE);
				printf("Runtime Error.\n\n");
			}
		#else
			throw runtime_error("Unknown operation.");
		#endif
	}
	#ifdef __WIN32
		system(("del "+s+".in").c_str());
		system(("del "+s+".ans").c_str());
		system(("del "+s+".out").c_str());
		system("del ___TMP___");
	#elif __linux__
		system(("rm "+s+".in").c_str());
		system(("rm "+s+".ans").c_str());
		system(("rm "+s+".out").c_str());
		system("rm ___TMP___");
	#endif
	return 0;
}
