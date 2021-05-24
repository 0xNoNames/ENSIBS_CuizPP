#include <QApplication>
#include "serveurgui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServeurGUI gui;
    gui.show();
    return a.exec();
}
