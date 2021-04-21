#ifndef SEARCH_IN_TREE_H
#define SEARCH_IN_TREE_H
#include "get_all_file.h"
#include "QRegExp"
#include <QObject>
using namespace std;

//extern unordered_map<int, node> um;
//extern int end;

void split(string s, char ch, vector<string> &vst);//字符串切片函数，将s按ch切片。切片结果放进vst
void del(const vector<string> &vst);//响应删除文件，在树里删除文件对应节点
void updata(const vector<string> &vst, string name);//更新文件，在树里更新文件名
void append(const vector<string> &vst, string name);//添加文件，在树里添加一个节点
int findNode(const vector<string> &vst);//给定路径，返回节点位置。参数为按'\\'切片出来的vector
string getPath(int x);//给定节点位置，返回绝对路径
void cal_next(char *str, int *next, int len);//KMP计算next
int KMP(char *str, char *ptr);//KMP
bool ok(string s, string p1, bool f, bool reg);//判断是否匹配。参数为母串，模式串，是否区分大小写以及是否使用正则



#endif // SEARCH_IN_TREE_H
