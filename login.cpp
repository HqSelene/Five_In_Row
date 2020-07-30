#include "login.h"
#include "ui_login.h"
#include "data.h"
#include "gameengine.h"
#include <QMessageBox>

LogIn::LogIn(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogIn),interface(nullptr)
{
    ui->setupUi(this);
    setFixedSize(WIDE,HEIGHT);
    back=new QPixmap;
    back->load(":/picture/loginBack.jpg");
}

LogIn::~LogIn()
{
    delete ui;
}

void LogIn::paintEvent(QPaintEvent*e)
{
    QPainter* painter = new QPainter();
    painter->begin(this);
    painter->drawPixmap(0,0,WIDE,HEIGHT,*back);
    painter->end();
}

void LogIn::on_pushButton_3_clicked()
{


    QMessageBox *msgBox = new QMessageBox(QMessageBox::Question, tr("对话框标题"), tr("对话框内容"), QMessageBox::Yes | QMessageBox::No);
    msgBox->setText("您想选择黑棋还是白棋？");
    // 将原本显示“Yes”的按钮改变
    msgBox->button(QMessageBox::No)->setText("黑棋(先手)");
    // 将原本显示“No”的按钮改变
    msgBox->button(QMessageBox::Yes)->setText("白棋(后手)");
    // 弹出对话框
    int choice=msgBox->exec();
    if(choice==QMessageBox::Yes)
    {
        interface=new MainWindow(0);
    }
    else if(choice==QMessageBox::No)
    {
        interface=new MainWindow(1);
    }
    this->hide();
    connect(interface,SIGNAL(sendsignal()),this,SLOT(reshow()));
    interface->show();
}

void LogIn::on_pushButton_4_clicked()
{
    this->close();
}

