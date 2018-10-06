#ifndef SUPPOSEFONT_H
#define SUPPOSEFONT_H

#include <QFont>

class SupposeFont : public QFont
{

private:
    SupposeFont();
    static SupposeFont* createInstance();
public:
    ~SupposeFont();
    static SupposeFont* instance();
    static void set(QString fam, int size, bool bold);
};

#endif // SUPPOSEFONT_H
