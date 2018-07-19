#include <QtWidgets>

#include "generalpurposescreenbuilder.h"

GeneralPurposeScreenBuilder::GeneralPurposeScreenBuilder(QString keywordExtension, QStringList description, QStringList MSText, QString *variant, QMap<QString, QMap<QString, QString>> *speciesMSTAbbreviationName, QMap<QString, QString> *variantAbbreviationNames, int startYear, QWidget *parent) : QDialog(parent)
{
    qDebug() << "Inside GeneralPurposeScreenBuilder default constructor";
    qDebug() << "Selected variant: " + *variant;
    this->setObjectName("Dynamic Window");
    year = new int;
    *year = startYear;
    qDebug() << "Start year: " << *year;
    variantFVS = variant;
    speciesMSTAN = speciesMSTAbbreviationName;
    createButtonBox();
    currentField = new QString;
    QWidget *extensionKeyword = new QWidget;
    QLabel *name = new QLabel("Name:\t");
    font = new QFont("MS Shell Dlg 2", 10);
    font->setBold(true);
    name->setFont(*font);
    title = new QLineEdit();
    title->setFont(*font);
    title->setText(keywordExtension);
    title->setObjectName("instanceTitle");
    dynamLineEdits.append(title);
    defaultLineValue.append(keywordExtension);
    this->setFont(*font);
    QListView *keyDesc = new QListView();
    QStringListModel *descModel = new QStringListModel(description);
    keyDesc->setModel(descModel);
    keyDesc->setFont(*font);
    keyDesc->setMinimumWidth(keyDesc->sizeHintForColumn(0) + 24);
    keyDesc->setEditTriggers(QAbstractItemView::NoEditTriggers);
    keyDesc->setStyleSheet("border-style: none");
    keyDesc->setFocusPolicy(Qt::NoFocus);
    keyDesc->setSelectionMode(QAbstractItemView::NoSelection);


    QWidget *dynamBodyHolder = new QWidget;
    QFormLayout *dynamBody = new QFormLayout;
    keyDesc->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
//    keyDesc->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    keyDesc->setSizeAdjustPolicy(QListView::AdjustToContents);
    keyDesc->setMinimumHeight(descModel->rowCount()*keyDesc->sizeHintForRow(0)); // <-- get product of rows and rows size, use for ListView size
    bool inField = false/*, inFieldValue = false, inFieldTitle = false*/, fieldAdded = false;
    QRegularExpression field("f([0-9]+): ?({.*})?");
    QRegularExpression fieldVar("f([0-9]+){[\\w+\\s?]+}: ?({.*})?");/*7*/
    QRegularExpression fieldValue("f(\\d)+v:");
    QRegularExpression fieldValueVar("f(\\d)+v{[\\w+\\s?]+}:");
    QRegularExpression fieldTitle("f(\\d)+title:");
    QStringList comboBoxProperties, longTitleTemp;
    QString fieldDescription, fieldType, fieldNum;
    QLineEdit *tempLineEdit;
    QLabel *tempLabel;
    foreach (QString line, MSText) {
        qDebug() << ":P" << line;
        if(line.contains(field) || inField || line.contains(fieldVar))
        {
            bool valid = true;/*7*/
            QStringList variantList; // <- consider placing above during revision, and variantList.clear(); here
            if(line.contains(field))
            {
                qDebug() << "Field located: " << line.mid(line.indexOf("{") + 1);
                fieldNum = line.left(line.indexOf(":"));
                qDebug() << "Field Num:" << fieldNum;
            }
            else if(line.contains(fieldVar))
            {
                qDebug() << "Variant Field located: " << line.mid(line.lastIndexOf("{") + 1);
                fieldNum = line.left(line.indexOf("{"));
                qDebug() << "Variant Field Num:" << fieldNum;
                variantList = QString(line.mid(line.indexOf("{")+1, (line.indexOf("}")-(line.indexOf("{")+1)))).split(" ");
                qDebug() << "Variant(s):" << variantList << ", value:" << (line.mid(line.lastIndexOf("{")+1)).remove("}");
                valid = variantList.contains(*variantFVS);
            }
            if(line.contains("{") && line.contains("}") && valid && *currentField != "title")
            {
                inField = false;
                line = line.mid(line.lastIndexOf("{")+1).remove("}");
                qDebug() << "Type:" << line.left(line.indexOf(" "));
                fieldType = line.left(line.indexOf(" "));
                qDebug() << "Field Description:" << line.right(line.size() - (line.indexOf(" ")+1));
                fieldDescription.append(line.right(line.size() - (line.indexOf(" ")+1)));
                if(fieldDescription == "scheduleBox")
                {
                    createScheduleBox(dynamBody);
                    yearCycleLine->setFont(*font);
                    conditionLine->setFont(*font);
                    yearCycleButton->setFont(*font);
                    conditionButton->setFont(*font);
                    yearCycleRButton->setFont(*font);
                    conditionRButton->setFont(*font);
                    *currentField = conditionLine->objectName();
                    fieldDescription.clear();
                    fieldAdded = true;
                }
                else if(fieldType == "speciesSelection")
                {
                    createSpeciesSelectionComboBox(fieldDescription);
                    dynamBody->addRow(speciesSelectionQLabel, speciesSelectionComboBox);
                    speciesSelectionComboBox->setFont(*font);
                    speciesSelectionQLabel->setFont(*font);
                    *currentField = fieldType;
                    fieldDescription.clear();
                    fieldAdded = true;
                }
                else if(fieldType == "noInput")
                {
                    qDebug() << "noInput";
                    if(fieldDescription.contains(QRegularExpression("\\w")))
                        dynamBody->addRow(new QLabel(fieldDescription));
                    else
                        dynamBody->addRow(new QLabel("\t\t\t\t"));
                    fieldDescription.clear();
                    fieldAdded = true;
                }
                else if(fieldType.contains("listButton", Qt::CaseInsensitive))/*7*/
                {/*7*/
                    qDebug() << fieldType;
                    tempLabel = new QLabel(fieldDescription);
                    tempLabel->setFont(*font);
                    if(fieldType.contains("longListButton"))
                        dynamBody->addRow(tempLabel);
                    comboBoxProperties.clear();
                    *currentField = fieldType;
                    fieldDescription.clear();
                    fieldAdded = true;
                }
                else
                    *currentField = fieldType;
            }
            else if(!line.contains("{") && inField && currentField->contains("listButton", Qt::CaseInsensitive) && valid)/*7*/
            {/*7*/   // read lines that fill QComboxBox, place them in QStringList
                if(line.contains(QRegularExpression("\\w")))
                    while(line.at(0) == " ")
                        line.remove(0, 1);
                if(line.contains("}"))/*7*/
                {
                    inField = false;
                    line.remove("}");
                }
                    if(line.size() > 0)
                    {
                        if(line.at(0) == ">")/*7*/
                        {
                            currentField->prepend(">");
                            comboBoxProperties.append(line.right(line.size()-1));
                            comboBoxProperties.prepend(line.right(line.size()-1));
                            (inField) ? qDebug() << *currentField << "inField:" << line.right(line.size()-1) : qDebug() << *currentField << "has the selected property:" << line.right(line.size()-1);
                        }
                        else if(line.contains(QRegularExpression("\\w")))
                        {
                            comboBoxProperties.append(line);
                            qDebug() << *currentField << "inField:" << line;
                        }
                    }
                if(!inField)
                {
                    QString selected = comboBoxProperties.first();
                    if(currentField->contains(">"))
                        comboBoxProperties.removeFirst();
                    qDebug() << "selected" << *currentField << "value:" << selected;
                    QStringListModel *comboBoxPropertiesModel = new QStringListModel;
                    comboBoxPropertiesModel->setStringList(comboBoxProperties);
                    bool duplicate = false;
                    if(dynamComboBoxes.size() > 0)
                        if(dynamComboBoxes.value(dynamComboBoxes.size() - 1)->objectName() == fieldNum)
                            duplicate = true;
                    if(!duplicate)
                    {
                        dynamComboBoxes.append(new QComboBox);
                        int comboBoxLocation = dynamComboBoxes.size() - 1;
                        dynamComboBoxes.value(comboBoxLocation)->setModel(comboBoxPropertiesModel);
                        dynamComboBoxes.value(comboBoxLocation)->setCurrentText(selected);
                        dynamComboBoxes.value(comboBoxLocation)->setFont(*font);
                        dynamComboBoxes.value(comboBoxLocation)->setObjectName(fieldNum);
                        dynamComboBoxes.value(comboBoxLocation)->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
                        (currentField->contains("long")) ? dynamBody->addRow(dynamComboBoxes.value(comboBoxLocation)) : dynamBody->addRow(tempLabel, dynamComboBoxes.value(comboBoxLocation));
                        defaultComboValue.append(selected);
                    }
                    fieldAdded = true;
                }
            }
            else if(!line.contains("{") && inField && *currentField == "title" && valid)
            {
                if(line.contains("}"))
                {
                    inField = false;
                    line.remove("}");
                    if(line.size() > 0)
                        longTitleTemp.append(line);
                    QListView *longTitle = new QListView();
                    for (int i = 0; i < longTitleTemp.size(); i++) {
                        if(QString(longTitleTemp.at(i)).contains("\\"))
                            if((i + 1) < longTitleTemp.size())
                            {
                                longTitleTemp.replace(i, QString(longTitleTemp.value(i)).remove("\\").append(longTitleTemp.value(i+1)));
                                longTitleTemp.removeAt(i+1);
                                i--;
                            }
                    }
                    longTitle->setModel(new QStringListModel(longTitleTemp));
                    longTitle->setFont(*font);
                    longTitle->setMinimumWidth(longTitle->sizeHintForColumn(0) + 24);
                    longTitle->setMaximumHeight(longTitle->sizeHintForRow(0) * longTitleTemp.size());
                    longTitle->setEditTriggers(QAbstractItemView::NoEditTriggers);
                    longTitle->setStyleSheet("border-style: none");
                    longTitle->setFocusPolicy(Qt::NoFocus);
                    longTitle->setSelectionMode(QAbstractItemView::NoSelection);
                    dynamBody->addRow(longTitle);
                    fieldAdded = true;
                    currentField->clear();
                }
                else
                    longTitleTemp.append(line);
            }
            else if(line.contains("{") && valid)
            {
                if (*currentField == "title")
                {
                    if(line.contains("{") && line.contains("}"))
                    {
                        currentField->clear();
                        qDebug() << "Field title located" << line.mid(line.indexOf("{")+1);
                        QString title = line.mid(line.indexOf("{")+1).remove("}");
                        tempLabel = new QLabel(title);
                        tempLabel->setFont(*font);
                        dynamBody->addRow(tempLabel);
                    }
                    else
                    {
                        qDebug() << "Inside title" << line;
                        longTitleTemp.append(line.remove("{"));
                    }
                }
                else if(currentField->contains("listButton", Qt::CaseInsensitive))/*7*/
                {
                    line.remove("{");
                    if(!line.contains("}") && comboBoxProperties.size() == 0)
                    {
                        qDebug() << "Treating" << line << "as first item in dynamic listButton";
                        comboBoxProperties.append(line);
                    }
                    else
                        qDebug() << "Uncaught listButton" << line;
                }
                else/*7*/
                {
                    qDebug() << "Field Description:" << line.right(line.size() - (line.indexOf(" ")+1));
                    fieldDescription.append(line.right(line.size() - (line.indexOf(" ")+1)));
                    qDebug() << "Field Description updated:" << fieldDescription;
                }
            }
            else if(valid)
            {
                inField = true;
            }
            if(!inField && valid) // move to after Value and Title
            {
            //try vectors of widgets to get/set entered data
                if(!fieldAdded)
                {
                    tempLabel = new QLabel(fieldDescription);
                    tempLabel->setFont(*font);
                    tempLineEdit = new QLineEdit();
                    tempLineEdit->setFont(*font);
                    if(fieldType.contains("intNumberBox"))
                        tempLineEdit->setValidator(new QIntValidator());
                    if(fieldType.contains("numberBox"))
                        tempLineEdit->setValidator(new QDoubleValidator());
                    bool duplicate = false;
                    if(dynamLineEdits.size() > 1)
                        if(dynamLineEdits.value(dynamLineEdits.size() - 1)->objectName() == fieldNum)
                            duplicate = true;
                    if(!duplicate)
                    {
                        if(fieldType.contains("longTextEdit"))
                        {
                            dynamBody->addRow(tempLabel);
                            dynamBody->addRow(tempLineEdit);
                        }
                        else
                            dynamBody->addRow(tempLabel, tempLineEdit);
                        dynamLineEdits.append(tempLineEdit);
                        dynamLineEdits.value(dynamLineEdits.size() - 1)->setObjectName(fieldNum);
                        qDebug() << "Line Edit #" + QString(dynamLineEdits.size() - 1) + " has the Object name:" << dynamLineEdits.last()->objectName();
                        defaultLineValue.append("");
                    }
                    fieldDescription.clear();
                }
                else
                    fieldAdded = false;
            }

        }
        else if(line.contains("speciesCode"))/*7*/
        {
            QStringList variantList = QString(line.mid(line.indexOf("{")+1, (line.indexOf("}")-(line.indexOf("{")+1)))).split(" ");
            if(variantList.contains(*variantFVS))
                dynamComboBoxes.value(dynamComboBoxes.size() - 1)->setObjectName("speciesCode");
        }
        if(line.contains(fieldTitle))
        {
            if(line.contains("}"))
            {
                qDebug() << "Field title located" << line.mid(line.indexOf("{")+1);
                QString title = line.mid(line.indexOf("{")+1).remove("}");
                tempLabel = new QLabel(title);
                tempLabel->setFont(*font);
                dynamBody->addRow(tempLabel);
            }
            else
            {
                qDebug() << "Field title begun" << line.mid(line.indexOf("{")+1);
                *currentField = "title";
                inField = true;
                longTitleTemp.clear();
                if(line.indexOf(":") + 2 < line.size())
                    longTitleTemp.append(line.mid(line.indexOf("{")+1));
            }
        }
        if(line.contains(fieldValue) || line.contains(fieldValueVar))/*7*/
        {
            QString value;
            bool valid = true;
            QStringList variantList;
            if(line.contains(fieldValueVar))/*7*/
            {
                fieldNum = line.left(line.indexOf("{"));
                qDebug() << "Variant Field Num:" << fieldNum;
                variantList = QString(line.mid(line.indexOf("{")+1, (line.indexOf("}")-(line.indexOf("{")+1)))).split(" ");
                (line.size() > (line.indexOf(":")+1)) ? qDebug() << "Variant(s):" << variantList << ", value:" << (line.mid(line.lastIndexOf("{")+1)).remove("}") : qDebug() << "Variant(s):" << variantList;
                valid = variantList.contains(*variantFVS);
            }
            else
                fieldNum = line.left(line.indexOf(":"));
            if(currentField->contains("listButton", Qt::CaseInsensitive) && valid)/*7*/
            {   // first value in dynamic comboBox
                qDebug() << "Variant" << *currentField << "Field located: " << line;
                inField = true;
                if(line.at(0) == ">")
                {
                    currentField->prepend(">");
                    if(line.size() > line.indexOf(">")+1)/*7*/
                    {
                        comboBoxProperties.append(line.mid(line.indexOf(">")+1));
                        comboBoxProperties.prepend(line.mid(line.indexOf(">")+1));
                    }
                    else/*7*/
                        comboBoxProperties.prepend(" ");
                    (line.size() > line.indexOf(">")+1) ? qDebug() << *currentField << "value to be selected:" << line.mid(line.indexOf(">")+1) : qDebug() << "Blank longListButton value to be selected";
                }
                else/*7*/
                {
                    if(line.size() > line.indexOf(":")+1)
                        (line.mid(line.lastIndexOf("{")+1) == "\\" || line.mid(line.lastIndexOf("{")+1) == "\\n") ? qDebug() << "blank found" : qDebug() << *currentField << "starts with:" << line.mid(line.lastIndexOf("{")+1);
                    if(line.mid(line.lastIndexOf("{")+1) != "\\" && line.mid(line.lastIndexOf("{")+1) != "\\n" && line.size() > line.indexOf(":")+1) comboBoxProperties.append(line.mid(line.lastIndexOf("{")+1));
                }
            }
            else/*7*/
            {
                (line.contains(fieldValue)) ? qDebug() << "Field value located" << (value = line.mid(line.indexOf("{")+1).remove("}")) : qDebug() << "Variant dependent Field value located" << (value = line.mid(line.lastIndexOf("{")+1).remove("}"));
                (line.contains(fieldValue)) ? fieldNum = line.left(line.indexOf(":")) : fieldNum = line.left(line.indexOf("{"));
                qDebug() << "Variant Field Num:" << fieldNum;
                if(*currentField == "scheduleBox" && valid)
                {
                    qDebug() << value;
                    conditionLine->setText(value);
                    defaultLineValue.replace(defaultLineValue.size()-1, conditionLine->text());
                    currentField->clear();
                }
                else if(*currentField == "speciesSelection" && valid)
                {
                    qDebug() << line.mid(line.indexOf("{")+1);
//                    qDebug() << speciesMSTAbbreviationName->value(*variantFVS).value(line.mid(line.indexOf("{")+1));
                    speciesSelectionComboBox->setCurrentText(speciesMSTAbbreviationName->value(*variantFVS).value(value));
                    defaultComboValue.replace(defaultComboValue.size()-1, speciesMSTAbbreviationName->value("species_" + *variantFVS).value(value));
                    currentField->clear();
                }
                else if(*currentField == "sliderBox" && valid)
                {
                    QStringList boxProperties = value.split(" ");
                    qDebug() << "default number box amount altered to " + boxProperties.at(0);
                    if(boxProperties.at(0) != "blank")
                        tempLineEdit->setText(boxProperties.at(0));
                    // Defines textbox to limit user input to numbers, with customized Lowest and Highest
                    if(boxProperties.size() > 2)
                    {
                        QDoubleValidator *custom = new QDoubleValidator(QString(boxProperties.at(1)).toDouble(), QString(boxProperties.at(2)).toDouble(), 10 - QString(boxProperties.at(2)).size());
                        custom->setNotation(QDoubleValidator::StandardNotation);
                        tempLineEdit->setValidator(custom);
                        tempLineEdit->setObjectName(QString::number(dynamLineEdits.size()));
                        connect(dynamLineEdits.value(dynamLineEdits.size()-1),  SIGNAL(textEdited(QString)), this, SLOT(inputMod(QString)));
                        qDebug() << "Sync attempted";
                    }
                    defaultLineValue.replace(defaultLineValue.size()-1, boxProperties.at(0));
                }
                else if(currentField->contains("textEdit", Qt::CaseInsensitive) && valid) // catches both longTextEdit and textEdit
                {
                    tempLineEdit->setText(value);
                    defaultLineValue.replace(defaultLineValue.size()-1, value);
                }
                else if(currentField->contains("numberBox", Qt::CaseInsensitive) && valid)
                {
                    QStringList boxProperties = value.split(" ");
                    if(boxProperties.at(0) != "blank")
                    {
                        qDebug() << "default number box amount altered to " + boxProperties.at(0);
                        tempLineEdit->setText(boxProperties.at(0));
                        defaultLineValue.replace(defaultLineValue.size()-1, boxProperties.at(0));
                    }
                    if(boxProperties.size() > 2)
                    {
                        // Defines textbox to limit user input to numbers, with customized Lowest and Highest
                        QValidator *custom;
                        if(*currentField == "numberBox" || QString(boxProperties.at(1)).contains("."))
                            custom = new QDoubleValidator(QString(boxProperties.at(1)).toDouble(), QString(boxProperties.at(2)).toDouble(), 10 - QString(boxProperties.at(2)).size());
                        else
                            custom = new QIntValidator(QString(boxProperties.at(1)).toInt(), QString(boxProperties.at(2)).toInt());
                        tempLineEdit->setValidator(custom);
                        tempLineEdit->setObjectName(QString::number(dynamLineEdits.size()));
                        connect(dynamLineEdits.value(dynamLineEdits.size()-1),  SIGNAL(textEdited(QString)), this, SLOT(inputMod(QString)));
                    }
                }
            }
        }
    }

    dynamBodyHolder->setStyleSheet("background-color: rgb(255, 255, 255)");
    dynamBodyHolder->setFont(*font);
    dynamBody->addRow(keyDesc);
    dynamBodyHolder->setLayout(dynamBody);

    QFormLayout *layout = new QFormLayout;
    layout->addRow(name, title);
    extensionKeyword->setLayout(layout);
    mainLayout = new QGridLayout;
    mainLayout->addWidget(extensionKeyword, 0, 0);
    mainLayout->addWidget(dynamBodyHolder, 3, 0);
    mainLayout->addWidget(buttonBox, 4, 0);
    setLayout(mainLayout);
    acceptButton->setFont(*font);
    cancelButton->setFont(*font);
    resetButton->setFont(*font);
    editButton->setFont(*font);

}

GeneralPurposeScreenBuilder::~GeneralPurposeScreenBuilder()
{
    qDebug() << "Inside GeneralPurposeScreenBuilder deconstructor";
    if(speciesSelection)
    {
        qDebug() << "Inside GeneralPurposeScreenBuilder speciesSelection pointer deconstructor";
        delete speciesSelectionModel;
        delete speciesSelectionQLabel;
        delete speciesSelectionComboBox;
    }
    if(scheduleBox)
    {
        qDebug() << "Inside GeneralPurposeScreenBuilder scheduleBox pointer deconstructor";
        delete yearCycleLine;
        delete conditionLine;
        delete yearCycleButton;
        delete conditionButton;
        delete yearCycleRButton;
        delete conditionRButton;
        delete scheduleBoxWidget;
    }
    if(defaultComboValue.size() > 0)
        dynamComboBoxes.clear();
    if(dynamLineEdits.size() > 0)
        dynamLineEdits.clear();
    delete font;
    delete title;
    delete mainLayout;
    delete editButton;
    delete resetButton;
    delete cancelButton;
    delete acceptButton;
    delete buttonBox;
}

void GeneralPurposeScreenBuilder::accept()
{
    qDebug() << "Inside accept function";
    (title) ? qDebug() << "Title:" << title->text() : qDebug() << "ERROR";
    for (int i = 0; i < dynamLineEdits.size(); i++)
    {
        qDebug() << "Selected value:" << dynamLineEdits.at(i)->text();
        if(dynamLineEdits.at(i)->validator() != 0)
            if(QVariant(dynamLineEdits.at(i)->validator()->property("top")).isValid())
            if(dynamLineEdits.at(i)->objectName().contains("Defined Number Box "))
                qDebug() << dynamLineEdits.at(i)->objectName() << dynamLineEdits.at(i)->validator()->property("top").toString();
    }
    for (int i = 0; i < dynamComboBoxes.size(); i++)
    {
        qDebug() << "Selected value:" << dynamComboBoxes.at(i)->currentText();
        if(speciesSelection || dynamComboBoxes.at(i)->objectName().contains("speciesCode"))
        {
            if(speciesMSTAN->value("species_" + *variantFVS).values().contains(dynamComboBoxes.at(i)->currentText()))
            {
                qDebug() << speciesMSTAN->value("species_" + *variantFVS).key(dynamComboBoxes.at(i)->currentText());
            }
            else if(dynamComboBoxes.at(i)->currentText().contains("All affected species"))
                qDebug() << speciesMSTAN->value("species_" + *variantFVS).key(dynamComboBoxes.at(i)->currentText().remove(" affected"));
        }
    }
}

void GeneralPurposeScreenBuilder::reset()
{
    qDebug() << "Inside reset function";
    qDebug() << "Reseting line edits";
    for (int i = 0; i < dynamLineEdits.size(); i++){
        dynamLineEdits.value(i)->setText(defaultLineValue.at(i));
    }
    qDebug() << "Reseting combo boxes";
    for (int i = 0; i < dynamComboBoxes.size(); i++){
        dynamComboBoxes.value(i)->setCurrentText(defaultComboValue.at(i));
    }
}

void GeneralPurposeScreenBuilder::edit()
{
    qDebug() << "Inside edit function";
}

void GeneralPurposeScreenBuilder::speciesComboBoxSelection()
{
    qDebug() << "Inside speciesComboBoxSelection function";
    qDebug() << speciesSelectionComboBox->currentText() << speciesMSTAN->value("species_" + *variantFVS).key(speciesSelectionComboBox->currentText());
}

void GeneralPurposeScreenBuilder::scheduleBoxSelection()
{
    qDebug() << "Inside scheduleBoxSelection function";
    if(yearCycleRButton->isChecked())
    {
        conditionLine->setEnabled(false);
        conditionButton->setEnabled(false);
        yearCycleButton->setEnabled(true);
        yearCycleLine->setEnabled(true);
    }
    else if(conditionRButton->isChecked())
    {
        yearCycleLine->setEnabled(false);
        yearCycleButton->setEnabled(false);
        conditionButton->setEnabled(true);
        conditionLine->setEnabled(true);
    }
}

void GeneralPurposeScreenBuilder::inputMod(QString lineEditValue)
{
    QWidget *selected = this->focusWidget();
    double userInput = lineEditValue.toDouble();
    qDebug() << "Inside inputMod function with lineEdit " + selected->objectName() + " and value" << lineEditValue;
    if(selected->objectName().toInt() <= dynamLineEdits.size())
    {
        int lineEditNum = selected->objectName().toInt()-1;
        double low  = dynamLineEdits.at(lineEditNum)->validator()->property("bottom").toString().toDouble();
        double high = dynamLineEdits.at(lineEditNum)->validator()->property("top").toString().toDouble();
        qDebug() << dynamLineEdits.at(lineEditNum)->validator()->property("top");
        qDebug() << dynamLineEdits.at(lineEditNum)->validator()->property("bottom");
        qDebug() << "Low:"  << dynamLineEdits.at(lineEditNum)->validator()->property("bottom").toString();
        qDebug() << "High:" << dynamLineEdits.at(lineEditNum)->validator()->property("top").toString();
        qDebug() << "User Input:" << userInput;
        if(userInput > high)
        {
            qDebug() << "User input of" << userInput << "is higher than" << high;
            dynamLineEdits.value(lineEditNum)->setText(QString::number(high));
        }
        else if(userInput <= high && userInput >= low)
            qDebug() << "Acceptable input";
        else if(lineEditValue.contains("."))
        {
            if(lineEditValue.left(lineEditValue.indexOf(".")).toInt() < low)
                dynamLineEdits.value(lineEditNum)->setText(QString::number(low) + ".");
        }
    }
}

void GeneralPurposeScreenBuilder::createScheduleBox(QFormLayout *dynamicBody)
{
    qDebug() << "Inside GeneralPurposeScreenBuilder::createScheduleBox";
    scheduleBox = true;
    scheduleBoxWidget = new QWidget;
    QFormLayout *conditionSelect = new QFormLayout;
    QFormLayout *selectYear = new QFormLayout;
    QWidget *conditionSelectWidget = new QWidget;
    QWidget *selectYearWidget = new QWidget;
    yearCycleRButton = new QRadioButton;
    conditionRButton = new QRadioButton;
    yearCycleLine = new QLineEdit;
    conditionLine = new QLineEdit;
    dynamLineEdits.append(yearCycleLine);
    dynamLineEdits.append(conditionLine);
    yearCycleButton = new QPushButton;
    conditionButton = new QPushButton;
    yearCycleButton->setText(" Select Year ");
    yearCycleButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    yearCycleLine->setText(QString::number(*year));
    defaultLineValue.append(QString::number(*year));                 // for yearCycleLine
    yearCycleLine->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    conditionButton->setText(" years after Condition is met ");
    conditionButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    conditionButton->setEnabled(false);
    defaultLineValue.append("");                    // for conditionLine
    conditionLine->setText("0");
    conditionLine->setEnabled(false);
    conditionLine->setObjectName("scheduleBox");
    conditionLine->setMaximumSize(conditionLine->sizeHint()*4);
    conditionLine->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    yearCycleRButton->setChecked(true);
    yearCycleRButton->setText("Schedule by Year/Cycle");
    conditionRButton->setText("Schedule by Condition");
    dynamicBody->addRow(yearCycleRButton, conditionRButton);
    selectYear->addRow(yearCycleLine, yearCycleButton);
    selectYearWidget->setLayout(selectYear);
    conditionSelect->addRow(conditionLine, conditionButton);
    conditionSelectWidget->setLayout(conditionSelect);
    conditionSelectWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    dynamicBody->addRow(selectYearWidget, conditionSelectWidget);
    connect(yearCycleRButton, SIGNAL(clicked()), this, SLOT(scheduleBoxSelection()));
    connect(conditionRButton, SIGNAL(clicked()), this, SLOT(scheduleBoxSelection()));
}

void GeneralPurposeScreenBuilder::createButtonBox()
{
    qDebug() << "Inside GeneralPurposeScreenBuilder::createButtonBox";
    buttonBox = new QDialogButtonBox;
    acceptButton = new QPushButton;
    cancelButton = new QPushButton;
    resetButton = new QPushButton;
    editButton = new QPushButton;
    acceptButton = buttonBox->addButton("Ok", QDialogButtonBox::YesRole);
    resetButton = buttonBox->addButton("Reset", QDialogButtonBox::ActionRole);
    cancelButton = buttonBox->addButton("Cancel", QDialogButtonBox::RejectRole);
    editButton = buttonBox->addButton("Use Editor", QDialogButtonBox::ActionRole);
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(reset()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(edit()));
}

void GeneralPurposeScreenBuilder::createSpeciesSelectionComboBox(QString fieldDesc)
{
    qDebug() << "Inside GeneralPurposeScreenBuilder::createSpeciesSelectionComboBox";
    qDebug() << "species_" + *variantFVS;
    qDebug() << speciesMSTAN->value("species_" + *variantFVS).keys();
    speciesSelection = true;
    defaultComboValue.append("All species");
    speciesSelectionComboBox = new QComboBox;
    speciesSelectionModel = new QStringListModel;
    speciesSelectionQLabel = new QLabel(fieldDesc+"\t\t\t\t");
    speciesSelectionQLabel->setAlignment(Qt::AlignLeft);
    speciesSelectionModel->setStringList(speciesMSTAN->value("species_" + *variantFVS).values());
    speciesSelectionModel->sort(0);
    speciesSelectionComboBox->setModel(speciesSelectionModel);
    speciesSelectionComboBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    speciesSelectionComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    speciesSelectionComboBox->setCurrentText("All species");
    speciesSelectionComboBox->setObjectName("speciesSelection");
    speciesSelectionComboBox->setMinimumHeight(24);
    dynamComboBoxes.append(speciesSelectionComboBox);
    connect(speciesSelectionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(speciesComboBoxSelection()));
}
