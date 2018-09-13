#ifndef GENERALPURPOSESCREENBUILDER_H
#define GENERALPURPOSESCREENBUILDER_H

#include <QTimer>
#include <QQueue>
#include <QVector>
#include <QDialog>
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
    GeneralPurposeScreenBuilder(QString keywordExtension, QStringList description, QStringList MSText, QString *variant, QMap<QString, QMap<QString, QString>> *speciesMSTAbbreviationName, QMap<QString, QMap<QString, QString>> *hapPaMSTNumAbbrev, QMap<QString, QMap<QString, QString>> *forestMSTNumberName, int startYear, QWidget *parent = 0);
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
    void habPaComboBoxSelection();
    void forestComboBoxSelection();
    void speciesComboBoxSelection();
    void inputErrorAlert(QLineEdit *input);
    void liveInputMod(QString lineEditValue);
    void selectionChange(QWidget *from, QWidget *to);

private:
    void createButtonBox();
    void modifyInput(QLineEdit *input);
    void createScheduleBox(QFormLayout *dynamicBody);
    void createHabPaSelectionComboBox(QString fieldDesc);
    void createForestSelectionComboBox(QString fieldDesc);
    void createSpeciesSelectionComboBox(QString fieldDesc);
    void noInputRemovalCheck(QFormLayout *dynamicBody, QString fieldNum);

    // Generic
    int *year;
    QFont *font;
    bool validInput;
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

    // Habitat Type Plant Association Selection
    bool habPaSelection = false;
    QLabel *habPaSelectionQLabel;
    QComboBox *habPaSelectionComboBox;
    QStringListModel *habPaSelectionModel;
    QMap<QString, QMap<QString, QString>> *habPaMSTNA;

    // Forest Selection
    bool forestSelection = false;
    QLabel *forestSelectionQLabel;
    QComboBox *forestSelectionComboBox;
    QStringListModel *forestSelectionModel;
    QMap<QString, QMap<QString, QString>> *forestMSTNN;

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
