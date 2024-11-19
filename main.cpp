#include <QApplication>
#include "Interface.h"
#define HIGHT 500
#define WIDHT 350

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Interface window;
    window.resize(HIGHT, WIDHT);
    window.setWindowTitle("Compressor");
    window.show();
    return app.exec();
}