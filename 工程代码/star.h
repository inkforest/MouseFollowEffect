#ifndef STAR_H
#define STAR_H

#include "drop.h"

class Star : public Drop
{
    Q_OBJECT
public:
    Star(QWidget *parent);
    ~Star();
    // 显示动画
    void showAni(const QPoint &point);
    // 设置星星外接圆半径
    void setRadius(int r);
    // 设置星星坠落速度
    void setSpeed(int v);
    //    // 由于边缘的判定标准不一样，需要重载判定函数（已经弃用）
    //    bool isInParentWidget(const QPoint &thisPos);
private:
    //星星坠落的速度
    int vy;
    // 星星多边形
    QPolygonF starPolygon;
    // 动画显示过程中的scale值
    double aniScale;
    // 动画值改变时的操作
    void onAnimationChanged(QVariant value);
    // 重载绘画操作
    void paintEvent(QPaintEvent *event);
};

#endif // STAR_H
