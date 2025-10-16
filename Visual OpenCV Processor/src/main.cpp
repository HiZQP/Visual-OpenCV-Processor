#include "VisualOpenCVProcessor.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    qRegisterMetaType<NodeEditorManager::RegistryItemCreator>();
    QApplication app(argc, argv);
    VisualOpenCVProcessor window;
    window.show();
    return app.exec();
}
