
#include "mainwindow.h"

#include <QtDeclarative>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_view(new QDeclarativeView(this))
    , m_context(m_view->rootContext())
{
    connect(m_view->engine(), SIGNAL(quit()), SLOT(close()));

    m_view->setSource(QUrl("qrc:/qml/main.qml"));
    m_view->setResizeMode(QDeclarativeView::SizeRootObjectToView);

    setCentralWidget(m_view);
}

MainWindow::~MainWindow()
{
}

