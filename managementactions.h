#pragma once

#include "generalpurposescreenbuilder.h"
#include <QStringListModel>
#include "mainwindow.h"
#include <QDialog>
#include <QDebug>
#include <QFile>

namespace Ui {
class ManagementActions;
}

class GeneralPurposeScreenBuilder; // forward declaration to allow numerous modeless GPSB windows

class ManagementActions : public QDialog
{
    Q_OBJECT

public:
    explicit ManagementActions(QMap<QString, qint64> *parmMainSectionMap, QFile *parameters, QWidget *parent = nullptr);
    QVector<GeneralPurposeScreenBuilder *> containerGPSB;
    void setTitlesActions();
    ~ManagementActions();

private slots:
    void on_ManagementTitles_listView_clicked(const QModelIndex &index);

    void on_Close_clicked();

    void on_ManagmentActions_listView_clicked(const QModelIndex &index);

    void on_ManagementTitles_listView_activated(const QModelIndex &index);

private:
    QFile *parm = nullptr;
    Ui::ManagementActions *ui;
    QMap<QString, qint64> *parmMap = nullptr;
    QModelIndex *managementTitleIndex = nullptr;
    QMap<QString, QString> *categoryNumberTitle = nullptr;
    QStringList *mainSectionText = nullptr, *description = nullptr;
    QMap<QString, QStringList> *categoryTitlesAndActions = nullptr;
    QStringListModel *managementTitles = nullptr, *managementActions = nullptr;
};
