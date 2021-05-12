#include "get_all_file.h"

vector<char*> dri;
HANDLE handle;

//如果获取USN文件出现问题，会弹出错误框，然后程序结束
void errorBox(string s)
{
    MessageBoxA(NULL, (LPCSTR)(s.c_str()), "ERROR", MB_OK+16);
    exit(0);
}

bool isAdmin()
{
    bool bElevated = 0;
    HANDLE hToken = NULL;
    if(!OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken)) return 0;
    TOKEN_ELEVATION tokenEle;
    DWORD dwRetLen = 0;
    if ( GetTokenInformation( hToken, TokenElevation, &tokenEle, sizeof(tokenEle), &dwRetLen ) )
    {
        if ( dwRetLen == sizeof(tokenEle) )
        {
            bElevated = tokenEle.TokenIsElevated;
        }
    }
    CloseHandle( hToken );
    return bElevated;
}

//检查是否为NTFS文件系统，参数为磁盘名
bool checkNTFS(char* name)
{
    char nameBuf[15];
    if(GetVolumeInformationA(name,NULL,0,NULL,NULL,NULL,nameBuf,15))//获取磁盘信息
        return strcmp(nameBuf,"NTFS") ? 0 : 1;
    return 0;
}
//获取所有磁盘
void getDriver()
{
    DWORD bufLen = GetLogicalDriveStrings(0,NULL);//复制缓冲区需要的字符串的长度
    char driverBuf[bufLen];//接收缓冲区内容的数组，用于存储系统盘符
    //最后得到的结果形如 'C' ':' '\\' '\0' 'D' ':' '\\' '\0' '\0'
    GetLogicalDriveStringsA(bufLen,(LPSTR)driverBuf);//获取整个系统盘符名字符串
    char* driverName = driverBuf;//方便从头开始遍历整个数组
    while(*driverName != '\0')//因为有跳过'\0',所以如果遇到'\0'就说明已经得到了所有的盘了
    {
        char* real_buf = new char[5];//为了把NTFS的都放进一个vector里，就new了一个char数组
        strcpy(real_buf, driverName);
        if(checkNTFS(real_buf))//如果是NTFS，加进dri里。dri存放所有文件系统为NTFS的磁盘名
        {
            real_buf[2]=0;//为了以后方便用，只取前两个字符，盘符最后的'\\'丢弃
            dri.push_back(real_buf);//没有delete是因为dri里的只是地址，delete之后，dri里的地址就是不可用的。野指针？
        }
        else
            delete real_buf;//如果发现不是NTFS，就delete，避免浪费空间
        driverName += strlen(driverName)+1;// 让指针跳到下一个盘符名并跳过'\0'
    }

}
//获取磁盘句柄，参数为磁盘名
bool getHandle(char* name)
{
    string Name="\\\\.\\"+(string)name;//转换成string类型方便进行去尾操作,即'\\'
    Name.erase(Name.find_last_of(":")+1);
    //需要管理员权限，我实在是做不到在代码里加了，找不到一个有用的方法
    handle = CreateFileA(&Name[0],GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE,\
                         NULL,OPEN_EXISTING,FILE_ATTRIBUTE_READONLY,NULL);//打开USN日志文件
    if(handle != INVALID_HANDLE_VALUE) return true;
    else
    {
//        printf("获取驱动盘句柄失败，error：%d\n",GetLastError());
        string err="获取驱动盘句柄失败, 请联系开发者。错误代码："+to_string(GetLastError());
        errorBox(u8(err.c_str()));
        return false;
    }
}
//关闭USN Journal数据文件
void closeUsn(USN_JOURNAL_DATA &ujd,DWORD &br)
{
    DELETE_USN_JOURNAL_DATA dujd= {ujd.UsnJournalID,USN_DELETE_FLAG_DELETE};
    DeviceIoControl(handle,FSCTL_DELETE_USN_JOURNAL,&dujd,sizeof(dujd),NULL,0,&br,NULL);
}
//获得USN信息
bool getUsnLogInformation(DWORD &br, USN_JOURNAL_DATA &ujd)
{
    if(DeviceIoControl(handle,FSCTL_QUERY_USN_JOURNAL,NULL,0,&ujd,sizeof(ujd),&br,NULL)) return true;
    else
    {
        printf("获取USN Journal信息失败%d\n",GetLastError());
        string err="获取USN Journal信息失败, 请联系开发者。错误代码："+to_string(GetLastError());
        errorBox(u8(err.c_str()));
        return false;
    }
}
//初始化USN
bool initUsnLog(DWORD &br)
{
    CREATE_USN_JOURNAL_DATA cujd= {0,0};
    if(DeviceIoControl(handle,FSCTL_CREATE_USN_JOURNAL,&cujd,sizeof(cujd),NULL,0,&br,NULL)) return true;
    else
    {
        printf("初始化USN Journal文件失败，%d\n",GetLastError());
        string err="初始化USN Journal文件失败, 请联系开发者。错误代码："+to_string(GetLastError());
        errorBox(u8(err.c_str()));
        return false;
    }
}
//获取USN Journal数据，即所有文件名
void getUsnData(USN_JOURNAL_DATA &ujd, map<ll, vector<Fnode> > &ma)//获取USN Journal数据
{
//    cout<<"ma.size="<<ma.size()<<endl;
    char buf[buf_len];//缓冲区，大小为4K
    DWORD usnDataSize;//USN Journal数据的大小
    PUSN_RECORD usnRecord;
    MFT_ENUM_DATA med= {0,0,ujd.NextUsn};
    for(; DeviceIoControl(handle,FSCTL_ENUM_USN_DATA,&med,sizeof(med),buf,buf_len,&usnDataSize,NULL); \
            med.StartFileReferenceNumber = *(USN *)&buf)//获取下一页的USN记录，应该吧，我是这么理解的
    {
        DWORD len = usnDataSize-sizeof(USN); //本页USN记录的长度
        usnRecord=(PUSN_RECORD)((PCHAR)buf+sizeof(USN));//获取第一个USN记录
        while(len)
        {
            const int Len = usnRecord->FileNameLength;//将宽字符文件名转换为多字符，方便阅读
            char fileName[MAX_PATH] = {0};
            WideCharToMultiByte(CP_ACP,NULL,usnRecord->FileName,Len/2,fileName,Len,NULL,NULL);
            ma[usnRecord->ParentFileReferenceNumber].push_back\
                    (make_pair(usnRecord->FileReferenceNumber,fileName));//存储信息
            DWORD recordLen = usnRecord->RecordLength;
            len -= recordLen;
            usnRecord=(PUSN_RECORD)((PCHAR)usnRecord+recordLen); //获取下一个USN记录
        }
    }
}
//采用递归的方法求出每个节点的文件类型，参数为节点位置
void checkType(int x)
{
    if(!x) return ;//如果x=0，表示该节点为空，直接返回
    if(um[x].type == -1) um[x].type=getCheckType(um[x].name.c_str());//获得文件类型并赋值给type
    checkType(um[x].son);//递归到孩子节点
    checkType(um[x].bro);//递归到兄弟节点
}
//根据文件后缀名直接返回文件类型，参数为文件名。虽然不准确，但应该能用，常用的文件类型应该就这么几种
int getCheckType(const char* name)
{
    QString s=QString::fromLocal8Bit(name);
    s=s.toLower();
    if(s.endsWith(".jpg") || s.endsWith(".png") || s.endsWith(".bmp") || s.endsWith("gif") || s.endsWith("jpeg")) return 3;//图片
    else if(s.endsWith(".mp4")) return 1;//视频
    else if(s.endsWith(".mp3") || s.endsWith(".m4a")) return 2;//音频
    else if(s.endsWith(".cpp") || s.endsWith(".c") || s.endsWith(".h") \
            || s.endsWith(".py") || s.endsWith(".java") || s.endsWith(".txt")\
            || s.endsWith(".html") || s.endsWith(".xml") || s.endsWith("htm")) return 4;//文本文档
    else if(s.endsWith(".zip") || s.endsWith(".rar") || s.endsWith(".7z")) return 5;//压缩包
    else if(s.endsWith(".exe")) return 6;//可执行文件
    else return -1;//类型未知
}
//vector<int> ves;
//递归建树。参数为父节点位置，父节点文件编号，以及存储了文件信息的map
void build(int fa, ll pfrn, map<ll, vector<Fnode> > &ma)
{
//    if(endx >= 50) return ;
    vector<Fnode> ve=ma[pfrn];//可认为ve是文件夹里的文件列表
    if(ve.size()==0)
    {
        return ;
    }
    else
    {
        um[fa].type=7;//如果他的子树不为空，则他一定是文件夹
        for(auto i:ve)//为每一个子文件都新建一个节点
        {
            um[++endx]={i.second, -1, fa, 0, 0};//依次为 文件名，文件类型，父节点位置，子节点位置，兄弟节点位置
            if(!um[fa].son) um[fa].son=endx;
            else um[endx-1].bro=endx;
        }
        int p=um[fa].son;//因为采用的孩子兄弟表示法建树，要得到所有子树只能从父节点的第一个子节点开始，不停访问该节点的兄弟
        for(auto i:ve)//递归建立起每一颗子树
        {
            build(p, i.first, ma);
            p=um[p].bro;
        }
    }
}
//获得某磁盘下的所有文件，并建树。参数为盘符以及根节点的位置
void startGet(char* i, int head)
{
//    cout<<"start get "<<i<<", "<<head<<endl;
    if(getHandle(i))//成功获得了句柄
    {
        DWORD br;
        USN_JOURNAL_DATA ujd;
//        cout<<"get handle ok\n";
        if(initUsnLog(br))//初始化成功
        {
//            cout<<"init log ok\n";
            if(getUsnLogInformation(br, ujd))//信息获取成功
            {
//                cout<<"get info ok\n";
                map<ll, vector<Fnode> >ma;//pfrn映射子文件frn与文件名。因为一个文件夹可能会有多个文件，故为一对多，使用vector
                getUsnData(ujd, ma);//获取所有文件名，并将该盘内的信息全部存入ma中
//                cout<<"start build\n";
                build(head, TrueRoot, ma);//根据ma里的内容建树
                ma.clear();//情况
                closeUsn(ujd, br);//打开了之后要记得关闭
            }
        }
    }
}
