#ifndef GET_ALL_FILE_H
#define GET_ALL_FILE_H
#include <bits/stdc++.h>
#include <QObject>
#include <windows.h>
#include <QString>
#include <time.h>
//#include <QSqlQuery>
//#include <QDebug>
//#include <QSqlDatabase>
using namespace std;
#define u8(x) QObject::trUtf8(x).toLocal8Bit().toStdString()
//采用孩子兄弟表示法建树，为了方便找到父亲，所以加了一个父亲的位置
//利用数组模拟树，节点的位置即为数组的下标
//再利用哈希表实现下标与内容的映射。由于STL提供了哈希表的容器，就不自己造轮子了
struct node
{
    string name;//文件名
    int type;//文件类型
    int father;//文件父节点的位置
    int son;//儿子的位置
    int bro;//兄弟的位置


    friend bool operator==(const node& a, const node& b)//重载等于号
    {
        return a.name==b.name && a.type==b.type && a.father==b.father && a.son==b.son && a.bro==b.bro;
    }

    friend bool operator!=(const node& a, const node& b)//重载不等于
    {
        return !(a==b);
    }
};
typedef long long ll;
typedef pair<long long, string> Fnode;//文件的frn映射一个文件名
extern vector<char*> dri;//记录有哪些卷
static const ll TrueRoot = 0x5000000000005;//5 0000 0000 0005//windows盘符的frn都是它，与机器无关
#define buf_len 0x10000//缓冲区大小，4k
extern unordered_map<int, node> um;//文件树，下标映射内容
extern int endx;//最后一个节点的下标
extern HANDLE handle;//磁盘句柄
//extern QSqlDatabase db;
//extern QSqlQuery query;

void startGet(char*,int);//获取所有文件，参数为盘符与父节点位置
void errorBox(string s);//弹窗显示错误，参数为要显示的信息
bool checkNTFS(char* name);//检查是否是NTFS文件系统
void getDriver();//获得所有的盘符
bool getHandle(char *name);//获取句柄
void closeUsn(USN_JOURNAL_DATA &ujd,DWORD &br);//关闭USN数据文件
void getUsnData(USN_JOURNAL_DATA &ujd, map<ll, vector<Fnode> > &ma);//获取USN数据
bool getUsnLogInformation(DWORD &br, USN_JOURNAL_DATA &ujd);//获取USN日志信息
bool initUsnLog(DWORD &br);//初始化USN日志
void checkType(int x);//递归检查每个文件的类型，参数为该节点的位置
int getCheckType(const char* name);//判断文件的类型，参数为文件名。该方法并不准确，但勉强够用
bool isAdmin();
bool InitAndCon();

#endif // GET_ALL_FILE_H
