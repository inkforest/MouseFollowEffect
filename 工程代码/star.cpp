#include "star.h"
#include "QPicture"
#include <cmath>
#include <QRandomGenerator>
#include <QTime>
#include <qdebug.h>

// 角度转弧度制
double AngleToRad(double angle);
double AngleToRad(double angle)
{
    return 3.14 * angle / 180.0;
}

Star::Star(QWidget *parent)
    : Drop(parent), vy(10) // 坠落速度
      ,
      aniScale(1) // 默认scale为1
{
    // 画一个星星，星星外接圆圆心为(radius, radius)
    starPolygon << QPointF(radius, 0);
    starPolygon << QPointF(radius + radius * sin(AngleToRad(36)), radius + radius * cos(AngleToRad(36)));
    starPolygon << QPointF(radius - radius * sin(AngleToRad(72)), radius - radius * cos(AngleToRad(72)));
    starPolygon << QPointF(radius + radius * sin(AngleToRad(72)), radius - radius * cos(AngleToRad(72)));
    starPolygon << QPointF(radius - radius * sin(AngleToRad(36)), radius + radius * cos(AngleToRad(36)));

    QRandomGenerator *random = new QRandomGenerator(QTime::currentTime().msecsSinceStartOfDay());
    setColor(QColor(random->bounded(256), random->bounded(256),
                    random->bounded(256), 255)
                 .darker(70));
}
Star::~Star()
{
}

void Star::setRadius(int r)
{
    // 修改scale间接达到修改半径的效果
    aniScale = r / radius;
}

void Star::showAni(const QPoint &point)
{
    // 通过动画类不断进行重绘
    animation->setStartValue(aniScale);
    // 由于后续处理需要用到除value，因此end value不能设为0
    animation->setEndValue(0.01);
    animation->setDuration(duration);

    connect(animation, &QVariantAnimation::valueChanged, this, &Star::onAnimationChanged);
    connect(animation, &QVariantAnimation::finished, this, &Star::close);
    animation->start();

    // 将全局坐标转换为局部坐标，还要让鼠标在星星外接圆的圆心处
    x = this->parentWidget()->mapFromGlobal(point).x() - radius;
    y = this->parentWidget()->mapFromGlobal(point).y() - radius - vy;
    show();
}

void Star::onAnimationChanged(QVariant value)
{
    aniScale = value.toDouble();
    update();
}

void Star::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 反锯齿
    painter.setPen(Qt::NoPen);                     // 不需要描边
    painter.setBrush(QBrush(color));

    //思路：painter的scale每一次都变得更小，那么每次变动都画一个更小的星星
    QPainterPath rectPath;
    QRect a(0, 0, this->parentWidget()->width() / aniScale, this->parentWidget()->height() / aniScale);
    rectPath.addRect(a);
    QPainterPath path;
    path.addPolygon(starPolygon);
    path.setFillRule(Qt::WindingFill);
    y += vy;
    // scale改变以后坐标单位值改变，需要用原来的坐标除scale，保持星星的位置不偏离鼠标
    path.translate(x / aniScale, y / aniScale);

    // scale为坐标的单位值，原本的scale为1。设定scale值后，
    // 原始坐标(a,b)变为(a*scale,b*scale)，通过这个特性使每次画的星星变小
    painter.scale(aniScale, aniScale);
    painter.drawPath(rectPath & path);
}

//bool Star::isInParentWidget(const QPoint &thisPos)
//{

//        QWidget *p=this->parentWidget();
//        QPoint *parentPos=new QPoint(p->mapToGlobal(QPoint(0,0)));
//        QSize *s=new QSize(p->size().width()-0.3*radius,p->size().height()-0.3*radius);
//        QRect *pArea=new QRect(QPoint(parentPos->rx()+0.3*radius,parentPos->ry()+0.3*radius), *s);
//        return pArea->contains(thisPos);
//}
