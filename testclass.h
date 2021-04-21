#ifndef TESTCLASS_H
#define TESTCLASS_H
//#pragma execution_character_set("GB2312")
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QDesktopServices>
#include <QMessageBox>
#include <QCloseEvent>
#include <QClipboard>
#include <QMimeData>
//#include <QMetaType>
//#include <QVector>
#include "reject.h"
#include <string>
#include <QScrollBar>
#include <QTableWidgetItem>
#include <QList>
#include <QUrl>
#include <thread>
#include <QFileDialog>
#include <bits/stdc++.h>
#include <windows.h>
#include <time.h>
#include <unistd.h>
#include "watcher.h"
#include "setting.h"
//#include "search_in_tree.h"
using namespace std;
//qRegisterMetatType<QVector<int> >("QVector<int>");
//变量的声明，未定义
extern unordered_map<int, node> um;//全局变量，文件位置映射文件内容。//孩子兄弟表示法建树，使用哈希表模拟数组以模拟树
extern vector<char*> dri;//全局变量，所有为NTFS文件系统的盘
extern mutex mu;//互斥锁
extern bool Update_Is_Run;//是否更新
extern int endx;//最后一个节点的位置，值只会不停的增大，但应该还不至于超过int最大值。
//因为程序中的树是用的哈希表实现，所以这个值再大不会出现数组过大的情况

typedef unsigned int Tnode;//未被使用
typedef pair<string, string> pa;//pair<file path, file name>
//sort排序的 排序函数参数
bool cmpNA(pa , pa);//按文件名升序
bool cmpND(pa , pa);//按文件名降序
bool cmpPA(pa , pa);//按路径升序
bool cmpPD(pa , pa);//按路径降序
namespace Ui {
class testClass;
}

class testClass : public QMainWindow
{
    Q_OBJECT

public:
    testClass(QWidget *parent = 0);
    ~testClass();
    reject *rej;//帮助窗口
    setting *Setting;
    vector<pa> ans;//存储搜索结果
private:
    Ui::testClass *ui;//图形化窗口
    void createMenu();//系统托盘区右键显示的菜单
    void closeEvent(QCloseEvent *e);//关闭窗口事件的监听
    void insertItem(int lines, std::string name, std::string path);//往表格里填入数据。参数为待填入数据的行与数据
    void startToGet();//获取文件内容并建树
    void writeToFile(Tnode* head);//未被使用
    void startSearch(string);//开始搜索，参数为模式串
    int getShowType();//获取显示的文件类型
    void readFile();//读取文件数据
    void startThread();//新线程 监听文件更新
    void MyFind(int p, string path, bool &fc, bool &fu, int &type, string &s);//未使用
    string GbkToUtf8(const char *src_str);//GBK转UTF8
    void preView(int, int, bool f);//文件内容预览
    void outDriver();
    void init();
    void tempTw();
    string old_name;
    std::thread th;//其他线程
    std::thread th_update;//监听更新的线程
    bool th_exit;//是否结束搜索
    bool initEnd;
//    mutex mu;
private slots:
    void twShow(int getRet);//表格内容展示，懒加载
    void openFilePath();//打开文件路径
    void runFile();//打开文件
    void copyFilePath();//复制文件绝对路径到剪切板
    void deleteFile();//删除文件
    void renameFile();//文件重命名action的槽函数
    void filePropertise();//文件属性
    void exportAnswer();//搜索结果导出
    void exit_action();//程序结束
    void selectAll();//全选
    void antiSelect();//反选
    void copyFile();//复制文件到剪切板
    void cutFile();//剪切文件到剪切板
    void showPreview();//文件预览
    void showStatusBar();//状态栏
    void sortByName();
    void sortByPath();
    void sortByAsec();//升序
    void sortByDesc();
    void caseSensitive();//区分大小写
    void useRegex();//使用正则
    void showAll();
    void onlyAudio();
    void onlyVideo();
    void onlyCompare();
    void onlyPicture();
    void onlyDocument();
    void onlyExecutable();
    void showHelp();//帮助
    void searchGrammer();//搜索语法
    void regexGrammer();//正则语法
    void englishUse();//英文显示
    void chineseUse();//中文显示
    void Minimal();//最小化到托盘区
    void search();//搜索
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);//托盘图标监听
    void action1_triggered();//打开窗口
    void action2_triggered();//退出程序
    void startRename(int, int);//准备文件重命名
    void startRename(QTableWidgetItem*);
    void startRE(int, int);//文件重命名
    void startRE(QTableWidgetItem*);

    void preView(QTableWidgetItem*);
    void show_setting();
    void sortByColumn(int);
    void SlotTempTwShow();
    void twSelectionChanged();
signals:
    void tempTwShow();
};

#endif // TESTCLASS_H
