#include "reject.h"

reject::reject(QWidget *parent) : QWidget(parent)//构造函数
{
    this->resize(500,500);//设置窗口大小
    vbl = new QVBoxLayout(this);//使用布局只是为了让文本框自适应窗口大小
    te = new QTextEdit(this);//文本框
    te->setReadOnly(true);//设置只读
    vbl->addWidget(te);
}
//虽然认为没用，但养成好习惯
//new了的要记得delete
reject::~reject()//析构函数
{
    delete te;
    delete vbl;
}
