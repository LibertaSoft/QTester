#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xmlreader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile file( QDir::currentPath() + QDir::separator() + "test.xml" );
    qDebug() << QDir::currentPath();
    if( ! file.open(QFile::ReadOnly) ){
        qDebug() << "file not open" << file.errorString();
    }
    XmlReader r(&file);

    QMap<QString,QVariant> d;
    float version;
    r.readHeader(version);
//    d = r.readNext();
    qDebug() << d;
//    d = r.readNext();
    qDebug() << d;
//    d = r.readNext();
    qDebug() << d;

    file.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
