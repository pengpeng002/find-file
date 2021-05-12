#include "setting.h"

int searchLimit=10000;
setting::setting(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(500, 500);
    lineEdit=new QLineEdit(this);
    ok=new QPushButton(this);
    ok->setText("ok");
    cancel=new QPushButton(this);
    cancel->setText("cancel");
    label=new QLabel(this);
    label->setText("search limit items");
    label->setGeometry(0,0,200,30);
    label->move(5, 5);
    lineEdit->resize(40, 20);
    lineEdit->move(130, 10);
    lineEdit->setText(QString("%1").arg(searchLimit));
    ok->move(300, 460);
    cancel->move(400, 460);
    connect(ok, SIGNAL(clicked(bool)), this, SLOT(setLimit()));
    connect(cancel, SIGNAL(clicked(bool)), this, SLOT(cancelSet()));
}

void setting::setLimit()
{
    QString s=lineEdit->text();
    bool ok;
    int num=s.toInt(&ok);
    if(ok)
    {
        if(num==0) num=INT_MAX;
        searchLimit=num;
    }
    this->hide();
}

void setting::cancelSet()
{
    lineEdit->setText(QString("%1").arg(searchLimit));
    this->hide();
}
