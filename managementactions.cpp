#include "managementactions.h"
#include "ui_managementactions.h"

ManagementActions::ManagementActions(QStringList &managementCategory, QMap<QString, qint64> *parmMainSectionMap, QFile *parameters, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManagementActions)
{
    ui->setupUi(this);
    parm = parameters;
    parmMap = parmMainSectionMap;
    setWindowTitle("Management Actions");
    categoryTitlesAndActions = new QMap<QString, QStringList>;
    QStringList categoryActions;
    QRegularExpression category("[c]\\d\\d?:");
    for (int i = 0; i < managementCategory.size()-1; ++i) {
        QString title, holder = managementCategory.at(i);
        if(holder.size() > 0)
            if(!(holder.at(0) == "!"))
                if(holder.contains(category))
                {
                    title = managementCategory.at(++i);
                    holder = managementCategory.at(++i);
                    while(holder.size() > 0 && !holder.contains(category))
                    {
                        if(holder != "}")       // catches and removes "}"
                        {
                            if(holder.contains("}")) holder.remove('}'); // catches and removes inline '}'
                            categoryActions.append(holder);
                        }
                        holder = managementCategory.at(++i);
                    }
                    categoryTitlesAndActions->insert(title, categoryActions);
                    categoryActions.clear();
                }
    }
    managementTitleIndex = new QModelIndex();
    managementTitles = new QStringListModel(this);
    managementActions = new QStringListModel(this);
    managementTitles->setStringList(categoryTitlesAndActions->keys());
    ui->ManagementTitles_listView->setModel(managementTitles);
}

ManagementActions::~ManagementActions()
{
    delete managementTitles;
    delete managementActions;
    delete managementTitleIndex;
    delete categoryTitlesAndActions;
    delete ui;
}

void ManagementActions::on_ManagementTitles_listView_clicked(const QModelIndex &index)
{
    qDebug() << "Management Title " << managementTitles->data(index).toString() << " clicked.";
    *managementTitleIndex = index;
    //   Retrives title QString from index, sets value of managementActions to QStringList from mapped title
    QStringList DisplayActions;
    foreach (QString action, categoryTitlesAndActions->value(managementTitles->data(index).toString())) {
        int indexSemicolon = action.indexOf(';');
        if (indexSemicolon > -1)
        {
            action.remove(indexSemicolon, action.size()-1);
            DisplayActions.append(action);
        }
    }
    managementActions->setStringList(DisplayActions);
    ui->ManagmentActions_listView->setModel(managementActions);
}

void ManagementActions::on_ManagmentActions_listView_clicked(const QModelIndex &index)
{
    QString actionName = categoryTitlesAndActions->value(managementTitles->data(*managementTitleIndex).toString()).at(index.row());
    QString actionInfo = actionName;
    int indexSemicolon = actionName.indexOf(';');
    if (indexSemicolon == -1)
    {
        qDebug() << "No semicolon in corresponding mapped line.";
        int semicolonIndex = -1;
        int modelIndex = 0;
        while (semicolonIndex != index.row())                // iterates through QStringList associated with Management Title, counts rows with semicolons
        {
            actionName = categoryTitlesAndActions->value(managementTitles->data(*managementTitleIndex).toString()).at(modelIndex);
            indexSemicolon = actionName.indexOf(';');
            if (indexSemicolon > 0) semicolonIndex++;        // counts rows with semicolons
            modelIndex++;
        }
        actionInfo = actionName;
    }
    actionName.remove(indexSemicolon, actionName.size()-1);
    actionInfo.remove(0, indexSemicolon + 2);
    qDebug() << "Management Action " << actionName << " clicked, contains " << actionInfo;

        //Begin Dynamic Window (Adapt to Action Information
//    if(actionInfo.contains("keyword.base."))
//        actionInfo.in
    QStringList actionInfoSplit;
    QString managementTitle;
    if(actionInfo.contains(' ')){
        actionInfoSplit = actionInfo.split(' ');
        qDebug() << "Holder contains: " << actionInfoSplit;
        if(actionInfoSplit.at(1) == "GPSB")
        {
            qDebug() << "GPSB: General Purpose Screen Builder";
            qDebug() << "Management Title: " << actionInfoSplit.at(2);
        }
        else if(QString(actionInfoSplit.at(1)).contains("Win"))
        {
            managementTitle = QString(actionInfoSplit.at(1)).remove(QString(actionInfoSplit.at(1)).size()-3, 3);
//            qDebug() << "Attempting to pull data from mapped file: " << MainWindow::readSectionFromMappedLoc(*parm, qint64(parmMap->value(holder.at(1))));
            qDebug() << "Window: " << managementTitle;
            if(!(managementTitle.contains("Plant") || managementTitle.contains("Thin")))
            {
                qDebug() << "Management Title: " << managementTitle;
            }
        }
    }
}

void ManagementActions::on_Close_clicked()
{
    qDebug() << "Management Actions Close Button Clicked.";
    this->close();
}
