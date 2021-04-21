#ifndef REJECT_H
#define REJECT_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>

class reject : public QWidget
{
    Q_OBJECT
public:
    explicit reject(QWidget *parent = nullptr);//构造函数
    ~reject();//析构函数
    QVBoxLayout *vbl;//布局
    QTextEdit *te;//文本框
    void isNull();
signals:

public slots:
};

#endif // REJECT_H
