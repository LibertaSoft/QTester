#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "xmlreader.h"
#include "questbutton.h"

#include <QRadioButton>
#include <QtXml>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), AnswersVLay(nullptr), AnswersLay(nullptr)
{
    ui->setupUi(this);


    QFile file( QDir::currentPath() + "/../QTester/Project/" + "it_test.xml" );
    qDebug() << QDir::currentPath();
    if( ! file.open(QFile::ReadOnly) ){
        qDebug() << "file not open" << file.errorString();
    }else{

        XmlReader r(&file);

        QMap<QString,QVariant> d;
        float version(0);
        r.readHeader(version);

        int row(0);
        int col(0);
        int num(0);

        while( r.atEnd() == false ){
            xmlTags::xmlTags x = r.readNext2(lesson);
            switch(x){
            case xmlTags::lesson:
                //                qDebug() << "Lesson";
                //                qDebug() << lesson.language;
                //                qDebug() << lesson.name;
                //                qDebug() << lesson.title;
                break;
            case xmlTags::theme:
                //                qDebug() << "Theme";
                //                qDebug() << t.difficulty;
                //                qDebug() << t.name;
                //                qDebug() << t.title;
                break;
            case xmlTags::question:
                //                qDebug() << "Question";
                //                qDebug() << q.text;
                //                qDebug() << q.type;
                //                qDebug() << q.recomendedTimeInMinutes;
                if(col==5){
                    ++row;
                    col = 0;
                }
                ++col; ++num;
                QuestButton *b = new QuestButton(num, this);
                b->setText( QString::number(num) );
                connect( b, &QuestButton::activated, this, &MainWindow::questionChanged );
                ui->gridLayout_2->addWidget( b, row, col );
            }
        }

        file.close();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::questionChanged(int num)
{
    _currentNumQuestion = num;

    ui->ChBox_ErrorInQuestion->setChecked( lesson.themes[0].questions[num].markAsError );
    ui->ChBox_DontKnow->setChecked( lesson.themes[0].questions[num].markAsDontKnow );
    ui->ChBox_ReturnLater->setChecked( lesson.themes[0].questions[num].markAsReturn );

    QString type = lesson.themes[0].questions[num].type;
    QString text = lesson.themes[0].questions[num].text;
    int len = lesson.themes[0].questions[num].answers.size();
    if( AnswersVLay ){
        delete AnswersVLay;
    }
    if( AnswersLay ){
        delete AnswersLay;
    }

    AnswersLay = new QWidget;
    AnswersVLay = new QVBoxLayout;
    AnswersLay->setLayout( AnswersVLay );

    ui->VLay_Answers->addWidget( AnswersLay );
    ui->Label_Question->setText(text);
    if( type.toLower() == "radio" ){
        for(int i(0); i < len; ++i){
            QString txt = lesson.themes[0].questions[num].answers[i].text;
            AnswersVLay->addWidget( new QRadioButton(txt) );
        }
    }else if( type.toLower() == "check" ){
        for(int i(0); i < len; ++i){
            QString txt = lesson.themes[0].questions[num].answers[i].text;
            AnswersVLay->addWidget( new QCheckBox(txt) );
        }
    }else if( type.toLower() == "text" ){
        for(int i(0); i < len; ++i){
            QString txt = lesson.themes[0].questions[num].answers[i].text;
            // #ToDo : Создавать 1 виджет и проверять на несколько ответов
            AnswersVLay->addWidget( new QLineEdit );
        }
    }else if( type.toLower() == "regexp" ){
        for(int i(0); i < len; ++i){
            QString txt = lesson.themes[0].questions[num].answers[i].text;
            // #ToDo : Создавать 1 виджет и проверять на несколько ответов
            AnswersVLay->addWidget( new QLineEdit );
        }
    }else{
        qCritical() << "Question type is undefined";
    }
}

void MainWindow::on_ChBox_ReturnLater_toggled(bool checked)
{
    lesson.themes[0].questions[_currentNumQuestion].markAsReturn = checked;
}

void MainWindow::on_ChBox_DontKnow_toggled(bool checked)
{
    lesson.themes[0].questions[_currentNumQuestion].markAsDontKnow = checked;
}

void MainWindow::on_ChBox_ErrorInQuestion_toggled(bool checked)
{
    lesson.themes[0].questions[_currentNumQuestion].markAsError = checked;
}
