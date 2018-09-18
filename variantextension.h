#ifndef VARIANTEXTENSION_H
#define VARIANTEXTENSION_H

#include <QStringListModel>
#include <QStringList>
#include <QDialog>
#include <QDebug>
#include <QMap>

namespace Ui {
class VariantExtension;
}

class VariantExtension : public QDialog
{
    Q_OBJECT

public:
    explicit VariantExtension(QString *variant, QMap<QString, QString> *variantExtensions, QMap<QString, QString> *variantAbbreviationNames, QMap<QString, QString> *extensionAbbreviationNames, bool *variantLocked, QWidget *parent = 0);
    QString startingVariant;
    ~VariantExtension();

signals:
    void variantChanged();

private slots:
    void on_comboBox_variant_activated(const QString &arg1);

    void on_comboBox_program_activated(const QString &arg1);

    void on_pushButton_close_clicked();

    void on_pushButton_lock_clicked();

private:
    bool *locked;
    QString *variantFVS;
    Ui::VariantExtension *ui;
    QStringListModel *variantModel, *programModel, *extensionModel;
    QMap<QString, QString> *programVariant, *variantAbbreviationNamesMap, *programExtensions, *extensionAbbreviationNamesMap;
};

#endif // VARIANTEXTENSION_H
