#ifndef COLORBALL_H
#define COLORBALL_H

#include "drop.h"
#include <QWidget>
#include <QPainter>
#include <QVariantAnimation>


class ColorBall: public Drop
{
    Q_OBJECT
public:
    ColorBall(QWidget *parent=nullptr);
    ~ColorBall();

    // 显示动画
    void showAni(const QPoint &point);
    // 设置初始不透明度
    void setOpacity(int o);
    // 设置横向移动速度
    void setVx(int v);
    // 设置纵向移动速度
    void setVy(int v);
    // 设置移动速度
    void setVxy(int v1, int v2);
private:
    // 小球的透明度
    int opacity;
    // 小球移动的坐标值
    int vx,vy;
    // 小球的初始坐标
    int x,y;
    // 透明度改变时的操作
    void onAnimationChanged(QVariant value);
    // 重载绘画操作
    void paintEvent(QPaintEvent *event);
};

#endif // COLORBALL_H
