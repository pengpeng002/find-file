#include "testclass.h"
#include <QApplication>
#include <windows.h>

int main(int argc, char *argv[])
{
//    HRESULT ret = URLDownloadToFileA(0, "https://lupic.cdn.bcebos.com/20191203/3016190996_14.jpg", "C:\\Users\\pengpeng\\Desktop\\123.jpg", 0, 0);
    QApplication a(argc, argv);
    testClass w;
    w.show();

    return a.exec();
}
