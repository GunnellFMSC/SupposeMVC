#include "modifypreferenceswindow.h"
#include "ui_modifypreferenceswindow.h"

ModifyPreferencesWindow::ModifyPreferencesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyPreferencesWindow)
{
    ui->setupUi(this);
    setFixedSize(size());
    setWindowTitle("Modify Preferences");
    defaultPreferencesModel = new QStringListModel(this);
    QString PreferencesFilePath = QDir::currentPath();
    //PreferencesFilePath += "/suppose.prf";
    PreferencesFilePath += "/example.prf";
    defaultPreferencesSettings = new QSettings(PreferencesFilePath, QSettings::IniFormat);
    defaultPreferencesList = new QStringList;
    *defaultPreferencesList = defaultPreferencesSettings->allKeys();
    if(defaultPreferencesList->size() > 0)
    {
        qDebug() << "Default Preferences File opened successfully!";
        displayFileContents();
    }
    else
        qDebug() << "Default Preferences File was not found!";
}

ModifyPreferencesWindow::ModifyPreferencesWindow(QSettings &defaultPreferenceSettings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyPreferencesWindow)
{
    ui->setupUi(this);
    setFixedSize(size());
    setWindowTitle("Modify Preferences");
    defaultPreferencesModel = new QStringListModel(this);
    defaultPreferencesSettings = new QSettings;
    defaultPreferencesSettings = &defaultPreferenceSettings;
    defaultPreferencesList = new QStringList;
    *defaultPreferencesList = defaultPreferencesSettings->allKeys();
    if(defaultPreferencesList->size() > 0)
    {
        qDebug() << "Passed-in Preferences File opened successfully!";
        displayFileContents();
    }
    else
        qDebug() << "Default Preferences File was not found!";
}

ModifyPreferencesWindow::~ModifyPreferencesWindow()
{
    defaultPreferencesList->clear();
    qDebug() << "Default Preferences List successfully cleared!";
    delete defaultPreferencesList;
    qDebug() << "Default Preferences List successfully deleted!";
    delete defaultPreferencesModel;
    qDebug() << "Default Preferences Model successfully deleted!";
    delete ui;
    qDebug() << "Modify Preferences Window ui successfully deleted!";
}

void ModifyPreferencesWindow::updateFileContents()
{
    qDebug() << "Preferences window's updateFileContents accesed";
    foreach (QString string, *defaultPreferencesList) {
        if (string.contains("defaultDataBaseName", Qt::CaseSensitive)) {
             qDebug() << "Update defaultDataBaseName success!";
             defaultPreferencesSettings->setValue(string, ui->lineEdit_defaultDatabaseName->displayText());
        }
        else if (string.contains("defaultEditor", Qt::CaseSensitive)){
             qDebug() << "Update defaultEditor success!";
             defaultPreferencesSettings->setValue(string, ui->lineEdit_defaultEditor->displayText());
        }
        else if (string.contains("defaultLocationsFileName", Qt::CaseSensitive)){
             qDebug() << "Update defaultLocationsFileName success!";
             defaultPreferencesSettings->setValue(string, ui->lineEdit_defaultLocationsFile->displayText());
        }
        else if (string.contains("defaultParametersFileName", Qt::CaseSensitive)){
             qDebug() << "Update defaultParametersFileName success!";
             defaultPreferencesSettings->setValue(string, ui->lineEdit_defaultParametersFile->displayText());
        }
        else if (string.contains("defaultProcessPlots", Qt::CaseSensitive)){
             qDebug() << "Update defaultProcessPlots success!";
              defaultPreferencesSettings->setValue(string, (ui->button_defaultProcessPlots->isChecked()) ? ("y") : ("n"));
        }
        else if (string.contains("defaultSegmentCycle", Qt::CaseSensitive)){
             qDebug() << "Update defaultSegmentCycle success!";
              defaultPreferencesSettings->setValue(string, (ui->button_defaultSegmentCycle->isChecked()) ? ("y") : ("n"));
        }
        else if (string.contains("defaultUsePPE", Qt::CaseSensitive)){
             qDebug() << "Update defaultUsePPE success!";
             defaultPreferencesSettings->setValue(string, (ui->button_defaultUsePPE->isChecked()) ? ("y") : ("n"));
        }
        else if (string.contains("defaultVariant", Qt::CaseSensitive)){
             qDebug() << "Update defaultVariant success!";
             defaultPreferencesSettings->setValue(string, ui->lineEdit_defaultVariant->displayText());
        }
        else if (string.contains("defaultWorkingDirectory", Qt::CaseSensitive)){
             qDebug() << "Update defaultWorkingDirectory success!";
             defaultPreferencesSettings->setValue(string, ui->lineEdit_defaultWorkingDirectory->displayText());
        }
        else {
             qDebug() << "Update Mapping Error!";
        }
    }
}

void ModifyPreferencesWindow::displayFileContents()
{
    defaultPreferencesModel->setStringList(*defaultPreferencesList);
    foreach (QString string, *defaultPreferencesList) {
        qDebug() << string;
        if (string.contains("defaultDataBaseName", Qt::CaseSensitive)) {
             qDebug() << "defaultDataBaseName success!";
             ui->lineEdit_defaultDatabaseName->setText(defaultPreferencesSettings->value(string).toString());
        }
        else if (string.contains("defaultEditor", Qt::CaseSensitive)){
             qDebug() << "defaultEditor success!";
             ui->lineEdit_defaultEditor->setText(defaultPreferencesSettings->value(string).toString());
        }
        else if (string.contains("defaultLocationsFileName", Qt::CaseSensitive)){
             qDebug() << "defaultLocationsFileName success!";
             ui->lineEdit_defaultLocationsFile->setText(defaultPreferencesSettings->value(string).toString());
        }
        else if (string.contains("defaultParametersFileName", Qt::CaseSensitive)){
             qDebug() << "defaultParametersFileName success!";
             ui->lineEdit_defaultParametersFile->setText(defaultPreferencesSettings->value(string).toString());
        }
        else if (string.contains("defaultProcessPlots", Qt::CaseSensitive)){
             qDebug() << "defaultProcessPlots success!";
             QString holder = defaultPreferencesSettings->value(string).toString();
             (holder == "y" || holder == "n") ? ((holder == "y") ? (on_button_defaultProcessPlots_clicked(true)) : (on_button_defaultProcessPlots_clicked(false))) : (ui->button_defaultProcessPlots->setText(""));
        }
        else if (string.contains("defaultSegmentCycle", Qt::CaseSensitive)){
             qDebug() << "defaultSegmentCycle success!";
             QString holder = defaultPreferencesSettings->value(string).toString();
             (holder == "y" || holder == "n") ? ((holder == "y") ? (on_button_defaultSegmentCycle_clicked(true)) : (on_button_defaultSegmentCycle_clicked(false))) : (ui->button_defaultSegmentCycle->setText(""));
        }
        else if (string.contains("defaultUsePPE", Qt::CaseSensitive)){
             qDebug() << "defaultUsePPE success!";
             QString holder = defaultPreferencesSettings->value(string).toString();
             (holder == "y" || holder == "n") ? ((holder == "y") ? (on_button_defaultUsePPE_clicked(true)) : (on_button_defaultUsePPE_clicked(false))) : (ui->button_defaultUsePPE->setText(""));
        }
        else if (string.contains("defaultVariant", Qt::CaseSensitive)){
             qDebug() << "defaultVariant success!";
             ui->lineEdit_defaultVariant->setText(defaultPreferencesSettings->value(string).toString());
        }
        else if (string.contains("defaultWorkingDirectory", Qt::CaseSensitive)){
             qDebug() << "defaultWorkingDirectory success!";
             ui->lineEdit_defaultWorkingDirectory->setText(defaultPreferencesSettings->value(string).toString());
        }
        else {
             qDebug() << "Display Mapping Error!";
        }
    }
}

void ModifyPreferencesWindow::on_button_defaultToWorkingDirectory_clicked()
{
    qDebug() << "Preference window's \"Set Default to Current Working Directory\" Button clicked";
}

void ModifyPreferencesWindow::on_button_apply_clicked()
{
    qDebug() << "Preferences window's Apply Button clicked";
    updateFileContents();
}

void ModifyPreferencesWindow::on_button_close_clicked()
{
    qDebug() << "Preferences window's Close Button clicked";
    this->close();
}

void ModifyPreferencesWindow::on_button_defaultUsePPE_clicked(bool checked)
{
    qDebug() << "Preferences window's Use PPE Button bool function accessed with bool = " << checked;
    if (checked)
    {
         ui->button_defaultUsePPE->setStyleSheet("background-color: rgb(100, 200, 34)");
         ui->button_defaultUsePPE->setText("Yes");
         ui->button_defaultUsePPE->setChecked(true);
    }
    else
    {
        ui->button_defaultUsePPE->setStyleSheet("background-color: rgb(255, 57, 43)");
        ui->button_defaultUsePPE->setText("No");
        ui->button_defaultUsePPE->setChecked(false);
    }
}

void ModifyPreferencesWindow::on_button_defaultSegmentCycle_clicked(bool checked)
{
    qDebug() << "Preferences window's Segment Cycle Button bool function accessed with bool = " << checked;
    if (checked)
    {
         ui->button_defaultSegmentCycle->setStyleSheet("background-color: rgb(100, 200, 34)");
         ui->button_defaultSegmentCycle->setText("Yes");
         ui->button_defaultSegmentCycle->setChecked(true);
    }
    else
    {
        ui->button_defaultSegmentCycle->setStyleSheet("background-color: rgb(255, 57, 43)");
        ui->button_defaultSegmentCycle->setText("No");
        ui->button_defaultSegmentCycle->setChecked(false);
    }
}

void ModifyPreferencesWindow::on_button_defaultProcessPlots_clicked(bool checked)
{
    qDebug() << "Preferences window's Process Plots as Stands Button bool function accessed with bool = " << checked;
    if (checked)
    {
         ui->button_defaultProcessPlots->setStyleSheet("background-color: rgb(100, 200, 34)");
         ui->button_defaultProcessPlots->setText("Yes");
         ui->button_defaultProcessPlots->setChecked(true);
    }
    else
    {
        ui->button_defaultProcessPlots->setStyleSheet("background-color: rgb(255, 57, 43)");
        ui->button_defaultProcessPlots->setText("No");
        ui->button_defaultProcessPlots->setChecked(false);
    }
}

void ModifyPreferencesWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        on_button_apply_clicked();
        return;
    }
    QDialog::keyPressEvent(event);
}
