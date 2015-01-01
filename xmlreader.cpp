#include "xmlreader.h"

XmlReader::XmlReader(QIODevice* stream)
    :_streamReader(stream), _testerFormat(false)
{}

bool XmlReader::isQTesterFormat()
{
     return _testerFormat;
}

bool XmlReader::readHeader()
{
    _token = _streamReader.readNext(); // <?xml ... ?>
    _token = _streamReader.readNext(); // <!DOCTYPE QTester>"
    if( _streamReader.isDTD() ){
        if( _streamReader.dtdName() != "QTester" ){
            qCritical() << "[XmlReader::readHeader] DOCTYPE is not a 'QTester'.";
            return false;
        }
        _testerFormat = true;
    }else{
        qCritical() << "[XmlReader::readHeader] DOCTYPE is expected.";
        return false;
    }
    // <QTester>
    _token = _streamReader.readNext();
    if (_token == QXmlStreamReader::StartElement){
        if( _streamReader.name() == "QTester" ){
            return true;
        }else{
            qCritical() << "[XmlReader::readHeader] The tag a 'QTester' is expected.";
            _testerFormat = false;
            return false;
        }
    }
    return true;
}

bool XmlReader::readHeader(float &version)
{
    _token = _streamReader.readNext(); // <?xml ... ?>
    _token = _streamReader.readNext(); // <!DOCTYPE QTester>"
    if( _streamReader.isDTD() ){
        if( _streamReader.dtdName() != "QTester" ){
            qCritical() << "[XmlReader::readHeader] DOCTYPE is not a 'QTester'.";
            return false;
        }
        qDebug() << "DTD is true";
        _testerFormat = true;
    }else{
        qCritical() << "[XmlReader::readHeader] DOCTYPE is expected.";
        return false;
    }
    // <QTester version="0.1">
    _token = _streamReader.readNext();
    if (_token == QXmlStreamReader::StartElement){
        qDebug() << _streamReader.name();
        if( _streamReader.name() == "QTester" ){
            QXmlStreamAttributes attr = _streamReader.attributes();
            bool ok = false;
            version  = attr.value("version").toFloat(&ok);
            if( !ok )
                return false;
            return true;
        }else{
            qCritical() << "[XmlReader::readHeader] The tag a 'QTester' is expected.";
            return false;
        }
    }
    return true;
}

xmlTags::xmlTags XmlReader::readNext2(Lesson &l)
{
    Answer a;
    Question q;
    Theme t;
    _streamReader.readNext();
    while( _streamReader.tokenType() != QXmlStreamReader::StartElement && !hasError() && !atEnd() ){
        _streamReader.readNext();
    }

    if ( _streamReader.name() == "lesson" ){
        // <lesson language="ru" name="it" title="Информационные технологии">
        QXmlStreamAttributes attr = _streamReader.attributes();
        l.language = QLocale( attr.value("language").toString() ).language();
        l.name     = attr.value("name").toString();
        l.title    = attr.value("title").toString();
        return xmlTags::lesson;
    }else if ( _streamReader.name() == "theme" ){
        // <theme name="introduction" title="Введение" difficulty="0">
        QXmlStreamAttributes attr = _streamReader.attributes();
        t.difficulty = attr.value("difficulty").toShort(/* #FixMe : (!ok) */);
        t.name       = attr.value("name").toString();
        t.title      = attr.value("title").toString();
        l.themes.push_back(t);
        return xmlTags::theme;
        // #ToDo : Нужно полностью зачитывать Question
        // Иначе - не понятно когда объект полностью инициализирован.
    }else if ( _streamReader.name() == "question" ){
        // <question recomendedTimeInMinutes="1.5">
        QXmlStreamAttributes attr = _streamReader.attributes();
        q.recomendedTimeInMinutes = attr.value("recomendedTimeInMinutes").toFloat(/* #FixMe : (!ok) */);
        if( ! readQuestion(q) ){
            qCritical() << "Question was reaing at problem.";
            qDebug() << "qType: " << q.type;
            qDebug() << "qText: " << q.text;
        }
        l.themes.front().questions.push_back(q);
        return xmlTags::question;
    }
    return xmlTags::lesson;
}

QMap<QString, QVariant> XmlReader::readNext()
{

    _currentData.clear();
    if( _streamReader.atEnd() ){
        qCritical() << "[XmlReader::readNext] " << _streamReader.errorString();
        return _currentData;
    }
    if ( _streamReader.name() == "lesson"){
        _currentSection = xmlTags::lesson;
        qDebug() << _streamReader.name();
        _streamReader.readNext();
    }
    while(_streamReader.tokenType() != QXmlStreamReader::EndElement
          && _streamReader.name() == "theme"){

        if ( _streamReader.name() == "theme"){
            _currentSection = xmlTags::theme;
        }

        _streamReader.readNext();

        if( _streamReader.name() == "question" ){
            // #Bug
            Question q;
            if( ! readQuestion(q) ){
                qCritical() << "[XmlReader::readNext] " << _streamReader.errorString();
                _currentData.clear();
            }
        }
    }
    if( _streamReader.hasError() ){
        qCritical() << "[XmlReader::readNext] " << _streamReader.errorString();
        return _currentData;
    }
    return _currentData;
}


bool XmlReader::readQuestion(Question &question)
{
/*
<question>
    <text>
        Разработчик языка C++
    </text>
    <answers type="radio"> // type="{check,radio,text,regexp}"
        <answer valid="false">Стив Джобс</answer>
        <answer valid="false">Кен Томпсон</answer>
        <answer valid="true">Бьёрн Строуструп</answer>
        <answer valid="false">Денис Ритчи</answer>
    </answers>
</question>
*/
    while ( !(_streamReader.tokenType() == QXmlStreamReader::EndElement
              && _streamReader.name() == "question") ){
        qDebug() << _streamReader.name() << "||" << _streamReader.text();
        if( _streamReader.hasError() ){

            qCritical() << "[XmlReader::readItem] " << _streamReader.errorString();
            return false;
        }
        _streamReader.readNext();
        if ( _streamReader.tokenType() == QXmlStreamReader::StartElement
             && _streamReader.name() == "text" ){

             _streamReader.readNext();
             question.text = _streamReader.text().toString();
        }
        if (_streamReader.tokenType() == QXmlStreamReader::StartElement
             && _streamReader.name() == "answers" ){

            QXmlStreamAttributes attr = _streamReader.attributes();
            question.type = attr.value("type").toString();
        }
        if(_streamReader.tokenType() == QXmlStreamReader::StartElement
            && _streamReader.name() == "answer" ){

            QXmlStreamAttributes attr = _streamReader.attributes();
            Answer answer;
            answer.valid = ( attr.value("valid").toString().toLower() == "true" )? true : false;
            _streamReader.readNext();
            answer.text = _streamReader.text().toString();

            question.answers.push_back( answer );
        }
    }
    return true;
}

bool XmlReader::atEnd() const
{
    return _streamReader.atEnd();
}

bool XmlReader::hasError() const
{
    return _streamReader.hasError();
}

xmlTags::xmlTags XmlReader::currentSection() const
{
    return _currentSection;
}
