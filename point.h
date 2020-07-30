#ifndef POINT_H
#define POINT_H
#include <QPixmap>
#include <QPainter>
class Point
{
    //int place;
    int first,second;
    double _x;
    double _y;
    int statue;//是否有棋子，0无，1为黑棋，2为白棋
    QPixmap* object;
public:
    Point(double a,double b,int i,int j);
    bool ifCanPut(){return !statue;}
    bool ifNear(double x,double y);
    void setChess(int a);
    void show(QPainter* p);
    double getX(){return _x;}
    double getY(){return _y;}

    //int getPlace(){return place;}
    int getColor(){return statue;}
    int getI(){return first;}
    int getJ(){return second;}

};

#endif // POINT_H
