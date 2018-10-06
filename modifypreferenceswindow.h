#ifndef MODIFYPREFERENCESWINDOW_H
#define MODIFYPREFERENCESWINDOW_H

#include <QDir>
#include <QDebug>
#include <QDialog>
#include <QSettings>
#include <QKeyEvent>
#include "mainwindow.h"
#include <QStringListModel>

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
    QSettings *defaultPreferencesSettings = NULL;
    QStringList *defaultPreferencesList = nullptr;
    QStringListModel *defaultPreferencesModel = NULL;

    void displayFileContents();
    void updateFileContents();
    void keyPressEvent(QKeyEvent *event);
};

#endif // MODIFYPREFERENCESWINDOW_H
