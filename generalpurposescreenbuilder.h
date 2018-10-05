#ifndef GENERALPURPOSESCREENBUILDER_H
#define GENERALPURPOSESCREENBUILDER_H

#include <QTimer>
#include <QQueue>
#include <QVector>
#include <QDialog>
#include <QSettings>
#include <QCheckBox>
#include <QListView>
#include <QComboBox>
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
    GeneralPurposeScreenBuilder(QString windowTitle, QString description, QWidget *parent = 0);
    GeneralPurposeScreenBuilder(QString window, QString category, QStringList MSText, int startYear, QWidget *parent = 0);
    GeneralPurposeScreenBuilder(QString keywordExtension, QStringList description, QStringList MSText, QString *variant, int startYear, QWidget *parent = 0);
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
    void noInputRemovalCheck(QFormLayout *dynamicBody, QString fieldNum);

    // Generic
    bool validInput;
    int *year = NULL;
    QLineEdit *title = NULL;
    QString *variantFVS = NULL;
    QString *currentField = NULL;
    QVector<bool> defaultCheckValue;
    QVector<QString> defaultLineValue;
    QVector<QString> defaultComboValue;
    QVector<QLineEdit *> dynamLineEdits;
    QVector<QComboBox *> dynamComboBoxes;
    QVector<QCheckBox *> dynamCheckBoxes;

    // Button Box
    QDialogButtonBox *buttonBox;
    QPushButton *acceptButton, *editButton, *resetButton, *cancelButton;

    // Schedule Box
    QLabel *yearCycleLabel = NULL;
    QWidget *scheduleBoxWidget = NULL;
    QPushButton *conditionButton = NULL;
    QLineEdit *yearCycleLine = NULL, *conditionLine = NULL;
    QRadioButton *yearCycleRButton = NULL, *conditionRButton = NULL;

    // Special Window
    QVector<QRadioButton *> dynamRadioButtons;
//    QVector<QLabel *> dynamLables;
};

#endif // GENERALPURPOSESCREENBUILDER_H
