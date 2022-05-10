#include "QKst.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QKst w;
    w.show();
    return a.exec();
}
