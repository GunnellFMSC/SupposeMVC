#pragma once

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
    explicit ModifyPreferencesWindow(QWidget *parent = nullptr);
    explicit ModifyPreferencesWindow(QSettings &defaultPreferenceSettings, QWidget *parent = nullptr);

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
    QSettings *defaultPreferencesSettings = nullptr;
    QStringList *defaultPreferencesList = nullptr;
    QStringListModel *defaultPreferencesModel = nullptr;

    void displayFileContents();
    void updateFileContents();
    void keyPressEvent(QKeyEvent *event);
};
