#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QPainter>
#include <QPaintEvent>
#include <QFont>
#include <QLabel>
#include <QObject>
extern int searchLimit;
class setting : public QWidget
{
    Q_OBJECT
public:
    explicit setting(QWidget *parent = nullptr);
    QLineEdit *lineEdit;
    QPushButton *ok, *cancel;
    QComboBox *comb;
    QLabel *label;
    void isNull();
signals:

public slots:
    void setLimit();
    void cancelSet();

};

#endif // SETTING_H
