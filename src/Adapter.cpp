#include "../include/Adapter.h"


void Adapter::updatePath(std::string &path)
{
    fileManager->setPath(path);
    fileManager->setInfo();
    fileManager->getInfo();
}


void Adapter::remove()
{
    fileManager->remove();
}
