#pragma once

#include <QString>
#include <QObject>
#include "mainwindow.h"
#include "variantextension.h"

class Variant : public QObject //https://stackoverflow.com/questions/22985894/qt-cant-connect-a-subclass-of-qgraphicsview-signal-not-inherited-to-slot/22986102#22986102
{
    Q_OBJECT

private:
    Variant();
    static Variant* createInstance();
    QString variantFVS;
    QString abbreviation;
//    QStringList extensionsFVS;
    bool variantLocked;
    static void setLock(bool lock);
    static void setVariant(QString abbrev, QString name, /*QStringList extensions,*/ bool lock = false);

    friend class VariantExtension;
    friend class MainWindow;
public:
    ~Variant();
    static Variant* instance();
    static bool locked();
    static QString name();
    static QString abbrev();

signals:
    void variantChanged();

};
