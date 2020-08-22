#include "widget.h"
#include <QDebug>
#include <QString>
#include <QIcon>

//默认的窗口大小
const int WIDTH=1024;
const int HEIGHT=(9/16.0)*WIDTH;
QString areaName[]={"左上","右上","左下","右下"};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //设置窗口的图标
    setWindowIcon(QIcon(":/pic/rat.png"));
    this->setMinimumSize(WIDTH,HEIGHT);
    //借助QPalette设置窗口的背景颜色为白色
    QPalette pal;
    pal.setColor(QPalette::Background,QColor("white"));
    this->setPalette(pal);
    //设置窗口的标题
    this->setWindowTitle("mouse_follow_effect");
    //绘制区域的分界线
    horizonLine=new QFrame(this);
    verticalLine=new QFrame(this);
    //区域提示语
    label=new QLabel(this);
    label->setAlignment(Qt::AlignLeft);
    label->setText("您当前不在任何一个绘画区。");
    label->show();
    label->setAutoFillBackground(true);
    //背景色为白色，不然会被分割线盖住
    label->setPalette(QPalette(Qt::white));

    //每个区域的效果提示
    QString tipText[]={
        "水滴效果","喷枪效果","彩球效果","星星效果"
    };

    //窗口小区域为4个
    smallwidgetNum=4;
    //分别初始化每个区域
    for(int i=0;i<smallwidgetNum;i++)
    {
        //大小
        area[i]=new QRect;
        //每个区域的子控件
        wid[i]=new QWidget(this);
        //每个区域的效果提示语
        tips[i]=new QLabel(this);
        wid[i]->setMinimumSize(QSize(WIDTH/2, HEIGHT/2));
        wid[i]->setMouseTracking(true);
        tips[i]->setAlignment(Qt::AlignRight);
        tips[i]->setText(tipText[i]);
    }

    //其中3个效果用到的圆的半径
    radiusOfCircle=20;
    //用于控制绘画数的变量
    count=0;

    // 计时器
    timer=new QTimer(this);
    // 连接槽函数和信号，信号发出后触发槽函数
    connect(timer, &QTimer::timeout, this, &Widget::timeout);
    // 计时器开始计时，间隔为10毫秒
    timer->start(10);
}

//析构函数
Widget::~Widget()
{
}

//窗口改变大小的时候，各个控件需要做的适应性操作
void Widget::resizeEvent(QResizeEvent *event)
{
    QRect rec=this->rect();
    QSize sz=event->size();
    int width=sz.width()/2;
    int hei=sz.height()/2;

    // 水平分界线，高度为3像素的frame
    horizonLine->setGeometry(0,sz.height()/2,sz.width(),3);
    horizonLine->setFrameShape(QFrame::HLine);
    horizonLine->setFrameShadow(QFrame::Sunken);
    horizonLine->raise();
    //垂直分界线，宽度为3像素
    verticalLine->setGeometry(sz.width()/2,0,3,sz.height());
    verticalLine->setFrameShape(QFrame::VLine);
    verticalLine->setFrameShadow(QFrame::Sunken);
    verticalLine->raise();

    label->move(rec.center()-QPoint(label->text().length()*5.5,6));
    label->raise();

    area[0]->setRect(0,0,width,hei);
    area[1]->setRect(width,0,width,hei);
    area[2]->setRect(0,hei,width,hei);
    area[3]->setRect(width,hei,width,hei);

    for(int i=0;i<smallwidgetNum;i++)
    {
        wid[i]->resize(width,hei);
        wid[i]->move(area[i]->topLeft());
        tips[i]->move(area[i]->center()+QPoint(-24,-hei/2+3));
        tips[i]->show();
    }
}

// 另一种方法：捕获mouseMoveEvent，但无法捕捉到窗口外的鼠标坐标
//void Widget::mouseMoveEvent(QMouseEvent *event)
//{
//    //区域的标识
//    int areaNum=GetArea(event->pos());

//    //不同区域有不同的提示语
//    if(areaNum+1)
//    {
//        label->setText("您当前位于区域"+areaName[areaNum]+"。");
//        label->adjustSize();
//    }

//    //到达哪个区域就显示哪个效果
//    switch (areaNum)
//    {
//        case 0:
//        {
//            //控制水滴的数量
//            count=count>15?0:count+1;
//            if(count)   break;
//            Drop *drop=new Drop(wid[0]);
//            drop->setFocusPolicy(Qt::NoFocus);
//            drop->setRadius(radiusOfCircle);
//            // 动画播放完毕后自动删除
//            drop->setAttribute(Qt::WA_DeleteOnClose);
//            drop->showAni(event->globalPos());
//            break;
//        }
//        case 1:
//        {
//            Spray *spray=new Spray(wid[1]);
//            spray->setRadius(radiusOfCircle);
//            // 动画播放完毕后自动删除
//            spray->setAttribute(Qt::WA_DeleteOnClose);
//            spray->showAni(event->globalPos());
//            break;
//        }
//        case 2:
//        {
//            ColorBall *ball=new ColorBall(wid[2]);
//            ball->setRadius(radiusOfCircle);
//            // 动画播放完毕后自动删除
//            ball->setAttribute(Qt::WA_DeleteOnClose);
//            ball->showAni(event->globalPos());
//            break;
//        }
//        case 3:
//        {
//            // 控制星星的数量
//            count=count>7?0:count+1;
//            if(count)break;
//            Star *star=new Star(wid[3]);
//            star->setAttribute(Qt::WA_DeleteOnClose);
//            star->showAni(event->globalPos());
//            break;
//        }
//    default:
//        label->setText("您当前不在任何一个绘画区。");
//    }
//}

//判断鼠标当前在哪个区域
int Widget::GetArea()
{
    QPoint pos=QCursor::pos();
    for(int i=0;i<4;i++)
    {
        QSize sz=area[i]->size();
        QPoint p=mapToGlobal(area[i]->topLeft());
        if(QRect(p,sz).contains(pos))
            return i;
    }
    return -1;
}

void Widget::timeout()
{
    QPoint pos=QCursor::pos();

    //区域的标识
    int areaNum=GetArea();

    //不同区域有不同的提示语
    if(areaNum+1)
    {
        label->setText("您当前位于区域"+areaName[areaNum]+"。");
        label->adjustSize();
    }

    //到达哪个区域就显示哪个效果
    switch (areaNum)
    {
        case 0:
        {
            //控制水滴的数量
            if(isInLimit(10)) break;
            Drop *drop=new Drop(wid[areaNum]);
            drop->setRadius(radiusOfCircle);
            // 动画播放完毕后自动删除
            drop->setAttribute(Qt::WA_DeleteOnClose);
            drop->showAni(pos);
            break;
        }
        case 1:
        {
            Spray *spray=new Spray(wid[areaNum]);
            spray->setRadius(radiusOfCircle);
            // 动画播放完毕后自动删除
            spray->setAttribute(Qt::WA_DeleteOnClose);
            spray->showAni(pos);
            break;
        }
        case 2:
        {
            ColorBall *ball=new ColorBall(wid[areaNum]);
            ball->setRadius(radiusOfCircle);
            // 动画播放完毕后自动删除
            ball->setAttribute(Qt::WA_DeleteOnClose);
            ball->showAni(pos);
            break;
        }
        case 3:
        {
            // 控制星星的数量
            if(isInLimit(7)) break;
            Star *star=new Star(wid[areaNum]);
            // 动画播放完毕后自动删除
            star->setAttribute(Qt::WA_DeleteOnClose);
            star->showAni(pos);
            break;
        }
    default:
        label->setText("您当前不在任何一个绘画区。");
        label->adjustSize();
        count=0;
    }
}

bool Widget::isInLimit(int limit)
{
    count=count>limit?0:count+1;
    return count;
}
