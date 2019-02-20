#pragma once

#include "generalpurposescreenbuilder.h"
#include <QStringListModel>
#include "mainwindow.h"
#include <QDialog>
#include <QDebug>
#include <QFile>

namespace Ui {
class FVSKeywordsWindow;
}

class FVSKeywordsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FVSKeywordsWindow(QMap<QString, qint64> *parmMainSectionMap, QFile *parameters, QWidget *parent = nullptr);
    void setExtensionCategoryKeywordModels();
    ~FVSKeywordsWindow();

private slots:
    void on_keyword_listView_doubleClicked(const QModelIndex &index);

    void on_extension_listView_clicked(const QModelIndex &index);

    void on_category_listView_clicked(const QModelIndex &index);

    void on_keyword_listView_clicked(const QModelIndex &index);

    void on_selectKeyword_lineEdit_returnPressed();

    void on_pushButton_Accept_clicked();

    void on_pushButton_Reset_clicked();

    void on_pushButton_Close_clicked();

    void on_extension_listView_activated(const QModelIndex &index);

    void on_category_listView_activated(const QModelIndex &index);

    void on_keyword_listView_activated(const QModelIndex &index);

private:
    QFile *parm;
    Ui::FVSKeywordsWindow *ui;
    QMap<QString, qint64> *parmMap;
    QString previousKeywordSelection;
    QMap<QString, QStringList> *extensionCategories;
    QStringListModel *extensionsModel, *categoriesModel, *keywordsModel;
    QMap<QString, QMap<QString, QString>> *keywordDictionary, *keyword_E_MST;
    QStringList *extensions, *categories, *extensionCategoryKeywords, *description, *mainSectionText;
    QMap<QString, QString> *extenAbbrevName, *categoryAbbreviationNames, *categoryKeywords, *keywordExtension, *varExten;
};
