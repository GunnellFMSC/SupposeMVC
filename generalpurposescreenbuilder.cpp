#include <QtWidgets>

#include "generalpurposescreenbuilder.h"

GeneralPurposeScreenBuilder::GeneralPurposeScreenBuilder(QString keywordExtension, QStringList description, QStringList MSText, QString *variant, QMap<QString, QMap<QString, QString>> *mainSectionTextDictionary, int startYear, QWidget *parent) : QDialog(parent)
{
    qDebug() << "Inside GeneralPurposeScreenBuilder default constructor";
    qDebug() << "Selected variant: " + *variant;
    this->setObjectName("Dynamic Window");
    year = new int;
    *year = startYear;
    qDebug() << "Start year: " << *year;
    variantFVS = new QString;
    *variantFVS = *variant;
    dictionaryMST = new QMap<QString, QMap<QString, QString>>;
    *dictionaryMST = *mainSectionTextDictionary;
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
    keyDesc->setSizeAdjustPolicy(QListView::AdjustToContents);
    keyDesc->setMinimumHeight(descModel->rowCount()*keyDesc->sizeHintForRow(0)); // <-- get product of rows and rows size, use for ListView size
    bool inField = false/*, inFieldValue = false, inFieldTitle = false*/, fieldAdded = false;
    QRegularExpression field("f([0-9]+): ?({.*})?");
    QRegularExpression fieldVar("f([0-9]+){[\\w+\\s?]+}: ?({.*})?");
    QRegularExpression fieldValue("f(\\d)+v:");
    QRegularExpression fieldValueVar("f(\\d)+v{[\\w+\\s?]+}:");
    QRegularExpression fieldTitle("f(\\d)+title:");
    QRegularExpression fieldTitleVar("f(\\d)+title{[\\w+\\s?]+}:");
    QStringList comboBoxProperties, longTitleTemp;
    QString fieldDescription, fieldType, fieldNum;
    QStringList variantList;
    QLineEdit *tempLineEdit;
    QLabel *tempLabel;

    auto variantListCheck = [&](QString line)->bool { qDebug() << "Variant list check lambda function used.";
        variantList = QString(line.mid(line.indexOf("{")+1, (line.indexOf("}")-(line.indexOf("{")+1)))).split(" ");
        return variantList.contains(*variantFVS);};

    foreach (QString line, MSText) {
        qDebug() << "Line:" << line; // DEBUG outputs every line for QA
        if(line.contains(field) || inField || line.contains(fieldVar))
        {
            bool valid = true;
            variantList.clear();        // <- refreshes variantList
            if(line.contains(field))
            {// sets fieldNum for regular field
                qDebug() << "Field located: " << line.mid(line.indexOf("{") + 1);
                fieldNum = line.left(line.indexOf(":"));
                qDebug() << "Field Num:" << fieldNum;
            }
            else if(line.contains(fieldVar))
            {// sets fieldNum for variant dependent field and determines field validity
                qDebug() << "Variant Dependent Field located: " << line.mid(line.lastIndexOf("{") + 1);
                fieldNum = line.left(line.indexOf("{"));
                qDebug() << "Variant Field Num:" << fieldNum;
                valid = variantListCheck(line);
                qDebug() << "Variant(s):" << variantList << ", value:" << (line.mid(line.lastIndexOf("{")+1)).remove("}");
            }
            if(line.contains("{") && line.contains("}") && valid && !currentField->contains("title"))
            {// if line specifies fieldType, gathers information and categorizes appropriately
                inField = false;
                line = line.mid(line.lastIndexOf("{")+1).remove("}");
                qDebug() << "Type:" << line.left(line.indexOf(" "));
                fieldType = line.left(line.indexOf(" "));
                qDebug() << "Field Description:" << line.right(line.size() - (line.indexOf(" ")+1));
                fieldDescription.append(line.right(line.size() - (line.indexOf(" ")+1)));
                if(dynamBody->rowCount() > 0)
                    noInputRemovalCheck(dynamBody, fieldNum);
                if(fieldType == "scheduleBox")
                {
                    createScheduleBox(dynamBody);
                    yearCycleLine->setFont(*font);
                    conditionLine->setFont(*font);
                    yearCycleLabel->setFont(*font);
                    conditionButton->setFont(*font);
                    yearCycleRButton->setFont(*font);
                    conditionRButton->setFont(*font);
                    *currentField = conditionLine->objectName();
                    dynamLineEdits.value(dynamLineEdits.size() - 2)->setObjectName(dynamLineEdits.value(dynamLineEdits.size() - 2)->objectName()+fieldNum);
                    dynamLineEdits.value(dynamLineEdits.size() - 1)->setObjectName(dynamLineEdits.value(dynamLineEdits.size() - 1)->objectName()+fieldNum);
                    fieldDescription.clear();
                    fieldAdded = true;
                }
                else if(fieldType == "speciesSelection" || fieldType == "habPaSelection" || fieldType == "forestSelection")
                {
                    QString type = fieldType == "speciesSelection"?"species_":(fieldType == "habPaSelection"?"HabPa_":"Forests_");
                    qDebug() << type + "Selection found";
                    createSpecialSelectionComboBox(type);
                    QLabel *selectionQLabel = new QLabel(fieldDescription + "\t\t\t\t");
                    selectionQLabel->setAlignment(Qt::AlignLeft);
                    dynamBody->addRow(selectionQLabel, dynamComboBoxes.last());
                    dynamComboBoxes.last()->setObjectName(type + "Selection");
                    dynamComboBoxes.last()->setFont(*font);
                    selectionQLabel->setFont(*font);
                    *currentField = fieldType;
                    fieldDescription.clear();
                    fieldAdded = true;
                }
                else if(fieldType == "noInput")
                {
                    *currentField = fieldType;
                    if(dynamLineEdits.value(dynamLineEdits.size() - 1)->objectName().contains(fieldNum))
                    {
                        dynamLineEdits.remove(dynamLineEdits.size() - 1);
                        dynamBody->removeRow((dynamBody->rowCount() - 1));
                    }
                    QLabel *noInputText = new QLabel();
                    if(fieldDescription.contains(QRegularExpression("\\w")))
                        noInputText->setText(fieldDescription);
                    else
                        noInputText->setText("\t\t\t\t");
                    noInputText->setFont(*font);
                    dynamBody->addRow(noInputText);
                    dynamBody->itemAt(dynamBody->count() - 1)->widget()->setObjectName("noInput"+fieldNum);
                    qDebug() << "noInput widget name: " << dynamBody->itemAt(dynamBody->rowCount() - 1)->widget()->objectName();

                    fieldDescription.clear();
                    fieldAdded = true;
                }
                else if(fieldType == "checkBox")
                {
                    dynamCheckBoxes.append(new QCheckBox(fieldDescription));
                    dynamCheckBoxes.last()->setObjectName("checkBox" + QString::number(dynamCheckBoxes.size()));
                    dynamBody->addRow(dynamCheckBoxes.last());
                    dynamCheckBoxes.last()->setFont(*font);
                    defaultCheckValue.append(false);
                    *currentField = fieldType;
                    fieldDescription.clear();
                    fieldAdded = true;
                }
                else if(fieldType.contains("listButton", Qt::CaseInsensitive))
                {
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
            else if(!line.contains("{") && inField && currentField->contains("listButton", Qt::CaseInsensitive) && valid)
            {   // read lines that fill QComboxBox, place them in QStringList
                if(line.contains(QRegularExpression("\\w")))//
                    while(line.at(0) == " ")//
                        line.remove(0, 1);//
                if(line.contains("}"))
                {
                    inField = false;
                    line.remove("}");
                }
                if(line.size() > 0)
                {
                    if(line.contains(QRegularExpression("\\w")))
                    {
                        comboBoxProperties.append(line);
                        qDebug() << *currentField << "inField:" << line;
                    }
                }
                if(!inField)
                {
                    fieldAdded = addDynamComboBox(comboBoxProperties, dynamBody, tempLabel, fieldNum);
                    dynamComboBoxes.value(dynamComboBoxes.size()-1)->setFont(*font);
                    valid = fieldAdded;
                }
            }
            else if(!line.contains("{") && inField && currentField->contains("title") && valid)
            {// if current line is part of a Title that takes up multiple lines in .prm file
                if(line.contains("}"))
                {// if end of long Title
                    inField = false;
                    line.remove("}");
                    if(line.size() > 0)
                        longTitleTemp.append(line);
                    QListView *longTitle = new QListView();
                    longTitle->setObjectName(*currentField);
                    for (int i = 0; i < longTitleTemp.size(); i++)
                    {// examine every line within the temporary QStringList
                        if(QString(longTitleTemp.at(i)).contains("\\"))
                            if((i + 1) < longTitleTemp.size())
                            {// If the line being examined contains a backslash and is not last, append the next line to it and reexamine
                                longTitleTemp.replace(i, QString(longTitleTemp.value(i)).remove("\\").append(longTitleTemp.value(i+1)));
                                longTitleTemp.removeAt(i+1);
                                i--;
                            }
                    }
                    longTitle->setFont(*font);
                    longTitle->setModel(new QStringListModel(longTitleTemp));
                    longTitle->setMinimumWidth(longTitle->sizeHintForColumn(0) + 24);
                    longTitle->setMaximumHeight(longTitle->sizeHintForRow(0) * longTitleTemp.size());
                    longTitle->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
                    longTitle->setEditTriggers(QAbstractItemView::NoEditTriggers);
                    longTitle->setSelectionMode(QAbstractItemView::NoSelection);
                    longTitle->setStyleSheet("border-style: none");
                    longTitle->setFocusPolicy(Qt::NoFocus);
                    dynamBody->addRow(longTitle);
                    fieldAdded = true;
                    currentField->clear();
                }
                else
                    longTitleTemp.append(line);
            }
            else if(line.contains("{") && valid)
            {
                if (currentField->contains("title"))
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
                else if(currentField->contains("listButton", Qt::CaseInsensitive))
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
                else
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
                    if(fieldType.contains("numberBox", Qt::CaseInsensitive))
                        tempLineEdit->setValidator(new QDoubleValidator());
                    bool duplicate = false;
                    if(dynamBody->rowCount() > 0)
                        noInputRemovalCheck(dynamBody, fieldNum);
                    if(dynamLineEdits.size() > 1)
                    {
                        if(dynamLineEdits.value(dynamLineEdits.size() - 1)->objectName().contains(fieldNum))
                            duplicate = true;
                    }
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
        else if(line.contains("speciesCode{"))
        {
            if(variantListCheck(line))
                dynamComboBoxes.value(dynamComboBoxes.size() - 1)->setObjectName("speciesCode");
        }
        if(line.contains(fieldTitle))
        {
            int itemNum = dynamBody->count() - 1;
            // ternary expression returns true if title is unique or first field item
            bool unique = (itemNum >= 0) ? ((dynamBody->itemAt(itemNum)->widget()->objectName() == "titleVar" + fieldNum) ? false:true):true;
            if(line.contains("}") && unique)
            {
                qDebug() << "Field title located" << line.mid(line.indexOf("{")+1);
                QString title = line.mid(line.indexOf("{")+1).remove("}");
                tempLabel = new QLabel(title);
                tempLabel->setFont(*font);
                tempLabel->setObjectName("title" + fieldNum);
                dynamBody->addRow(tempLabel);
            }
            else if(unique)
            {
                qDebug() << "Field title begun" << line.mid(line.indexOf("{")+1);
                *currentField = "title" + fieldNum;
                inField = true;
                longTitleTemp.clear();
                if(line.indexOf(":") + 2 < line.size())
                    longTitleTemp.append(line.mid(line.indexOf("{")+1));
            }
        }
        else if(line.contains(fieldTitleVar))
        {
            qDebug() << "Variant dependent Field Title located" << line;
            fieldNum = line.left(line.indexOf("t"));
            if(dynamBody->rowCount() > 0)
                noInputRemovalCheck(dynamBody, fieldNum);
            qDebug() << "Variant Field Num:" << fieldNum;
            bool valid = variantListCheck(line);
            (line.size() > (line.indexOf(":")+1)) ? qDebug() << "Variant(s):" << variantList << ", value:" << (line.mid(line.lastIndexOf("{")+1)).remove("}") : qDebug() << "Variant(s):" << variantList;
            line = line.mid(line.indexOf(":"));
            if(line.contains("}") && valid)
            {
                qDebug() << "Field title located" << line.mid(line.lastIndexOf("{")+1);
                QString title = line.mid(line.lastIndexOf("{")+1).remove("}");
                tempLabel = new QLabel(title);
                tempLabel->setFont(*font);
                dynamBody->addRow(tempLabel);
            }
            else if(valid)
            {
                qDebug() << "Field title begun" << line.mid(line.lastIndexOf("{")+1);
                *currentField = "titleVar" + fieldNum;
                inField = true;
                longTitleTemp.clear();
                if(line.indexOf(":") + 2 < line.size())
                    longTitleTemp.append(line.mid(line.indexOf("{")+1));
            }
        }
        if(line.contains(fieldValue) || line.contains(fieldValueVar))
        {
            QString value;
            bool valid = true;
            if(line.contains(fieldValueVar))
            {
                qDebug() << "Variant dependent Field value located" << (value = line.mid(line.lastIndexOf("{")+1).remove("}"));
                fieldNum = line.left(line.indexOf("{"));
                qDebug() << "Variant Field Num:" << fieldNum;
                valid = variantListCheck(line);
                (line.size() > (line.indexOf(":")+1)) ? qDebug() << "Variant(s):" << variantList << ", value:" << (line.mid(line.lastIndexOf("{")+1)).remove("}") : qDebug() << "Variant(s):" << variantList;
                line = line.mid(line.indexOf(":"));
            }
            else
            {
                qDebug() << "Field value located" << (value = line.mid(line.indexOf("{")+1).remove("}"));
                fieldNum = line.left(line.indexOf(":"));
                line.remove(fieldNum + ":");
                if(line.size() <= 1) valid = false, inField = true;
                if(valid) line.remove("{");
            }
            if(currentField->contains("listButton", Qt::CaseInsensitive) && valid)
            {   // first value in dynamic comboBox
                qDebug() << "Variant" << *currentField << "Field located: " << line;
                if(line.contains("}"))
                {
                    line.remove(":{");
                    addDynamComboBox(QStringList(line.remove("}")), dynamBody, tempLabel, fieldNum);
                    dynamComboBoxes.value(dynamComboBoxes.size()-1)->setFont(*font);
                }
                else
                    inField = true;
                if(inField)
                {
                    if(line.size() > line.indexOf(":")+1)
                        (line.mid(line.lastIndexOf("{")+1) == "\\" || line.mid(line.lastIndexOf("{")+1) == "\\n") ? qDebug() << "blank found" : qDebug() << *currentField << "starts with:" << line.mid(line.lastIndexOf("{")+1);
                    if(line.mid(line.lastIndexOf("{")+1) != "\\" && line.mid(line.lastIndexOf("{")+1) != "\\n" && line.size() > line.indexOf(":")+1) comboBoxProperties.append(line.mid(line.lastIndexOf("{")+1));
                }
            }
            else if(valid)
            {
                qDebug() << "Variant Field Num:" << fieldNum;
                if(*currentField == "scheduleBox" && valid)
                {
                    qDebug() << value;
                    conditionLine->setText(value);
                    defaultLineValue.replace(defaultLineValue.size()-1, conditionLine->text());
                    currentField->clear();
                }
                else if(*currentField == "species_Selection" && valid)
                {
                    qDebug() << "Species Selection specification found:" << value;
                    if(!value.contains("deleteAll"))
                    {
                        dynamComboBoxes.last()->setCurrentText(dictionaryMST->value("species_" + *variantFVS).value(value));
                        defaultComboValue.replace(defaultComboValue.size()-1, dictionaryMST->value("species_" + *variantFVS).value(value));
                    }
                    else if(value == "deleteAll")
                    {
                        dynamComboBoxes.last()->removeItem(dynamComboBoxes.last()->findText("All species"));
//                        speciesSelectionComboBox->insertItem(0, " ");
//                        speciesSelectionComboBox->setCurrentText(" ");
//                        defaultComboValue.replace(defaultComboValue.size()-1, " ");
                        defaultComboValue.replace(defaultComboValue.size()-1, dynamComboBoxes.last()->currentText());
                    }
                    else if(value.contains("deleteAll "))
                    {
                        value.remove("deleteAll ");
                        dynamComboBoxes.last()->removeItem(dynamComboBoxes.last()->findText("All species"));
                        dynamComboBoxes.last()->setCurrentText(dictionaryMST->value("species_" + *variantFVS).value(value));
                        defaultComboValue.replace(defaultComboValue.size()-1, dynamComboBoxes.last()->currentText());
                    }
                    currentField->clear();
                }
                else if((currentField->contains("numberBox", Qt::CaseInsensitive) || currentField->contains("sliderBox", Qt::CaseInsensitive)) && valid)
                {// sliderBox functionality has been merged with the various numberBoxes
                    QString intRange = "2147483647", doubleRange = "9999999.99";
                    QStringList boxProperties = value.split(" ");
                    if(boxProperties.contains("Fmax"))
                        for(int i = 0; i < boxProperties.size(); i++)
                            if(boxProperties.at(i).contains("Fmax"))
                                boxProperties.replace(i, intRange);
                    QDoubleValidator *custom = new QDoubleValidator();
                    if(boxProperties.at(0) != "blank")
                    {
                        qDebug() << "default number box amount altered to " + boxProperties.at(0);
                        tempLineEdit->setText(boxProperties.at(0));
                        defaultLineValue.replace(defaultLineValue.size()-1, boxProperties.at(0));
                    }
                    if(boxProperties.size() > 2)
                    {
                        // Defines textbox to limit user input to numbers, with customized Lowest and Highest
                        if(currentField->contains("int"))
                        {
                            custom->setRange(QString(boxProperties.at(1)).toDouble(), QString(boxProperties.at(2)).toDouble(), 0);
                            custom->setObjectName("integer" + fieldNum);
                            qDebug() << custom->objectName();
                        }
                        else
                        {
                            custom->setRange(QString(boxProperties.at(1)).toDouble(), QString(boxProperties.at(2)).toDouble(), 9 - QString(boxProperties.at(2)).size());
                            custom->setObjectName("double" + fieldNum);
                            qDebug() << custom->objectName();
                        }
                    }
                    else
                    {
                        // Default definition textbox to limit user input to numbers, if Lowest and Highest unspecified in parm file
                        if(currentField->contains("int"))
                        {
                            custom->setRange(intRange.toDouble()*-1, intRange.toDouble(), 0);
                            custom->setObjectName("integer" + fieldNum);
                            qDebug() << custom->objectName();
                        }
                        else
                        {
                            custom->setRange(doubleRange.toDouble()*-1, doubleRange.toDouble(), 9);
                            custom->setObjectName("double" + fieldNum);
                            qDebug() << custom->objectName();
                        }
                    }
                    tempLineEdit->setMaxLength(14);
                    tempLineEdit->setValidator(custom);
//                    tempLineEdit->setMinimumSize(tempLineEdit->sizeHint());
//                    tempLineEdit->setMaximumSize(tempLineEdit->sizeHint()*3);
                    tempLineEdit->setObjectName("numberBox" + QString::number(dynamLineEdits.size()));
//                    tempLineEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
                    connect(dynamLineEdits.value(dynamLineEdits.size()-1), SIGNAL(textEdited(QString)), this, SLOT(liveInputMod(QString)));
                    connect(tempLineEdit, &QLineEdit::editingFinished, [=]() {modifyInput(tempLineEdit);qDebug() << "EDITING FINISHED!";}); // lambda, http://doc.qt.io/qt-5/qobject.html#connect-4
//                    connect(tempLineEdit, SIGNAL(returnPressed()), this, [=]()->void {modifyInput(*tempLineEdit);});
                    if(boxProperties.size() > 2)
                        tempLineEdit->setToolTip("Input is inclusively bound from "  + QString(boxProperties.at(1)) + " to " + QString(boxProperties.at(2)));
                    else
                        tempLineEdit->setToolTip("Input is inclusively bound from "  + numberToQString(custom->bottom()) + " to " + numberToQString(custom->top()));
                }
                else if(currentField->contains("textEdit", Qt::CaseInsensitive) && valid) // catches both longTextEdit and textEdit
                {
                    tempLineEdit->setText(value);
                    defaultLineValue.replace(defaultLineValue.size()-1, value);
                }
                else if(*currentField == "checkBox" && valid)
                {// if value is 1, box is checked, else unchecked
                    value.toInt() == 1 ? dynamCheckBoxes.last()->setChecked(true) : dynamCheckBoxes.last()->setChecked(false);
                    defaultCheckValue.replace(defaultCheckValue.size()-1, value.toInt());
                }
            }
        }
    }

    // connects the application's focusChanged signal to this GPSB window to allow examination of user input after selection ends (http://doc.qt.io/qt-5/qapplication.html#focusChanged)
    connect(QApplication::instance(),  SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(selectionChange(QWidget*, QWidget*)));

    connect(this, SIGNAL(inputError(QLineEdit*)), this, SLOT(inputErrorAlert(QLineEdit*)));

    dynamBodyHolder->setStyleSheet("background-color: rgb(255, 255, 255)");
    dynamBodyHolder->setFont(*font);
    dynamBody->addRow(keyDesc);
    dynamBodyHolder->setLayout(dynamBody);

    QFormLayout *layout = new QFormLayout;
    layout->addRow(name, title);
    extensionKeyword->setLayout(layout);

    // Creates scroll area (https://forum.qt.io/topic/31890/solved-layout-with-scrollbar/8)
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    scrollArea->setWidgetResizable (true);
    scrollArea->setWidget(dynamBodyHolder); // adds dynamically created body to scroll area

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(extensionKeyword, 0, 0);
    mainLayout->addWidget(scrollArea, 2, 0);
//    mainLayout->addWidget(dynamBodyHolder, 3, 0);
    mainLayout->addWidget(buttonBox, 4, 0);
    acceptButton->setFont(*font);
    cancelButton->setFont(*font);
    resetButton->setFont(*font);
    editButton->setFont(*font);
    setLayout(mainLayout);
    // sets initial window size with respect to contained objects and screen
    QRect rectGPSB, screenActual = qApp->desktop()->availableGeometry();
    rectGPSB.setWidth((dynamBody->sizeHint().width() * 1.1 < screenActual.width()*0.9) ? (dynamBody->sizeHint().width() * 1.1):(screenActual.width()*0.9));
    rectGPSB.setHeight((mainLayout->sizeHint().height() * 1.1 < screenActual.height()*0.9) ? (mainLayout->sizeHint().height() * 1.1):(screenActual.height()*0.9));
    this->setGeometry(rectGPSB);
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(), screenActual)); // centers window (http://doc.qt.io/qt-5/qstyle.html#alignedRect, https://wiki.qt.io/How_to_Center_a_Window_on_the_Screen)
    validInput = true;
}

GeneralPurposeScreenBuilder::~GeneralPurposeScreenBuilder()
{
    qDebug() << "Inside GeneralPurposeScreenBuilder deconstructor";

    delete dictionaryMST;

    if(scheduleBox)
    {// yearCycleLine & conditionLine are deleted in dynamLineEdits
//        qDebug() << "Inside GeneralPurposeScreenBuilder scheduleBox pointer deconstructor";
        delete yearCycleLabel;
        delete conditionButton;
        delete yearCycleRButton;
        delete conditionRButton;
        delete scheduleBoxWidget;
    }
    while(!dynamComboBoxes.empty())
    {
//        qDebug() << "Inside GeneralPurposeScreenBuilderdynamComboBoxes  deconstructor";
        delete dynamComboBoxes.back();
        dynamComboBoxes.pop_back();
    }
    while(!dynamCheckBoxes.empty())
    {
//        qDebug() << "Inside GeneralPurposeScreenBuilder dynamCheckBoxes deconstructor";
        delete dynamCheckBoxes.back();
        dynamCheckBoxes.pop_back();
    }
    while(!dynamLineEdits.empty())
    {// deletes "title" pointer
//        qDebug() << "Inside GeneralPurposeScreenBuilder dynamLineEdits deconstructor";
        delete dynamLineEdits.back();
        dynamLineEdits.pop_back();
    }
//    qDebug() << "Inside GeneralPurposeScreenBuilder generic deconstructor";
    delete year;
    delete font;
    delete variantFVS;
    delete currentField;
    delete buttonBox; // deletes editButton, resetButton, acceptButton, & cancelButton
}

void GeneralPurposeScreenBuilder::accept()
{
    qDebug() << "Inside accept function";
    if(validInput)
    {
        (title) ? qDebug() << "Title:" << title->text() : qDebug() << "ERROR";
        for (int i = 0; i < dynamLineEdits.size(); i++)
        {
            qDebug() << "Selected value " + QString::number(i) + ":" << dynamLineEdits.at(i)->text();
            if(dynamLineEdits.at(i)->objectName().contains("schedule"))
                if((dynamLineEdits.at(i)->objectName().contains("Year") && yearCycleRButton->isChecked()) || (dynamLineEdits.at(i)->objectName().contains("Condition") && conditionRButton->isChecked()))
                    qDebug() << "Send:" << dynamLineEdits.at(i)->text() << "for" << dynamLineEdits.at(i)->objectName();
            if(dynamLineEdits.at(i)->validator() != 0)
            {
                if(QVariant(dynamLineEdits.at(i)->validator()->property("bottom")).isValid() && QVariant(dynamLineEdits.at(i)->validator()->property("top")).isValid())
                    qDebug() << dynamLineEdits.at(i)->objectName() << "has lower bound of" << dynamLineEdits.at(i)->validator()->property("bottom").toString() << "and upper bound of" << dynamLineEdits.at(i)->validator()->property("top").toString();
                if(dynamLineEdits.at(i)->hasAcceptableInput())
                {
                    if(dynamLineEdits.at(i)->text().size() > 10)
                        qDebug() << "Send:" << dynamLineEdits.at(i)->text().remove(11, 99) << "for" << dynamLineEdits.at(i)->objectName();
                    else
                        qDebug() << "Send:" << dynamLineEdits.at(i)->text() << "for" << dynamLineEdits.at(i)->objectName();
                }
                else
                {
                    qDebug() << "Input for line" << dynamLineEdits.at(i)->objectName() << "unnacceptable.";
                    emit inputError(dynamLineEdits.at(i));
                    validInput = false;
                    break;
                }
            }
        }
        for (int i = 0; i < dynamComboBoxes.size(); i++)
        {
            qDebug() << "Selected value:" << dynamComboBoxes.at(i)->currentText();
            if(dynamComboBoxes.at(i)->objectName().contains("species_") || dynamComboBoxes.at(i)->objectName().contains("speciesCode") || dynamComboBoxes.at(i)->objectName().contains("Forests_") || dynamComboBoxes.at(i)->objectName().contains("HabPa_"))
            {
                if(dictionaryMST->value("species_" + *variantFVS).values().contains(dynamComboBoxes.at(i)->currentText()))
                {
                    qDebug() << "Send:" << dictionaryMST->value("species_" + *variantFVS).key(dynamComboBoxes.at(i)->currentText());
                }
                else if(dynamComboBoxes.at(i)->currentText().contains("All affected species"))
                    qDebug() << "Send:" << dictionaryMST->value("species_" + *variantFVS).key(dynamComboBoxes.at(i)->currentText().remove(" affected"));
                else if(dictionaryMST->value("Forests_" + *variantFVS).values().contains(dynamComboBoxes.at(i)->currentText()))
                    qDebug() << "Send:" << dictionaryMST->value("Forests_" + *variantFVS).key(dynamComboBoxes.at(i)->currentText());
                else if(dictionaryMST->value("HabPa_" + *variantFVS).values().contains(dynamComboBoxes.at(i)->currentText()))
                    qDebug() << "Send:" << dictionaryMST->value("HabPa_" + *variantFVS).key(dynamComboBoxes.at(i)->currentText());
            }
            else
                qDebug() << "Send:" << dynamComboBoxes.at(i)->currentText();
        }
        for (int i = 0; i < dynamCheckBoxes.size(); i++)
        {
            qDebug() << "Check box" << i+1 << "has the value:" << dynamCheckBoxes.at(i)->isChecked() << "for \"is checked\".";
        }
        if(validInput) this->close();
    }
    else
    {
        bool doubleCheck = true;
        for (int i = 0; i < dynamLineEdits.size(); i++)
            if(!dynamLineEdits.at(i)->hasAcceptableInput())
            {
                doubleCheck = false;
                inputErrorAlert(dynamLineEdits.value(i));
            }
        if(doubleCheck)
            validInput = doubleCheck/*, acceptButton->clicked()*/;
        else
            qDebug() << "Last input by user was invalid";
    }
}

void GeneralPurposeScreenBuilder::reset()
{
    qDebug() << "Inside reset function" << resetButton->objectName();
    if(scheduleBox)
        yearCycleRButton->setChecked(true);
    qDebug() << "Reseting line edits";
    for (int i = 0; i < dynamLineEdits.size(); i++){
        dynamLineEdits.value(i)->setText(defaultLineValue.at(i));
        modifyInput(dynamLineEdits.value(i));
    }
    qDebug() << "Reseting combo boxes";
    for (int i = 0; i < dynamComboBoxes.size(); i++){
        qDebug() << dynamComboBoxes.at(i)->objectName();
        dynamComboBoxes.value(i)->setCurrentText(defaultComboValue.at(i));
    }
    qDebug() << "Reseting check boxes";
    for (int i = 0; i < defaultCheckValue.size(); i++)
    {
        qDebug() << defaultCheckValue.at(i);
        dynamCheckBoxes.value(i)->setChecked(defaultCheckValue.at(i));
    }
}

void GeneralPurposeScreenBuilder::edit()
{
    qDebug() << "Inside edit function";
}

void GeneralPurposeScreenBuilder::scheduleBoxSelection()
{
    qDebug() << "Inside scheduleBoxSelection function";
    if(yearCycleRButton->isChecked())
    {
        conditionButton->setEnabled(false);
        conditionLine->setEnabled(false);
        yearCycleLabel->setEnabled(true);
        yearCycleLine->setEnabled(true);
    }
    else if(conditionRButton->isChecked())
    {
        yearCycleLine->setEnabled(false);
        yearCycleLabel->setEnabled(false);
        conditionButton->setEnabled(true);
        conditionLine->setEnabled(true);
    }
}

//      Removes "e" and "+" from user input in number lines
void GeneralPurposeScreenBuilder::liveInputMod(QString lineEditValue)
{
    QWidget *selected = this->focusWidget();
    if(selected->objectName().remove("numberBox").toInt() <= dynamLineEdits.size())
    {
        int lineEditNum = selected->objectName().remove("numberBox").toInt()-1;
        if(lineEditValue.contains("e"))
            dynamLineEdits.value(lineEditNum)->setText(lineEditValue.remove("e"));
        if(lineEditValue.contains("+"))
            dynamLineEdits.value(lineEditNum)->setText(lineEditValue.remove("+"));
    }
}

/**** GeneralPurposeScreenBuilder::selectionChange(QWidget*from, QWidget* to) ****
 *                                                                               *
 *                 This function catches widget selection changes.               *
 *  This allows for alterations to user input to take place after the user has   *
 *  deselected the text box, warning promts to apper when needed, & much more.   *
 *  It could be used in conqunction with inputMod for all edits to user input.   *
 *                                                                               *
 *********************************************************************************/
void GeneralPurposeScreenBuilder::selectionChange(QWidget* from, QWidget* to)
{
    qDebug() << "Inside GeneralPurposeScreenBuilder::selectionChange";
    validInput = true;
    if(from != NULL && to != NULL && to != resetButton && to != cancelButton)
        if(from->inherits("QLineEdit"))
        {
            qDebug() << "Selection changed from" << from->objectName() << "to" << to->objectName();
            QLineEdit *input = qobject_cast<QLineEdit*>(from);
            if(input->validator() != 0)
            {
                QString inputString = input->text();
                qDebug() << input->validator()->objectName() << "was previously selected";
                if(input->validator()->objectName().contains("integer"))
                {
                    qDebug() << "int input located";
                    if(inputString.contains("."))
                    {// removes errant decimal point
                        inputString.truncate(inputString.indexOf("."));
                        input->setText(inputString);
                    }
                }
                else
                    if(!inputString.contains(".")) // if double does not contain decimal point, add one to end to avoid FVS interpretation as "implied decimal point"
                        input->setText(inputString + ".");
                if(input->hasAcceptableInput())
                {
                    qDebug() << "Valid user input for" << from->objectName();
                    modifyInput(input);
                }
                else
                {
                    validInput = false;
                    qDebug() << "Intermediate or invalid user input.";
                    if(input->validator()->property("bottom").isValid()) // executes code if input's validator has the "bottom" function (doc.qt.io/qt-5/qobject.html#property)
                        emit inputError(input);
                }
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
    yearCycleRButton->setObjectName("YearRadio");
    conditionRButton->setObjectName("ConditionRadio");
    yearCycleLine = new QLineEdit;
    conditionLine = new QLineEdit;
    dynamLineEdits.append(yearCycleLine);
    dynamLineEdits.append(conditionLine);
    yearCycleLabel = new QLabel;
    conditionButton = new QPushButton;
    yearCycleLabel->setText(" Select Year ");
    yearCycleLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    conditionLine->setObjectName("scheduleBoxCondition");
    yearCycleLine->setObjectName("scheduleBoxYear");
    yearCycleLine->setText(QString::number(*year));
    defaultLineValue.append(QString::number(*year));                 // for yearCycleLine
    yearCycleLine->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    conditionButton->setText(" years after Condition is met ");
    conditionButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    conditionButton->setEnabled(false);
    defaultLineValue.append("0");                    // for conditionLine
    conditionLine->setText("0");
    conditionLine->setEnabled(false);
    conditionLine->setMaximumSize(conditionLine->sizeHint()*4);
    conditionLine->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    yearCycleRButton->setChecked(true);
    yearCycleRButton->setText("Schedule by Year/Cycle");
    conditionRButton->setText("Schedule by Condition");
    dynamicBody->addRow(yearCycleRButton, conditionRButton);
    selectYear->addRow(yearCycleLine, yearCycleLabel);
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
    acceptButton->setObjectName("Ok");
    resetButton->setObjectName("Reset");
    cancelButton->setObjectName("Cancel");
    editButton->setObjectName("Use Editor");
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(reset()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(edit()));
}

void GeneralPurposeScreenBuilder::createSpecialSelectionComboBox(QString type)
{
    qDebug() << "Inside GeneralPurposeScreenBuilder::createSpecialSelectionComboBox";
    qDebug() << type + *variantFVS;
    qDebug() << dictionaryMST->value(type + *variantFVS).keys();
    type == "species_" ? defaultComboValue.append("All species"):defaultComboValue.append(""); // for reset
    QComboBox *selectionComboBox = new QComboBox;
    QStringListModel *selectionModel = new QStringListModel;
    selectionModel->setStringList(dictionaryMST->value(type + *variantFVS).values());
    selectionModel->sort(0);
    selectionComboBox->setModel(selectionModel);
    selectionComboBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    selectionComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    selectionComboBox->setMinimumHeight(24);
    dynamComboBoxes.append(selectionComboBox);
    // lambda expression for selection changes
    connect(selectionComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](){qDebug() << selectionComboBox->objectName() << "selection:" << selectionComboBox->currentText() << dictionaryMST->value(type + *variantFVS).key(selectionComboBox->currentText());});
}

/******** GeneralPurposeScreenBuilder::inputErrorAlert(QLineEdit *input) *********
 *                                                                               *
 *               This function notifies user of incorrect input.                 *
 *      When the user enters input outside of the required bounds, the text      *
 *   contained within the box temporarily displays the inclusive boundry, its    *
 *  font is set to red and the window's acceptButton is disabled until the user  *
 *  enters acceptable input. The modifyInput function reverts the font to black. *
 *      If QLineEdit is blank and 0 is valid input, modifyInput() is called.     *
 *                                                                               *
 *********************************************************************************/
void GeneralPurposeScreenBuilder::inputErrorAlert(QLineEdit *input)
{
    qDebug() << "Input Error Alert!";
    qDebug() << input->text();
    if(!input->text().contains("Bound from "))
    {
        QString inputString;
        bool blank = ((input->text().size() == 0) || (input->text() == ".")) ? true : false;
        blank ? inputString = "0" : inputString = input->text();
        double userInput = inputString.toDouble();
        qDebug() << "Inside inputErrorAlert function with lineEdit " + input->objectName() + " and value" << inputString;
        if(input->objectName().remove("numberBox").toInt() <= dynamLineEdits.size())
        {
            double high = input->validator()->property("top").toDouble();
            double low  = input->validator()->property("bottom").toDouble();
            if((!blank || low > 0) && !input->hasAcceptableInput())
            {
                QApplication::beep();
                QPalette palette;
                palette.setColor(QPalette::Base, Qt::black);
                palette.setColor(QPalette::Text, Qt::red);
                acceptButton->setDisabled(true);
                input->setPalette(palette);
                qDebug() << "Input is inclusively bound from"  << numberToQString(low) << "to" << numberToQString(high);
                qDebug() << "User Input number:" << numberToQString(userInput);
                input->setMaxLength(36);
                input->setText("Bound from "  + numberToQString(low) + " to " + numberToQString(high));
                QTimer::singleShot(1000, Qt::CoarseTimer, this, [=]()->void {input->setText(inputString);}); // lambda
                input->setFocus();
            }
            else
                modifyInput(input);
        }
        else
            qDebug() << "Error in inputErrorAlert";
    }
}

// Ensures the font color is black, acceptButton is enabled, and 0 favored syntax
void GeneralPurposeScreenBuilder::modifyInput(QLineEdit *input)
{
    if(input->validator() != 0)
    {
        QString inputString = input->text();
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::white);
        palette.setColor(QPalette::Text, Qt::black);
        acceptButton->setDisabled(false);
        input->setPalette(palette);
        input->setMaxLength(14);
        double userInput = inputString.toDouble();
        qDebug() << "Inside modifyInput function with lineEdit " + input->objectName() + " and value" << inputString;
        if(userInput == 0)
            input->setText("0");
        if(input->validator()->objectName().contains("double") && !input->text().contains("."))
            input->setText(input->text() + ".0");
    }
}

void GeneralPurposeScreenBuilder::noInputRemovalCheck(QFormLayout *dynamBody, QString fieldNum)
{// removes previous entry from dynamBody if it was "noInput" of same fieldNum
    QString objectName = dynamBody->itemAt(dynamBody->count() - 1)->widget()->objectName();
    if(objectName.contains("noInput"+fieldNum))
    {
        dynamBody->removeRow(dynamBody->rowCount() - 1);
        qDebug()<< "noInput removed for " << fieldNum;
    }
}

bool GeneralPurposeScreenBuilder::addDynamComboBox(QStringList comboBoxProperties, QFormLayout *dynamBody, QLabel *tempLabel, QString fieldNum)
{// adds a combo box to the dynamBody if the field number is unique
    bool fieldAdded = true, newLineCharFound = false;
    QString selected = "NO CURRENT SELECTION";
    QStringList comboBoxPropertiesActual;
    QString newLine = "\\n";
    foreach (QString option, comboBoxProperties) {
        if(option.contains(newLine)) newLineCharFound = true; // take note of newline character discovery
    }//replace with if(comboBoxProperties.contains(newLine)?
    if(newLineCharFound)
    {// break each line containing "\n" into multiple comboBox options
        QStringList newLineCatcher;
        for(int i = 0; i < comboBoxProperties.size(); i++)
        {
            if(i == 0 && currentField->contains(">") && QString((comboBoxProperties.first())).contains(newLine)) // catches "\n" in designated selection
                comboBoxProperties.value(i) = QString(comboBoxProperties.value(i)).left(QString(comboBoxProperties.value(i)).indexOf(newLine) - 1);
            if(QString(comboBoxProperties.value(i)).contains(newLine))
            {
                qDebug() << "New Line found inside comboBox line:" << QString(comboBoxProperties.value(i));
                newLineCatcher = QString(comboBoxProperties.value(i)).split(newLine);
                for(int j = 0; j < newLineCatcher.size(); j++)
                {
                    if(QString(newLineCatcher.value(j)).contains(QRegularExpression("\\w")))
                        while(QString(newLineCatcher.value(j)).at(0) == " ") // if line contains information, remove any preceeding blank spaces
                            newLineCatcher.replace(j, QString(newLineCatcher.value(j)).remove(0, 1));
                    else if(QString(newLineCatcher.value(j)).size() == 0)
                        newLineCatcher.replace(j, " ");
                    if(QString(newLineCatcher.value(j)).at(0) == ">")
                    {
                        qDebug() << "Combo Box selection specified as" << QString(newLineCatcher.value(j)).mid(QString(newLineCatcher.value(j)).indexOf(">")+1);
                        if(QString(newLineCatcher.value(j)).size() > QString(newLineCatcher.value(j)).indexOf(">")+1)
                        {
                            selected = QString(newLineCatcher.value(j)).mid(QString(newLineCatcher.value(j)).indexOf(">")+1);
                            while(selected.at(0) == " ") selected.remove(0, 1);
                            comboBoxPropertiesActual.append(selected);
                        }
                        else
                        {
                            comboBoxPropertiesActual.prepend(" ");
                        }
                        (QString(newLineCatcher.value(j)).size() > QString(newLineCatcher.value(j)).indexOf(">")+1) ? qDebug() << *currentField << "value to be selected:" << QString(newLineCatcher.value(j)).mid(QString(newLineCatcher.value(j)).indexOf(">")+1) : qDebug() << "Blank longListButton value to be selected";
                    }
                    else
                    {
                        comboBoxPropertiesActual.append(QString(newLineCatcher.value(j)));
                    }
                }
            }
            qDebug() << "Entry for comboBox item:" << QString(comboBoxPropertiesActual.value(i));
        }
    }
    else
    {
        foreach (QString option, comboBoxProperties) {
            if(option.at(0) == ">")
            {
                if(option.size() > option.indexOf(">")+1)
                {
                    selected = option.mid(option.indexOf(">")+1);
                    while(selected.at(0) == " ") selected.remove(0, 1);
                    comboBoxPropertiesActual.append(selected);
                }
                else
                    comboBoxPropertiesActual.prepend(" ");
            }
            else
                comboBoxPropertiesActual.append(option);
        }
    }
    if(selected == "NO CURRENT SELECTION")
        selected = comboBoxPropertiesActual.first();
    qDebug() << "Selected" << *currentField << "value:" << selected;
    QStringListModel *comboBoxPropertiesModel = new QStringListModel;
    comboBoxPropertiesModel->setStringList(comboBoxPropertiesActual);
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
//        dynamComboBoxes.value(comboBoxLocation)->setFont(*font);
        dynamComboBoxes.value(comboBoxLocation)->setObjectName(fieldNum);
        dynamComboBoxes.value(comboBoxLocation)->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        (currentField->contains("long")) ? dynamBody->addRow(dynamComboBoxes.value(comboBoxLocation)) : dynamBody->addRow(tempLabel, dynamComboBoxes.value(comboBoxLocation));
        defaultComboValue.append(selected);
    }
    else
        fieldAdded = false;
    return fieldAdded;
}

QString GeneralPurposeScreenBuilder::numberToQString(double number)
{
    QString result = QString::number(number, 'f', 9); // format the number in standard notation with 9 decimals of precision
    if(number == 0)
        result = "0";
    else if(result.endsWith("0"))
    {
        while(result.endsWith("0"))
            result.remove(result.length()-1, 1);

        if(result.endsWith("."))
            result.remove(result.length()-1, 1);
    }
    return result;
}
