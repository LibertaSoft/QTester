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
    int _currentNumQuestion;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void questionChanged(int num);

private slots:
    void on_ChBox_ReturnLater_toggled(bool checked);
    void on_ChBox_DontKnow_toggled(bool checked);
    void on_ChBox_ErrorInQuestion_toggled(bool checked);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
