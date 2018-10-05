#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QDir>
#include <QMap>
#include <QFile>
#include <QLabel>
#include <QSettings>
#include <QStringListModel>
#include <QRegularExpression>
#include "supposefont.h"
#include "dictionarymst.h"
#include "variantextension.h"
#include "fvskeywordswindow.h"
#include "managementactions.h"
#include "modifypreferenceswindow.h"
#include "generalpurposescreenbuilder.h"

namespace Ui {
class MainWindow;
}

// forward declarations to enable pointers for window non-modality
class FVSKeywordsWindow;
class ManagementActions;
class VariantExtension;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    bool mapParmsMainSectionText();
    static QStringList readSectionFromMappedLoc(QIODevice &parms, qint64 location);
    static void readSectionToLists(QStringList *mainSectionText, QStringList *description);
    static void makeDictionaryFromSection(QMap<QString, QString> *dictionary, QStringList wordDefinitionsRaw, QRegularExpression separater =  QRegularExpression(": +{"), QRegularExpression definitionExcess = QRegularExpression("}"), QRegularExpression wordExcess = QRegularExpression("\\s"), bool oneToMany = false);
    static QMap<QString, QString> *variantExtensions, *extensionAbbreviationNames, *variantAbbreviationNames;
    static QMap<QString, QMap<QString, QString>> keyword_Exten_MainSecTitle;
    QMap<QString, qint64> parmMainSectionMap;
    QString preferencesFileName;
    static QString *variant;
    QSettings *preferences;
    bool variantLocked;
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

    void on_actionSelect_Variant_and_Extension_triggered();

private:
    Ui::MainWindow *ui;
    FVSKeywordsWindow *FVSAddKeywordsWindow;
    ManagementActions *ManagementActionsWindow;
    VariantExtension *selectVariantExtensionWindow;
};

#endif // MAINWINDOW_H
