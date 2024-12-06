#include <QApplication>
#include "Interface.h"
#include "Adapter.h"


int main(int argc, char *argv[])
{
    FileManager *fileManager = new FileManager();
    Compressor *compressor = new Compressor();
    Archiver *archiver = new Archiver(compressor);
    Adapter *adapter = new Adapter(fileManager, archiver, compressor);

    QApplication app(argc, argv);
    Interface *interface  = new Interface(adapter);
    interface->show();
    return app.exec();
}