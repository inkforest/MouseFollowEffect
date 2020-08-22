#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); //GUI程序
    Widget w;
    w.show();
    return a.exec(); //开启事件循环
}
