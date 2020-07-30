#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <QStack>
#include <QVector>
#include "point.h"



class Algorithm
{
    int AI_score,hum_score;//对目前的情况进行评分
    QStack<Point*> AIC;
    QStack<Point*> personC;

    int allChess[15][15];//当前棋局
    int gradeBoard[15][15];//当前记分板

    bool first_hand;//先手还是后手
    int dir[8]={-1,1,0,1,1,1,1,0};
    int enemy_color,AI_color;//记录颜色

    int k1,k2;//记分比例
    int pre_x,pre_y;//判断落子区域
    //记录棋子密集处的范围
    QPair<int,int> min;
    QPair<int,int> max;
public:
    Algorithm(int all[15][15],QStack<Point*> AI,QStack<Point*> person);
    void choose();//选择算法；


    int calculate(int x,int y,int color,int board[15][15]);//评价某一点
    int evaluate(int board[15][15]);//评估整体

    int max_min(int board[15][15],int deep,int alpha,int beta,int color);//Alpha-Beta减枝的极大极小值

    void Greedy_Search();//贪心算法

    void Hill_Search(int _i,int _j,int score);//爬山算法，找到局部最优解

    void GA_Search();//遗传算法
    int find_Best_child(QVector<int> parent1,QVector<int> parent2,QVector<QPoint> valid,int size);//遗传算法中寻找子代最优
    int cal_gene(QVector<int> a,QVector<QPoint> valid);//计算适应率
    int tar_x;
    int tar_y;

};

#endif // ALGORITHM_H
