#include "watcher.h"
mutex mu;
queue<FileNode> qu;
void watcher(char* vol)
{
    printf("%s\n", vol);
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
                    mu.lock();
                    switch (info->Action) {
                    case FILE_ACTION_ADDED:
                        qu.push({0, path, name});
//                        append(path, name);
                        break;
                    case FILE_ACTION_REMOVED:
                        qu.push({1,par,""});
                        break;
                    case FILE_ACTION_RENAMED_OLD_NAME:
                        old_name=name;
                        break;
                    case FILE_ACTION_RENAMED_NEW_NAME:
                        qu.push({2, path+"\\"+old_name, name});
//                        updata(path, old_name, name);
                    }
                    mu.unlock();
                    // If next entry index is 0, it means there is no next entry
                    if (info->NextEntryOffset == 0)
                    {
                        break;
                    }
                }
            }

        }
}

//void updateTree()
//{
//    while(true)
//    {
//        if(!qu.empty())
//        {
//            if(qu.size() >= 50)
//            {
//                cout<<"update too slow???????????????????????????????????????????????????????\n";
//            }
//            FileNode no=qu.front();
//            qu.pop();
//            vector<string> vst;
////            cout<<no.op<<":"<<no.path<<" \\ "<<no.name<<endl;
//            split(no.path, '\\', vst);
//            mu.lock();
//            if(no.op==0)
//            {
//                append(vst, no.name);
//            }
//            else if(no.op==1)
//            {
//                del(vst);
//            }
//            else
//            {
//                updata(vst, no.name);
//            }
//            mu.unlock();
//        }
//        else
//            Sleep(10);
//    }
//}

void updateTree()
{
    bool tempf=1;
//    qDebug() <<"start update tree\n";
    while(Update_Is_Run)
    {
        if(!Update_Is_Run) return;
        ifstream in("C:\\Program Files\\FindFile\\update.dat", ios::in|ios::binary);
        //数据格式：操作 路径长度 路径 文件名长度 文件名
        if(in)
        {
//            qDebug() << " read start";
            clock_t st=clock();
            int op, len;
            while(in.read((char*)&op, 4))
            {
                string path="";
                string name="";
                char ch;
                in.read((char*)&len, 4);
                while(len--) in.read((char*)&ch, 1), path+=ch;
                in.read((char*)&len, 4);
                while(len--) in.read((char*)&ch, 1), name+=ch;
                vector<string> vst;
                split(path, '\\', vst);
                mu.lock();//加锁
//                cout<<"op:"<<op<<", path:"<<path<<", name:"<<name;
                if(op==0)
                    append(vst, name);//op=0 新增文件
                else if(op==1)//op=1 删除文件
                    del(vst);
                else if(op==2)//op=2 更新文件名
                    updata(vst, name);
//                else
//                    cout<<"op error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!please check code!!!!!!!!!!!!!!!!!!!!!\n";
                mu.unlock();//解锁

            }
            in.close();
//            qDebug() << "read com";
            clock_t ed=clock();
            if(tempf)
            {
//                cout<<"update complited! use time : "<<ed-st<<endl;
                tempf=0;
            }
            ofstream no("C:\\Program Files\\FindFile\\update.dat", ios::out|ios::trunc);//感觉好像每秒新建一个文件又删除不太好，所以选择清空文件内容
            no.close();
//			system("del out.dat");
        }
        else
        {
            qDebug() << "open fail";
        }
        Sleep(1000);
//        qDebug() << Update_Is_Run;
    }
//    qDebug() <<"sdfsfsfsfsfs";
    return;
}

//printf("File name: %s, ", file_name);
//printf("Action: ");
//switch (info->Action)
//{
//case FILE_ACTION_ADDED:
//	printf("Added \n");
//	break;
//case FILE_ACTION_REMOVED:
//	printf("Removed \n");
//	break;
//case FILE_ACTION_RENAMED_OLD_NAME:
//	printf("Rename old name \n");
//	break;
//case FILE_ACTION_RENAMED_NEW_NAME:
//	printf("Rename new name \n");
//	break;
//}
