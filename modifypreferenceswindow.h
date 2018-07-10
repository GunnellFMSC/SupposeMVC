#ifndef MODIFYPREFERENCESWINDOW_H
#define MODIFYPREFERENCESWINDOW_H

#include <QDialog>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QKeyEvent>
#include <QStringListModel>
#include "mainwindow.h"

namespace Ui {
class ModifyPreferencesWindow;
}

class ModifyPreferencesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyPreferencesWindow(QWidget *parent = 0);
    explicit ModifyPreferencesWindow(QSettings &defaultPreferenceSettings, QWidget *parent = 0);

    ~ModifyPreferencesWindow();

private slots:
    void on_button_defaultToWorkingDirectory_clicked();

    void on_button_apply_clicked();

    void on_button_close_clicked();

    void on_button_defaultUsePPE_clicked(bool checked);

    void on_button_defaultSegmentCycle_clicked(bool checked);

    void on_button_defaultProcessPlots_clicked(bool checked);

private:
    Ui::ModifyPreferencesWindow *ui;
    QStringListModel *defaultPreferencesModel;
    QSettings *defaultPreferencesSettings;
    QStringList *defaultPreferencesList = nullptr;

    void displayFileContents();
    void updateFileContents();
    void keyPressEvent(QKeyEvent *event);
//    enum EditLine {defaultDataBaseName,
//                   defaultEditor,
//                   defaultLocationsFileName,
//                   defaultParametersFileName,
//                   defaultProcessPlots,
//                   defaultSegmentCycle,
//                   defaultUsePPE,
//                   defaultVariant,
//                   defaultWorkingDirectory};
};

#endif // MODIFYPREFERENCESWINDOW_H
