#include <QApplication>

#include "AlgoBase.h"
#include "Config.h"
#include "QtManager.h"

int main(int argc, char *argv[])
{

    auto config = std::make_shared<Config>("test");
    config->setNumberOfThreads(8);

    if (!config->readFile(config->getLibDir()+"/Data/cloud.ply")) {
        std::cerr << "Can not read the file" << std::endl;
        return 0;
    }

    QApplication app(argc, argv);

    QtManager window;
    window.setConfig(config);
    window.setWindow();

    window.resize(800, 600);
    window.show();

    return app.exec();
}