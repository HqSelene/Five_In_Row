#include "algorithm.h"
#include "data.h"
#include <algorithm>
#include <QPair>
#include <QMap>
#include <QQueue>
#include <QTime>
#include <iostream>
using namespace std;
int num_score[6]={0,1,20,600,4000,1000000};//五元组评分表

Algorithm::Algorithm(int all[15][15],QStack<Point*> AI,QStack<Point*> person):AI_score(0),hum_score(0),AIC(AI),personC(person),min(224,224),max(0,0)
{
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    first_hand=AI.size()==person.size()?true:false;
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++)
        {
            allChess[i][j]=all[i][j];
        }
    }
    memset(gradeBoard,0,sizeof (gradeBoard));
    if(AI.size()!=0)
    {
        AI_color=AI.top()->getColor();
        enemy_color=3-AI_color;
    }
    if(person.size()!=0)
    {
        enemy_color=person.top()->getColor();
        AI_color=3-enemy_color;
        pre_x=person.top()->getI();
        pre_y=person.top()->getJ();
    }

    for(int i=0;i<15;i++)//寻找到密集点的边界
    {
        for(int j=0;j<15;j++)
        {
            if(allChess[i][j]!=0)
            {
                if(i<min.first)
                    min.first=i;
                if(i>max.first)
                    max.first=i;
                if(j<min.second)
                    min.second=j;
                if(j>max.second)
                    max.second=j;
            }
        }
    }
}

void Algorithm::choose()//选择算法
{
    tar_x=7,tar_y=7;
    if(AIC.size()==0&&personC.size()==0)
        return;

    if(first_hand)//先手注重进攻，后手注重防守
    {
        k1=ATTACK_HUM_COE;
        k2=ATTACK_AI_COE;
    }
    else
    {
        k1=DEFEND_HUM_COE;
        k2=DEFEND_AI_COE;
    }

    int score=evaluate(allChess);


    //GA_Search();
    //max_min(allChess,2,-1000000000,+100000000,AI_color);
    //Greedy_Search();
    //Hill_Search(pre_x,pre_y,-10000000);



    //综合算法
    if(abs(score)<80)//如果分数较小，可以使用遗传算法
        GA_Search();
    else if(AI_score>hum_score)//如果AI分数更高，采取强势一些的min_max搜索
    {
        Greedy_Search();
    }
    else if(AI_score<hum_score)//如果人类更强势,采取贪心
    {
        max_min(allChess,2,-1000000000,+100000000,AI_color);
    }
    else
        Hill_Search(pre_x,pre_y,-10000000);

}

//评价函数，每个点最多20个五元组，根据当前棋形计算得分值，
int Algorithm::calculate(int x,int y,int color,int board[15][15])
{
    int score=0;
    int five_tuple[4][5];//记录四个方向出现五元组的种类
    memset(five_tuple,0,sizeof (five_tuple));

    //根据每个五元组的类型，计算分数
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<5;j++)
        {
            int enemy=0,my=0;
            int x_tmp=x,y_tmp=y;
            for(int k=0;k<=j&&x_tmp>=0&&y_tmp>=0&&x_tmp<15&&y_tmp<15;k++)
            {
                if(board[x_tmp][y_tmp]==3-color)
                {
                    enemy=1;
                    break;
                }
                else if(board[x_tmp][y_tmp]==color)
                {
                    my++;
                }
                x_tmp-=dir[i*2];
                y_tmp-=dir[i*2+1];
            }
            x_tmp=x+dir[i*2];y_tmp=y+dir[i*2+1];
            for(int k=1;k<=4-j&&x_tmp>=0&&y_tmp>=0&&x_tmp<15&&y_tmp<15;k++)
            {
                if(board[x_tmp][y_tmp]==3-color)
                {
                    enemy=1;
                    break;
                }
                else if(board[x_tmp][y_tmp]==color)
                {
                    my++;
                }
                x_tmp+=dir[i*2];
                y_tmp+=dir[i*2+1];
            }

            if(enemy==0&&my<6&&my>=1)
            {

                five_tuple[i][my-1]=1;
                score+=num_score[my];
            }
        }
    }

    //如果不同方向有特殊的五元组，可以加分
    if((five_tuple[0][2]+five_tuple[1][2]+five_tuple[2][2]+five_tuple[3][2])>=2){score+=DOUBLE_RULE_3;}
    if((five_tuple[0][3]+five_tuple[1][3]+five_tuple[2][3]+five_tuple[3][3])>=2){score+=DOUBLE_RULE_4;}
    if((five_tuple[0][4]+five_tuple[1][4]+five_tuple[2][4]+five_tuple[3][4])>=2){score+=DOUBLE_RULE_5;}
    if((five_tuple[0][3]+five_tuple[1][3]+five_tuple[2][3]+five_tuple[3][3])>=1&&(five_tuple[0][2]+five_tuple[1][2]+five_tuple[2][2]+five_tuple[3][2])>=1){score+=RULE_3_4;}
    if((five_tuple[0][2]+five_tuple[1][2]+five_tuple[2][2]+five_tuple[3][2])>=1&&(five_tuple[0][4]+five_tuple[1][4]+five_tuple[2][4]+five_tuple[3][4])>=1){score+=RULE_3_5;}
    if((five_tuple[0][3]+five_tuple[1][3]+five_tuple[2][3]+five_tuple[3][3])>=1&&(five_tuple[0][4]+five_tuple[1][4]+five_tuple[2][4]+five_tuple[3][4])>=1){score+=RULE_4_5;}

    return score;
}

int Algorithm::evaluate(int board[15][15])//局面评价函数
{
    int score1=0,score2=0;
    for(int i=0;i<personC.size();i++)
    {
        score1+=calculate(personC[i]->getI(),personC[i]->getJ(),enemy_color,board);
    }

    for(int i=0;i<AIC.size();i++)
    {
        score2+=calculate(AIC[i]->getI(),AIC[i]->getJ(),AI_color,board);
    }
    AI_score=k2*score2;
    hum_score=k1*score1;
    return AI_score-hum_score;
}

//爬山，在对方落棋点周围找局部最优解,然后在最优解周围找该解是不是局部最优解，如果是则返回，不是的话继续找，直到出现满足条件的解
void Algorithm::Hill_Search(int _i, int _j, int score)
{
    int H_score=-1000000,tar_i=0,tar_j=0;
    for(int i=0;i<4;i++)//以最后一个落棋点为中心，检查四个大方向
    {

        int n=5,x=_i-dir[i*2],y=_j-dir[i*2+1];
        while(n>=1&&x>=0&&y>=0&&x<15&&y<15)
        {

            if(allChess[x][y]==0)
            {
                allChess[x][y]=enemy_color;
                gradeBoard[x][y]=k1*calculate(x,y,enemy_color,allChess)-pow(x-_i,2)-pow(x-_j,2);
                allChess[x][y]=AI_color;
                gradeBoard[x][y]+=k2*calculate(x,y,AI_color,allChess);
                allChess[x][y]=0;
                if(gradeBoard[x][y]>H_score)//找周围最优解
                {
                    H_score=gradeBoard[x][y];
                    tar_i=x;tar_j=y;
                }
            }

            x-=dir[i*2];
            y-=dir[i*2+1];
            n--;
        }
        x=_i+dir[i*2],y=_j+dir[i*2+1],n=5;
        while(n>=1&&x>=0&&y>=0&&x<15&&y<15)
        {

            if(allChess[x][y]==0)
            {
                allChess[x][y]=enemy_color;
                gradeBoard[x][y]=k1*calculate(x,y,enemy_color,allChess)+pow(x-_i,2)+pow(x-_j,2);
                allChess[x][y]=AI_color;
                gradeBoard[x][y]+=k2*calculate(x,y,AI_color,allChess);
                allChess[x][y]=0;

                if(gradeBoard[x][y]>H_score)
                {
                    H_score=gradeBoard[x][y];
                    tar_i=x;tar_j=y;
                }
            }

            x+=dir[i*2];
            y+=dir[i*2+1];
            n--;
        }
    }
    if(H_score>score)
    {
        Hill_Search(tar_i,tar_j,H_score);
        tar_x=tar_i,tar_y=tar_j;
    }
    else
    {
        return;
    }
}

void Algorithm::Greedy_Search()//贪心算法，寻找整个状态的最优解
{
    int H_score=-1,tar_i=0,tar_j=0;

    for(int i=0;i<15;i++)//遍历所有棋盘
    {
        for(int j=0;j<15;j++)
        {
            if(allChess[i][j]==0)
            {
                allChess[i][j]=enemy_color;//对周围进行评分，找到最优的点

                //加上位置评分，避免出现位置同分却把棋子下在对棋局影响不大的地方
                gradeBoard[i][j]=k1*calculate(i,j,enemy_color,allChess)-pow((max.first-min.first)/2-i,2)-pow((max.second-min.second)/2-j,2);
                allChess[i][j]=AI_color;
                gradeBoard[i][j]+=k2*calculate(i,j,AI_color,allChess);
                if(gradeBoard[i][j]>H_score)
                {
                    H_score=gradeBoard[i][j];
                    tar_i=i;tar_j=j;
                }
                allChess[i][j]=0;
            }
        }
    }

    tar_x=tar_i;tar_y=tar_j;
}



int Algorithm::max_min(int board[15][15],int deep,int alpha,int beta,int color)//用color判断min还是max
{
    if(deep<=0)//如果深度为0返回局面得分
    {
        return evaluate(board);
    }
    if(color==AI_color)//要最大，MAX
    {
        for(int i=min.first-4;i<=max.first+4;i++)//在棋盘密集处四周找空点
        {
            for(int j=min.second-4;j<=max.second+4;j++)
            {
                if(i>=0&&i<15&&j>=0&&j<15&&allChess[i][j]==0)
                {
                    board[i][j]=color;
                    int x=max_min(board,deep-1,alpha,beta,3-color);//计算当前分数
                    board[i][j]=0;
                    if(x>alpha)//比较x与alpha的值
                     {
                        if(deep==2)//到达决策层记录节点
                        {
                            tar_x=i;
                            tar_y=j;
                        }
                        alpha=x;
                         if(alpha>=beta)
                             return alpha;
                     }
                }
            }
        }
        return alpha;
    }
    else if(color==enemy_color)//要最小
    {
        for(int i=min.first-4;i<=max.first+4;i++)
        {
            for(int j=min.second-4;j<=max.second+4;j++)
            {
                if(i>=0&&i<15&&j>=0&&j<15&&allChess[i][j]==0)
                {
                    board[i][j]=color;
                    int x=max_min(board,deep-1,alpha,beta,3-color);
                    board[i][j]=0;
                    if(x<beta)
                     {
                        beta=x;
                         if(alpha>=beta)
                             return beta;
                     }
                }
            }
        }
    }
     return beta;
}

//遗传算法
void Algorithm::GA_Search()
{
    QVector<QPoint> valid;//产生空位

    for(int i=min.first-1;i<=max.first+1;i++)
    {
        for(int j=min.second-1;j<=max.second+1;j++)
        {
            if(i>=0&&i<15&&j>=0&&j<15&&allChess[i][j]==0)
            {
                valid.push_back(QPoint(i,j));
            }
        }
    }

    //随机产生两个parent，parent基因序列代表AI和人交叉下棋的放置棋子策略
    QVector<int> parent1,parent2;
    QMap<int,int> a;
    int size=valid.size()<8?valid.size():8;//parent的基因长度，最长为8
    for(int i=0;i<size;i++)//随机产生基因序列
    {
        int j;
        while(1)
        {
            j=rand()%valid.size();
            if(a[j]==0)
            {
                parent1.push_back(j);
                a[j]=1;
                break;
            }
        }
    }
    a.clear();
    for(int i=0;i<8;i++)
    {
        int j;
        while(1)
        {
            j=rand()%valid.size();
            if(a[j]==0)
            {
                parent2.push_back(j);
                a[j]=1;
                break;
            }
        }
    }

    int number=find_Best_child(parent1,parent2,valid,size);
    tar_x=valid[number].x();
    tar_y=valid[number].y();

    parent1.clear();parent2.clear();
    valid.clear();
}

int Algorithm::find_Best_child(QVector<int> parent1,QVector<int> parent2,QVector<QPoint> valid,int size)//产生最优的子代
{

    for(int i=0;i<6;i++)//交配产生6代子代
    {
        QVector<int> child1=parent1,child2=parent2;

        //基因重组，此处选择p1三个点固定，其他点和p2按顺序交换
        for(int n=0;n<2;n++)
        {
            int b[3];//随机产生三个点固定
            b[0]=rand()%size;b[1]=rand()%size;b[2]=rand()%size;
            QVector<int> x;

            for(int j=0,k=0;j<size&&k<size;j++,k++)//除了固定的点，其他点进行交换
            {
                while(j==b[0]||j==b[1]||j==b[2])
                {
                    j++;
                }
                while(k<size&&(parent2[k]==parent1[b[0]]||parent2[k]==parent1[b[1]]||parent2[k]==parent1[b[2]]))
                    k++;
                if(j<size&&k<size)
                {
                    int tmp=parent1[j];
                    parent1[j]=parent2[k];
                    parent2[k]=tmp;
                }
            }
            if(!n)//第一次把交叉后的记在child中
            {
                QVector<int> tmp=child1;
                child1=parent1;
                parent1=tmp;

                tmp=child2;
                child2=parent2;
                parent2=tmp;
            }
        }

        //在四个字代中择出最优点
        QMap<int,QVector<int>> cal;//找出最具有优势的两个子代杂交
        QPair<QVector<int>,int> p1(parent1,cal_gene(parent1,valid));
        QPair<QVector<int>,int> p2(parent2,cal_gene(parent2,valid));
        QPair<QVector<int>,int> c1(child1,cal_gene(child1,valid));
        QPair<QVector<int>,int> c2(child2,cal_gene(child2,valid));

        cal[p1.second]=p1.first;
        cal[p2.second]=p2.first;
        cal[c1.second]=c1.first;
        cal[c2.second]=c2.first;
        int x[4]={p1.second,p2.second,c1.second,c2.second};

        //给优势基因排序
        for(int i=0;i<3;i++)
        {
            for(int j=1;j<4;j++)
            {
                if(x[i]<x[j])
                {
                    int tmp=x[i];
                    x[i]=x[j];
                    x[j]=tmp;
                }
            }
        }
        parent1=cal[x[0]];//将适应率为前2名的子代选为父类
        parent2=cal[x[1]];

        int b[4];//随机产生基因变异,基因片段交换顺序
        b[0]=rand()%size;b[1]=rand()%size;b[2]=rand()%size;b[3]=rand()%size;
        for(int k=0;k<2;k++)
        {
            int tmp=parent1[b[k*2]];
            parent1[b[k*2]]=parent1[b[2*k+1]];
            parent1[b[k*2+1]]=tmp;

            tmp=parent2[b[k*2]];
            parent2[b[k*2]]=parent2[b[2*k+1]];
            parent2[b[k*2+1]]=tmp;
        }

    }
    return parent1[0];
}

//计算适应率，由于越靠后的棋子影响越小，所以第一代*1，第二代*0.96，第三代*0.96*0.96......
int Algorithm::cal_gene(QVector<int> a,QVector<QPoint> valid)
{
    double sum=0,x=1;
    for(int i=0;i<a.size();i++)
    {
        if(i%2==0)
            allChess[valid[a[i]].x()][valid[a[i]].y()]=AI_color;
        else
            allChess[valid[a[i]].x()][valid[a[i]].y()]=enemy_color;
        sum+=x*evaluate(allChess);
        x*=0.96;
    }

    for(int i=0;i<a.size();i++)
    {
        allChess[valid[a[i]].x()][valid[a[i]].y()]=0;
    }
    return sum;
}





