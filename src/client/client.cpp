#include <QApplication>
#include "widget.h"


int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    widget w;
    w.show();
    return a.exec();
}
