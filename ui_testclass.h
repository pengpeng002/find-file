#ifndef UI_TESTCLASS_H
#define UI_TESTCLASS_H
//#pragma execution_character_set("GBK")
#include <QWidget>
#include <QAction>
#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QActionGroup>
#include <QApplication>
#include <QMainWindow>
#include <QStatusBar>
#include <QAxWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QTableWidget>
#include <QSplitter>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QTableWidgetItem>
#include <QScrollBar>
#include <QHeaderView>
#include <QComboBox>
#include <QPainter>
#include <QLabel>
#include <QGraphicsView>
//#include <QToolBar>
#define u8(x) QObject::trUtf8(x)
//有点不好写注释，放弃了。想看就自己理解吧。我只是为了方便写代码就按功能放在不同函数里了
class Ui_testClass
{
public:
    QWidget *cen;
    QMenuBar *menuBar;
    QStatusBar *statusBar;
    QMenu *file_menu, *edit_menu, *view_menu, *search_menu, *help_menu, *language_menu, *sort_menu;
    QAction *openFilePath_action, *runFile_action, *copyPath_action, *delete_action, *rename_action;
    QAction *propertise_action, *export_action, *exit_action, *minimal_action;
    QAction *selectAll_action, *antiSelect_action, *copyFile_action, *cutFile_action;
    QAction *preview_action, *statusBar_action, *research_action;
    QAction *caseSensitive_action, *useRegex_action;
    QAction *all_action, *audio_action, *video_action, *compare_action, *document_action, *picture_action, *exe_action;
    QAction *help_action, *regexGrammer_action, *searchGrammer_action, *setting_action;
    QAction *english_action, *chinese_action;
    QAction *name_action, *path_action, *asec_action, *desc_action;
    QActionGroup *group1, *group2, *group3, *group4;
    QVBoxLayout *qvbl;
    QHBoxLayout *hb1, *hb2;
    QWidget *widget1, *widget2;
    QLineEdit *lineEdit;//, *combo_edit;
    QPushButton *pushButton;
    QTableWidget *tw;
    QTextEdit *textEdit;
    QAxWidget *qaw;
    QSplitter *sp;
    QLabel *label, *la;
    QSystemTrayIcon *tray;
    QMenu *menu, *num;
    QAction *action1, *action2;
    QPainter *painter;
    QGraphicsView *gview;
//    QComboBox *combo;
//    QToolBar *toolBar;
    void initUi(QMainWindow *testClass)
    {
        if(testClass->objectName().isEmpty())
            testClass->setObjectName("testClass");

        newOthers(testClass);
        initLayout();
        newMenu(testClass);
        newAction(testClass);
        addAction();
        setCheck();
        setWidgets();
        setShortCut();
        testClass->resize(800,530);
        testClass->setCentralWidget(cen);
        testClass->setStatusBar(statusBar);
        testClass->setMenuBar(menuBar);
//        testClass->addToolBar(toolBar);

        retranslateUi(testClass);
        QMetaObject::connectSlotsByName(testClass);

    }

    void retranslateUi(QMainWindow *testClass)
    {
        testClass->setWindowTitle(QApplication::translate("testClass", "FindFile", Q_NULLPTR));
        testClass->setWindowIcon(QIcon(":/Rewrite.ico"));
        setEnglish();
        connection(testClass);
    }

    void setEnglish()
    {
        pushButton->setText(QApplication::translate("testClass", "search", Q_NULLPTR));
        file_menu->setTitle(QApplication::translate("testClass", "File(F)", Q_NULLPTR));
        edit_menu->setTitle(QApplication::translate("testClass", "Edit(E)", Q_NULLPTR));
        view_menu->setTitle(QApplication::translate("testClass", "View(V)", Q_NULLPTR));
        search_menu->setTitle(QApplication::translate("testClass", "Search(S)", Q_NULLPTR));
        help_menu->setTitle(QApplication::translate("testClass", "Help(H)", Q_NULLPTR));
        language_menu->setTitle(QApplication::translate("testClass", "Language(L)", Q_NULLPTR));
        sort_menu->setTitle(QApplication::translate("testClass", "sort", Q_NULLPTR));

        openFilePath_action->setText(QApplication::translate("testClass", "open file path", Q_NULLPTR));
        runFile_action->setText(QApplication::translate("testClass", "run file", Q_NULLPTR));
        copyPath_action->setText(QApplication::translate("testClass", "copy file path", Q_NULLPTR));
        delete_action->setText(QApplication::translate("testClass", "delete", Q_NULLPTR));
        rename_action->setText(QApplication::translate("testClass", "rename", Q_NULLPTR));
        propertise_action->setText(QApplication::translate("testClass", "propertise", Q_NULLPTR));
        export_action->setText(QApplication::translate("testClass", "export", Q_NULLPTR));
        minimal_action->setText(QApplication::translate("testClass", "minimal to tray", Q_NULLPTR));
        exit_action->setText(QApplication::translate("testClass", "exit", Q_NULLPTR));
        caseSensitive_action->setText(QApplication::translate("testClass", "case sensetive", Q_NULLPTR));
        useRegex_action->setText(QApplication::translate("testClass", "use regex", Q_NULLPTR));
        selectAll_action->setText(QApplication::translate("testClass", "select all", Q_NULLPTR));
        antiSelect_action->setText(QApplication::translate("testClass", "anti select", Q_NULLPTR));
        copyFile_action->setText(QApplication::translate("testClass", "copy file", Q_NULLPTR));
        cutFile_action->setText(QApplication::translate("testClass", "cut file", Q_NULLPTR));
        preview_action->setText(QApplication::translate("testClass", "preview", Q_NULLPTR));
        statusBar_action->setText(QApplication::translate("testClass", "status bar", Q_NULLPTR));
        research_action->setText(QApplication::translate("testClass", "research", Q_NULLPTR));
        name_action->setText(QApplication::translate("testClass", "name", Q_NULLPTR));
        path_action->setText(QApplication::translate("testClass", "path", Q_NULLPTR));
        asec_action->setText(QApplication::translate("testClass", "asec", Q_NULLPTR));
        desc_action->setText(QApplication::translate("testClass", "desc", Q_NULLPTR));
        all_action->setText(QApplication::translate("testClass", "all", Q_NULLPTR));
        video_action->setText(QApplication::translate("testClass", "video", Q_NULLPTR));
        audio_action->setText(QApplication::translate("testClass", "audio", Q_NULLPTR));
        picture_action->setText(QApplication::translate("testClass", "picture", Q_NULLPTR));
        document_action->setText(QApplication::translate("testClass", "document", Q_NULLPTR));
        compare_action->setText(QApplication::translate("testClass", "compress", Q_NULLPTR));
        exe_action->setText(QApplication::translate("testClass", "exectable", Q_NULLPTR));
        help_action->setText(QApplication::translate("testClass", "help", Q_NULLPTR));
        searchGrammer_action->setText(QApplication::translate("testClass", "search grammer", Q_NULLPTR));
        regexGrammer_action->setText(QApplication::translate("testClass", "regex grammer", Q_NULLPTR));
        setting_action->setText(QApplication::translate("testClass", "setting", Q_NULLPTR));
        english_action->setText(QApplication::translate("testClass", "english", Q_NULLPTR));
        chinese_action->setText(QApplication::translate("testClass", "简体中文", Q_NULLPTR));
        action1->setText(QApplication::translate("testClass", "open window", Q_NULLPTR));
        action2->setText(QApplication::translate("testClass", "force exit", Q_NULLPTR));
    }

    void setChinese()
    {
        pushButton->setText(QApplication::translate("testClass", "搜索", Q_NULLPTR));
        file_menu->setTitle(QApplication::translate("testClass", "文件(F)", Q_NULLPTR));
        edit_menu->setTitle(QApplication::translate("testClass", "编辑(E)", Q_NULLPTR));
        view_menu->setTitle(QApplication::translate("testClass", "视图(V)", Q_NULLPTR));
        search_menu->setTitle(QApplication::translate("testClass", "搜索(S)", Q_NULLPTR));
        help_menu->setTitle(QApplication::translate("testClass", "帮助(H)", Q_NULLPTR));
        language_menu->setTitle(QApplication::translate("testClass", "语言(L)", Q_NULLPTR));
        sort_menu->setTitle(QApplication::translate("testClass", "排序", Q_NULLPTR));

        openFilePath_action->setText(QApplication::translate("testClass", "打开文件路径", Q_NULLPTR));
        runFile_action->setText(QApplication::translate("testClass", "打开运行文件", Q_NULLPTR));
        copyPath_action->setText(QApplication::translate("testClass", "复制文件路径", Q_NULLPTR));
        delete_action->setText(QApplication::translate("testClass", "删除文件", Q_NULLPTR));
        rename_action->setText(QApplication::translate("testClass", "重命名", Q_NULLPTR));
        propertise_action->setText(QApplication::translate("testClass", "查看属性", Q_NULLPTR));
        export_action->setText(QApplication::translate("testClass", "导出", Q_NULLPTR));
        minimal_action->setText(QApplication::translate("testClass", "最小化到托盘", Q_NULLPTR));
        exit_action->setText(QApplication::translate("testClass", "退出程序", Q_NULLPTR));
        caseSensitive_action->setText(QApplication::translate("testClass", "区分大小写", Q_NULLPTR));
        useRegex_action->setText(QApplication::translate("testClass", "使用正则语法", Q_NULLPTR));
        selectAll_action->setText(QApplication::translate("testClass", "全选", Q_NULLPTR));
        antiSelect_action->setText(QApplication::translate("testClass", "反选", Q_NULLPTR));
        copyFile_action->setText(QApplication::translate("testClass", "复制文件", Q_NULLPTR));
        cutFile_action->setText(QApplication::translate("testClass", "剪切文件", Q_NULLPTR));
        preview_action->setText(QApplication::translate("testClass", "预览", Q_NULLPTR));
        statusBar_action->setText(QApplication::translate("testClass", "状态栏", Q_NULLPTR));
        research_action->setText(QApplication::translate("testClass", "重新搜索", Q_NULLPTR));
        name_action->setText(QApplication::translate("testClass", "名字", Q_NULLPTR));
        path_action->setText(QApplication::translate("testClass", "路径", Q_NULLPTR));
        asec_action->setText(QApplication::translate("testClass", "升序", Q_NULLPTR));
        desc_action->setText(QApplication::translate("testClass", "降序", Q_NULLPTR));
        all_action->setText(QApplication::translate("testClass", "所有", Q_NULLPTR));
        video_action->setText(QApplication::translate("testClass", "视频", Q_NULLPTR));
        audio_action->setText(QApplication::translate("testClass", "音频", Q_NULLPTR));
        picture_action->setText(QApplication::translate("testClass", "图片", Q_NULLPTR));
        document_action->setText(QApplication::translate("testClass", "文档", Q_NULLPTR));
        compare_action->setText(QApplication::translate("testClass", "压缩包", Q_NULLPTR));
        exe_action->setText(QApplication::translate("testClass", "可执行文件", Q_NULLPTR));
        help_action->setText(QApplication::translate("testClass", "帮助", Q_NULLPTR));
        searchGrammer_action->setText(QApplication::translate("testClass", "搜索语法", Q_NULLPTR));
        regexGrammer_action->setText(QApplication::translate("testClass", "正则语法", Q_NULLPTR));
        setting_action->setText(QApplication::translate("testClass", "设置", Q_NULLPTR));
        english_action->setText(QApplication::translate("testClass", "english", Q_NULLPTR));
        chinese_action->setText(QApplication::translate("testClass", "简体中文", Q_NULLPTR));
        action1->setText(QApplication::translate("testClass", "打开主界面", Q_NULLPTR));
        action2->setText(QApplication::translate("testClass", "强制退出", Q_NULLPTR));
    }

private:
    void newOthers(QMainWindow *testClass)
    {
        cen=new QWidget(testClass);
        statusBar=new QStatusBar(testClass);
        menuBar=new QMenuBar(testClass);
        widget1=new QWidget(testClass);
        sp=new QSplitter(Qt::Horizontal,testClass);
        lineEdit=new QLineEdit(testClass);
        pushButton=new QPushButton(testClass);
        tray=new QSystemTrayIcon(testClass);
        tw=new QTableWidget(testClass);
        textEdit=new QTextEdit(testClass);
        qaw=new QAxWidget(testClass);
        qvbl=new QVBoxLayout(cen);
        hb1=new QHBoxLayout(widget1);
        label=new QLabel(statusBar);
        painter=new QPainter(textEdit);
        gview=new QGraphicsView(testClass);
//        la=new QLabel(testClass);
//        combo=new QComboBox(testClass);
//        combo_edit=new QLineEdit(testClass);
//        toolBar=new QToolBar(testClass);

        cen->setObjectName("centralWidget");
        menuBar->setObjectName("menuBar");
        statusBar->setObjectName("statusBar");
        qvbl->setObjectName("QVBoxLayout");
        hb1->setObjectName("QHBoxLayout");
    }

    void newMenu(QMainWindow *testClass)
    {
        menu=new QMenu(testClass);
        file_menu=new QMenu(menuBar);
        edit_menu=new QMenu(menuBar);
        view_menu=new QMenu(menuBar);
        search_menu=new QMenu(menuBar);
        help_menu=new QMenu(menuBar);
        language_menu=new QMenu(menuBar);
        sort_menu=new QMenu(menuBar);
        num=new QMenu(menuBar);
        file_menu->setObjectName("file_menu");
        edit_menu->setObjectName("edit_menu");
        view_menu->setObjectName("view_menu");
        search_menu->setObjectName("search_menu");
        help_menu->setObjectName("help_menu");
        language_menu->setObjectName("language_menu");
    }

    void newAction(QMainWindow *testClass)
    {
        group1=new QActionGroup(testClass);
        group2=new QActionGroup(testClass);
        group3=new QActionGroup(testClass);
        group4=new QActionGroup(testClass);

        action1=new QAction(testClass);
        action2=new QAction(testClass);
        openFilePath_action=new QAction(testClass);
        openFilePath_action->setObjectName("open_file_path_action");
        runFile_action=new QAction(testClass);
        copyFile_action=new QAction(testClass);
        runFile_action=new QAction(testClass);
        delete_action=new QAction(testClass);
        rename_action=new QAction(testClass);
        propertise_action=new QAction(testClass);
        export_action=new QAction(testClass);
        exit_action=new QAction(testClass);
        selectAll_action=new QAction(testClass);
        antiSelect_action=new QAction(testClass);
        copyPath_action=new QAction(testClass);
        cutFile_action=new QAction(testClass);
        preview_action=new QAction(testClass);
        statusBar_action=new QAction(testClass);
        research_action=new QAction(testClass);
        caseSensitive_action=new QAction(testClass);
        useRegex_action=new QAction(testClass);
        all_action=new QAction(testClass);
        audio_action=new QAction(testClass);
        video_action=new QAction(testClass);
        compare_action=new QAction(testClass);
        document_action=new QAction(testClass);
        exe_action=new QAction(testClass);
        picture_action=new QAction(testClass);
        help_action=new QAction(testClass);
        searchGrammer_action=new QAction(testClass);
        regexGrammer_action=new QAction(testClass);
        setting_action=new QAction(testClass);
        english_action=new QAction(testClass);
        chinese_action=new QAction(testClass);
        name_action=new QAction(testClass);
        path_action=new QAction(testClass);
        asec_action=new QAction(testClass);
        desc_action=new QAction(testClass);
        minimal_action=new QAction(testClass);
    }

    void connection(QMainWindow *testClass)
    {
        QObject::connect(tw->verticalScrollBar(), SIGNAL(sliderMoved(int)), testClass, SLOT(twShow(int)));
        QObject::connect(tw->verticalScrollBar(), SIGNAL(valueChanged(int)), testClass, SLOT(twShow(int)));
        QObject::connect(tw->verticalScrollBar(), SIGNAL(rangeChanged(int,int)), testClass, SLOT(twShow(int)));
        QObject::connect(openFilePath_action,SIGNAL(triggered(bool)), testClass, SLOT(openFilePath()));
        QObject::connect(runFile_action,SIGNAL(triggered(bool)), testClass, SLOT(runFile()));
        QObject::connect(copyPath_action,SIGNAL(triggered(bool)), testClass, SLOT(copyFilePath()));
        QObject::connect(delete_action,SIGNAL(triggered(bool)), testClass, SLOT(deleteFile()));
        QObject::connect(rename_action,SIGNAL(triggered(bool)), testClass, SLOT(renameFile()));
        QObject::connect(propertise_action,SIGNAL(triggered(bool)), testClass, SLOT(filePropertise()));
        QObject::connect(export_action,SIGNAL(triggered(bool)), testClass, SLOT(exportAnswer()));
        QObject::connect(exit_action,SIGNAL(triggered(bool)), testClass, SLOT(exit_action()));
        QObject::connect(selectAll_action,SIGNAL(triggered(bool)), testClass, SLOT(selectAll()));
        QObject::connect(antiSelect_action,SIGNAL(triggered(bool)), testClass, SLOT(antiSelect()));
        QObject::connect(copyFile_action,SIGNAL(triggered(bool)), testClass, SLOT(copyFile()));
        QObject::connect(cutFile_action,SIGNAL(triggered(bool)), testClass, SLOT(cutFile()));
        QObject::connect(preview_action,SIGNAL(triggered(bool)), testClass, SLOT(showPreview()));
        QObject::connect(statusBar_action,SIGNAL(triggered(bool)), testClass, SLOT(showStatusBar()));
        QObject::connect(research_action,SIGNAL(triggered(bool)), testClass, SLOT(Research()));
        QObject::connect(name_action,SIGNAL(triggered(bool)), testClass, SLOT(sortByName()));
        QObject::connect(path_action,SIGNAL(triggered(bool)), testClass, SLOT(sortByPath()));
        QObject::connect(asec_action,SIGNAL(triggered(bool)), testClass, SLOT(sortByAsec()));
        QObject::connect(desc_action,SIGNAL(triggered(bool)), testClass, SLOT(sortByDesc()));
        QObject::connect(caseSensitive_action,SIGNAL(triggered(bool)), testClass, SLOT(caseSensitive()));
        QObject::connect(useRegex_action,SIGNAL(triggered(bool)), testClass, SLOT(useRegex()));
        QObject::connect(all_action,SIGNAL(triggered(bool)), testClass, SLOT(showAll()));
        QObject::connect(audio_action,SIGNAL(triggered(bool)), testClass, SLOT(onlyAudio()));
        QObject::connect(video_action,SIGNAL(triggered(bool)), testClass, SLOT(onlyVideo()));
        QObject::connect(compare_action,SIGNAL(triggered(bool)), testClass, SLOT(onlyCompare()));
        QObject::connect(picture_action,SIGNAL(triggered(bool)), testClass, SLOT(onlyPicture()));
        QObject::connect(document_action,SIGNAL(triggered(bool)), testClass, SLOT(onlyDocument()));
        QObject::connect(exe_action,SIGNAL(triggered(bool)), testClass, SLOT(onlyExecutable()));
        QObject::connect(help_action,SIGNAL(triggered(bool)), testClass, SLOT(showHelp()));
        QObject::connect(searchGrammer_action,SIGNAL(triggered(bool)), testClass, SLOT(searchGrammer()));
        QObject::connect(regexGrammer_action,SIGNAL(triggered(bool)), testClass, SLOT(regexGrammer()));
        QObject::connect(setting_action,SIGNAL(triggered(bool)), testClass, SLOT(show_setting()));
        QObject::connect(english_action,SIGNAL(triggered(bool)), testClass, SLOT(englishUse()));
        QObject::connect(chinese_action,SIGNAL(triggered(bool)), testClass, SLOT(chineseUse()));
        QObject::connect(minimal_action,SIGNAL(triggered(bool)), testClass, SLOT(Minimal()));
        QObject::connect(action1, SIGNAL(triggered(bool)), testClass, SLOT(action1_triggered()));
        QObject::connect(action2, SIGNAL(triggered(bool)), testClass, SLOT(action2_triggered()));
        QObject::connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),\
                         testClass, SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
        QObject::connect(pushButton, SIGNAL(clicked(bool)), testClass, SLOT(search()));
        QObject::connect(lineEdit, SIGNAL(returnPressed()), testClass, SLOT(search()));
//        QObject::connect(tw, SIGNAL(cellPressed(int,int)), testClass, SLOT(startRE(int, int)));
//        QObject::connect(tw, SIGNAL(cellChanged(int,int)), testClass, SLOT(startRE(int, int)));
//        QObject::connect(tw, SIGNAL(cellPressed(int,int)), testClass, SLOT(preView(int, int)));
        QObject::connect(tw, SIGNAL(itemClicked(QTableWidgetItem*)), testClass, SLOT(preView(QTableWidgetItem*)));
        QObject::connect(tw, SIGNAL(cellDoubleClicked(int,int)), testClass, SLOT(startRename(int, int)));
//        QObject::connect(tw, SIGNAL(cellEntered(int,int)), testClass, SLOT(preView(int, int)));
        QObject::connect(tw, SIGNAL(itemChanged(QTableWidgetItem*)), testClass, SLOT(startRE(QTableWidgetItem*)));
        QObject::connect(tw->horizontalHeader(), SIGNAL(sectionClicked(int)), testClass, SLOT(sortByColumn(int)));

//        QObject::connect(tw, SIGNAL(cellActivated(int,int)), testClass, SLOT(twCellActived(int, int)));
//        QObject::connect(tw, SIGNAL(cellChanged(int,int)), testClass, SLOT(twCellChanged(int, int)));
//        QObject::connect(tw, SIGNAL(cellEntered(int,int)), testClass, SLOT(twCellEntered(int, int)));
//        QObject::connect(tw, SIGNAL(cellPressed(int,int)), testClass, SLOT(twCellPressed(int, int)));
//        QObject::connect(tw, SIGNAL(itemActivated(QTableWidgetItem*)), testClass, SLOT(twCellActived(QTableWidgetItem*)));
//        QObject::connect(tw, SIGNAL(itemPressed(QTableWidgetItem*)), testClass, SLOT(twCellPresses(QTableWidgetItem*)));
//        QObject::connect(tw, SIGNAL(itemEntered(QTableWidgetItem*)), testClass, SLOT(twCellEntered(QTableWidgetItem*)));
        QObject::connect(tw, SIGNAL(itemSelectionChanged()), testClass, SLOT(twSelectionChanged()));
//        QObject::connect(tw, SIGNAL(entered(QModelIndex)), testClass, SLOT(twEntered(QModelIndex)));
    }

    void addAction()//给menu添加action
    {
        menuBar->addAction(file_menu->menuAction());
        menuBar->addAction(edit_menu->menuAction());
        menuBar->addAction(view_menu->menuAction());
        menuBar->addAction(search_menu->menuAction());
        menuBar->addAction(help_menu->menuAction());
        menuBar->addAction(language_menu->menuAction());
        menu->addAction(action1);
        menu->addAction(action2);
        menu->addAction(exit_action);
        file_menu->addAction(openFilePath_action);
        file_menu->addAction(runFile_action);
        file_menu->addAction(copyPath_action);
        file_menu->addAction(delete_action);
        file_menu->addAction(rename_action);
        file_menu->addAction(propertise_action);
        file_menu->addSeparator();
        file_menu->addAction(export_action);
        file_menu->addAction(minimal_action);
        file_menu->addAction(exit_action);

        edit_menu->addAction(selectAll_action);
        edit_menu->addAction(antiSelect_action);
        edit_menu->addSeparator();
        edit_menu->addAction(copyFile_action);
        edit_menu->addAction(cutFile_action);

        view_menu->addAction(preview_action);
        view_menu->addAction(statusBar_action);
        view_menu->addAction(research_action);
        view_menu->addSeparator();
        view_menu->addAction(sort_menu->menuAction());

        sort_menu->addAction(group3->addAction(name_action));
        sort_menu->addAction(group3->addAction(path_action));
        sort_menu->addSeparator();
        sort_menu->addAction(group4->addAction(asec_action));
        sort_menu->addAction(group4->addAction(desc_action));

        search_menu->addAction(caseSensitive_action);
        search_menu->addAction(useRegex_action);
        search_menu->addSeparator();
        search_menu->addAction(group1->addAction(all_action));
        search_menu->addAction(group1->addAction(audio_action));
        search_menu->addAction(group1->addAction(video_action));
        search_menu->addAction(group1->addAction(picture_action));
        search_menu->addAction(group1->addAction(document_action));
        search_menu->addAction(group1->addAction(compare_action));
        search_menu->addAction(group1->addAction(exe_action));

        help_menu->addAction(help_action);
        help_menu->addAction(searchGrammer_action);
        help_menu->addAction(regexGrammer_action);
        help_menu->addAction(setting_action);

        language_menu->addAction(group2->addAction(english_action));
        language_menu->addAction(group2->addAction(chinese_action));


    }

    void setCheck()//给action设置checkable
    {
        preview_action->setCheckable(true);
        statusBar_action->setCheckable(true);
        caseSensitive_action->setCheckable(true);
        useRegex_action->setCheckable(true);
        all_action->setCheckable(true);
        audio_action->setCheckable(true);
        video_action->setCheckable(true);
        compare_action->setCheckable(true);
        picture_action->setCheckable(true);
        document_action->setCheckable(true);
        exe_action->setCheckable(true);
        name_action->setCheckable(true);
        path_action->setCheckable(true);
        asec_action->setCheckable(true);
        desc_action->setCheckable(true);
        english_action->setCheckable(true);
        chinese_action->setCheckable(true);

        selectAll_action->setChecked(true);
        preview_action->setChecked(true);
        statusBar_action->setChecked(true);
        all_action->setChecked(true);
        english_action->setChecked(true);
        name_action->setChecked(true);
        asec_action->setChecked(true);
    }

    void initLayout()//设置布局
    {
        cen->setLayout(qvbl);
        qvbl->addWidget(widget1);
        qvbl->addWidget(sp);
        qvbl->setStretchFactor(widget1, 0);
        qvbl->setStretchFactor(sp,8);
        widget1->setLayout(hb1);
        hb1->addWidget(lineEdit);
        hb1->addWidget(pushButton);
        hb1->setStretchFactor(lineEdit,8);
        hb1->setStretchFactor(pushButton,0);
        sp->addWidget(tw);
        sp->addWidget(textEdit);
        sp->addWidget(qaw);
        sp->addWidget(gview);
//        sp->addWidget(painter);
//        sp->addWidget(la);
        sp->setStretchFactor(0,6);
        sp->setStretchFactor(1,4);
        sp->setStretchFactor(2,4);
        sp->setStretchFactor(3,4);
    }

    void setWidgets()//设置各个控件的属性
    {
        tray->setToolTip(QObject::trUtf8("FindFile"));
        tray->setContextMenu(menu);
        QIcon icon = QIcon(":/Rewrite.ico");
        tray->setIcon(icon);

        QStringList headers;
        headers<<u8("name")<<u8("path");
        tw->setColumnCount(2);
        tw->setHorizontalHeaderLabels(headers);
        tw->setSelectionBehavior(QAbstractItemView::SelectRows);
        tw->setRowCount(0);
        tw->setColumnWidth(0, 130);
        tw->setColumnWidth(1, 300);
        tw->verticalHeader()->setHidden(true);
        tw->horizontalHeader()->setSortIndicatorShown(true);
        tw->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
        tw->show();//130 300

        textEdit->setAcceptRichText(false);
        textEdit->setReadOnly(true);
        textEdit->setText("请选择要预览的文件");
        textEdit->setEnabled(false);
        textEdit->setAlignment(Qt::AlignCenter);
//        textEdit->hide();

        qaw->setHidden(true);//AppXq0fevzme2pys62n3e0fbqa7peapykr8v
        qaw->setControl("{6BF52A52-394A-11d3-B153-00C04F79FAA6}");

        statusBar->addWidget(label);
        statusBar->setSizeGripEnabled(false);
        statusBar->setStyleSheet(QString("QStatusBar::item{border: 0px}"));
        label->setAlignment(Qt::AlignLeft);


//        gview->setFixedSize(textEdit->size());
        gview->hide();


//        QStringList sl;
//        sl<<"100"<<"1000"<<"10000"<<"infinity";
//        combo->addItems(sl);
//        combo->setEditable(false);
//        combo->setLineEdit(combo_edit);
//        combo->setToolTip("search limit");

//        toolBar->addWidget(combo);
    }

    void setShortCut()//设置快捷键
    {
        openFilePath_action->setShortcut(QKeySequence("Ctrl+Shift+O"));
        runFile_action->setShortcut(QKeySequence("Ctrl+R"));
        copyPath_action->setShortcut(QKeySequence("Ctrl+Shift+C"));
        copyFile_action->setShortcut(QKeySequence("Ctrl+C"));
        cutFile_action->setShortcut(QKeySequence("Ctrl+X"));
        selectAll_action->setShortcut(QKeySequence("Ctrl+A"));
        export_action->setShortcut(QKeySequence("Ctrl+S"));
        propertise_action->setShortcut(QKeySequence("Ctrl+Return"));
        exit_action->setShortcut(QKeySequence("Ctrl+Q"));
        preview_action->setShortcut(QKeySequence("Ctrl+P"));
    }
};

namespace Ui {
    class testClass: public Ui_testClass {};
}
#endif // UI_TESTCLASS_H
