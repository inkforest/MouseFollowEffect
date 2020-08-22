#ifndef WIDGET_H
#define WIDGET_H

#include "drop.h"
#include "spray.h"
#include "colorball.h"
#include "star.h"
#include <QWidget>
#include <QFrame>
#include <QResizeEvent>
//#include <QMouseEvent>
#include <QLabel>
#include <QTimer>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    // 四个子区域的对应矩形
    QRect *area[4];
    // 获得鼠标所在的区域
    int GetArea();
    // 基础图形圆的半径
    int radiusOfCircle;
    // 控制图形的数量
    int count;
    // limit越大则会显示的数量越少
    bool isInLimit(int limit);
private:
    // 两条分割线
    QFrame *horizonLine, *verticalLine;
    // 四个子控件
    QWidget *wid[4];
    // 中间的提示语
    QLabel *label;
    // 特效的提示语
    QLabel *tips[4];
    // 计时器
    QTimer *timer;
    // 子控件的数量
    int smallwidgetNum;
    // 窗口大小改变时的适应性操作
    void resizeEvent(QResizeEvent *event) override;
    // 计时器到期时的操作
    void timeout();
    // 最开始的实现方法：捕获mouseMoveEvent
    //    void mouseMoveEvent(QMouseEvent *event) override;
};
#endif // WIDGET_H
