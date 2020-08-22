#ifndef DROP_H
#define DROP_H

#include <QtWidgets/QWidget>
#include <QPainter>
#include <QVariantAnimation>


class Drop : public QWidget
{
    Q_OBJECT

public:
    Drop(QWidget *parent = Q_NULLPTR);
    ~Drop();

    // 设置水滴颜色
    void setColor(QColor color);

    // 设置水滴半径
    void setRadius(int radius);

    // 显示水滴
    void showAni(const QPoint &point);

    // 设置存活时间
    void setDuration(int d);
//    bool isInParentWidget(const QPoint &thisPos);

protected:
    // 水滴动画类
    QVariantAnimation* animation;
    // 动画存活时间
    int duration;
    // 水滴实际半径
    int radius;
    // 水滴变化的半径
    int animationRadius;
    // 水滴的颜色
    QColor color;
    // 水滴的横坐标
    int x;
    // 水滴的纵坐标
    int y;
    // 半径改变时的操作
    void onAnimationChanged(QVariant value);
    // 重载绘画操作
    void paintEvent(QPaintEvent *event);
};

#endif // DROP_H
