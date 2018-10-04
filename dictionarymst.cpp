#include "dictionarymst.h"
#include "singleton.h"

DictionaryMST::DictionaryMST()
{
}

DictionaryMST* DictionaryMST::createInstance()
{
    return new DictionaryMST();
}

DictionaryMST::~DictionaryMST()
{
}

DictionaryMST* DictionaryMST::instance()
{
    return Singleton<DictionaryMST>::instance(DictionaryMST::createInstance);
}

QMap<QString, QString> DictionaryMST::innerMap(QString mapTitle)
{
    return DictionaryMST::instance()->value(mapTitle);
}

DictionaryMST::iterator DictionaryMST::addMap(QString &mainSectionText, QMap<QString, QString> &relation)
{
    return DictionaryMST::instance()->insert(mainSectionText, relation);
}
