#include <stdio.h>
#include <windows.h>
#include <string>
#include <bits/stdc++.h>
#include <mutex>
#include <thread>
using namespace std;
struct node
{
	int op;
	string path;
	string name;
};
queue<node> qu;
SERVICE_STATUS ss;
SERVICE_STATUS_HANDLE ssh;
vector<char*> dri;
vector<thread*> ve;
bool isRun=1;
mutex mu1, mu2;
typedef struct TEST_INFO {
    DWORD NextEntryOffset;
    DWORD Action;
    DWORD FileNameLength;
    WCHAR FileName[100];
}_TEST_INFO;
void writeInfo(int op, string path, string name)
{
//	mu2.lock();
	ofstream out("C:\\Program Files\\FindFile\\update.dat",ios::app|ios::binary);
	out.write((char*)&op, 4);
	int len=path.size();
	out.write((char*)&len, 4);
	out.write(path.data(), len);
	len=name.size();
	out.write((char*)&len, 4);
	out.write(name.data(), len);
	out.close();
//	mu2.unlock();
}
void WINAPI ControlHandle(DWORD op)
{
	switch(op)
	{
		case SERVICE_CONTROL_STOP:
//			ss.dwCurrentState = SERVICE_STOP_PENDING;
//			SetServiceStatus(ssh,&ss);
//			isRun=0;
			break;
		case SERVICE_CONTROL_INTERROGATE://ѯ��
			break;
		case SERVICE_CONTROL_PAUSE:
			break;
		case SERVICE_CONTROL_CONTINUE:
			break;
	}
}
void watcher(char* vol)
{
	string old_name;
    BOOL _watchRequestResult = false;
    OVERLAPPED _ovl = { 0 };
    _ovl.hEvent = CreateEventA(NULL, TRUE, FALSE, NULL);
    HANDLE _directoryHandle = CreateFileA(vol, FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_DELETE | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);
    // This should be quite enough to fit multiple file events
    static constexpr DWORD ResultDataSize = 100;
//        const int ResultDataSize=100;
    _TEST_INFO _resultData[ResultDataSize] = { 0 };
    char file_name[MAX_PATH];
    while (true)
    {
        _watchRequestResult = ::ReadDirectoryChangesW(
            _directoryHandle,
            (LPVOID)_resultData,
            ResultDataSize * sizeof(_TEST_INFO),
            TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME,
            NULL,
            &_ovl,
            NULL
        );

        DWORD _ovlBytesReturned = 0;

        if (::GetOverlappedResult(_directoryHandle, &_ovl, &_ovlBytesReturned, TRUE))
        {
            int byteIndex = 0;

            while (TRUE)
            {
            	if(!isRun) break;
                _TEST_INFO* info = reinterpret_cast<_TEST_INFO*>(reinterpret_cast<char*>(&_resultData[0]) + byteIndex);
                byteIndex += info->NextEntryOffset;
                memset(file_name, 0, strlen(file_name));
                WideCharToMultiByte(CP_ACP, 0, info->FileName, info->FileNameLength / 2, file_name, 99, NULL, NULL);
                string par=vol;
                par+=file_name;//获取包括文件名在内的文件全路径
                string path, name;
                int len=par.length()-1;
                for(int i=len;i>=0;i--)//找到最后一个\\， 全面为路径，后面为名字
                {
                    if(par[i]=='\\')
                    {
                        name=par.substr(i+1);
                        path=par.substr(0, i);
                        break;
                    }
                }
                mu1.lock();
                switch (info->Action) {
                case FILE_ACTION_ADDED:
//                    	cout<<"test2   op:"<<0<<", path:"<<path<<", name:"<<name<<endl;
                    qu.push({0, path, name});
//                        writeInfo(0, path, name);
                    break;
                case FILE_ACTION_REMOVED:
                    qu.push({1, par, ""});
//                        writeInfo(1, par, "");
//                        cout<<"test2   op:"<<1<<", path:"<<par<<", name:"<<""<<endl;
                    break;
                case FILE_ACTION_RENAMED_OLD_NAME:
                    old_name=name;
                    break;
                case FILE_ACTION_RENAMED_NEW_NAME:
//                    	cout<<"test2   op:"<<2<<", path:"<<path+"\\"+old_name<<", name:"<<name<<endl;
                    qu.push({2, path+"\\"+old_name, name});
//                        writeInfo(2, path+"\\"+old_name, name);
                    break;
                }
                mu1.unlock();
                // If next entry index is 0, it means there is no next entry
                if (info->NextEntryOffset == 0)
                {
                    break;
                }
            }
        }

    }
}
void watch()
{
	while(isRun)
	{
		while(!qu.empty())
		{
			mu1.lock();
			node no=qu.front();
			qu.pop();
			mu1.unlock();
//			mu2.lock();
			writeInfo(no.op, no.path, no.name);
//			mu2.unlock();
//			cout<<"test2   op:"<<no.op<<", path:"<<no.path<<", name:"<<no.name<<endl;
		}
	}
}
void WINAPI Main()
{
	ss.dwServiceType=SERVICE_WIN32;
	ss.dwCurrentState=SERVICE_START_PENDING;
	ss.dwControlsAccepted=SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
	ss.dwWin32ExitCode=0;
	ss.dwCheckPoint=0;
	ss.dwWaitHint=0;
	ssh=RegisterServiceCtrlHandler("FindFileService",ControlHandle);
	if(!ssh) return;
	if(!SetServiceStatus(ssh,&ss)) return ;
	ss.dwWin32ExitCode = S_OK;
	ss.dwCurrentState = SERVICE_RUNNING;
	if(!SetServiceStatus(ssh,&ss)) return ;
	
	
	ifstream in("C:\\Program Files\\FindFile\\driver.dat",ios::in|ios::binary);
	int len;
	in.read((char*)&len, 4);
	string name="";
	char ch;
	for(int i=0;i<len;i++)
	{
		in.read((char*)&ch, 1);
		name+=ch;
		in.read((char*)&ch, 1);
		name+=ch;
		name+="\\";
		char* real_buf = new char[5];
		strcpy(real_buf, name.c_str());
		name="";
		dri.push_back(real_buf);
		thread *th=new thread(watcher, real_buf);
		ve.push_back(th);
		Sleep(100);
	}
	in.close();
	thread th(watch);
	th.join();
	for(auto i:ve) (*i).join();
	for(auto i:dri) delete i;
	
	
	ss.dwCurrentState = SERVICE_STOPPED;
	if(!SetServiceStatus(ssh,&ss)) return ;
}
int main()
{
	SERVICE_TABLE_ENTRY ste[]={{"FindFileService",(LPSERVICE_MAIN_FUNCTION)Main},{NULL,NULL}};
	StartServiceCtrlDispatcher(ste);
}
