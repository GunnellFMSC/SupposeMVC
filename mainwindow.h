#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QDir>
#include <QMap>
#include <QFile>
#include <QSettings>
#include <QStringListModel>
#include <QRegularExpression>
#include "fvskeywordswindow.h"
#include "managementactions.h"
#include "modifypreferenceswindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    bool mapParmsMainSectionText();
    static QStringList readSectionFromMappedLoc(QIODevice &parms, qint64 location);
//    static void make121DictionaryFromSection(QMap<QString, QString> *dictionary, QStringList wordDefinitionsRaw, QRegularExpression separater, QRegularExpression wordExcess, QRegularExpression definitionExcess);
    static void makeDictionaryFromSection(QMap<QString, QString> *dictionary, QStringList wordDefinitionsRaw, QRegularExpression separater =  QRegularExpression(": +{"), QRegularExpression definitionExcess = QRegularExpression("}"), QRegularExpression wordExcess = QRegularExpression("\\s"), bool oneToMany = false);
    static QMap<QString, QMap<QString, QString>> keyword_Exten_MainSecTitle, speciesMSTAbbreviationName;
    QMap<QString, QString> *variantExtensions, *extensionAbbreviationNames, *variantAbbreviationNames;
    QMap<QString, qint64> parmMainSectionMap;
    QString preferencesFileName, variant;
    QSettings *preferences;
    QFile *parameters;

private slots:
    void on_button_Exit_clicked();

    void on_button_RunSim_clicked();

    void on_actionSuppose_Preferences_triggered();

    void on_button_SelectStands_clicked();

    void on_button_SetTimeScale_clicked();

    void on_button_SelectManagement_clicked();

    void on_button_SelectOutputs_clicked();

    void on_button_AddKeywords_clicked();

    void on_button_InsertFromFile_clicked();

    void on_button_SelectPostProcessors_clicked();

    void on_button_SelectModifiers_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
