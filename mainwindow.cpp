#include "mainwindow.h"
#include "ui_mainwindow.h"

QMap<QString, QMap<QString, QString>> MainWindow::keyword_Exten_MainSecTitle;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->button_RunSim->setEnabled(false);
//    PreferencesModel = new QStringListModel(this);
    preferencesFileName = "/example.prf";
    mapParmsMainSectionText();
}

MainWindow::~MainWindow()
{
    delete preferences;
    delete parameters;
    delete ui;
}
/********************* MainWindow::mapParmsMainSectionText() *********************
 *                                                                               *
 *        This function maps the Parameters file into main sections.             *
 *    This is done by getting the Parameters file name listed in preferences,    *
 *   then logging every "//start " text literal location in parmMainSectionMap.  *
 *                                                                               *
 *********************************************************************************/
bool MainWindow::mapParmsMainSectionText()
{
    bool mapSuccess;
    QString PreferencesFilePath = QDir::currentPath();
    preferences = new QSettings(PreferencesFilePath + preferencesFileName, QSettings::IniFormat);
    if (preferences->contains("General Preferences/defaultParametersFileName"))
    {
        qDebug() << "Locate defaultParametersFileName success!";
        qDebug() << preferences->value("General Preferences/defaultParametersFileName").toString();
        qDebug() << PreferencesFilePath + "/" + preferences->value("General Preferences/defaultParametersFileName").toString();
        parameters = new QFile(PreferencesFilePath + "/" + preferences->value("General Preferences/defaultParametersFileName").toString());
        qDebug() << "File name held in parameters variable:";
        qDebug() << parameters->fileName();
        if(parameters->exists())
        {// Specified Parameters File Successfully Found!
            QString keywordTemp;
            QMap<QString, QString> exten_MainSectionTitleTemp;
            parmMainSectionMap.clear();
            qDebug() << "\nmapParmsMainSections";
            parameters->open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream qinFile(parameters);
            int i = 0;
            int charCount = 0;
            while (!qinFile.atEnd())                               // <- reads batch to QStringList line if found
            {
                QString readLine = qinFile.readLine();
                charCount += readLine.length() + 2;
                if(readLine.contains("//start "))
                {
                    readLine.remove("//start ", Qt::CaseSensitive);
                    parmMainSectionMap.insert(readLine, charCount);
                    if(readLine.contains("keyword."))
                    {
                        int firstPeriod = readLine.indexOf("."), secondPeriod = readLine.lastIndexOf(".");
                        keywordTemp = readLine.right(readLine.size() - (secondPeriod+1));
                        if(!keyword_Exten_MainSecTitle.value(keywordTemp).isEmpty())//{qDebug() << ":D" << keyword_Exten_MainSecTitle.value(keywordTemp);
                            exten_MainSectionTitleTemp = keyword_Exten_MainSecTitle.value(keywordTemp);//}
                        qDebug() << readLine << " at position " << charCount << "\tKeyword:" << keywordTemp << "Extension Abbrevation:" << readLine.mid(firstPeriod+1, secondPeriod-(firstPeriod+1));
                        exten_MainSectionTitleTemp.insert(readLine.mid(firstPeriod+1, secondPeriod-(firstPeriod+1)), readLine);
                        keyword_Exten_MainSecTitle.insert(keywordTemp, exten_MainSectionTitleTemp);
                        //qDebug() << keyword_Exten_MainSecTitle.value(keywordTemp);
                        exten_MainSectionTitleTemp.clear();
                    }
                    else
                        qDebug() << readLine << " at position " << charCount;
                    i++;
                }
            }
            qDebug() << "(" << i << ") ParmReader::mapParmsMainSectionText: Parms read Successful!\n";
            parameters->close();
            mapSuccess = true;
        }
        else{
            qDebug() << "CRITICAL ERROR: Specified Parameters File not found! (MainWindow::mapParmsMainSectionText)";
            mapSuccess = false;
        }
    }
    else{
        qDebug() << "CRITICAL ERROR: defaultParametersFileName not found! MainWindow::mapParmsMainSectionText)";
        mapSuccess= false;
    }
    return mapSuccess;
}

void MainWindow::on_button_Exit_clicked()
{
    qDebug() << "Exit Button clicked.";
    this->close();
}

void MainWindow::on_button_RunSim_clicked()
{
    qDebug() << "Run Button clicked";
}

void MainWindow::on_actionSuppose_Preferences_triggered()
{
    qDebug() << "Suppose Preferences clicked";
    ModifyPreferencesWindow ModifyPreferences(*preferences, this);
    ModifyPreferences.exec();
      // ~~~REFRESH ALTERED ITEMS HERE!~~~ COMPLETED: PARM UPDATE
    QString PreferencesFilePath = QDir::currentPath();
    if (parameters->fileName() != PreferencesFilePath + "/" + preferences->value("General Preferences/defaultParametersFileName").toString())
    {
        qDebug() << "Refresh Parameters";
        qDebug() << "Before: " + parameters->fileName();
        parameters = new QFile(PreferencesFilePath + "/" + preferences->value("General Preferences/defaultParametersFileName").toString());
        qDebug() << "After: " + parameters->fileName();
        mapParmsMainSectionText();
        if (parameters->fileName() == PreferencesFilePath + "/" + preferences->value("General Preferences/defaultParametersFileName").toString())
            qDebug() << "Success!";
    }
    else
         qDebug() << "Parameters unchanged";
}

void MainWindow::on_button_SelectStands_clicked()
{
    qDebug() << "Select Stands Button clicked";
}

void MainWindow::on_button_SetTimeScale_clicked()
{
    qDebug() << "Set Time Scale Button clicked";
}
// CONSIDER MOVING THIS FUNCTION TO SEPARATE FILE
QStringList MainWindow::readSectionFromMappedLoc(QIODevice &parms, qint64 location)
{
    parms.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream qinFile(&parms);
    QString readLine;
    QStringList mainSectionBlock;
    parms.seek(location);
    readLine = qinFile.readLine();
    while(!qinFile.atEnd() && !readLine.contains("//end"))
    {
//       qDebug() << readLine;
       mainSectionBlock.append(readLine);
       readLine = qinFile.readLine();
    }
    parms.close();
    return mainSectionBlock;
}

// create a one-to-one(121) word(key) to definition(value) dictionary(QMap)
void MainWindow::make121DictionaryFromSection(QMap<QString, QString> *dictionary, QStringList wordDefinitionsRaw, QRegularExpression separater, QRegularExpression wordExcess, QRegularExpression definitionExcess)
{
    QStringList wordAndDefinitions;
    foreach (QString line, wordDefinitionsRaw) {
        if(line.contains(separater))
        {
            wordAndDefinitions = line.split(separater);
            QString word = QString(wordAndDefinitions.at(0)).remove(wordExcess);
            QString definition = QString(wordAndDefinitions.at(1)).remove(QRegularExpression(definitionExcess));
//            qDebug() << "Word: " << word << "\tDefinition" << definition;
            if(!QStringList(dictionary->keys()).contains(word))
                dictionary->insert(word, definition);
        }
    }
}

void MainWindow::on_button_SelectManagement_clicked()
{
    qDebug() << "Select Management Button clicked";
    QStringList managementCategory;
    managementCategory = MainWindow::readSectionFromMappedLoc(*parameters, parmMainSectionMap["mgmtCategories"]);
    ManagementActions ManagementActionsWindow(managementCategory, &parmMainSectionMap, parameters, this);
    ManagementActionsWindow.exec();
}

void MainWindow::on_button_SelectOutputs_clicked()
{
    qDebug() << "Select Outputs Button clicked";
}

void MainWindow::on_button_AddKeywords_clicked()
{
    qDebug() << "Add Keywords Button clicked";
    FVSKeywordsWindow FVSAddKeywordsWindow(&parmMainSectionMap, parameters, this);
    FVSAddKeywordsWindow.exec();
}

void MainWindow::on_button_InsertFromFile_clicked()
{
    qDebug() << "Insert From File Button clicked";
}

void MainWindow::on_button_SelectPostProcessors_clicked()
{
    qDebug() << "Select Post Processors Button clicked";
}

void MainWindow::on_button_SelectModifiers_clicked()
{
    qDebug() << "Select Modifiers Button clicked";
}
