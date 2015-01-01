#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "xmlreader.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QVBoxLayout *AnswersVLay;
    QWidget *AnswersLay;
    Lesson lesson;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void questionChanged(int num);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
