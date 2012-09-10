#include <QApplication>
#include <QtGui/QDesktopWidget>

#include "mainwindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainWindow view;
    QSize windowSize;

    windowSize.setWidth(QApplication::desktop()->screenGeometry().width());
    windowSize.setHeight(QApplication::desktop()->screenGeometry().height());
    view.resize(windowSize);

#if defined(Q_OS_SYMBIAN) || defined(QT_SIMULATOR) || defined(MEEGO_EDITION_HARMATTAN)
    view.showFullScreen();
#else
    view.show();
#endif

    return app.exec();
}
