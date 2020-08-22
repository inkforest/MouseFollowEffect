#include "drop.h"
#include<QDebug>

//构造函数，水滴类继承QWidget，同时初始化各个属性
Drop::Drop(QWidget *parent)
    : QWidget(parent)
    , animationRadius(0)
    , color(QColor(6,207,251,255))        // 默认为蓝色
    , radius(30)                        // 水滴的实际半径，默认为30
    , duration(350)
{
    this->move(this->parentWidget()->mapToGlobal(QPoint(0,0)));
    this->resize(this->parentWidget()->size());
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground);
    animation = new QVariantAnimation(this);
}

//析构函数
Drop::~Drop()
{
}

// 设置水滴的颜色
void Drop::setColor(QColor color)
{
    this->color = color;
}

// 设置水滴半径
void Drop::setRadius(int radius)
{
    this->radius = radius;
//    this->setMinimumSize(QSize(radius * 2, radius * 2));
}
// 设置存活时间
void Drop::setDuration(int d)
{
    this->duration=d;
}
void Drop::showAni(const QPoint &point)
{
    // 通过动画类不断进行重绘
    animation->setStartValue(0);
    animation->setEndValue(radius);
    animation->setDuration(duration);

    connect(animation, &QVariantAnimation::valueChanged, this, &Drop::onAnimationChanged);
    connect(animation, &QVariantAnimation::finished, this, &Drop::close);
    animation->start();

    // 获取水滴在区域内的相对坐标，即鼠标所在的区域相对坐标。
    x=this->parentWidget()->mapFromGlobal(point).x();
    y=this->parentWidget()->mapFromGlobal(point).y();
    this->show();
}

// 绘制鼠标移动时的水滴效果
void Drop::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);// 反锯齿
    painter.setPen(Qt::NoPen);                // 由于不需要描边, 设置为NoPen
    painter.setBrush(QBrush(color));

    // 思路就是先绘制一个固定大小的圆A，然后绘制同一圆心的透明的圆B
    // 然后通过动画类使圆B的半径从0增长到radius，以覆盖固定的圆A，显示的效果是一个圆环
    // 最终就看上去比较像水滴落消散的效果
    QPainterPath dropPath;
    dropPath.addEllipse(QPoint(x, y), radius, radius);
    QPainterPath hidePath;
    hidePath.addEllipse(QPoint(x, y), animationRadius, animationRadius);

    // 如何让图形不越过区域：用一个与父窗口区域大小相同的长方形，与所画的圆环做与操作
    QPainterPath rectPath;
    QRect a(0,0,this->parentWidget()->width(),this->parentWidget()->height());
    rectPath.addRect(a);
    painter.drawPath(rectPath&dropPath-hidePath);
}

//  动画类的value改变时的动作
void Drop::onAnimationChanged(QVariant value)
{
    // 不断增加圆B的半径值，并重绘
    // 这个值是根据start value、end value、duration做线性插值得到的
    animationRadius = value.toInt();
    update();
}
