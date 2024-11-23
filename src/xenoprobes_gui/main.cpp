/**
 * @file main.cpp
 *
 * @author Dan Keenan
 * @date 11/16/24
 * @copyright GNU GPLv3
 */

#include <QApplication>

#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("xenoprobes");
    app.setApplicationDisplayName(app.translate("main", "Xenoprobes"));
    app.setApplicationVersion("1.0.0");
    app.setWindowIcon(QIcon(":/dataprobe.png"));

    MainWindow main_window;
    main_window.show();

    return app.exec();
}
