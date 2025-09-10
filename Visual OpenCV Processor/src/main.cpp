#include "VisualOpenCVProcessor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    VisualOpenCVProcessor window;
    window.show();
    return app.exec();
}
