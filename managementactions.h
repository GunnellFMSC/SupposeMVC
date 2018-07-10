#ifndef MANAGEMENTACTIONS_H
#define MANAGEMENTACTIONS_H

#include <QStringListModel>
#include "mainwindow.h"
#include <QDialog>
#include <QDebug>
#include <QFile>

namespace Ui {
class ManagementActions;
}

class ManagementActions : public QDialog
{
    Q_OBJECT

public:
    explicit ManagementActions(QStringList &managementCategory, QMap<QString, qint64> *parmMainSectionMap, QFile *parameters, QWidget *parent = 0);
    ~ManagementActions();

private slots:
    void on_ManagementTitles_listView_clicked(const QModelIndex &index);

    void on_Close_clicked();

    void on_ManagmentActions_listView_clicked(const QModelIndex &index);

private:
    QFile *parm;
    Ui::ManagementActions *ui;
    QMap<QString, qint64> *parmMap;
    QModelIndex *managementTitleIndex;
    QMap<QString, QStringList> *categoryTitlesAndActions;
    QStringListModel *managementTitles, *managementActions;
};

#endif // MANAGEMENTACTIONS_H
