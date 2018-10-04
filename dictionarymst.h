#ifndef DICTIONARYMST_H
#define DICTIONARYMST_H

#include <QMap>

class DictionaryMST : private QMap<QString, QMap<QString, QString>>
{

private:
    DictionaryMST();
    static DictionaryMST* createInstance();

public:
    ~DictionaryMST();
    static DictionaryMST* instance();
    static QMap<QString, QString> innerMap(QString mapTitle);
    static iterator addMap(QString &mainSectionText, QMap<QString, QString> &relation);

};

#endif // DICTIONARYMST_H
