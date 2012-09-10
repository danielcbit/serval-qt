#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class QDeclarativeContext;
class QDeclarativeView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QSize viewSize() const;

public slots:

private:
    QDeclarativeView *m_view;
    QDeclarativeContext *m_context;
};

#endif
