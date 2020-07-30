#include "point.h"
#include "math.h"
#include "data.h"

Point::Point(double a,double b,int i,int j):first(i),second(j),_x(a),_y(b),statue(0),object(nullptr)
{
    object=new QPixmap;
}

bool Point::ifNear(double x,double y)
{
    if(statue==0&&(powf(x-_x,2)+powf(y-_y,2))<=SCANSCOPE*SCANSCOPE)
    {
        return true;
    }
    return false;
}

void Point::setChess(int a)
{
    statue=a;
    switch (a)
    {
    case 1:
    {
        object->load(":/picture/black.jpg");
    }
        break;
    case 2:
    {
        object->load(":/picture/white.jpg");
    }
        break;
    }
}

void Point::show(QPainter *p)
{
    p->drawPixmap(_x-SCANSCOPE,_y-SCANSCOPE,SCANSCOPE*2,SCANSCOPE*2,*object);
}

