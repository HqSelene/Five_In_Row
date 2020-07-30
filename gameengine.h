#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QStack>
#include <QTimer>
#include <QMouseEvent>
#include <QMainWindow>
#include <iostream>
#include "point.h"
#include "algorithm.h"
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum Player{ HUMAN,AI };//表示玩家,人类还是AI


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int x,QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent*e);
    void setChess(int i,int j,Player peo);//在p出放置color颜色的棋子

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent*e);

private slots:
    void on_pushButton_clicked();//返回
    void on_pushButton_2_clicked();//悔棋
    void on_pushButton_3_clicked();//重来一局
    void on_pushButton_4_clicked();
signals:
    void sendsignal();

private:
    int WIN;//0为正在进行，1为人类胜利，2为机器胜利
    Ui::MainWindow *ui;
    QStack<Point*> humChess;//棋子
    QStack<Point*> AIChess;//棋子
    QVector<Point*> allChess[15];//总棋局
    int situation[15][15];//记录目前走子状态
    QTimer* time;

    QPixmap* back;
    Player WhoseTurn;
    int colorOfHuman;//表示人执手棋子的颜色,1为白棋，2为黑棋

private:
    void reStart(int x);
    void play();//玩游戏
    void win(Player p);//判断有没有人赢
};
#endif // MAINWINDOW_H
