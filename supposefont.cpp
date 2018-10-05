#include "supposefont.h"
#include "singleton.h"

SupposeFont::SupposeFont()
{
}

SupposeFont* SupposeFont::createInstance()
{
    return new SupposeFont();
}

SupposeFont::~SupposeFont()
{
}

SupposeFont* SupposeFont::instance()
{
    return Singleton<SupposeFont>::instance(SupposeFont::createInstance);
}

void SupposeFont::set(QString fam, int size, bool bold)
{
    SupposeFont::instance()->setFamily(fam);
    SupposeFont::instance()->setPointSize(size);
    SupposeFont::instance()->setBold(bold);
}
