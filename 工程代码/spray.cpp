#include "spray.h"

//构造函数，水滴类继承QLbel，同时初始化各个属性
Spray::Spray(QWidget *parent)
    : Drop(parent)  //调用基类的构造函数
{
    setColor(QColor(253,230,161,255));    //默认颜色为淡黄色
    duration=500;
}

//析构函数
Spray::~Spray()
{
}


void Spray::showAni(const QPoint &point)
{
    // 通过动画类不断进行重绘
    animation->setStartValue(0);
    animation->setEndValue(radius);
    animation->setDuration(500);

    connect(animation, &QVariantAnimation::valueChanged, this, &Spray::onAnimationChanged);
    connect(animation, &QVariantAnimation::finished, this, &Spray::close);
    animation->start();

    // 获取在区域内的相对坐标，即鼠标所在的区域相对坐标。
    x=this->parentWidget()->mapFromGlobal(point).x();
    y=this->parentWidget()->mapFromGlobal(point).y();
    this->show();
}

// 绘制鼠标移动时的喷枪
void Spray::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);// 反锯齿
    painter.setPen(Qt::NoPen);                    // 图案不需要描边，设为NoPen
    painter.setBrush(QBrush(color));

    //思路是一开始圆的半径为0，慢慢扩大到设定值
    //随着鼠标的移动一直画这个动画，效果就像喷枪
    QPainterPath path;
    path.addEllipse(QPoint(x, y), animationRadius, animationRadius);
    QPainterPath rectPath;
    QRect a(0,0,this->parentWidget()->width(),this->parentWidget()->height());
    rectPath.addRect(a);

    painter.drawPath(rectPath&path);
}

void Spray::onAnimationChanged(QVariant value)
{
    // 不断增加圆B的半径值,并重绘
    animationRadius = value.toInt();
    update();
}
