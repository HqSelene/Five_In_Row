#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include<QPaintEvent>
#include<QPainter>
#include<QPixmap>
#include "gameengine.h"

namespace Ui {
class LogIn;
}

class LogIn : public QMainWindow
{
    Q_OBJECT

public:
    explicit LogIn(QWidget *parent = nullptr);
    ~LogIn();
    void paintEvent(QPaintEvent*e);

private slots:
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void reshow(){this->show();}

signals:
    void sendsignal();

private:
    Ui::LogIn *ui;
    MainWindow* interface;
    QPixmap* back;
};

#endif // LOGIN_H
