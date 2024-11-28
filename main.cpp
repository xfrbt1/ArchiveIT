#include <QApplication>
#include "Interface.h"
#include "Adapter.h"

#define HIGHT 500
#define WIDHT 350
#define PROJECT_NAME "ArchiveIT"


int main(int argc, char *argv[])
{
    FileManager *fileManager = new FileManager();
    Compressor *compressor = new Compressor();
    Adapter *adapter = new Adapter(compressor, fileManager);

    QApplication app(argc, argv);
    Interface *interface  = new Interface(adapter);

    interface->resize(HIGHT, WIDHT);
    interface->setWindowTitle(PROJECT_NAME);
    interface->show();

    return app.exec();
}