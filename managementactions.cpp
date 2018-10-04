#include "managementactions.h"
#include "ui_managementactions.h"

ManagementActions::ManagementActions(QMap<QString, qint64> *parmMainSectionMap, QFile *parameters, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManagementActions)
{
    ui->setupUi(this);
    parm = parameters;
    parmMap = parmMainSectionMap;
    description = new QStringList;
    mainSectionText = new QStringList;
    setWindowTitle("Management Actions");
    managementTitleIndex = new QModelIndex();
    managementTitles = new QStringListModel(this);
    managementActions = new QStringListModel(this);
    categoryNumberTitle = new QMap<QString, QString>;
    categoryTitlesAndActions = new QMap<QString, QStringList>;

    setTitlesActions();

    // reverts Management Actions window to initial state
    connect(this, &QDialog::finished, [=]() {ui->ManagementTitles_listView->clearSelection(); managementActions->setStringList(QStringList());ui->ManagmentActions_listView->setModel(managementActions); ui->ManagmentActions_listView->update();});
}

ManagementActions::~ManagementActions()
{
    delete description;
    delete mainSectionText;
    delete managementTitles;
    delete managementActions;
    delete categoryNumberTitle;
    delete managementTitleIndex;
    delete categoryTitlesAndActions;
    delete ui;
}

void ManagementActions::setTitlesActions()
{
    QStringList managementCategory = MainWindow::readSectionFromMappedLoc(*parm, parmMap->value("mgmtCategories"));
    bool estb = MainWindow::variantExtensions->value("FVS" + *MainWindow::variant).contains("estb");
    bool strp = MainWindow::variantExtensions->value("FVS" + *MainWindow::variant).contains("strp");
    QStringList categoryActions;
    categoryNumberTitle->clear();
    QRegularExpression category("[c]\\d\\d?:");
    for (int i = 0; i < managementCategory.size()-1; ++i)
    {
        QString title, holder = managementCategory.at(i);
        if(holder.size() > 0)
            if(!(holder.at(0) == "!"))
                if(holder.contains(category))
                {
                    title = managementCategory.at(++i);
                    categoryNumberTitle->insert(holder.left(holder.indexOf(":")), title);
                    holder = managementCategory.at(++i);
                    while(holder.size() > 0 && !holder.contains(category))
                    {
                        if(holder != "}")       // catches and removes "}"
                        {
                            if(holder.contains("}")) holder.remove('}'); // catches and removes inline '}'
                            // appends only if holder does not contain estb or strp, or if it contains the one the variant has as well
                            if ((!holder.contains("estb") && !holder.contains("strp")) || (holder.contains("estb") && estb) || (holder.contains("strp") && strp))
                                categoryActions.append(holder);
                        }
                        holder = managementCategory.at(++i);
                    }
                    if(categoryActions.size() > 0)
                        categoryTitlesAndActions->insert(title, categoryActions);
                    categoryActions.clear();
                }
    }
    managementActions->setStringList(QStringList());
    managementTitles->setStringList(categoryTitlesAndActions->keys());
    ui->ManagementTitles_listView->setModel(managementTitles);
}

void ManagementActions::on_ManagementTitles_listView_clicked(const QModelIndex &index)
{
    qDebug() << "Management Title " << managementTitles->data(index).toString() << "(" + categoryNumberTitle->key(managementTitles->data(index).toString()) + ")" << " clicked.";
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
    QMap<QString, QMap<QString, QString>> *keyword_E_MST = &(MainWindow::keyword_Exten_MainSecTitle);
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
            QString keyword, extension = actionInfoSplit.at(0);
            qDebug() << "GPSB: General Purpose Screen Builder";
            qDebug() << "Management Title: " << actionInfoSplit.at(2);
            QString(actionInfoSplit.at(2)).contains("keyword")?(keyword = QString(actionInfoSplit.at(2)).right(QString(actionInfoSplit.at(2)).size() - 1 - QString(actionInfoSplit.at(2)).lastIndexOf("."))):(keyword = actionInfoSplit.at(2));
            description->clear();
            mainSectionText->clear();
            if(parmMap->value(keyword_E_MST->value(keyword).value(extension)) == 0)
            {
                if(parmMap->value(keyword) == 0)
                    qDebug() << "Keyword" << keyword << "not found!";
                else
                {
                    qDebug() << keyword <<  parmMap->value(keyword);
                    qDebug() << keyword << "found in parm";
                    *mainSectionText = MainWindow::readSectionFromMappedLoc(*parm, parmMap->value(keyword));
                }
            }
            else
            {
                qDebug() << actionInfoSplit.at(2) << "found in keyword_Exten_MainSecTitle";
                qDebug() << keyword <<  parmMap->value(keyword_E_MST->value(keyword).value(extension));
                *mainSectionText = MainWindow::readSectionFromMappedLoc(*parm, parmMap->value(keyword_E_MST->value(keyword).value(extension)));
            }
            MainWindow::readSectionToLists(mainSectionText, description);
//            qDebug() << keyword <<  parmMap->value(keyword_E_MST->value(keyword).value(extensionTemp));
            GeneralPurposeScreenBuilder *dynamWin;
//            if(QStringList(mainSectionText->filter("scheduleBox")).size() == 0)
                dynamWin = new GeneralPurposeScreenBuilder(QString(actionName), QStringList(*description),  QStringList(*mainSectionText), MainWindow::variant, 2018, this);
//            else
//            {
//                qDebug() << "Place Secondary General Purpose Screen Builder constructor containing vectors in description and mainSectionText to allow for condition window here.";
//                containerGPSB.append(new GeneralPurposeScreenBuilder(QString(actionName), QStringList(*description),  QStringList(*mainSectionText), MainWindow::variant, &MainWindow::mainSectionTextDictionary, 2018, this));
//            }
//            containerGPSB.last()->show();
//            containerGPSB.last()->activateWindow();
            dynamWin->exec();
            dynamWin->deleteLater();
        }
        else if(QString(actionInfoSplit.at(1)).contains("Win"))
        {
            managementTitle = QString(actionInfoSplit.at(1)).remove(QString(actionInfoSplit.at(1)).size()-3, 3);
//            qDebug() << "Attempting to pull data from mapped file: " << MainWindow::readSectionFromMappedLoc(*parm, qint64(parmMap->value(holder.at(1))));
            qDebug() << "Window: " << managementTitle;
            if(!(managementTitle.contains("Plant") || managementTitle.contains("Thin")))
            {
                if(parmMap->value("management." + managementTitle, -1) >= 0)
                {
                    qDebug() << "Management Title: " << managementTitle << "at file location:" << parmMap->value("management." + managementTitle, -1);
                }
                else if(parmMap->value("management." + actionName, -1) >= 0)
                {
                    qDebug() << "Management Title: " << actionName << "at file location:" << parmMap->value("management." + actionName, -1);
                }
            }
            else if(managementTitle.contains("Plant"))
            {
                description->clear();
                mainSectionText->clear();
                QString managementTitleMST = managementTitle;
                managementTitleMST.remove("Partial").remove("Full");
                *mainSectionText = MainWindow::readSectionFromMappedLoc(*parm, parmMap->value("management." + managementTitleMST));
                GeneralPurposeScreenBuilder *dynamWin = new GeneralPurposeScreenBuilder(managementTitle, managementTitles->data(index).toString(), QStringList(*mainSectionText), 2018, this);
                dynamWin->exec();
                dynamWin->deleteLater();
            }
        }
    }
}

void ManagementActions::on_Close_clicked()
{
    qDebug() << "Management Actions Close Button Clicked.";
    this->close();
}

void ManagementActions::on_ManagementTitles_listView_activated(const QModelIndex &index)
{
    qDebug() << "Management Title " << managementTitles->data(index).toString() << " activated.";
    ui->ManagementTitles_listView->clicked(index);
}
