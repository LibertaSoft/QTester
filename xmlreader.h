#ifndef XMLREADER_H
#define XMLREADER_H

#include "globalenum.h"
#include <QtXml>
#include <QFile>
#include <QMap>

#include <QDebug>


struct Answer{
    bool valid;
    QString text;
};
struct Question{
    QString text;
    QString type;
    float recomendedTimeInMinutes;
    bool caseSensitivity;
    bool stripSpaces;
    bool markAsError;
    bool markAsReturn;
    std::vector<Answer> answers;
};
struct Theme{
    QString name;
    QString title;
    char difficulty;
    std::vector<Question> questions;
};
struct Lesson{
    QString name;
    QString title;
    QString language;
    std::vector<Theme> themes;
};



class XmlReader
{
    QXmlStreamReader            _streamReader;
    QXmlStreamReader::TokenType _token;
    QMap<QString,QVariant>      _currentData;
    xmlTags::xmlTags            _currentSection;
    bool                        _testerFormat;

    bool readQuestion();
public:
    XmlReader(QIODevice* stream);
    bool isQTesterFormat();
    bool readHeader();
    bool readHeader(float &version);
    QMap<QString, QVariant> readNext();

    bool atEnd()const;
    bool hasError()const;
    xmlTags::xmlTags currentSection()const;
};

#endif // XMLREADER_H
