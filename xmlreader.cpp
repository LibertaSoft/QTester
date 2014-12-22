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
            if( ! ok ){
                return false;
            }
            return true;
        }else{
            qCritical() << "[XmlReader::readHeader] The tag a 'QTester' is expected.";
            return false;
        }
    }
    return true;
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
            if( ! readQuestion() ){
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


bool XmlReader::readQuestion()
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
    int i(0);
    while ( !(_streamReader.tokenType() == QXmlStreamReader::EndElement
              && _streamReader.name() == "question") ){
        if( _streamReader.hasError() ){
            qCritical() << "[XmlReader::readItem] " << _streamReader.errorString();
            return false;
        }
        _streamReader.readNext();
        if ( _streamReader.tokenType() == QXmlStreamReader::StartElement
             && _streamReader.name() == "text" ){
             _streamReader.readNext();
             _currentData["textQuestion"] = _streamReader.text().toString();
        }
        if (_streamReader.tokenType() == QXmlStreamReader::StartElement
             && _streamReader.name() == "answers" ){
            QXmlStreamAttributes attr = _streamReader.attributes();
            QString type;
            _currentData["type"] = attr.value("type").toString();
        }
        if(_streamReader.tokenType() == QXmlStreamReader::StartElement
            && _streamReader.name() == "answer" ){
            ++i;
            QXmlStreamAttributes attr = _streamReader.attributes();
            QString valid;
            _currentData["validAnswer" + QString::number(i)] = attr.value("valid").toString();

            _streamReader.readNext();
            _currentData["answer" + QString::number(i)] = _streamReader.text().toString();

        }
    }
    _currentData["answerCount"] = i;
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
