#include "QKst.h"
#include "application.h"

//#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    Kst::Application app(argc, argv);
    
    app.initMainWindow();
    if (app.mainWindow()->initFromCommandLine()) {
        app.mainWindow()->show();
        return app.exec();
    }

    Kst::QKst w;
    w.show();
    return app.exec();
}
