#ifndef MANAGEMENTACTIONS_H
#define MANAGEMENTACTIONS_H

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
    explicit ManagementActions(QMap<QString, qint64> *parmMainSectionMap, QFile *parameters, QWidget *parent = 0);
    QVector<GeneralPurposeScreenBuilder *> containerGPSB;
    void setTitlesActions();
    ~ManagementActions();

private slots:
    void on_ManagementTitles_listView_clicked(const QModelIndex &index);

    void on_Close_clicked();

    void on_ManagmentActions_listView_clicked(const QModelIndex &index);

    void on_ManagementTitles_listView_activated(const QModelIndex &index);

private:
    QFile *parm = NULL;
    Ui::ManagementActions *ui;
    QMap<QString, qint64> *parmMap = NULL;
    QModelIndex *managementTitleIndex = NULL;
    QMap<QString, QString> *categoryNumberTitle = NULL;
    QStringList *mainSectionText = NULL, *description = NULL;
    QMap<QString, QStringList> *categoryTitlesAndActions = NULL;
    QStringListModel *managementTitles = NULL, *managementActions = NULL;
};

#endif // MANAGEMENTACTIONS_H
