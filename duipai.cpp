#include <bits/stdc++.h>
#ifdef _WIN32
	#include <windows.h>
#endif
using namespace std;
int time_lim,memory_lim;
bool OK=1;
string s,ex,checker,test_list;
vector<string>test_pt;
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
void quit(int id)
{
	if(id==0)
	{
		if(OK)
		{
			print("Accepted.",GREEN);
			printf("Accepted.\n");
		}
		else
		{
			print("NOT accepted.",RED);
			printf("NOT accepted.\n");
		}
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
	exit(id);
}
double time()
{
	return 1.0*clock()/CLOCKS_PER_SEC;
}
bool fileExists(const string& filename)
{
    ifstream file(filename);
    return file.good();
}
namespace judgement
{
	double start;
	void AC()
	{
		printf("RUNTIME : %.3lfs\n\n",time()-start);
		print("Accepted.\n",GREEN);
		return;
	}
	void WA()
	{
		printf("RUNTIME : %.3lfs\n\n",time()-start);
		print("Wrong Answer.\n",RED);
		OK=0;
		return;
	}
	void RE()
	{
		printf("Runtime Error.\n\n");
		print("Runtime Error.\n",PURPLE);
		OK=0;
		return;
	}
	void TLE()
	{
		printf("Time Limit Excceed.\n\n");
		print("Time Limit Excceed.\n",BLUE);
		OK=0;
		return;
	}
	struct ProcessResult
	{
		bool is_timeout;
		int exit_code;
	};
	void run_external_program(const string&program,int timeout_ms,int memory_lim_mb,promise<ProcessResult>&promise)
	{
		ProcessResult result= {false,-1};
	#ifdef _WIN32
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli={};
		HANDLE hJob=CreateJobObject(NULL,NULL);
		if(!hJob)
		{
			throw runtime_error("CreateJobObject failed.");
		}
		ZeroMemory(&si,sizeof(si));
		si.cb=sizeof(si);
		ZeroMemory(&pi,sizeof(pi));
		SIZE_T max_memory_bytes=static_cast<SIZE_T>(memory_lim_mb)*1024*1024;
		jeli.BasicLimitInformation.LimitFlags=JOB_OBJECT_LIMIT_PROCESS_MEMORY;
		jeli.ProcessMemoryLimit=max_memory_bytes;
		if(!SetInformationJobObject(hJob,JobObjectExtendedLimitInformation,&jeli,sizeof(jeli)))
		{
			CloseHandle(hJob);
			throw runtime_error("SetInformationJobObject failed");
		}
		if(!CreateProcess(NULL,const_cast<char*>(program.c_str()),NULL,NULL,FALSE,CREATE_SUSPENDED,NULL,NULL,&si,&pi))
		{
			CloseHandle(hJob);
			throw runtime_error("CreateProcess failed");
		}
		if(!AssignProcessToJobObject(hJob,pi.hProcess))
		{
			TerminateProcess(pi.hProcess,1);
			CloseHandle(hJob);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			throw runtime_error("AssignProcessToJobObject failed");
		}
		ResumeThread(pi.hThread);
		DWORD waitResult=WaitForSingleObject(pi.hProcess,timeout_ms);
		if(waitResult==WAIT_TIMEOUT)
		{
			TerminateProcess(pi.hProcess,1);
			result.is_timeout=true;
			result.exit_code=1;
		}
		else GetExitCodeProcess(pi.hProcess,reinterpret_cast<DWORD*>(&result.exit_code));
		CloseHandle(hJob);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	#else
		pid_t pid=fork();
		if(pid==-1)
		{
			throw runtime_error("fork failed");
		}
		else if(pid==0)
		{
			struct rlimit rl;
			rl.rlim_cur=rl.rlim_max=static_cast<rlim_t>(memory_lim_mb)*1024*1024;
			if(setrlimit(RLIMIT_AS,&rl)!=0)
			{
				perror("setrlimit failed");
				exit(EXIT_FAILURE);
			}
			execlp(program.c_str(),program.c_str(),(char*)nullptr);
			exit(EXIT_FAILURE);
		}
		else
		{
			auto start=chrono::steady_clock::now();
			while(chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now()-start).count()<timeout_ms)
			{
				int status;
				pid_t result_pid=waitpid(pid,&status,WNOHANG);
				if(result_pid==-1)
				{
					throw runtime_error("waitpid failed");
				}
				else if(result_pid>0)
				{
					if(WIFEXITED(status))
					{
						result.exit_code=WEXITSTATUS(status);
					}
					else if(WIFSIGNALED(status))
					{
						result.exit_code=WTERMSIG(status);
					}
					break;
				}
				els
				{
					this_thread::sleep_for(chrono::milliseconds(100));
				}
			}
			if(result.exit_code==-1)
			{
				kill(pid,SIGTERM);
				result.is_timeout=true;
				result.exit_code=SIGTERM;
				waitpid(pid,nullptr,0);
			}
		}
	#endif
		promise.set_value(result);
		return;
	}
	int run_process(string t,int timeout_ms,int memory_lim_mb)
	{
		int re;
		promise<ProcessResult> p;
		future<ProcessResult> f=p.get_future();
		thread t_run(run_external_program,t,timeout_ms,memory_lim_mb,ref(p));
		try
		{
			ProcessResult result=f.get();
			if(result.is_timeout)
			{
				re=2;
			}
			else
			{
				if(result.exit_code)
				{
					re=1;
				}
				else
				{
					re=0;
				}
			}
		}
		catch(const exception&e)
		{
			cerr<<e.what()<<'\n';
			exit(1);
		}
		if(t_run.joinable())
		{
			t_run.join();
		}
		return re;
	}
	void judge(string t,string inform,int timeout_ms,int memory_lim_mb)
	{
		print(inform,WHITE);
		printf(to_string(inform+"\n").c_str());
		if(!fileExists(to_string(t,".in"))||!fileExists(to_string(t,".",ex)))
		{
			print("File Error.\n",YELLOW);
			printf("File Error.\n\n");
			OK=0;
			return;
		}
		#ifdef _WIN32
			freopen("___TMP___","a",stdout);
			system(to_string("copy ",t,".in ",s,".in").c_str());
			system(to_string("copy ",t,".",ex," ",s,".ans").c_str());
			freopen("result.res","a",stdout);
			start=time();
			int res=run_process((s+".exe").c_str(),timeout_ms,memory_lim_mb);
			if(res==0)
			{
				cout<<flush;
				freopen("result.res","a",stdout);
				if(system(to_string(checker,".exe ",s,".ans ",s,".out").c_str()))
				{
					WA();
				}
				else
				{
					AC();
				}
			}
			else if(res==1)
			{
				RE();
			}
			else
			{
				TLE();
			}
		#elif __linux__
			freopen("___TMP___","a",stdout);
			system(to_string("cp ",t,".in ",s,".in").c_str());
			system(to_string("cp ",t,".",ex," ",s,".ans").c_str());
			freopen("result.res","a",stdout);
			double start=time();
			int res=run_process((s+".exe").c_str(),timeout_ms,memory_lim_mb);
			if(res==0)
			{
				cout<<flush;
				freopen("result.res","a",stdout);
				if(system(to_string("./",checker," ",s,".ans ",s,".out").c_str()))
				{
					WA();
				}
				else
				{
					AC();
				}
			}
			else if(res==1)
			{
				RE();
			}
			else
			{
				TLE();
			}
		#else
			throw runtime_error("Unknown operation.");
		#endif
		return;
	}
}
using namespace judgement;
pair<int,int> calc_test(string s)
{
	if(s.empty())
	{
		print("Illegal test point configuration.",RED);
		quit(2);
	}
	int p=-1;
	for(int i=0;i<(int)s.size();i++)
	{
		if(s[i]=='-')
		{
			if(~p)
			{
				print("Illegal test point configuration.",RED);
				quit(2);
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
		quit(2);
	}
	int p=-1;
	for(int i=0;i<(int)s.size();i++)
	{
		if(s[i]=='~')
		{
			if(~p)
			{
				print("Illegal test point configuration.",RED);
				quit(2);
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
				quit(2);
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
			quit(2);
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
				quit(2);
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
	print("Welcome to use syp's cross-checking.\nVisit https://www.luogu.com.cn/article/bavfmtbl to obtain the latest version.\nIf you encounter any problems, send an email to 3336088317@qq.com.\n------------------------------------------------------------------------\n",WHITE);
	#ifdef _WIN32
		SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
	#endif
	freopen("___TMP___","a",stderr);
	system("del result.res");
	if(fileExists("duipai.conf"))
	{
		freopen("duipai.conf","r",stdin);
		print("The configuration file already exists.\n",WHITE);
	}
	else
	{
		print("Please scan duipai.conf\n",WHITE);
	}
	cin>>s>>ex>>test_list>>checker>>time_lim>>memory_lim;
	if(!fileExists("duipai.conf"))
	{
		freopen("duipai.conf","w",stdout);
		printf("%s %s\n%s\n%s\n%d %d\n",s.c_str(),ex.c_str(),test_list.c_str(),checker.c_str(),time_lim,memory_lim);
	}
	freopen("result.res","a",stdout);
	freopen("result.res","a",stderr);
	test_pt=calc_list(test_list);
	print("Compiling...",WHITE);
	if(system(to_string("g++ -o2 -std=c++14 -Wall -Wextra -o ",s," ",s,".cpp").c_str()))
	{
		print("Compilation error.",YELLOW);
		printf("Compilation error.");
		exit(0);
	}
	judge(s+test_pt[0],to_string("Running on Pretest : "),time_lim,memory_lim);
	OK=1;
	for(auto x:test_pt)
	{
		string t=s+x;
		judge(t,to_string("Running on Task : ",t),time_lim,memory_lim);
	}
	quit(0);
	return 0;
}
