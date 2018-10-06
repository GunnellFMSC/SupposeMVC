#include "variant.h"
#include "singleton.h"

Variant::Variant()
{
}

Variant* Variant::createInstance()
{
    return new Variant();
}

Variant::~Variant()
{
}

void Variant::setLock(bool lock)
{
    Variant::instance()->variantLocked = lock;
}

void Variant::setVariant(QString abbrev, QString name, bool lock)
{
    Variant::instance()->variantFVS = name;
    Variant::instance()->variantLocked = lock;
    Variant::instance()->abbreviation = abbrev;
//    Variant::instance()->extensionsFVS = extensions;
    qDebug() << "Variant set";
    emit Variant::instance()->variantChanged();
}

Variant* Variant::instance()
{
    return Singleton<Variant>::instance(Variant::createInstance);
}

QString Variant::name()
{
    return Variant::instance()->variantFVS;
}

bool Variant::locked()
{
    return Variant::instance()->variantLocked;
}

QString Variant::abbrev()
{
    return Variant::instance()->abbreviation;
}

//QStringList Variant::extensions()
//{
//    return Variant::instance()->extensionsFVS;
//}
