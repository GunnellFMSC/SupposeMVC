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

//class GeneralPurposeScreenBuilder; // forward declaration to allow numerous modeless GPSB windows

class ManagementActions : public QDialog
{
    Q_OBJECT

public:
    explicit ManagementActions(QStringList &managementCategory, QMap<QString, qint64> *parmMainSectionMap, QFile *parameters, QWidget *parent = 0);
//    QVector<GeneralPurposeScreenBuilder *> containerGPSB;
    ~ManagementActions();

private slots:
    void on_ManagementTitles_listView_clicked(const QModelIndex &index);

    void on_Close_clicked();

    void on_ManagmentActions_listView_clicked(const QModelIndex &index);

    void on_ManagementTitles_listView_activated(const QModelIndex &index);

private:
    QFile *parm;
    Ui::ManagementActions *ui;
    QMap<QString, qint64> *parmMap;
    QModelIndex *managementTitleIndex;
    QStringList *mainSectionText, *description;
    QMap<QString, QStringList> *categoryTitlesAndActions;
    QStringListModel *managementTitles, *managementActions;
};

#endif // MANAGEMENTACTIONS_H
