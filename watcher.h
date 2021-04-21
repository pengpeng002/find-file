#ifndef WATCHER_H
#define WATCHER_H
#include <windows.h>
#include <vector>
#include <bits/stdc++.h>
#include "search_in_tree.h"
#include <mutex>
using namespace std;
typedef struct TEST_INFO {
    DWORD NextEntryOffset;
    DWORD Action;
    DWORD FileNameLength;
    WCHAR FileName[100];
}_TEST_INFO;
struct FileNode
{
    int op;
    string path;
    string name;
};
//extern mutex mu;
extern bool Update_Is_Run;
void watcher(char* vol);
void updateTree();
#endif // WATCHER_H
