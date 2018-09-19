#include "mainwindow.h"
#include "ui_mainwindow.h"

QMap<QString, QMap<QString, QString>> MainWindow::keyword_Exten_MainSecTitle;
QMap<QString, QMap<QString, QString>> MainWindow::mainSectionTextDictionary;
QMap<QString, QString>* MainWindow::extensionAbbreviationNames;
QMap<QString, QString>* MainWindow::variantAbbreviationNames;
QMap<QString, QString>* MainWindow::variantExtensions;
QString* MainWindow::variant;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    variantLocked = false;
    ui->button_RunSim->setEnabled(false);
//    PreferencesModel = new QStringListModel(this);
    preferencesFileName = "/example.prf";
    if(mapParmsMainSectionText())
    {
        int count = 0, habPa = 0, forests = 0;
        foreach (QString MainSectionText, parmMainSectionMap.keys())
        {
            if(MainSectionText.contains(QRegularExpression("species_\\w\\w$")))
            {// Species MST Abbreviation/Name
                QMap<QString, QString> *speciesAbbreviationName = new QMap<QString, QString>;
                speciesAbbreviationName->insert("All", "All species");
                qDebug() << ++count <<MainSectionText << "at position" << parmMainSectionMap.value(MainSectionText);
                makeDictionaryFromSection(speciesAbbreviationName, readSectionFromMappedLoc(*parameters, parmMainSectionMap.value(MainSectionText)), QRegularExpression(" ?{\\d+} ? ?:{"));
                mainSectionTextDictionary.insert(MainSectionText, *speciesAbbreviationName);
                qDebug() << mainSectionTextDictionary.value(MainSectionText).keys() <<(mainSectionTextDictionary.value(MainSectionText).keys().size() - 1);
            }
            else if(MainSectionText.contains(QRegularExpression("HabPa_\\w\\w$")))
            {// Habitat Type Plant Association MST Number/Abbreviation
                QMap<QString, QString> *habitatPlantNumberAbbreviation = new QMap<QString, QString>;
                qDebug() << ++habPa << MainSectionText << "at position" << parmMainSectionMap.value(MainSectionText);
                makeDictionaryFromSection(habitatPlantNumberAbbreviation, readSectionFromMappedLoc(*parameters, parmMainSectionMap.value(MainSectionText)), QRegularExpression("({blank})?:{"));
                mainSectionTextDictionary.insert(MainSectionText, *habitatPlantNumberAbbreviation);
                qDebug() << mainSectionTextDictionary.value(MainSectionText).keys() <<(mainSectionTextDictionary.value(MainSectionText).keys().size() - 1);
            }
            else if(MainSectionText.contains(QRegularExpression("Forests_\\w\\w$")))
            {// Forests MST Number/Name
                QMap<QString, QString> *forestNumberName = new QMap<QString, QString>;
                qDebug() << ++forests << MainSectionText << "at position" << parmMainSectionMap.value(MainSectionText);
                makeDictionaryFromSection(forestNumberName, readSectionFromMappedLoc(*parameters, parmMainSectionMap.value(MainSectionText)), QRegularExpression("({blank})?:{"));
                mainSectionTextDictionary.insert(MainSectionText, *forestNumberName);
                qDebug() << mainSectionTextDictionary.value(MainSectionText).keys() <<(mainSectionTextDictionary.value(MainSectionText).keys().size() - 1);
            }
            else if(MainSectionText.contains(QRegularExpression("conditions_list")))
            {
                QMap<QString, QString> *conditionsList = new QMap<QString, QString>;
                conditionsList->insert("1NULL1", ">>>>> No Selected Condition <<<<<");
                qDebug() << MainSectionText << "at position" << parmMainSectionMap.value(MainSectionText);
                makeDictionaryFromSection(conditionsList, readSectionFromMappedLoc(*parameters, parmMainSectionMap.value(MainSectionText)), QRegularExpression(":{"));
                mainSectionTextDictionary.insert(MainSectionText, *conditionsList);
                qDebug() << mainSectionTextDictionary.value(MainSectionText).keys() <<(mainSectionTextDictionary.value(MainSectionText).keys().size() - 1);
            }
        }
        count = 0;
        foreach (QString ext, extensionAbbreviationNames->keys())
            qDebug() << ++count << "Extension abbreviated: " << ext << "\tExtension Full Name: " << extensionAbbreviationNames->value(ext);
        count = 0;
        foreach (QString var, variantExtensions->keys())
            qDebug() << ++count << "Variant:" << var << "has the extensions" << variantExtensions->value(var);
        count = 0;
        foreach (QString var, variantAbbreviationNames->keys())
            qDebug() << ++count << "Variant abbreviated: " << var << "Variant Full Name: " << variantAbbreviationNames->value(var);
    }
    else
        qDebug() << "CRITICAL ERROR: Mapping Unsuccessful!";
    QStringList managementCategory = MainWindow::readSectionFromMappedLoc(*parameters, parmMainSectionMap["mgmtCategories"]);
    FVSAddKeywordsWindow = new FVSKeywordsWindow(&parmMainSectionMap, parameters, this);
    ManagementActionsWindow = new ManagementActions(managementCategory, &parmMainSectionMap, parameters, this);
    selectVariantExtensionWindow = new VariantExtension(variant, variantExtensions, variantAbbreviationNames, extensionAbbreviationNames, &variantLocked);
    selectVariantExtensionWindow->setWindowTitle("Select Variant and Extension");
//    connect(selectVariantExtensionWindow, SIGNAL(accepted()), this, SLOT(on_button_SelectModifiers_clicked()));
    connect(selectVariantExtensionWindow, &VariantExtension::variantChanged, [=](){FVSAddKeywordsWindow->setExtensionCategoryKeywordModels(); FVSAddKeywordsWindow->update();}); // potentially for every selection
    connect(selectVariantExtensionWindow, &QDialog::accepted, [=](){selectVariantExtensionWindow->startingVariant = *variant;}); // for window close only
}

MainWindow::~MainWindow()
{
    delete selectVariantExtensionWindow;
    delete extensionAbbreviationNames;
    delete variantAbbreviationNames;
    delete ManagementActionsWindow;
    delete FVSAddKeywordsWindow;
    delete variantExtensions;
    delete preferences;
    delete parameters;
    delete variant;
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
        variant = new QString(preferences->value("General Preferences/defaultVariant").toString());
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
                charCount += readLine.length() + 2;                // <- +2 due to \r\n? +1 for Unix?
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
            extensionAbbreviationNames = new QMap<QString, QString>;
            if(parmMainSectionMap.contains("extensions"))
                makeDictionaryFromSection(extensionAbbreviationNames, readSectionFromMappedLoc(*parameters, qint64(parmMainSectionMap.value("extensions"))));
            extensionAbbreviationNames->remove("bgc");
            variantExtensions = new QMap<QString, QString>;
            if(parmMainSectionMap.contains("programs"))
                makeDictionaryFromSection(variantExtensions, readSectionFromMappedLoc(*parameters, qint64(parmMainSectionMap.value("programs"))), QRegularExpression("\\s* {"), QRegularExpression("}\\s*:{}"), QRegularExpression(""), true);
            variantAbbreviationNames = new QMap<QString, QString>;
            if(parmMainSectionMap.contains("variants"))
                makeDictionaryFromSection(variantAbbreviationNames, readSectionFromMappedLoc(*parameters, qint64(parmMainSectionMap.value("variants"))), QRegularExpression(": +{"));
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
    qDebug() << "Main Window Exit Button clicked.";
    selectVariantExtensionWindow->close();
    ManagementActionsWindow->close();
    FVSAddKeywordsWindow->close();
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
        delete parameters;
        parameters = new QFile(PreferencesFilePath + "/" + preferences->value("General Preferences/defaultParametersFileName").toString());
        qDebug() << "After: " + parameters->fileName();
        mapParmsMainSectionText();
        if (parameters->fileName() == PreferencesFilePath + "/" + preferences->value("General Preferences/defaultParametersFileName").toString())
            qDebug() << "Successful Parameters Refresh!";
    }
    else
         qDebug() << "Parameters unchanged";
    if(*variant != preferences->value("General Preferences/defaultVariant").toString() && !variantLocked)
    {
        qDebug() << "Refresh Variant";
        qDebug() << "Before: " + *variant;
        *variant = preferences->value("General Preferences/defaultVariant").toString();
        qDebug() << "After: " + *variant;
        if (*variant == preferences->value("General Preferences/defaultVariant").toString())
            qDebug() << "Successful Variant Refresh!";
    }
    else if(*variant != preferences->value("General Preferences/defaultVariant").toString() && variantLocked)
        qDebug() << "Default Variant saved as" << preferences->value("General Preferences/defaultVariant").toString() << "but running Variant locked in as" << *variant;
    else
        qDebug() << "Variant unchanged";
}

void MainWindow::on_button_SelectStands_clicked()
{
    qDebug() << "Select Stands Button clicked";
}

void MainWindow::on_button_SetTimeScale_clicked()
{
    qDebug() << "Set Time Scale Button clicked";
}

void MainWindow::readSectionToLists(QStringList *mainSectionText, QStringList *description)
{
    int descriptionBegin = -1, descriptionEnd = -1;
    for (int i = 0; i < mainSectionText->size()-1; i++) // Locates and stores description
    {
        qDebug() << "Main Section Text line" << i << "holds the value" << QString(mainSectionText->at(i));
        if(QString(mainSectionText->at(i)).contains("description:"))
            descriptionBegin = i;
        else if(QString(mainSectionText->at(i)).contains(QRegularExpression("description{[\\w+\\s?]+}:")))
        {
            qDebug() << "Variant dependant description found.";
            QString line = mainSectionText->at(i);
            QStringList variantList = QString(line.mid(line.indexOf("{")+1, (line.indexOf("}")-(line.indexOf("{")+1)))).split(" ");
            (line.size() > (line.indexOf(":")+1)) ? qDebug() << "Variant(s):" << variantList << ", value:" << (line.mid(line.lastIndexOf("{")+1)).remove("}") : qDebug() << "Variant(s):" << variantList;
            if(variantList.contains(*variant))
                descriptionBegin = i++;
        }
        if(descriptionBegin >= 0 && descriptionEnd < 0)
            description->append(mainSectionText->at(i));
        if(QString(mainSectionText->at(i)).contains('}') && descriptionBegin >= 0 && descriptionEnd < 0)
            descriptionEnd = i;
        if(descriptionEnd > 0)
            break;
    }
    for (int i = descriptionEnd; i >= descriptionBegin; i--) // removes description
        mainSectionText->removeAt(i);
    mainSectionText->removeAll(QString("")); // removes spaces
    for (int i = 0; i < description->size(); ++i) {
        if(description->at(i) == "description:" || description->at(i) == "description:{\\") description->removeAt(i);
        else if(QString(description->at(i)).contains("description:")) description->replace(i, QString(description->at(i)).remove("description:"));
        if(QString(description->at(i)).contains("{")) description->replace(i, QString(description->at(i)).remove("{"));
        if(QString(description->at(i)).contains("}")) description->replace(i, QString(description->at(i)).remove("}"));
    }
    if(description->size() > 0)
        description->prepend("Description:");
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

// create a word(key) to definition(value) dictionary(QMap)
void MainWindow::makeDictionaryFromSection(QMap<QString, QString> *dictionary, QStringList wordDefinitionsRaw, QRegularExpression separater, QRegularExpression definitionExcess, QRegularExpression wordExcess, bool oneToMany)
{
    foreach (QString line, wordDefinitionsRaw) {
        if(line.contains(separater))
        {
            QStringList wordAndDefinitions = line.split(separater);
            QString word = QString(wordAndDefinitions.at(0)).remove(wordExcess);
            QString definition = QString(wordAndDefinitions.at(1)).remove(definitionExcess);
//            qDebug() << "Word: " << word << "\tDefinition" << definition;
            if(!QStringList(dictionary->keys()).contains(word) || oneToMany)
                dictionary->insertMulti(word, definition);
        }
    }
}

void MainWindow::on_button_SelectManagement_clicked()
{
    qDebug() << "Select Management Button clicked";
//    QStringList managementCategory;
//    managementCategory = MainWindow::readSectionFromMappedLoc(*parameters, parmMainSectionMap["mgmtCategories"]);
//    ManagementActions ManagementActionsWindow(managementCategory, &parmMainSectionMap, parameters, this);
//    ManagementActionsWindow.exec();
    ManagementActionsWindow->show();
    ManagementActionsWindow->activateWindow();
}

void MainWindow::on_button_SelectOutputs_clicked()
{
    qDebug() << "Select Outputs Button clicked";
}

void MainWindow::on_button_AddKeywords_clicked()
{
    qDebug() << "Add Keywords Button clicked";
    /*FVSKeywordsWindow FVSAddKeywordsWindow(&parmMainSectionMap, parameters, this);
    FVSAddKeywordsWindow.exec();*/ // for modal
    FVSAddKeywordsWindow->show(); // for modeless
    FVSAddKeywordsWindow->activateWindow();
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

void MainWindow::on_actionSelect_Variant_and_Extension_triggered()
{
    qDebug() << "Select Variant and Extension clicked";
    /*VariantExtension selectVariantExtensionWindow(variant, variantExtensions, variantAbbreviationNames, extensionAbbreviationNames, &variantLocked);
    selectVariantExtensionWindow.setWindowTitle("Select Variant and Extension");
    selectVariantExtensionWindow.exec();*/
    selectVariantExtensionWindow->show(); // modeless
    selectVariantExtensionWindow->activateWindow();
}
