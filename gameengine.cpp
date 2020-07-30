#include "gameengine.h"
#include "ui_mainwindow.h"
#include "data.h"
#include "algorithm.h"
#include <QMessageBox>
#include <QTime>

MainWindow::MainWindow(int x, QWidget *parent)
    : QMainWindow(parent),WIN(0)
    , ui(new Ui::MainWindow)
{
    setFixedSize(WIDE,HEIGHT);


    back=new QPixmap;
    back->load(":/picture/loginBack2.jpg");

    if(x)
    {
        WhoseTurn=HUMAN;
        colorOfHuman=1;
    }
    else
    {
        WhoseTurn=AI;
        colorOfHuman=2;
    }

    ui->setupUi(this);
    //setFixedSize(WIDE,HEIGHT);
    this->setMouseTracking(true);

    centralWidget()->setMouseTracking(true);
    setWindowFlags(Qt::WindowCloseButtonHint);

    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++)
        {
            Point* tmp=new Point(X_START+j*SPACING,Y_START+i*SPACING,i,j);
            allChess[i].push_back(tmp);
        }
    }

    time=new QTimer;
    time->start(20);
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    connect(time, SIGNAL(timeout()), this, SLOT(update()));

    memset(situation,0,sizeof(situation));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter *painter = new QPainter();
    painter->begin(this);
    painter->drawPixmap(0,0,WIDE,HEIGHT,*back);

    win(HUMAN);
    for(int i=0;i<15;i++)
    {
        painter->drawLine(allChess[i][0]->getX(),allChess[i][0]->getY(),allChess[i][14]->getX(),allChess[i][14]->getY());
        painter->drawLine(allChess[0][i]->getX(),allChess[0][i]->getY(),allChess[14][i]->getX(),allChess[14][i]->getY());
    }

    for(int i=0;i<humChess.size();i++)
    {
        humChess[i]->show(painter);

    }

    if(WIN==0)
    {
        play();
        win(AI);
    }

    for(int i=0;i<AIChess.size();i++)
    {
        AIChess[i]->show(painter);
    }

    painter->setPen(QColor(255, 10, 33));
    painter->setFont(QFont("STKaiti",28));

    if(WIN==1)
    {
        time->stop();
        QString str="才高八斗，在下敬佩";
        painter->drawText(160,50,str);
    }
    else if(WIN==2)
    {
        time->stop();
        QString str="一胜一负，兵家常势";
        painter->drawText(160,50,str);
    }
    else if(WIN==3)
    {
        time->stop();
        QString str="不错嘛，有潜力，再战！";
        painter->drawText(160,50,str);
    }

    painter->end();
    delete painter;
}

//放置棋子
void MainWindow::setChess(int i,int j,Player peo)
{
    if(peo==HUMAN)
    {
        allChess[i][j]->setChess(colorOfHuman);
        situation[i][j]=colorOfHuman;
        humChess.push(allChess[i][j]);
    }
    else if(peo==AI)
    {
        allChess[i][j]->setChess(3-colorOfHuman);
        situation[i][j]=3-colorOfHuman;
        AIChess.push(allChess[i][j]);
    }

}

//选择落棋点，如果该点可以放置，则压入
void MainWindow::mousePressEvent(QMouseEvent* e)
{
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<allChess[i].size();j++)
        {
            if(allChess[i][j]->ifNear(e->x(),e->y()))
            {
                if(allChess[i][j]->ifCanPut()&&WhoseTurn==HUMAN)
                {
                    setChess(i,j,WhoseTurn);
                    WhoseTurn=AI;
                }
                return;
            }
        }

    }
}

//如果该点可以放置，鼠标将变成手的形状
void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<allChess[i].size();j++)
        {
            if(allChess[i][j]->ifNear(e->x(),e->y()))
            {
                if(allChess[i][j]->ifNear(e->x(),e->y()))
                {
                    if(allChess[i][j]->ifCanPut())
                    {
                        this->setCursor(Qt::OpenHandCursor);
                        return;
                    }
                }
            }
        }

    }

    this->setCursor(Qt::ArrowCursor);
}


void MainWindow::reStart(int x)//刷新界面，根据选择先手、后手重新游戏
{
    if(x)
    {
        WhoseTurn=HUMAN;
        colorOfHuman=1;
    }
    else
    {
        WhoseTurn=AI;
        colorOfHuman=2;
    }

    while(!humChess.empty())
    {
        humChess.pop();
    }

    while(!AIChess.empty())
    {
        AIChess.pop();
    }

    for(int i=0;i<14;i++)
    {
        for(int j=0;j<allChess[i].size();j++)
        {
            allChess[i][j]->setChess(0);
        }

    }

    memset(situation,0,sizeof(situation));
}

void MainWindow::on_pushButton_clicked()//返回界面
{
    emit sendsignal();
    this->close();
}

void MainWindow::play()
{
    if(WhoseTurn==AI)//如果到了AI的游戏时间
    {
        Algorithm alg(situation,AIChess,humChess);
        alg.choose();
        setChess(alg.tar_x,alg.tar_y,AI);
        WhoseTurn=HUMAN;
    }
}



//判断落棋的一方是否胜利
void MainWindow::win(Player p)
{
    bool win=false;
    int color;
    int _i,_j;
    switch(p)
    {
    case HUMAN:
    {
        color=colorOfHuman;
        if(humChess.empty())
            return;
        _i=humChess.top()->getI();
        _j=humChess.top()->getJ();
    }
        break;
    case AI:
    {
        color=(colorOfHuman==2?1:2);
        if(AIChess.empty())
            return;
        _i=AIChess.top()->getI();
        _j=AIChess.top()->getJ();
    }
        break;
    }

    int dir[8]={-1,1,0,1,1,1,1,0};
    for(int i=0;i<4;i++)//以最后一个落棋点为中心，检查四个大方向
    {

        int flag=0,x=_i,y=_j;
        while(x>=0&&y>=0&&x<15&&y<15&&allChess[x][y]->getColor()==color)//上
        {
            flag++;
            x-=dir[i*2];
            y-=dir[i*2+1];
        }
        x=_i,y=_j;
        while(x>=0&&y>=0&&x<15&&y<15&&allChess[x][y]->getColor()==color)//上
        {
            flag++;
            x+=dir[i*2];
            y+=dir[i*2+1];
        }

        if(flag>=6)
        {
            win=true;
            break;
        }
    }
    if(win==true)
    {
        switch (p)
        {
        case HUMAN:
            WIN=1;
            break;
        case AI:
            WIN=2;
            break;
        }
    }
    if(humChess.size()+AIChess.size()==255)
        WIN=3;
}


void MainWindow::on_pushButton_2_clicked()//悔棋
{
    if(WhoseTurn==HUMAN&&humChess.size()!=0)
    {
        int x=humChess.top()->getI(),y=humChess.top()->getJ();
        humChess.pop();
        allChess[x][y]->setChess(0);
        if(AIChess.size()!=0)
        {
            x=AIChess.top()->getI();
            y=AIChess.top()->getJ();
            AIChess.pop();
            allChess[x][y]->setChess(0);
        }
    }
}

void MainWindow::on_pushButton_3_clicked()
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
        reStart(0);
    }
    else if(choice==QMessageBox::No)
    {
        reStart(1);
    }
    time->start();
    WIN=0;
}

void MainWindow::on_pushButton_4_clicked()
{
    this->close();
}
