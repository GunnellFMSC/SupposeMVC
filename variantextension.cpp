#include "variantextension.h"
#include "ui_variantextension.h"

VariantExtension::VariantExtension(QMap<QString, QString> *variantExtensions, QMap<QString, QString> *variantAbbreviationNames, QMap<QString, QString> *extensionAbbreviationNames, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VariantExtension)
{
    qDebug() << "Inside VariantExtension constructor";
    ui->setupUi(this);
    bool climate = false, b = false;
    variantModel = new QStringListModel;
    programModel = new QStringListModel;
    extensionModel = new QStringListModel;
    QStringList fvsPrograms;
    variantAbbreviationNamesMap = variantAbbreviationNames;
    extensionAbbreviationNamesMap = extensionAbbreviationNames;
    programVariant = new QMap<QString, QString>;
    programExtensions = new QMap<QString, QString>;
    foreach (QString var, variantExtensions->values())
    {
        QString program = variantExtensions->key(var);
//        qDebug() << program;
        if(((program.size() > 5 && climate) || (program.size() > 5 && b)) && !fvsPrograms.contains(program))
        {
            if (program.at(5) == 'b' && b)
                fvsPrograms.append(program);
            if(program.at(5) == 'c' && climate)
                fvsPrograms.append(program);
        }
        else if(program.size() == 5 && !fvsPrograms.contains(program))
            fvsPrograms.append(program);
//        qDebug() << "Variant program: " << program << "Variant short Name: " << var.left(2);
        programVariant->insertMulti(program, var.left(2));
        programExtensions->insert(program, var.right(var.size()-3));
    }
    foreach (QString var, variantAbbreviationNamesMap->keys()) {
        if(!programVariant->values().contains(var)){ qDebug() << "Removing" << var;
            variantAbbreviationNamesMap->remove(var);}
    }
    qDebug() <<programVariant->values() << programVariant->keys() << "\n" << programExtensions->keys() << programExtensions->values();
    variantModel->setStringList(variantAbbreviationNames->values());
    variantModel->sort(0);
    programModel->setStringList(fvsPrograms);
    ui->comboBox_variant->setModel(variantModel);
    ui->comboBox_program->setModel(programModel);
    ui->comboBox_variant->setCurrentText(Variant::name());
    on_comboBox_variant_activated(Variant::name());
    ui->comboBox_variant->setDisabled(Variant::locked());
    ui->comboBox_program->setDisabled(Variant::locked());
}

VariantExtension::~VariantExtension()
{
    delete programExtensions;
    delete programVariant;
    delete extensionModel;
    delete variantModel;
    delete programModel;
    delete ui;
}

void VariantExtension::on_comboBox_variant_activated(const QString &arg1)
{
    qDebug() << "FVS Variant activated" << arg1 << variantAbbreviationNamesMap->key(arg1) << programVariant->key(variantAbbreviationNamesMap->key(arg1));
//     qDebug() << programExtensions->value(programVariant->key(variantAbbreviationNamesMap->key(arg1)));
    ui->comboBox_program->setCurrentText(programVariant->key(variantAbbreviationNamesMap->key(arg1)));
    qDebug() << "Variant: " + variantAbbreviationNamesMap->value(variantAbbreviationNamesMap->key(ui->comboBox_variant->currentText())) + " (" +variantAbbreviationNamesMap->key(ui->comboBox_variant->currentText()) + ") selected.";
    Variant::abbrev() = variantAbbreviationNamesMap->key(ui->comboBox_variant->currentText());
//    emit variantChanged(); // FVS reconstruction for every selection
    QStringList FVSExtensionsFull, FVSExtensionsAbbrev = QString(programExtensions->value(programVariant->key(variantAbbreviationNamesMap->key(arg1)))).split(" ");
//     qDebug() << FVSExtensionsAbbrev;
    FVSExtensionsFull.append(extensionAbbreviationNamesMap->value("base"));
    foreach (QString ext, FVSExtensionsAbbrev)
        FVSExtensionsFull.append(extensionAbbreviationNamesMap->value(ext));
    FVSExtensionsFull.sort();
    extensionModel->setStringList(FVSExtensionsFull);
    ui->listView_extensions->setModel(extensionModel);
}

void VariantExtension::on_comboBox_program_activated(const QString &arg1)
{
    qDebug() << "FVS Program activated" << arg1 << programVariant->value(arg1) << variantAbbreviationNamesMap->value(programVariant->value(arg1));
    ui->comboBox_variant->setCurrentText(variantAbbreviationNamesMap->value(programVariant->value(arg1)));
    on_comboBox_variant_activated(variantAbbreviationNamesMap->value(programVariant->value(arg1)));
}

void VariantExtension::on_pushButton_lock_clicked()
{
    qDebug() << "VariantExtension::Lock clicked";
    qDebug() << "Variant: " + ui->comboBox_variant->currentText() + " (" +variantAbbreviationNamesMap->key(ui->comboBox_variant->currentText()) + ") locked in.";
    if(ui->comboBox_variant->currentText() != Variant::name())
        Variant::setVariant(variantAbbreviationNamesMap->key(ui->comboBox_variant->currentText()), ui->comboBox_variant->currentText(), true);
    else if (!Variant::locked())
        Variant::setLock(true);
    ui->comboBox_variant->setDisabled(true);
    ui->comboBox_program->setDisabled(true);
    ui->pushButton_lock->setDisabled(true);
}

void VariantExtension::on_pushButton_close_clicked()
{
    qDebug() << "VariantExtension::Close clicked";
    if(ui->comboBox_variant->currentText() != Variant::name())
        Variant::setVariant(variantAbbreviationNamesMap->key(ui->comboBox_variant->currentText()), ui->comboBox_variant->currentText(), Variant::locked());
    qDebug() << Variant::name() << Variant::abbrev();
    this->accept();
}
