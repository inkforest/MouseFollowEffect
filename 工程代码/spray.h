#ifndef SPRAY_H
#define SPRAY

#include "drop.h"

class Spray : public Drop
{
    Q_OBJECT

public:
    Spray(QWidget *parent = Q_NULLPTR);
    ~Spray();

    // 显示喷枪
    void showAni(const QPoint &point);

protected:
    // 喷枪半径改变时的操作
    void onAnimationChanged(QVariant value);
    // 重载绘画操作
    void paintEvent(QPaintEvent *event);
};

#endif // SPRAY_H
