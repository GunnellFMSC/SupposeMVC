#pragma once

#include <QTimer>
#include <QQueue>
#include <QVector>
#include <QDialog>
#include <QSettings>
#include <QCheckBox>
#include <QListView>
#include <QComboBox>
#include "variant.h"
#include <QFormLayout>
#include <QStringList>
#include <QRadioButton>
#include "supposefont.h"
#include "dictionarymst.h"
#include <QStringListModel>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QGridLayout;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class GeneralPurposeScreenBuilder : public QDialog
{
    Q_OBJECT

public:
    GeneralPurposeScreenBuilder(QString windowTitle, QString description, QWidget *parent = nullptr);
    GeneralPurposeScreenBuilder(QString window, QString category, QStringList MSText, int startYear, QWidget *parent = nullptr);
    GeneralPurposeScreenBuilder(QString keywordExtension, QStringList description, QStringList MSText, int startYear, QWidget *parent = nullptr);
    bool addDynamComboBox(QStringList comboBoxProperties, QFormLayout *dynamBody, QLabel *tempLabel, QString fieldNum);
    QString numberToQString(double number);
    ~GeneralPurposeScreenBuilder();

signals:
    void inputError(QLineEdit *input);

private slots:
    void edit();
    void reset();
    void accept();
    void scheduleBoxSelection();
    void inputErrorAlert(QLineEdit *input);
    void liveInputMod(QString lineEditValue);
    void selectionChange(QWidget *from, QWidget *to);

private:
    void createButtonBox();
    void modifyInput(QLineEdit *input);
    void createScheduleBox(QFormLayout *dynamicBody);
    void createSpecialSelectionComboBox(QString type);// for Selection of Species, Habitat Type Plant Association, Forests
    void createPlantNaturalBox(QFormLayout *dynamicBody);
    void noInputRemovalCheck(QFormLayout *dynamicBody, QString fieldNum);
    void parseForm(int formIndex, QStringList &resultStrings, QVector<QString> acceptedInput);

    // Generic
    int *year = nullptr;
    bool validInput, parmsFunc;
    QString *currentField = nullptr;
    QVector<bool> defaultCheckValue;
    QVector<QString> parmsAnswerForm;
    QVector<QString> defaultLineValue;
    QVector<QString> defaultComboValue;
    QVector<QLineEdit *> dynamLineEdits;
    QVector<QComboBox *> dynamComboBoxes;
    QVector<QCheckBox *> dynamCheckBoxes;
    QMap<QString, QString> dynamArrayValue;

    // Button Box
    QDialogButtonBox *buttonBox;
    QPushButton *acceptButton, *editButton, *resetButton, *cancelButton;

    // Schedule Box
    QLabel *yearCycleLabel = nullptr;
    QWidget *scheduleBoxWidget = nullptr;
    QPushButton *conditionButton = nullptr;
    QLineEdit *yearCycleLine = nullptr, *conditionLine = nullptr;
    QRadioButton *yearCycleRButton = nullptr, *conditionRButton = nullptr;

    // Special Window
    QVector<QRadioButton *> dynamRadioButtons;
//    QVector<QLabel *> dynamLables;
};

//class NumberBox : public QLineEdit
//{
//public:
//    QValidator *boundaries;

//}
