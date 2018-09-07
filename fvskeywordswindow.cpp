#include "fvskeywordswindow.h"
#include "ui_fvskeywordswindow.h"

FVSKeywordsWindow::FVSKeywordsWindow(QMap<QString, qint64> *parmMainSectionMap, QFile *parameters, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FVSKeywordsWindow)
{
    qDebug() << "Inside FVSKeywordsWindow Constructor";
    ui->setupUi(this);
    parm = parameters;
    parmMap = parmMainSectionMap;
    setWindowTitle("Use FVS Keywords");
    varExten = MainWindow::variantExtensions;
    keyword_E_MST = &(MainWindow::keyword_Exten_MainSecTitle);
    extenAbbrevName = MainWindow::extensionAbbreviationNames;
    extensions = new QStringList();
    keywordsModel = new QStringListModel(this);
    extensionsModel = new QStringListModel(this);
    categoriesModel = new QStringListModel(this);
    /*     Add base if defined     */
    if(extenAbbrevName->keys().contains("base")) extensions->append(extenAbbrevName->value("base"));
    /*     loop through the variant's extensions, ignore variants, add the full extension name to QStringList extensions    */
    foreach (QString extenAbbrev, (varExten->value("FVS" + *MainWindow::variant)).split(" "))
        if(!MainWindow::variantAbbreviationNames->contains(extenAbbrev))
        {
            qDebug() << "The Extension abbreviation" << extenAbbrev << "has the full name" << extenAbbrevName->value(extenAbbrev);
            extensions->append(extenAbbrevName->value(extenAbbrev)); // add the full extension name to QStringList
        }
    extensions->sort(Qt::CaseInsensitive);
    extensionsModel->setStringList(*extensions);
//    extensionsModel->sort(0);
    categoryAbbreviationNames = new QMap<QString, QString>;
    categoryAbbreviationNames->insert("all", "All keywords");
    MainWindow::makeDictionaryFromSection(categoryAbbreviationNames, MainWindow::readSectionFromMappedLoc(*parameters, qint64(parmMap->value("keyword_categories"))));
    extensionCategories = new QMap<QString, QStringList>;
    categoryKeywords = new QMap<QString, QString/*List*/>;
    keywordDictionary = new QMap<QString, QMap<QString, QString>>;
    keywordExtension = new QMap<QString, QString>;
    QStringList keywordListRaw = MainWindow::readSectionFromMappedLoc(*parameters, qint64(parmMap->value("keyword_list")));
    QStringList keywordExtensionCategories, extenCatetemp;
    bool midKeywordDefinition = false;
    bool strpToDo = false;
    QMap<QString, QString> extenDefTemp;
    QString keywordTemp, defTemp, extenTemp, extenTempII;
    qDebug() << "Raw Keyword List: " << keywordListRaw.size();
    QRegularExpression definition("{.+}: *");
    QRegularExpression brackets("{?.+}?:?");
    QRegularExpression bracket1("{");
    QRegularExpression bracket2("}:?");
    foreach (QString line, keywordListRaw) {
        if(line.size() > 0)
            if(line.at(0) != "!"){
                if(line.contains(definition))
                {
                    qDebug() << "Line Raw: " << line;
                    if(!line.left(10).contains(' ')){
                        qDebug() << "Parm error found (missing space)";
                        line.insert(line.indexOf("{"), ' ');
                        qDebug() << "Corrected Line Raw: " << line;
                    }
                    if(line.contains("}:{"))
                    {
                        qDebug() << "Parm error found (missing new line)";
                        keywordTemp = line.left(8);
                        keywordTemp.remove(" ");
                        qDebug() << keywordTemp;
                        int dictionaryBracket = line.lastIndexOf('{');
                        defTemp = line.right(line.size() - dictionaryBracket);
                        line.remove(dictionaryBracket, line.size() - dictionaryBracket);
                        qDebug() << defTemp;
                        if(defTemp.contains(bracket1)) defTemp.remove(bracket1);
                        if(defTemp.contains(bracket2)){defTemp.remove(bracket2);
                            if(!keywordDictionary->value(keywordTemp).isEmpty()) // keyword already entered, alter existing QMap
                            {
//                                qDebug() << line.mid(line.indexOf(bracket1) + 1, (line.indexOf(bracket2) - (line.indexOf("{") +1)));
                                extenDefTemp = keywordDictionary->value(keywordTemp);
                            }
                            qDebug() << line.mid(line.indexOf(bracket1) + 1, (line.indexOf(bracket2) - (line.indexOf("{") +1)));
                            // Get the name of the Extension by finding the first instance of both brackets and extracting the string inside, place that string in a QMap with definition
                            extenDefTemp.insert(line.mid(line.indexOf(bracket1) + 1, (line.indexOf(bracket2) - (line.indexOf(bracket1) +1))), defTemp.mid(0));
                            keywordDictionary->insert(keywordTemp, extenDefTemp);
                            qDebug() << keywordDictionary->value(keywordTemp);
                            extenDefTemp.clear();
                        }
                        else midKeywordDefinition = true;
                        qDebug() << defTemp;
                    }
                    if(line.contains(bracket1) && line.contains(bracket2))
                    {
                        line.remove(bracket1);
                        line.remove(bracket2);
                    }
                    keywordExtensionCategories = line.split(" ", QString::SplitBehavior::SkipEmptyParts);
                    qDebug() << "keywordExtensionCategories Raw: " << keywordExtensionCategories;
                    foreach (QString potentialRemovedExtension, keywordExtensionCategories) {
                        if(extenAbbrevName->keys().contains("!" + potentialRemovedExtension))
                            keywordExtensionCategories.removeOne(potentialRemovedExtension);
                    }
                    qDebug() << "keywordExtensionCategories Reduced: " << keywordExtensionCategories;
                    qDebug() << "Keyword: " << keywordExtensionCategories.at(0);
                    keywordTemp = keywordExtensionCategories.at(0);
                    qDebug() << "Extension: " << keywordExtensionCategories.at(1);
                    extenTemp = keywordExtensionCategories.at(1);
                    if(keywordExtensionCategories.size() > 2)
                        if(keywordExtensionCategories.at(2) == "strp")
                        {
                            qDebug() << "strp located";
                            strpToDo = true;
                            keywordExtensionCategories.removeFirst();
                        }
                    do{
                        keywordExtension->insertMulti(keywordTemp, extenTemp);
                        extenCatetemp.append(categoryAbbreviationNames->value("all"));
                        for(int i = 2; i < keywordExtensionCategories.size(); i++)
                            extenCatetemp.append(categoryAbbreviationNames->value(keywordExtensionCategories.at(i)));
                        foreach (QString category, extenCatetemp) {
                            // the extension name is prepended to the category in categoryKeywords to insure respective behavior
                            categoryKeywords->insertMulti(extenTemp + category, keywordTemp);
                        }
                        QStringList ectempTemp = extensionCategories->value(extenTemp);
                        qDebug() << ectempTemp;
                        if(ectempTemp.size() > 0)
                            foreach (QString holder, ectempTemp)
                                if (!extenCatetemp.contains(holder)) extenCatetemp.append(holder);
                        extensionCategories->insert(extenTemp, extenCatetemp);
                        extenCatetemp.clear();
                        if(strpToDo)
                            (extenTemp == "strp") ? (strpToDo = false) : ((extenTemp = "strp").contains(keywordExtensionCategories.at(1)));
                    }while(strpToDo == true);
                    if(keywordExtensionCategories.at(0) != keywordTemp)
                        extenTempII = keywordExtensionCategories.at(0);
                    keywordExtensionCategories.clear();
                }
                else if (line.at(0) == "{")
                {
                    line.remove(bracket1);
                    qDebug() << "Definition: " << line;
                    if(line.contains("}")){ /*keywordDefinition->insert(keywordTemp, line.remove("}"));*/
                        if(!keywordDictionary->value(keywordTemp).isEmpty()){ // keyword already entered, alter existing QMap
                            qDebug() << "Inside second dictionary assignment conditional";
                            extenDefTemp = keywordDictionary->value(keywordTemp);}
                        qDebug() << extenDefTemp;//
                        if(extenTempII.size() > 0)
                        {
                            extenDefTemp.insert(extenTempII, line.remove(bracket2));
                            extenTempII.clear();
                        }
                        extenDefTemp.insert(extenTemp, line.remove(bracket2));
                        keywordDictionary->insert(keywordTemp, extenDefTemp);
                        qDebug() << keywordDictionary->value(keywordTemp);//
                        extenDefTemp.clear();
                    }
                    else {
                        defTemp = line;
                        midKeywordDefinition = true;
                    }
                }
                else if(midKeywordDefinition == true)
                {
                    if(line.contains(bracket2))
                    {
//                        keywordDefinition->insert(keywordTemp, defTemp.append(line.remove(bracket2)));
                        {
                            if(!keywordDictionary->value(keywordTemp).isEmpty()) // keyword already entered, alter existing QMap
                                extenDefTemp = keywordDictionary->value(keywordTemp);
                            qDebug() << extenDefTemp;
                            extenDefTemp.insert(extenTemp, defTemp.append(" " +line.remove(bracket2)));
                            if(extenTempII.size() > 0)
                            {
                                extenDefTemp.insert(extenTempII, defTemp);
                                extenTempII.clear();
                            }
                            keywordDictionary->insert(keywordTemp, extenDefTemp);
                            qDebug() << keywordDictionary->value(keywordTemp);
                            extenDefTemp.clear();
                        }
                        midKeywordDefinition = false;
                        qDebug() << "Definition of: " << keywordTemp << "is: " << keywordDictionary->value(keywordTemp).value(extenTemp);
                    }
                    else {defTemp.append(" " + line); qDebug() << "Definition part appended";}
                }
            }
    }
    extensionCategoryKeywords = new QStringList;
    ui->extension_listView->setModel(extensionsModel);
    ui->extension_listView->setCurrentIndex(extensionsModel->index(0));
    ui->extension_listView->clicked(extensionsModel->index(0));
}

FVSKeywordsWindow::~FVSKeywordsWindow()
{
    delete extensionCategoryKeywords;
    delete categoryAbbreviationNames;
    delete extensionCategories;
    delete keywordDictionary;
    delete extensionsModel;
    delete categoriesModel;
    delete keywordsModel;
    delete extensions;
//    delete categories;
//    delete keywords;
    delete ui;
}

void FVSKeywordsWindow::on_extension_listView_clicked(const QModelIndex &index)
{
    qDebug() << "FVSKeywords Window Extension " << extensionsModel->data(index).toString() << " clicked.";
    ui->extension_listView->setCurrentIndex(index);
    categoriesModel->setStringList(extensionCategories->value(extenAbbrevName->key(extensionsModel->data(index).toString())));
    categoriesModel->sort(0);
    ui->category_listView->setModel(categoriesModel);
    ui->category_listView->setCurrentIndex(categoriesModel->index(0));
    ui->category_listView->clicked(categoriesModel->index(0));
    ui->keywordDescription_label->setText("No keyword picked");
    ui->selectKeyword_lineEdit->setText("");
}

void FVSKeywordsWindow::on_category_listView_clicked(const QModelIndex &index)
{
    qDebug() << "FVSKeywords Window Category " << categoriesModel->data(index).toString() << " clicked.";
    delete extensionCategoryKeywords;
    extensionCategoryKeywords = new QStringList;
    foreach (QString keyword, categoryKeywords->values(extenAbbrevName->key(extensionsModel->data(ui->extension_listView->currentIndex()).toString()) + categoriesModel->data(index).toString())) {
        if (keywordExtension->values(keyword).contains(extenAbbrevName->key(extensionsModel->data(ui->extension_listView->currentIndex()).toString()))){
            qDebug() << keyword << "found in " << categoriesModel->data(index).toString() << "of" << extensionsModel->data(ui->extension_listView->currentIndex()).toString();
            if (!extensionCategoryKeywords->contains(keyword)) extensionCategoryKeywords->prepend(keyword);
        }
    }
    extensionCategoryKeywords->sort(Qt::CaseInsensitive);
    keywordsModel->setStringList(*extensionCategoryKeywords);
    ui->keyword_listView->setModel(keywordsModel);
}

void FVSKeywordsWindow::on_keyword_listView_clicked(const QModelIndex &index)
{
    qDebug() << "FVSKeywords Window Keyword" << keywordsModel->data(index).toString() <<  "Clicked.";
    QString selectedExtension = extenAbbrevName->key(extensionsModel->data(ui->extension_listView->currentIndex()).toString());
//    qDebug() << "Extension abbreviation: " << selectedExtension;
    QString selectedKeyword = keywordsModel->data(index).toString();
//    qDebug() << "Keyword" << selectedKeyword << "has the associated extensions: " << keywordDictionary->value(selectedKeyword).keys() << "\nand the corresponding definition is:" << keywordDictionary->value(selectedKeyword).value(selectedExtension)/*value(selectedExtension)*/;
    //                                    keywordDictionary->value(selected keyword).value(selected extension); // gets the full extension name from the index and returns the abbreviation from the name using the map
//    ui->keywordDescription_label->setText(keywordDictionary->value(keywordsModel->data(index).toString()).value(selectedExtension));
    ui->keywordDescription_label->setText(keywordDictionary->value(selectedKeyword).value(selectedExtension));
    ui->selectKeyword_lineEdit->setText(keywordsModel->data(index).toString());
}

void FVSKeywordsWindow::on_keyword_listView_doubleClicked(const QModelIndex &index)
{
    QString keyword = keywordsModel->data(index).toString();
    qDebug() << "FVSKeywords Window Keyword" << keyword <<  "Double-Clicked.";
    ui->selectKeyword_lineEdit->returnPressed();

    QString extensionName = extensionsModel->data(ui->extension_listView->currentIndex()).toString();
    qDebug() << "Extension: " << extensionName;
    //↓ finds Main Sectin Text using keyword_E_MST QMap
    QString extensionTemp = extenAbbrevName->key(extensionsModel->data(ui->extension_listView->currentIndex()).toString());
    qDebug() << extensionTemp;
    if(extensionTemp == "estb" || extensionTemp ==  "strp")
        if (keyword_E_MST->value(keyword).value(extensionTemp).isEmpty())
            extensionTemp = "estbstrp";
    qDebug() << "Main Section Title:" << keyword_E_MST->value(keyword).value(extensionTemp);

    //↓ finds Main Sectin Text by checking for keyword in every single Main Section text
    QStringList mainSections = parmMap->keys();
//    qDebug() << mainSections.at(mainSections.indexOf(QRegularExpression(keywordExtension->value(keywordsModel->data(index).toString())+"."+keywordsModel->data(index).toString())));
    foreach(QString mainSection, mainSections)
    {
        if(mainSection.contains("."+ keyword))
            if(mainSection.contains(extenAbbrevName->key(extensionsModel->data(ui->extension_listView->currentIndex()).toString())))
                qDebug() << "Main Section Title:" << mainSection;
    }
    description = new QStringList();
    mainSectionText = new QStringList();
    qDebug() << keyword <<  parmMap->value(keyword_E_MST->value(keyword).value(extensionTemp));
    *mainSectionText = MainWindow::readSectionFromMappedLoc(*parm, parmMap->value(keyword_E_MST->value(keyword).value(extensionTemp)));
    MainWindow::readSectionToLists(mainSectionText, description);
    GeneralPurposeScreenBuilder dynamWin(QString(extensionName + ": " + keyword), QStringList(*description),  QStringList(*mainSectionText), MainWindow::variant, &MainWindow::speciesMSTAbbreviationName, 2018, this);
    dynamWin.exec();
}

void FVSKeywordsWindow::on_selectKeyword_lineEdit_returnPressed()
{
    qDebug() << "FVSKeywords Window select keyword line edit entered.";
    QString selectedExtension = extenAbbrevName->key(extensionsModel->data(ui->extension_listView->currentIndex()).toString());
    QString selectedKeyword = keywordsModel->data(ui->keyword_listView->currentIndex()).toString();
    qDebug() << "Extension: " + selectedExtension + ", Keyword: " + selectedKeyword + ", Keyword textbox contains: " + ui->selectKeyword_lineEdit->text();
    (extensionCategoryKeywords->contains(ui->selectKeyword_lineEdit->text()))?( qDebug() << ui->selectKeyword_lineEdit->text() << "Found"):( qDebug() << ui->selectKeyword_lineEdit->text() << "not found");
}

void FVSKeywordsWindow::on_pushButton_Accept_clicked()
{
    qDebug() << "FVSKeywords Window Accept Button Clicked.";
    ui->selectKeyword_lineEdit->returnPressed();
}

void FVSKeywordsWindow::on_pushButton_Reset_clicked()
{
    qDebug() << "FVSKeywords Window Reset Button Clicked.";
}

void FVSKeywordsWindow::on_pushButton_Close_clicked()
{
    qDebug() << "FVSKeywords Window Close Button Clicked.";
    this->close();
}

void FVSKeywordsWindow::on_extension_listView_activated(const QModelIndex &index)
{
    ui->extension_listView->setCurrentIndex(index);
    ui->extension_listView->clicked(index);
}

void FVSKeywordsWindow::on_category_listView_activated(const QModelIndex &index)
{
    ui->category_listView->setCurrentIndex(index);
    ui->category_listView->clicked(index);
}

void FVSKeywordsWindow::on_keyword_listView_activated(const QModelIndex &index)
{
    ui->keyword_listView->clicked(index);
//    ui->keyword_listView->doubleClicked(index);
}
