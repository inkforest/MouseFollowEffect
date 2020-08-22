#include "colorball.h"
#include <QTime>
#include <cmath>
#include <QRandomGenerator>

// 不透明度转alpha值
int opacityToAlpha(const int &opacity)
{
    return round(0.01*opacity*255);
}

ColorBall::ColorBall(QWidget *parent)
    : Drop(parent)
    , opacity(100)                      //小球的不透明度，默认是100
{
    //随机数生成器，参数为当前时间的种子
    QRandomGenerator *random=new QRandomGenerator(QTime::currentTime().msecsSinceStartOfDay());
    //随机生成颜色
    color=QColor(random->bounded(256),random->bounded(256),random->bounded(256),opacity).darker(70);
    vx=random->bounded(-5,6);
    vy=random->bounded(-5,6);
}

//析构函数
ColorBall::~ColorBall()
{
}

void ColorBall::showAni(const QPoint &point)
{
    // 通过动画类不断进行重绘
    animation->setStartValue(opacity);
    animation->setEndValue(0);
    animation->setDuration(duration);

    connect(animation, &QVariantAnimation::valueChanged, this, &ColorBall::onAnimationChanged);
    connect(animation, &QVariantAnimation::finished, this, &ColorBall::close);
    animation->start();

    //将传过来的显示屏坐标转换成与父窗口的相对坐标
    x=this->parentWidget()->mapFromGlobal(point).x();
    y=this->parentWidget()->mapFromGlobal(point).y();
    show();
}

// 彩球效果
void ColorBall::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);// 反锯齿
    painter.setPen(Qt::NoPen);                    // 不需要描边

    //思路就是每次重绘，小球的坐标分别挪动到(x+vx,y+vy)
    //直到小球的不透明度降到0
    color.setAlpha(opacityToAlpha(opacity));
    painter.setBrush(QBrush(color));

    QPainterPath path;
    path.addEllipse(QPoint(x,y),radius,radius);
    QPainterPath rectPath;
    QRect a(0,0,this->parentWidget()->width(),this->parentWidget()->height());
    rectPath.addRect(a);

    painter.drawPath(rectPath&path);
}

void ColorBall::onAnimationChanged(QVariant value)
{
    // 不断减少小球的不透明度，修改坐标，并重绘
    opacity = value.toInt();
    x+=vx;
    y+=vy;
    update();
}

void ColorBall::setOpacity(int o)
{
    opacity=o;
}

void ColorBall::setVx(int v)
{
    vx=v;
}

void ColorBall::setVy(int v)
{
    vy=v;
}

void ColorBall::setVxy(int v1, int v2)
{
    vx=v1;
    vy=v2;
}
