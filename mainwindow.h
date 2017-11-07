#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QEvent>
#include <QMainWindow>
#include "gstmediasource.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int argc, char *argv[],QWidget *parent = 0);
    ~MainWindow();
    bool eventFilter (QObject *obj, QEvent *event);
protected:
//    void timerEvent(QTimerEvent *event);

private:
    Ui::MainWindow *ui;
    gstMediaSource *source;

};

#endif // MAINWINDOW_H
