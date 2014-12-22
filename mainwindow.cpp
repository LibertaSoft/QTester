#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xmlreader.h"

#include <QToolButton>
#include <QtXml>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile file( QDir::currentPath() + QDir::separator() + "test.xml" );
    qDebug() << QDir::currentPath();
    if( ! file.open(QFile::ReadOnly) ){
        qDebug() << "file not open" << file.errorString();
    }else{

        XmlReader r(&file);

        QMap<QString,QVariant> d;
        float version(0);
        r.readHeader(version);
        qDebug() << "version: " << version;
//        std::vector<Lesson> lessons;
//        Lesson l = {"it","Инф тех", "russian"};

//        QGridLayout *gl = new QGridLayout(this);

//        for(int i = 0; i < 4; ++i ){
//            for(int j = 0; j < 10; ++j ){
//                gl->addWidget( new QPushButton( QString::number(i) + QString::number(j) ), j, i );
//            }
//        }

//        ui->ScrollArea_Questions->setLayout( gl );

//        while( (r.atEnd() == false) && (r.hasError() == false) ){
//            d = r.readNext();
//        }

//        d = r.readNext();
//        qDebug() << d;

        file.close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
