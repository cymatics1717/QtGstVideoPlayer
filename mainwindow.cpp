#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeyEvent>


MainWindow::MainWindow(int argc, char *argv[], QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->label);
    installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::KeyPress){
        QKeyEvent *event = static_cast<QKeyEvent *>(e);
        if(event){
//            qDebug() << event->text();
//            return true;
            switch (event->key()) {
            case Qt::Key_Q:
                qApp->quit();
                break;
            case Qt::Key_Return:
            {
                static int cnt =0;
                if(++cnt%2==1){
                    showFullScreen();
                } else {
                    showNormal();
                }
            }
                break;
            default:
                break;
            }
        }
    } else if(e->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *event = static_cast<QMouseEvent *>(e);
        if(event/*&&event->button()==Qt::RightButton*/){
//            source->run();
        }
    }

    return QObject::eventFilter(obj,e);
}

QLabel* MainWindow::label()
{
    return ui->label;
}

