#ifndef GENERALPURPOSESCREENBUILDER_H
#define GENERALPURPOSESCREENBUILDER_H

#include <QDialog>
#include <QQueue>
#include <QVector>
#include <QSettings>
#include <QListView>
#include <QComboBox>
#include <QFormLayout>
#include <QStringList>
#include <QRadioButton>
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
    GeneralPurposeScreenBuilder(QString keywordExtension, QStringList description, QStringList MSText, QString *variant, QMap<QString, QMap<QString, QString>> *speciesMSTAbbreviationName, QMap<QString, QString> *variantAbbreviationNames, int startYear, QWidget *parent = 0);
    bool addDynamComboBox(QStringList comboBoxProperties, QFormLayout *dynamBody, QLabel *tempLabel, QString fieldNum);
//    bool variantListCheck(QString &line, QStringList &variantList, QString &fieldNum = null);
    ~GeneralPurposeScreenBuilder();
private slots:
    void accept();
    void reset();
    void edit();
    void scheduleBoxSelection();
    void speciesComboBoxSelection();
    void inputMod(QString lineEditValue);

private:
    void createButtonBox();
    void createScheduleBox(QFormLayout *dynamicBody);
    void createSpeciesSelectionComboBox(QString fieldDesc);
    void noInputRemovalCheck(QFormLayout *dynamicBody, QString fieldNum);

    // Generic
    int *year;
    QFont *font;
    QLineEdit *title;
    QString *variantFVS;
    QString *currentField;
    QGridLayout *mainLayout;
    QVector<QString> defaultLineValue;
    QVector<QString> defaultComboValue;
    QVector<QLineEdit *> dynamLineEdits;
    QVector<QComboBox *> dynamComboBoxes;

    // Button Box
    QDialogButtonBox *buttonBox;
    QPushButton *acceptButton, *editButton, *resetButton, *cancelButton;

    // Species Selection
    bool speciesSelection = false;
    QLabel *speciesSelectionQLabel;
    QComboBox *speciesSelectionComboBox;
    QStringListModel *speciesSelectionModel;
    QMap<QString, QMap<QString, QString>> *speciesMSTAN;

    // Schedule Box
    QLabel *yearCycleLabel;
    bool scheduleBox = false;
    QWidget *scheduleBoxWidget;
    QPushButton *conditionButton;
    QLineEdit *yearCycleLine, *conditionLine;
    QRadioButton *yearCycleRButton, *conditionRButton;
};

#endif // GENERALPURPOSESCREENBUILDER_H
