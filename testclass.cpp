#include "testclass.h"
#include "ui_testclass.h"
unordered_map<int, node> um;//变量的定义
int endx;
bool Update_Is_Run;
testClass::testClass(QWidget *parent)//构造函数
    : QMainWindow(parent),
      ui(new Ui::testClass)
{
    rej = new reject();//帮助窗口的类的实例化
    Setting = new setting();
    ui->initUi(this);//初始化UI界面
    QObject::connect(this, SIGNAL(tempTwShow()), this, SLOT(SlotTempTwShow()));
    th_exit=1;//初始化 搜索线程 可以执行
    initEnd=0;
    th=std::thread(&testClass::init, this);

}

testClass::~testClass()
{
    delete ui;//系统生成的
    delete rej;//可以不delete。因为它只有程序进程结束时才需要delete。但操作系统自己会回收资源
}

void testClass::init()
{
    if(!isAdmin()) errorBox(u8("请以管理员身份运行\n").toLocal8Bit().toStdString());
    if(access("C:\\Program Files\\FindFile\\driver.dat", F_OK )==-1 )
    {
        if(access("C:\\Program Files\\FindFile", 0) != 0) mkdir("C:\\Program Files\\FindFile");
        getDriver();//获取所有为NTFS文件系统的磁盘名
        outDriver();
    }
    string path=getcwd(NULL, 0);
    path+="\\AService.exe";
    SC_HANDLE sh=OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE);
    SC_HANDLE ch=CreateServiceA(sh,"FindFileService","FindFileService",SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,SERVICE_AUTO_START,SERVICE_ERROR_NORMAL,path.c_str(),
        NULL,NULL,"",NULL,"");
    if(ch) StartService(ch,0,NULL);
    else ch=OpenServiceA(sh, "FindFileService", SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);

    SERVICE_STATUS sta;
    QueryServiceStatus(ch, &sta);
    if(access("out.dat", F_OK)==-1 ||
            access("C:\\Program Files\\FindFile\\update.dat", F_OK )==-1 ||
            sta.dwCurrentState==SERVICE_STOPPED)
    {
        remove("C:\\Program Files\\FindFile\\update.dat");
        ofstream out("C:\\Program Files\\FindFile\\update.dat",ios::app|ios::binary);
        out.close();
        um[0]={"\\",7,0,0,0};//所有磁盘的根节点，实际不存在。 节点内容依次为文件名，文件类型，父节点位置，孩子节点位置，兄弟节点位置
        endx=0;//树的最后一个节点的位置
        startToGet();//开始获取所有文件名
        StartService(ch,0,NULL);
    }
    else
    {
        if(ui->english_action->isChecked())
            ui->label->setText("now updating...");
        else
            ui->label->setText(u8("正在更新。。。"));
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
        }
        in.close();
        readFile();
    }

    CloseServiceHandle(sh);
    CloseServiceHandle(ch);
    if(ui->english_action->isChecked())
        ui->label->setText("now you could search...");
    else
        ui->label->setText(u8("现在你可以搜索了。。。"));
    initEnd=1;
}

void testClass::outDriver()
{
    ofstream out("C:\\Program Files\\FindFile\\driver.dat",ios::out|ios::binary);
    int len=dri.size();
    out.write((char*)&len, 4);
    for(auto i:dri)
    {
        out.write(i,2);
    }
    out.close();
}

//读文件
void testClass::readFile()
{
    int first, len, type, father, son, bro;//文件位置，字符串长度，文件类型，父节点位置，子节点位置，兄弟节点位置
//    char na[MAX_PATH];//文件名。没有new，就可以不delete了
    clock_t st=clock();
    ifstream in("out.dat", ios::in|ios::binary);
    //数据格式： endx（最后一个节点的位置）   文件位置 文件名长度 文件名 类型 父节点位置 子节点位置 兄弟节点位置。。。
    in.read((char*)&endx, 4);
    while(in.read((char*)&first, 4))
    {
        string name="";
        char ch;
        in.read((char*)&len, 4);
        for(int i=0;i<len;i++) in.read((char*)&ch, 1), name+=ch;
        in.read((char*)&type, 4);
        in.read((char*)&father, 4);
        in.read((char*)&son, 4);
        in.read((char*)&bro, 4);
        um[first]={name, type, father, son, bro};
    }
    in.close();
    clock_t ed=clock();
//    cout<<"use time:"<<ed-st<<endl;
//    cout<<um.size()<<endl;

    startThread();
}
//监听文件的更新
void testClass::startThread()
{
//    cout<<"now, i am update!\n";
    {
//    for(auto i:dri)
//    {
//        char* temp=new char[4];
//        temp[0]=i[0], temp[1]=i[1];
//        temp[2]='\\', temp[3]='\0';
//        std::thread th(watcher,temp);
//        th.detach();
//    }
    }
    Update_Is_Run=1;//值为真表示继续监听。值为假表示停止监听
    th_update=std::thread(updateTree);
//    std::thread th(updateTree);
//    th.detach();
}

//table的显示 槽函数。参数为滚动条的位置 所对应的table的行数
void testClass::twShow(int getRet)
{
//    cout<<"this is twShow, getRet is "<<getRet<<endl;
    int twHeight=ui->tw->height();//table高度
    int count=ui->tw->rowCount();//table行数
    if(count==0 || ans.size()==0) return ;
    int lineHeight=ui->tw->rowHeight(0);//table的行高
    int pageNum=twHeight/lineHeight;//一屏所能显示的行数
    if(ui->tw->selectedItems().isEmpty())//table没有被选中的元素，则清空table。应该可以减少内存消耗
        ui->tw->clearContents();
    for(int i=getRet;i<min(getRet+pageNum, count);i++)
        //table里只放会显示出来的内容。屏幕无法显示的就不放数据
    {
        insertItem(i, ans[i].first, ans[i].second);
    }
}
//打开所有被选中的行的文件路径
void testClass::openFilePath()
{
    QList<QTableWidgetSelectionRange> list=ui->tw->selectedRanges();//所有被选中的行
    for(auto i:list)//遍历
    {
        for(int j=i.topRow();j<=i.bottomRow();j++)
        {
            system(("explorer /select,"+ans[j].second).c_str());//打开资源管理器并选中文件
        }
    }
}
//打开所有被选中的行对应的文件
void testClass::runFile()
{
    QList<QTableWidgetSelectionRange> list=ui->tw->selectedRanges();
    for(auto i:list)
    {
        for(int j=i.topRow();j<=i.bottomRow();j++)
        {
            system(("start "+ans[j].second).c_str());
        }
    }
}
//复制文件路径到剪切板
void testClass::copyFilePath()
{
    QList<QTableWidgetSelectionRange> list=ui->tw->selectedRanges();
    QClipboard *clip = QApplication::clipboard();
    string s="";
    for(auto i:list)
    {
        for(int j=i.topRow();j<=i.bottomRow();j++)
        {
            s+=ans[j].second;
        }
    }
    clip->setText(QString::fromLocal8Bit(s.c_str()));
}
//删除文件
void testClass::deleteFile()
{
    QList<QTableWidgetSelectionRange> list=ui->tw->selectedRanges();
    for(auto i:list)
    {
        for(int j=i.topRow();j<=i.bottomRow();j++)
        {
            remove(ans[j].second.c_str());
        }
    }
}
//文件重命名，记录旧文件名
void testClass::renameFile()
{
    QList<QTableWidgetItem*> list=ui->tw->selectedItems();
    if(list.length() == 2)//因为是整行选中，所有选中数为2表示只有一行选中
    {
        ui->tw->editItem(list.at(0));
        old_name = (list.at(0)->text()).toStdString();
    }
}
//打开文件属性框。由于程序无法提供文件的属性，作为补偿，提供查看属性框的功能
void testClass::filePropertise()
{
    QList<QTableWidgetSelectionRange> list=ui->tw->selectedRanges();
    for(auto i:list)
    {
        for(int j=i.topRow();j<=i.bottomRow();j++)
        {
            SHELLEXECUTEINFOA ShExecInfo = {0};
            ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
            ShExecInfo.fMask = SEE_MASK_INVOKEIDLIST ;
            ShExecInfo.hwnd = NULL;
            ShExecInfo.lpVerb = "properties";
            ShExecInfo.lpFile = ans[j].second.c_str();
            ShExecInfo.lpParameters = "";
            ShExecInfo.lpDirectory = NULL;
            ShExecInfo.nShow = SW_SHOW;
            ShExecInfo.hInstApp = NULL;
            ShellExecuteExA(&ShExecInfo);
        }
    }
}
//导出搜索结果，到txt文件
void testClass::exportAnswer()
{
    string name=QFileDialog::getSaveFileName(this,"select a out file", "C:\\",u8("文本文档(*.txt)"))\
            .toLocal8Bit().toStdString();
    if(name!="")
    {
        freopen(name.c_str(), "w", stdout);
        for(auto i:ans)
        {
            cout<<i.first<<" "<<i.second<<endl;
        }
        freopen("CON","w",stdout);
    }

}
//程序退出
void testClass::exit_action()
{
    action2_triggered();
//    QApplication *app;
//    app->exit(0);
}
//全选表格
void testClass::selectAll()
{
    ui->tw->selectAll();
}
//表格反选
void testClass::antiSelect()
{
//    printf("anti select");
    QList<QTableWidgetItem*> list=ui->tw->selectedItems();//获取所有被选中的
    ui->tw->selectAll();//表格全选
    for(auto i:list)//所有被选中的设置未被选中
    {
        i->setSelected(false);
    }
}
//复制文件到剪切板
void testClass::copyFile()
{
    QList<QTableWidgetSelectionRange> list=ui->tw->selectedRanges();
    QClipboard *clip = QApplication::clipboard();
    QList<QUrl> list2;
    for(auto i:list)
    {
        for(int j=i.topRow();j<=i.bottomRow();j++)
        {
            cout<<ans[j].second<<endl;
            QUrl url=QUrl::fromLocalFile(GbkToUtf8(ans[j].second.c_str()).c_str());
            list2.push_back(url);
        }
    }
    QMimeData *mimeData=new QMimeData();
    mimeData->setUrls(list2);
    clip->setMimeData(mimeData);
}
//文件剪切
void testClass::cutFile()
{
    QList<QTableWidgetSelectionRange> list=ui->tw->selectedRanges();
    QClipboard *clip = QApplication::clipboard();
    QList<QUrl> list2;
    for(auto i:list)
    {
        for(int j=i.topRow();j<=i.bottomRow();j++)
        {
            cout<<ans[j].second<<endl;
            QUrl url=QUrl::fromLocalFile(GbkToUtf8(ans[j].second.c_str()).c_str());
            list2.push_back(url);
        }
    }
    QMimeData *mimeData=new QMimeData();
    mimeData->setUrls(list2);
    int dropEffect = 2; //2表示剪切，5表示复制
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream << dropEffect;
    mimeData->setData("Preferred DropEffect", data);
    clip->setMimeData(mimeData);
}
//show preview
void testClass::showPreview()
{
    ui->textEdit->setVisible(ui->preview_action->isChecked());
}
//show status bar
void testClass::showStatusBar()
{
    ui->statusBar->setVisible(ui->statusBar_action->isChecked());
}
//sort by file name
void testClass::sortByName()
{
//    printf("sort by name");
    if(ui->asec_action->isChecked())
    {
        ui->tw->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
        sort(ans.begin(), ans.end(), cmpNA);//直接调用sort函数
    }
    else
    {
        ui->tw->horizontalHeader()->setSortIndicator(0, Qt::DescendingOrder);
        sort(ans.begin(), ans.end(), cmpND);
    }
    int getRet=ui->tw->horizontalScrollBar()->value();
    twShow(getRet);
}
//sort by file path
void testClass::sortByPath()
{
//    printf("sort by path");
    if(ui->asec_action->isChecked())
        ui->tw->horizontalHeader()->setSortIndicator(1, Qt::AscendingOrder), sort(ans.begin(), ans.end(), cmpPA);
    else
        ui->tw->horizontalHeader()->setSortIndicator(1, Qt::DescendingOrder), sort(ans.begin(), ans.end(), cmpPD);
    int getRet=ui->tw->horizontalScrollBar()->value();
    twShow(getRet);
}
//
void testClass::sortByAsec()
{
//    printf("sort by asec");
    if(ui->name_action->isChecked())
        ui->tw->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder), sort(ans.begin(), ans.end(), cmpNA);
    else
        ui->tw->horizontalHeader()->setSortIndicator(1, Qt::AscendingOrder), sort(ans.begin(), ans.end(), cmpPA);
    int getRet=ui->tw->horizontalScrollBar()->value();
    twShow(getRet);
}

void testClass::sortByDesc()
{
//    printf("sort by desc");
    if(ui->name_action->isChecked())
        ui->tw->horizontalHeader()->setSortIndicator(0, Qt::DescendingOrder), sort(ans.begin(), ans.end(), cmpND);
    else
        ui->tw->horizontalHeader()->setSortIndicator(1, Qt::DescendingOrder), sort(ans.begin(), ans.end(), cmpPD);
    int getRet=ui->tw->horizontalScrollBar()->value();
    twShow(getRet);

}
//use case sensitive
void testClass::caseSensitive()
{
//    printf("case sensitive");
}
//use regex search
void testClass::useRegex()
{
//    printf("use regex");
}

void testClass::showAll()
{
//    printf("show all");
}

void testClass::onlyAudio()
{
//    printf("audio");
}

void testClass::onlyVideo()
{
//    printf("video");
}

void testClass::onlyPicture()
{
//    printf("picture");
}

void testClass::onlyDocument()
{
//    printf("document");
}

void testClass::onlyCompare()
{
//    printf("compress");
}

void testClass::onlyExecutable()
{
//    printf("executable");
}
//显示帮助，没有帮助。。。
void testClass::showHelp()
{
    rej->te->setText(u8("sorry, no help"));
    rej->show();
}
//搜索语法
void testClass::searchGrammer()
{
    rej->te->setText(u8("通配符:\n\
                        *	匹配 0 个或多个字符.\n\
                        ?	匹配 1 个字符.\n\
指定搜索路径：\n\
                        : 指定搜索路径\n\
搜索示例：\n\
                        *.txt:C:\\Users\\Administrator\\Desktop 搜索管理员用户桌面上所有文件名以.txt结尾的文件，即搜索txt文件"));
    rej->show();
}
//正则表达式语法
void testClass::regexGrammer()
{
    rej->te->setText(u8("正则表达式语法:\n\
                            \n\
                            a|b		 匹配 a 或 b\n\
                            gr(a|e)y		 匹配 gray 或 grey\n\
                            .		 匹配任一字符\n\
                            [abc]		 匹配任一字符: a 或 b 或 c\n\
                            [^abc]		 匹配任一字符, 但不包括 a, b, c\n\
                            [a-z]		 匹配从 a 到 z 之间的任一字符\n\
                            [a-zA-Z]		 匹配从 a 到 z, 及从 A 到 Z 之间的任一字符\n\
                            ^		 匹配文件名的头部\n\
                            $		 匹配文件名的尾部\n\
                            ( )		 匹配标记的子表达式\n\
                            \n		 匹配第 nth 个标记的子表达式, nth 代表 1 到 9\n\
                            \b		 匹配字词边界\n\
                            *		 匹配前一项内容 0 或多次\n\
                            ?		 匹配前一项内容 0 或 1 次\n\
                            +		 匹配前一项内容 1 或多次\n\
                            *?		 匹配前一项内容 0 或多次 (懒人模式)\n\
                            +?		 匹配前一项内容 1 或多次 (懒人模式)\n\
                            {x}		 匹配前一项内容 x 次\n\
                            {x,}		 匹配前一项内容 x 或多次\n\
                            {x,y}		 匹配前一项内容次数介于 x 和 y 之间\n\
                            \\		 特殊转义字符\n\
"));
    rej->show();
}

void testClass::show_setting()
{
    Setting->show();
    Setting->lineEdit->setFocus();
}

//设置显示语言为中文
void testClass::chineseUse()
{
    ui->setChinese();

    QStringList headers;
    headers<<u8("文件名")<<u8("全路径");//table表头设置中文
    ui->tw->setHorizontalHeaderLabels(headers);

    QString temp=ui->label->text();//获取状态栏显示的内容
    string s=temp.toLocal8Bit().toStdString();
    if(s[0]<'a'||s[0]>'z') return;//不是小写字母，说明显示语言是中文
    if(s=="now searching...") ui->label->setText(u8("正在搜索。。。"));
    else if(s=="now sorting...") ui->label->setText(u8("正在排序。。。"));
    else if(s=="now updating...") ui->label->setText(u8("正在更新。。。"));
    else if(s=="now you could search...") ui->label->setText(u8("现在你可以搜索了。。。"));
    else if(temp.startsWith("now"))
    {
        char c=s[14];
        string s="正在搜索";
        s+=c;
        s+="盘。。。";
        ui->label->setText(u8(s.c_str()));
    }
    else
    {
        string out=string("找到了 ")+to_string(ans.size())+string(" 个元素");
        ui->label->setText(u8(out.c_str()));
    }
}
//设置显示语言为英文//注释参考显示中文
void testClass::englishUse()
{
    ui->setEnglish();

    QStringList headers;
    headers<<u8("name")<<u8("path");
    ui->tw->setHorizontalHeaderLabels(headers);

    QString temp=ui->label->text();
    string s=temp.toLocal8Bit().toStdString();
    if(s[0]>='a'&&s[0]<='z') return;//当前显示语言为英文
    if(s=="正在搜索。。。") ui->label->setText(u8("now searching..."));
    else if(s=="正在排序。。。") ui->label->setText(u8("now sorting..."));
    else if(s=="正在更新。。。") ui->label->setText(u8("now updating..."));
    else if(s=="现在你可以搜索了。。。") ui->label->setText("now you could search...");
    else if(temp.startsWith("正在搜索"))
    {
        char c=s[8];
        string s="now get volumn";
        s+=c;
        s+="...";
        ui->label->setText(s.c_str());
    }
    else
    {
        string out=string("find ")+to_string(ans.size())+string(" items");
        ui->label->setText(u8(out.c_str()));
    }
}
//程序主窗口显示
void testClass::action1_triggered()
{
    this->show();
}
//程序退出
void testClass::action2_triggered()
{
    delete rej;
    delete Setting;
    this->hide();//先隐藏显示窗口，
//    cout<<"now I will start exit, please wait a little\n";
    Update_Is_Run=0;//停止监听文件更新
    th_update.join();//等待监听文件更新线程的退出
    for(auto i:dri) delete i;//new了的要delete掉。虽然本程序即使不delete，也不至于内存泄露
    clock_t st=clock();
    {
//    FILE* p=fopen("out.dat", "rb");
//    fwrite(&endx, 4, 1, p);
//    for(auto i:um)
//    {
//        node temp=i.second;
//        int len=temp.name.size()+1;
//        fwrite(&i.first, 4, 1, p);
//        fwrite(temp.name.c_str(), len, 1, p);
//        fwrite(&temp.type, 4, 1, p);
//        fwrite(&temp.father, 4, 1, p);
//        fwrite(&temp.son, 4, 1, p);
//        fwrite(&temp.bro, 4, 1, p);
//    }
//    fclose(p);
    }
    ofstream out("out.dat", ios::out|ios::binary);
    //数据格式： endx（最后一个节点的位置）   文件位置 文件名长度 文件名 类型 父节点位置 子节点位置 兄弟节点位置。。。
    out.write((char*)&endx, 4);
    for(auto i:um)
    {
        node temp=i.second;
        int len=temp.name.size();
        out.write((char*)&i.first, 4);
        out.write((char*)&len, 4);
        out.write(temp.name.data(), len);
        out.write((char*)&temp.type, 4);
        out.write((char*)&temp.father, 4);
        out.write((char*)&temp.son, 4);
        out.write((char*)&temp.bro, 4);
    }
    out.close();
    clock_t ed=clock();
//    cout<<"use time:"<<ed-st<<"\nnow application exit!\n";
    exit(0);//进程结束，所有申请的资源都会被操作系统回收
}
//最小化到系统托盘区
void testClass::Minimal()
{
    this->hide();//当前窗口隐藏
    ui->tray->show();//托盘区显示图标
}
//监听托盘区图标
void testClass::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger://单击，不管单击双击都显示窗口，故无break
    case QSystemTrayIcon::DoubleClick://双击
        this->show();
        break;
    default://不管其他操作
        break;
    }
}
//监听程序退出，询问是否确定退出
void testClass::closeEvent(QCloseEvent *e)
{
    QMessageBox *msg=new QMessageBox();
    msg->setIcon(QMessageBox::Question);
    msg->setWindowTitle(u8("提示"));
    msg->setText(u8("确定要退出整个程序吗?"));
    msg->setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    msg->setDefaultButton(QMessageBox::No);
    msg->button(QMessageBox::Yes)->setText(u8("是的，确定退出"));
    msg->button(QMessageBox::No)->setText(u8("最小化到托盘区"));
    msg->button(QMessageBox::Cancel)->setText(u8("取消"));
    int res=msg->exec();
    if(res == QMessageBox::Yes)
    {
        action2_triggered();
        e->accept();
    }
    else if(res == QMessageBox::No)
    {
        Minimal();
        e->ignore();
    }
    else
    {
        e->ignore();
    }
}
//当点击搜索按钮或在输入框按下回车，触发此函数，表示进行搜索
void testClass::search()
{
    if(!initEnd) return;
    string s=ui->lineEdit->text().toLocal8Bit().toStdString();//获取输入框的内容
//    printf("input string is: %s\n", s.c_str());
    clock_t st=clock();
//    if(!th_exit)//如果之前的搜索还没结束，停止搜索
//    {
//        th_exit=1;
//        th.join();//等待上一次搜索的结束
//        cout<<"now join end!\n";
//    }
//    th=std::thread(&testClass::startSearch, this, s);//新开线程搜索，避免搜索时界面卡住
    if(ui->english_action->isChecked())
        ui->label->setText("now searching...");
    else
        ui->label->setText(u8("正在搜索。。。"));
    ui->label->repaint();
//    ui->label->show();
//    Sleep(500);
    startSearch(s);
    clock_t ed=clock();
//    printf("search end!, time use %d\n", ed-st);
}
//table放入元素
void testClass::insertItem(int lines, std::string name, std::string path)
{
//    cout<<"name="<<name<<"***path="<<path<<endl;
    QTableWidgetItem *item=new QTableWidgetItem(GbkToUtf8(name.c_str()).c_str());
//    cout<<"rowcount = "<<ui->tw->rowCount()<<endl;
    ui->tw->setItem(lines, 0, item);
    ui->tw->setItem(lines, 1, new QTableWidgetItem(GbkToUtf8(path.c_str()).c_str()));
//    QTableWidgetItem *
    item=ui->tw->item(lines, 1);
    item->setFlags(item->flags() & (~Qt::ItemIsEditable));

}
//获取所有文件并建树
void testClass::startToGet()
{
    int len=dri.size();
//    cout<<dri.size()<<endl;
    clock_t st=clock();
    //虽然实际上每个盘都是单独的一棵树，但我们可以将其视为一个节点的三棵子树
    for(int i=0;i<len;i++)//先将每个盘符作为父节点为其创建节点
    {
        um[endx+1]={dri[i], 7, 0, 0, 0};
        if(i) um[endx].bro=endx+1;
        else um[0].son=endx+1;
        endx++;
    }//然后获得该节点的子节点，并建树
    for(int i=0;i<len;i++)
    {
        if(ui->english_action->isChecked())
        {
            string s="now get volumn";
            s+=dri[i][0];
            s+="...";
            ui->label->setText(s.c_str());
        }
        else
        {
            string s="正在搜索";
            s+=dri[i][0];
            s+="盘。。。";
            ui->label->setText(u8(s.c_str()));
        }
        startGet(dri[i], i+1);
    }
    clock_t ed=clock();
//    cout<<"init use time: "<<ed-st<<endl;
//    cout<<"sizeof um is: "<<um.size()<<endl;
    std::thread th(checkType, 1);//监听更新
    th.detach();
    startThread();
}
//未使用
void testClass::writeToFile(Tnode* head)
{
//    int i=0;
//    while(head!=NULL)
//    {
//        i++;
//        head=head->next;
//    }
//    cout<<"file num is "<<i<<endl;
}
//未使用，应该是由于树太大了，导致递归的栈溢出，无法使用递归去查找。树过大的原因应该在于记录的路径，即最后一个参数
void testClass::MyFind(int p, string path, bool &fc, bool &fu, int &type, string &s)
{
    if(!p) return ;
    if(path!="") path+="\\";
    path+=um[p].name;
    if( (type==0 || um[p].type == type) && ok(um[p].name, s, fc, fu) )
        ans.push_back(make_pair(um[p].name, path));
    MyFind(um[p].son, path, fc, fu, type, s);
    MyFind(um[p].bro, path, fc, fu, type, s);
}
//stack<pair<int, string> > sta;
//开始搜索
void testClass::startSearch(string s)
{    
    th_exit=0;
    string fatherPath="";//被指定的父路径
    bool f=1, fc=ui->caseSensitive_action->isChecked(), fu=ui->useRegex_action->isChecked();
    int len=s.length();//输入字符串的长度
    for(int i=0;i<len;i++)
    {
        if(s[i]==':')//因为文件名不能包含冒号，所以找到的第一个冒号一定是用来指定父路径的
        {
            fatherPath=s.substr(i+1);//被指定的父路径 [i+1,len)
            s=s.substr(0,i);//搜索的模式串 [0,i)
            if((!fu) && f) s="*"+s+"*", f=0;//如果不使用通配符，且之前没有出现过星号，替用户加上星号
            break;
        }
        else if(s[i]=='*') f=0;//记录出现过星号
    }
    if((!fu) && f) s="*"+s+"*";//如果不使用通配符，且没有出现过星号，替用户加上星号
    vector<string> vst;
    split(fatherPath,'\\', vst);//将指定的父路径按'\\'分开，放入vector
    int p=findNode(vst), type=getShowType();//被指定的父路径的位置， 被指定的可显示文件类型

//    cout<<"i am searching...\n";
    stack<int> sta;//由于0用来表示了没有找到，但0同时也是所有盘的父节点位置，所以只能用-1表示未指定路径，即进行全盘搜索
    //自己最开始的时候没有设计好，自己作的。但如果要改，就会需要改很多地方。所以就没改了
    if(p==-1) sta.push(um[0].son);
    else if(p) sta.push(um[p].son);
    ans.clear();//清空上一次搜索结果的记录， ans记录搜索结果
    clock_t st=clock();
    ans.capacity();
    mu.lock();//加锁，避免在搜索的同时更新树，导致程序出现问题
    while(!sta.empty())//就是一个普通DFS，深度优先搜索。
    {
        int p=sta.top();
        node temp=um[p];
        sta.pop();
        if(!p) continue;
        if((type==0 || temp.type==type) && ok(temp.name, s, fc, fu))//如果该节点文件名符合搜索条件，加入ans
        {
            ans.push_back(make_pair(temp.name, getPath(p)));//在加入的时候才查找他的绝对路径
            if(ans.size() >= searchLimit) break;
            if(ans.size()==30)//虽然可以算出来一屏能显示的行数，但在第一次搜索的时候，由于table的行数为0，无法计算行高，会有异常
                //而且怎么想也不会超过30条，所以就直接设置30了。
            {
                tempTw();
            }
            else if(ans.size()%10000==1) ui->tw->setRowCount(ans.size());//更新滚动条，避免出现过了好几秒突然滚动条变短的情况
        }
        //当要查路径的节点很多时，会很慢。测试查找100w个节点的路径，需要近5s。也正因如此，我用了多线程来查找
        //而如果用户在没搜完就要搜其他的，即又开了一个搜索的线程，程序会异常。所以我用了th_exit来停止搜索

        sta.push(temp.son);//孩子
        sta.push(temp.bro);//兄弟
    }
    mu.unlock();//解锁
    clock_t ed=clock();
//    cout<<"find use time "<<ed-st<<endl;
    if(ui->english_action->isChecked())
        ui->label->setText("now sorting...");
    else
        ui->label->setText(u8("正在排序。。。"));
    ui->label->repaint();
    st=clock();
    if(ui->name_action->isChecked()) sortByName();
    //排序，90w条数据需要10多秒，但也没法优化啊。原因在于排序的元素是pair<string, string>，可我没法改进。。。
    else sortByPath();
    ed=clock();
//    cout<<"sort use time "<<ed-st<<endl;
    string show;
    if(ui->english_action->isChecked())
    {
        show="find ";
        show+=to_string(ans.size());
        show+=" items";
    }
    else
    {
        show="找到了 ";
        show+=to_string(ans.size());
        show+=" 个元素";
    }
    ui->label->setText(u8(show.c_str()));
//    cout<<"find items : "<<ans.size()<<endl;
    ui->tw->setRowCount(ans.size());
    if(ans.size() <= 50) tempTw();
    th_exit=1;
}
//gbk to utf8
string testClass::GbkToUtf8(const char *src_str)
{
    int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len + 1]; memset(str, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    string strTemp = str;
    if (wstr) delete[] wstr;
    if (str) delete[] str;
    return strTemp;
}
//sort by name asc
bool cmpNA(pa a, pa b)
{
    return a.first<b.first;
}
//sort by name desc
bool cmpND(pa a, pa b)
{
    return a.first>b.first;
}
//sort by path asc
bool cmpPA(pa a, pa b)
{
    return a.second < b.second;
}
//sort by name desc
bool cmpPD(pa a, pa b)
{
    return a.second > b.second;
}

void testClass::startRename(int a, int b)
{
    if(a >= ui->tw->rowCount() || a<0) return ;
    runFile();
//    startRename(ui->tw->item(a,b));
}
//记录旧名字
void testClass::startRename(QTableWidgetItem* item)
{
//    cout<<"record old name\n";
    if(item->column()!=1)
    old_name =item->text().toStdString();
}
//进行文件重命名
void testClass::startRE(int a, int b)
{
    if(a >= ui->tw->rowCount() || a<0 || old_name=="") return ;//不知道为什么，a和b有可能为-1，所以需要判断数据是否合法
//    cout<<"a="<<a<<", b="<<b<<endl;
    QTableWidgetItem* pathItem=ui->tw->item(a,1);
    string pathFull=pathItem->text().toLocal8Bit().toStdString();//获取绝对路径，包含旧文件名
    string path;//记录不含旧文件名的路径
    int len=pathFull.length();
    for(int i=len-1;i>=0;i--)//记录不含旧文件名的路径
    {
        if(pathFull[i]=='\\')
        {
            path=pathFull.substr(0, i+1);
            break;
        }
    }
//    cout<<pathFull<<"\n"<<path<<endl;
    QTableWidgetItem* item=ui->tw->item(a,0);
    string new_name=item->text().toLocal8Bit().toStdString();
    new_name=path+new_name;//新文件名，包含路径
    if(old_name != new_name && old_name!="")
    {
//        cout<<"i'm rename "<<ui->tw->item(a,b+1)->text().toLocal8Bit().toStdString().c_str()<<" to "<<new_name.c_str()<<"\n";
        rename(ui->tw->item(a,1)->text().toLocal8Bit().toStdString().c_str(), new_name.c_str());
        ui->tw->item(a,1)->setText(GbkToUtf8(new_name.c_str()).c_str());
    }
    old_name="";
}
void testClass::startRE(QTableWidgetItem *item)
{
    startRE(item->row(), item->column());
}

//获得要显示的文件类型
int testClass::getShowType()
{
    if(ui->all_action->isChecked()) return 0;
    else if(ui->video_action->isChecked()) return 1;
    else if(ui->audio_action->isChecked()) return 2;
    else if(ui->compare_action->isChecked()) return 5;
    else if(ui->document_action->isChecked()) return 4;
    else if(ui->exe_action->isChecked()) return 6;
    else if(ui->picture_action->isChecked()) return 3;
}
//文件内容预览
void testClass::preView(int a, int b, bool f)
{
    if(!ui->preview_action->isChecked()) return ;//如果当前为不预览状态，则不执行该函数
    if(a>=ui->tw->rowCount() || a<0) return ;//保证数据合法

//    cout<<"a,b = "<<a<<","<<b<<endl;
    QTableWidgetItem* item1=ui->tw->item(a,0);
    QTableWidgetItem* item2=ui->tw->item(a,1);
    if(f && ui->tw->selectedItems().length()==2 && item1->isSelected())
    {
//        startRename(item1);
//        return ;
    }
    ui->textEdit->clear();
    string s1=item1->text().toLocal8Bit().toStdString();//获取文件名和路径
    string s2=item2->text().toLocal8Bit().toStdString();
//    cout<<item->text().toLocal8Bit().toStdString()<<endl;
//    return ;//或许是由于太长了？反正下面的方法获得不了正确的文件名和路径
//    const char     *name=ui->tw->item(a,1)->text().toLocal8Bit().toStdString().c_str();
//    const char *fullName=ui->tw->item(a,1)->text().toLocal8Bit().toStdString().c_str();
    const char *name=s1.c_str(), *fullName=s2.c_str();
    int ft=getCheckType(name), st=getShowType();
//    cout<<"full name = "<<fullName<<"\n name= "<<name<<", type = "<<ft<<endl;
    if(ft==1 || ft==2)//视频或音频则调用Windows Media Player播放
    {
        ui->qaw->dynamicCall("URL",QString::fromLocal8Bit(fullName));
        ui->textEdit->setHidden(true);
        ui->qaw->show();
    }
    else
    {
        ui->qaw->setHidden(true);
        ui->textEdit->show();
        ui->textEdit->setEnabled(true);
        ui->textEdit->setReadOnly(true);
        ui->textEdit->setAcceptRichText(false);
        ui->textEdit->setAlignment(Qt::AlignLeft);
        if(ft==4)//文本文档
        {
            FILE* p=fopen(fullName, "r");
            if(!p)
            {
                cout<<"open file fail!\n";
                ui->textEdit->setText("打开文件失败");
                ui->textEdit->setEnabled(false);
                ui->textEdit->setAlignment(Qt::AlignCenter);
                return ;
            }
            char ch[1005];
            int line=0;
            while(!feof(p))
            {
                fgets(ch, 1000, p);
                ui->textEdit->insertPlainText(QString::fromLocal8Bit(ch));
                line++;
                if(line >= 50) break;
            }
            fclose(p);
            ui->textEdit->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);
        }
        else if(ft==3)//图片
        {
            string s="<img src=\"";//<img src="文件路径">
            s+=fullName;
            s+="\">";
            cout<<s<<endl;
    //        ui->textEdit->setStyleSheet("#img {width:100px;height:auto}");
            ui->textEdit->insertHtml(QString::fromLocal8Bit(s.c_str()));
        }
        else
        {
            ui->textEdit->setText("无法预览");
            ui->textEdit->setEnabled(false);
            ui->textEdit->setAlignment(Qt::AlignCenter);
        }
    }
}

void testClass::preView(QTableWidgetItem *item)
{
    preView(item->row(), item->column(), 1);
}

void testClass::sortByColumn(int col)
{
//    cout<<"col="<<col<<endl;
    if(col==0)
    {
        ui->name_action->setChecked(1);
        if(ui->tw->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder)
        {
            ui->asec_action->setChecked(1);
            sort(ans.begin(), ans.end(), cmpNA);
        }
        else
        {
            ui->desc_action->setChecked(1);
            sort(ans.begin(), ans.end(), cmpND);
        }
    }
    else
    {
        ui->path_action->setChecked(1);
        if(ui->tw->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder)
        {
            ui->asec_action->setChecked(1);
            sort(ans.begin(), ans.end(), cmpPA);
        }
        else
        {
            ui->desc_action->setChecked(1);
            sort(ans.begin(), ans.end(), cmpPD);
        }
    }
}

void testClass::SlotTempTwShow()
{
    int size=ans.size();
//    cout<<"i am try to show some!\n";
    ui->tw->setRowCount(size);
//    cout<<"now set row count ok\n";
    for(int i=0;i<size;i++)
    {
        insertItem(i, ans[i].first, ans[i].second);
    }
//    cout<<"end of i am try \n";
}

void testClass::tempTw()
{
    emit tempTwShow();
}

void testClass::twSelectionChanged()
{
//    cout<<"now selection changed\n";
    if(!ui->preview_action->isChecked()) return ;
    QList<QTableWidgetItem*> list=ui->tw->selectedItems();
    int len=list.length();
    if(len > 2) return;
    else if(len==0)
    {
        ui->qaw->setHidden(true);
        ui->textEdit->show();
        ui->textEdit->setText("请选择要预览的文件");
        ui->textEdit->setEnabled(false);
        ui->textEdit->setAlignment(Qt::AlignCenter);
    }
    else preView(list.at(1)->row(), list.at(1)->column(), 0);
}
