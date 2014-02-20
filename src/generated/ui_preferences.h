/********************************************************************************
** Form generated from reading UI file 'preferences.ui'
**
** Created: Wed Feb 5 19:08:10 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCES_H
#define UI_PREFERENCES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "colorlist.h"

QT_BEGIN_NAMESPACE

class Ui_PreferencesDialog
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tabGeneral;
    QGridLayout *gridLayout1;
    QComboBox *boardThemeCombo;
    QSpacerItem *spacerItem;
    QHBoxLayout *hboxLayout;
    QComboBox *pieceThemeCombo;
    QComboBox *pieceEffect;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *guessMoveCheck;
    QCheckBox *guessNextMove;
    QHBoxLayout *horizontalLayout_2;
    QLabel *labelWheel;
    QSpacerItem *horizontalSpacer;
    QLabel *labelFast;
    QSlider *minWheelCount;
    QLabel *label_10;
    QLabel *boardThemeLabel;
    ColorList *boardColorsList;
    QLabel *label;
    QCheckBox *boardFrameCheck;
    QLabel *pieceThemeLabel;
    QCheckBox *hilightCurrentMove;
    QWidget *tabAutoplay;
    QGridLayout *gridLayout_2;
    QGroupBox *gbBoardOptions;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_14;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_15;
    QSlider *autoPlayInterval;
    QLabel *label_16;
    QCheckBox *cbSaveAndContinue;
    QSpacerItem *verticalSpacer_2;
    QWidget *tabEngines;
    QGridLayout *gridLayout2;
    QWidget *engineEditWidget;
    QGridLayout *gridLayout3;
    QLineEdit *engineCommand;
    QLineEdit *engineDirectory;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_6;
    QLabel *label_5;
    QToolButton *commandButton;
    QLabel *label_3;
    QLineEdit *engineOptions;
    QRadioButton *engineProtocolUnknown;
    QToolButton *engineOptionMore;
    QToolButton *directoryButton;
    QSpacerItem *spacerItem1;
    QLineEdit *engineName;
    QRadioButton *engineProtocolUSHI;
    QGridLayout *gridLayout5;
    QPushButton *engineDownButton;
    QListWidget *engineList;
    QPushButton *deleteEngineButton;
    QPushButton *addEngineButton;
    QSpacerItem *horizontalSpace1;
    QPushButton *engineUpButton;
    QWidget *tabAdvanced;
    QVBoxLayout *verticalLayout1;
    QCheckBox *tablebaseCheck;
    QCheckBox *automaticECO;
    QCheckBox *useIndexFile;
    QCheckBox *cbAutoCommitDB;
    QGridLayout *gridLayout4;
    QSpinBox *limitSpin;
    QLabel *label_7;
    QLabel *label_8;
    QSpinBox *spinBoxRecentFiles;
    QLabel *label_9;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *defaultDataBasePath;
    QPushButton *browsePathButton;
    QSpacerItem *verticalSpacer1;
    QWidget *tabFonts;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *labelGameTextFontSize;
    QSpinBox *gameTextFontSizeSpin;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelListFontSize;
    QSpinBox *spinBoxListFontSize;
    QCheckBox *verticalTabs;
    QGroupBox *groupBox_2;
    QRadioButton *notationNumeric;
    QRadioButton *notationAlgebraic;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem2;
    QPushButton *okButton;
    QPushButton *applyButton;
    QPushButton *resetButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *PreferencesDialog)
    {
        if (PreferencesDialog->objectName().isEmpty())
            PreferencesDialog->setObjectName(QString::fromUtf8("PreferencesDialog"));
        PreferencesDialog->resize(553, 538);
        PreferencesDialog->setModal(true);
        gridLayout = new QGridLayout(PreferencesDialog);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(PreferencesDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setAutoFillBackground(false);
        tabGeneral = new QWidget();
        tabGeneral->setObjectName(QString::fromUtf8("tabGeneral"));
        gridLayout1 = new QGridLayout(tabGeneral);
        gridLayout1->setSpacing(6);
        gridLayout1->setContentsMargins(11, 11, 11, 11);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        boardThemeCombo = new QComboBox(tabGeneral);
        boardThemeCombo->setObjectName(QString::fromUtf8("boardThemeCombo"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(boardThemeCombo->sizePolicy().hasHeightForWidth());
        boardThemeCombo->setSizePolicy(sizePolicy);

        gridLayout1->addWidget(boardThemeCombo, 1, 2, 1, 1);

        spacerItem = new QSpacerItem(20, 150, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout1->addItem(spacerItem, 6, 0, 1, 1);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        pieceThemeCombo = new QComboBox(tabGeneral);
        pieceThemeCombo->setObjectName(QString::fromUtf8("pieceThemeCombo"));
        sizePolicy.setHeightForWidth(pieceThemeCombo->sizePolicy().hasHeightForWidth());
        pieceThemeCombo->setSizePolicy(sizePolicy);

        hboxLayout->addWidget(pieceThemeCombo);

        pieceEffect = new QComboBox(tabGeneral);
        pieceEffect->setObjectName(QString::fromUtf8("pieceEffect"));
        sizePolicy.setHeightForWidth(pieceEffect->sizePolicy().hasHeightForWidth());
        pieceEffect->setSizePolicy(sizePolicy);

        hboxLayout->addWidget(pieceEffect);


        gridLayout1->addLayout(hboxLayout, 0, 2, 1, 1);

        groupBox = new QGroupBox(tabGeneral);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        groupBox->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        guessMoveCheck = new QCheckBox(groupBox);
        guessMoveCheck->setObjectName(QString::fromUtf8("guessMoveCheck"));

        verticalLayout_2->addWidget(guessMoveCheck);

        guessNextMove = new QCheckBox(groupBox);
        guessNextMove->setObjectName(QString::fromUtf8("guessNextMove"));

        verticalLayout_2->addWidget(guessNextMove);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        labelWheel = new QLabel(groupBox);
        labelWheel->setObjectName(QString::fromUtf8("labelWheel"));

        horizontalLayout_2->addWidget(labelWheel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        labelFast = new QLabel(groupBox);
        labelFast->setObjectName(QString::fromUtf8("labelFast"));

        horizontalLayout_2->addWidget(labelFast);

        minWheelCount = new QSlider(groupBox);
        minWheelCount->setObjectName(QString::fromUtf8("minWheelCount"));
        minWheelCount->setMaximum(240);
        minWheelCount->setSingleStep(60);
        minWheelCount->setPageStep(60);
        minWheelCount->setOrientation(Qt::Horizontal);
        minWheelCount->setInvertedAppearance(false);
        minWheelCount->setTickPosition(QSlider::TicksBothSides);
        minWheelCount->setTickInterval(60);

        horizontalLayout_2->addWidget(minWheelCount);

        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_2->addWidget(label_10);


        verticalLayout_2->addLayout(horizontalLayout_2);


        gridLayout1->addWidget(groupBox, 9, 0, 1, 3);

        boardThemeLabel = new QLabel(tabGeneral);
        boardThemeLabel->setObjectName(QString::fromUtf8("boardThemeLabel"));

        gridLayout1->addWidget(boardThemeLabel, 1, 0, 1, 1);

        boardColorsList = new ColorList(tabGeneral);
        boardColorsList->setObjectName(QString::fromUtf8("boardColorsList"));

        gridLayout1->addWidget(boardColorsList, 2, 2, 1, 1);

        label = new QLabel(tabGeneral);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout1->addWidget(label, 2, 0, 1, 2);

        boardFrameCheck = new QCheckBox(tabGeneral);
        boardFrameCheck->setObjectName(QString::fromUtf8("boardFrameCheck"));

        gridLayout1->addWidget(boardFrameCheck, 3, 2, 1, 1);

        pieceThemeLabel = new QLabel(tabGeneral);
        pieceThemeLabel->setObjectName(QString::fromUtf8("pieceThemeLabel"));

        gridLayout1->addWidget(pieceThemeLabel, 0, 0, 1, 1);

        hilightCurrentMove = new QCheckBox(tabGeneral);
        hilightCurrentMove->setObjectName(QString::fromUtf8("hilightCurrentMove"));

        gridLayout1->addWidget(hilightCurrentMove, 4, 2, 1, 1);

        tabWidget->addTab(tabGeneral, QString());
        tabAutoplay = new QWidget();
        tabAutoplay->setObjectName(QString::fromUtf8("tabAutoplay"));
        gridLayout_2 = new QGridLayout(tabAutoplay);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gbBoardOptions = new QGroupBox(tabAutoplay);
        gbBoardOptions->setObjectName(QString::fromUtf8("gbBoardOptions"));
        verticalLayout_5 = new QVBoxLayout(gbBoardOptions);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_14 = new QLabel(gbBoardOptions);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        horizontalLayout_8->addWidget(label_14);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_3);

        label_15 = new QLabel(gbBoardOptions);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        horizontalLayout_8->addWidget(label_15);

        autoPlayInterval = new QSlider(gbBoardOptions);
        autoPlayInterval->setObjectName(QString::fromUtf8("autoPlayInterval"));
        autoPlayInterval->setMinimum(1000);
        autoPlayInterval->setMaximum(10000);
        autoPlayInterval->setSingleStep(1000);
        autoPlayInterval->setPageStep(5000);
        autoPlayInterval->setValue(3000);
        autoPlayInterval->setSliderPosition(3000);
        autoPlayInterval->setOrientation(Qt::Horizontal);
        autoPlayInterval->setTickPosition(QSlider::TicksBothSides);
        autoPlayInterval->setTickInterval(3000);

        horizontalLayout_8->addWidget(autoPlayInterval);

        label_16 = new QLabel(gbBoardOptions);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        horizontalLayout_8->addWidget(label_16);


        verticalLayout_5->addLayout(horizontalLayout_8);

        cbSaveAndContinue = new QCheckBox(gbBoardOptions);
        cbSaveAndContinue->setObjectName(QString::fromUtf8("cbSaveAndContinue"));

        verticalLayout_5->addWidget(cbSaveAndContinue);


        gridLayout_2->addWidget(gbBoardOptions, 0, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 1, 0, 1, 1);

        tabWidget->addTab(tabAutoplay, QString());
        tabEngines = new QWidget();
        tabEngines->setObjectName(QString::fromUtf8("tabEngines"));
        gridLayout2 = new QGridLayout(tabEngines);
        gridLayout2->setSpacing(6);
        gridLayout2->setContentsMargins(11, 11, 11, 11);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        engineEditWidget = new QWidget(tabEngines);
        engineEditWidget->setObjectName(QString::fromUtf8("engineEditWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(engineEditWidget->sizePolicy().hasHeightForWidth());
        engineEditWidget->setSizePolicy(sizePolicy2);
        gridLayout3 = new QGridLayout(engineEditWidget);
        gridLayout3->setSpacing(6);
        gridLayout3->setContentsMargins(11, 11, 11, 11);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        engineCommand = new QLineEdit(engineEditWidget);
        engineCommand->setObjectName(QString::fromUtf8("engineCommand"));
        engineCommand->setMinimumSize(QSize(300, 0));

        gridLayout3->addWidget(engineCommand, 1, 1, 1, 3);

        engineDirectory = new QLineEdit(engineEditWidget);
        engineDirectory->setObjectName(QString::fromUtf8("engineDirectory"));
        engineDirectory->setMinimumSize(QSize(300, 0));

        gridLayout3->addWidget(engineDirectory, 3, 1, 1, 3);

        label_2 = new QLabel(engineEditWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout3->addWidget(label_2, 3, 0, 1, 1);

        label_4 = new QLabel(engineEditWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy3(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy3);

        gridLayout3->addWidget(label_4, 4, 0, 1, 1);

        label_6 = new QLabel(engineEditWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout3->addWidget(label_6, 0, 0, 1, 1);

        label_5 = new QLabel(engineEditWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout3->addWidget(label_5, 2, 0, 1, 1);

        commandButton = new QToolButton(engineEditWidget);
        commandButton->setObjectName(QString::fromUtf8("commandButton"));
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(commandButton->sizePolicy().hasHeightForWidth());
        commandButton->setSizePolicy(sizePolicy4);

        gridLayout3->addWidget(commandButton, 1, 4, 1, 1);

        label_3 = new QLabel(engineEditWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout3->addWidget(label_3, 1, 0, 1, 1);

        engineOptions = new QLineEdit(engineEditWidget);
        engineOptions->setObjectName(QString::fromUtf8("engineOptions"));

        gridLayout3->addWidget(engineOptions, 2, 1, 1, 4);

        engineProtocolUnknown = new QRadioButton(engineEditWidget);
        engineProtocolUnknown->setObjectName(QString::fromUtf8("engineProtocolUnknown"));

        gridLayout3->addWidget(engineProtocolUnknown, 4, 2, 1, 1);

        engineOptionMore = new QToolButton(engineEditWidget);
        engineOptionMore->setObjectName(QString::fromUtf8("engineOptionMore"));
        sizePolicy4.setHeightForWidth(engineOptionMore->sizePolicy().hasHeightForWidth());
        engineOptionMore->setSizePolicy(sizePolicy4);

        gridLayout3->addWidget(engineOptionMore, 4, 4, 1, 1);

        directoryButton = new QToolButton(engineEditWidget);
        directoryButton->setObjectName(QString::fromUtf8("directoryButton"));
        sizePolicy4.setHeightForWidth(directoryButton->sizePolicy().hasHeightForWidth());
        directoryButton->setSizePolicy(sizePolicy4);
        directoryButton->setLayoutDirection(Qt::LeftToRight);

        gridLayout3->addWidget(directoryButton, 3, 4, 1, 1);

        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout3->addItem(spacerItem1, 4, 3, 1, 1);

        engineName = new QLineEdit(engineEditWidget);
        engineName->setObjectName(QString::fromUtf8("engineName"));

        gridLayout3->addWidget(engineName, 0, 1, 1, 4);

        engineProtocolUSHI = new QRadioButton(engineEditWidget);
        engineProtocolUSHI->setObjectName(QString::fromUtf8("engineProtocolUSHI"));

        gridLayout3->addWidget(engineProtocolUSHI, 4, 1, 1, 1);


        gridLayout2->addWidget(engineEditWidget, 1, 0, 1, 1);

        gridLayout5 = new QGridLayout();
        gridLayout5->setSpacing(6);
        gridLayout5->setObjectName(QString::fromUtf8("gridLayout5"));
        engineDownButton = new QPushButton(tabEngines);
        engineDownButton->setObjectName(QString::fromUtf8("engineDownButton"));

        gridLayout5->addWidget(engineDownButton, 3, 1, 1, 1);

        engineList = new QListWidget(tabEngines);
        engineList->setObjectName(QString::fromUtf8("engineList"));
        engineList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        engineList->setDragEnabled(false);
        engineList->setMovement(QListView::Static);
        engineList->setSortingEnabled(false);

        gridLayout5->addWidget(engineList, 0, 0, 5, 1);

        deleteEngineButton = new QPushButton(tabEngines);
        deleteEngineButton->setObjectName(QString::fromUtf8("deleteEngineButton"));

        gridLayout5->addWidget(deleteEngineButton, 1, 1, 1, 1);

        addEngineButton = new QPushButton(tabEngines);
        addEngineButton->setObjectName(QString::fromUtf8("addEngineButton"));

        gridLayout5->addWidget(addEngineButton, 0, 1, 1, 1);

        horizontalSpace1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout5->addItem(horizontalSpace1, 4, 1, 1, 1);

        engineUpButton = new QPushButton(tabEngines);
        engineUpButton->setObjectName(QString::fromUtf8("engineUpButton"));

        gridLayout5->addWidget(engineUpButton, 2, 1, 1, 1);


        gridLayout2->addLayout(gridLayout5, 0, 0, 1, 1);

        tabWidget->addTab(tabEngines, QString());
        tabAdvanced = new QWidget();
        tabAdvanced->setObjectName(QString::fromUtf8("tabAdvanced"));
        verticalLayout1 = new QVBoxLayout(tabAdvanced);
        verticalLayout1->setSpacing(6);
        verticalLayout1->setContentsMargins(11, 11, 11, 11);
        verticalLayout1->setObjectName(QString::fromUtf8("verticalLayout1"));
        tablebaseCheck = new QCheckBox(tabAdvanced);
        tablebaseCheck->setObjectName(QString::fromUtf8("tablebaseCheck"));

        verticalLayout1->addWidget(tablebaseCheck);

        automaticECO = new QCheckBox(tabAdvanced);
        automaticECO->setObjectName(QString::fromUtf8("automaticECO"));

        verticalLayout1->addWidget(automaticECO);

        useIndexFile = new QCheckBox(tabAdvanced);
        useIndexFile->setObjectName(QString::fromUtf8("useIndexFile"));

        verticalLayout1->addWidget(useIndexFile);

        cbAutoCommitDB = new QCheckBox(tabAdvanced);
        cbAutoCommitDB->setObjectName(QString::fromUtf8("cbAutoCommitDB"));

        verticalLayout1->addWidget(cbAutoCommitDB);

        gridLayout4 = new QGridLayout();
        gridLayout4->setSpacing(6);
        gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
        limitSpin = new QSpinBox(tabAdvanced);
        limitSpin->setObjectName(QString::fromUtf8("limitSpin"));
        limitSpin->setMinimum(0);
        limitSpin->setMaximum(150);
        limitSpin->setValue(10);

        gridLayout4->addWidget(limitSpin, 1, 1, 1, 1);

        label_7 = new QLabel(tabAdvanced);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout4->addWidget(label_7, 1, 0, 1, 1);

        label_8 = new QLabel(tabAdvanced);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout4->addWidget(label_8, 2, 0, 1, 1);

        spinBoxRecentFiles = new QSpinBox(tabAdvanced);
        spinBoxRecentFiles->setObjectName(QString::fromUtf8("spinBoxRecentFiles"));
        spinBoxRecentFiles->setMaximum(10);
        spinBoxRecentFiles->setValue(4);

        gridLayout4->addWidget(spinBoxRecentFiles, 2, 1, 1, 1);

        label_9 = new QLabel(tabAdvanced);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy5);

        gridLayout4->addWidget(label_9, 3, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        defaultDataBasePath = new QLineEdit(tabAdvanced);
        defaultDataBasePath->setObjectName(QString::fromUtf8("defaultDataBasePath"));
        QSizePolicy sizePolicy6(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(defaultDataBasePath->sizePolicy().hasHeightForWidth());
        defaultDataBasePath->setSizePolicy(sizePolicy6);

        horizontalLayout_5->addWidget(defaultDataBasePath);

        browsePathButton = new QPushButton(tabAdvanced);
        browsePathButton->setObjectName(QString::fromUtf8("browsePathButton"));

        horizontalLayout_5->addWidget(browsePathButton);


        gridLayout4->addLayout(horizontalLayout_5, 3, 1, 1, 1);


        verticalLayout1->addLayout(gridLayout4);

        verticalSpacer1 = new QSpacerItem(20, 329, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout1->addItem(verticalSpacer1);

        tabWidget->addTab(tabAdvanced, QString());
        tabFonts = new QWidget();
        tabFonts->setObjectName(QString::fromUtf8("tabFonts"));
        verticalLayout = new QVBoxLayout(tabFonts);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        labelGameTextFontSize = new QLabel(tabFonts);
        labelGameTextFontSize->setObjectName(QString::fromUtf8("labelGameTextFontSize"));

        horizontalLayout->addWidget(labelGameTextFontSize);

        gameTextFontSizeSpin = new QSpinBox(tabFonts);
        gameTextFontSizeSpin->setObjectName(QString::fromUtf8("gameTextFontSizeSpin"));
        gameTextFontSizeSpin->setMinimum(5);
        gameTextFontSizeSpin->setMaximum(30);
        gameTextFontSizeSpin->setValue(14);

        horizontalLayout->addWidget(gameTextFontSizeSpin);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        labelListFontSize = new QLabel(tabFonts);
        labelListFontSize->setObjectName(QString::fromUtf8("labelListFontSize"));

        horizontalLayout_3->addWidget(labelListFontSize);

        spinBoxListFontSize = new QSpinBox(tabFonts);
        spinBoxListFontSize->setObjectName(QString::fromUtf8("spinBoxListFontSize"));
        spinBoxListFontSize->setMinimum(5);
        spinBoxListFontSize->setMaximum(30);
        spinBoxListFontSize->setValue(14);

        horizontalLayout_3->addWidget(spinBoxListFontSize);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalTabs = new QCheckBox(tabFonts);
        verticalTabs->setObjectName(QString::fromUtf8("verticalTabs"));

        verticalLayout->addWidget(verticalTabs);

        groupBox_2 = new QGroupBox(tabFonts);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(513, 40));
        groupBox_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        notationNumeric = new QRadioButton(groupBox_2);
        notationNumeric->setObjectName(QString::fromUtf8("notationNumeric"));
        notationNumeric->setGeometry(QRect(111, 11, 150, 20));
        notationNumeric->setMinimumSize(QSize(150, 0));
        notationAlgebraic = new QRadioButton(groupBox_2);
        notationAlgebraic->setObjectName(QString::fromUtf8("notationAlgebraic"));
        notationAlgebraic->setGeometry(QRect(317, 11, 170, 20));
        notationAlgebraic->setMinimumSize(QSize(170, 0));

        verticalLayout->addWidget(groupBox_2);

        verticalSpacer = new QSpacerItem(20, 329, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        tabWidget->addTab(tabFonts, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setContentsMargins(4, 4, 4, 4);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        spacerItem2 = new QSpacerItem(121, 29, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem2);

        okButton = new QPushButton(PreferencesDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setDefault(true);

        hboxLayout1->addWidget(okButton);

        applyButton = new QPushButton(PreferencesDialog);
        applyButton->setObjectName(QString::fromUtf8("applyButton"));
        applyButton->setDefault(false);

        hboxLayout1->addWidget(applyButton);

        resetButton = new QPushButton(PreferencesDialog);
        resetButton->setObjectName(QString::fromUtf8("resetButton"));

        hboxLayout1->addWidget(resetButton);

        cancelButton = new QPushButton(PreferencesDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout1->addWidget(cancelButton);


        gridLayout->addLayout(hboxLayout1, 1, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        boardThemeLabel->setBuddy(boardThemeCombo);
        label->setBuddy(boardColorsList);
        pieceThemeLabel->setBuddy(pieceThemeCombo);
        label_7->setBuddy(limitSpin);
        label_8->setBuddy(spinBoxRecentFiles);
        label_9->setBuddy(defaultDataBasePath);
        labelGameTextFontSize->setBuddy(gameTextFontSizeSpin);
        labelListFontSize->setBuddy(spinBoxListFontSize);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(tabWidget, pieceThemeCombo);
        QWidget::setTabOrder(pieceThemeCombo, boardThemeCombo);
        QWidget::setTabOrder(boardThemeCombo, engineList);
        QWidget::setTabOrder(engineList, addEngineButton);
        QWidget::setTabOrder(addEngineButton, deleteEngineButton);
        QWidget::setTabOrder(deleteEngineButton, engineName);
        QWidget::setTabOrder(engineName, engineCommand);
        QWidget::setTabOrder(engineCommand, engineOptions);
        QWidget::setTabOrder(engineOptions, engineDirectory);
        QWidget::setTabOrder(engineDirectory, engineProtocolUSHI);
        QWidget::setTabOrder(engineProtocolUSHI, engineProtocolUnknown);
        QWidget::setTabOrder(engineProtocolUnknown, okButton);
        QWidget::setTabOrder(okButton, applyButton);
        QWidget::setTabOrder(applyButton, cancelButton);

        retranslateUi(PreferencesDialog);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(PreferencesDialog);
    } // setupUi

    void retranslateUi(QDialog *PreferencesDialog)
    {
        PreferencesDialog->setWindowTitle(QApplication::translate("PreferencesDialog", "Preferences", 0, QApplication::UnicodeUTF8));
        pieceEffect->clear();
        pieceEffect->insertItems(0, QStringList()
         << QApplication::translate("PreferencesDialog", "plain", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PreferencesDialog", "outline", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PreferencesDialog", "outline+shadow", 0, QApplication::UnicodeUTF8)
        );
        groupBox->setTitle(QApplication::translate("PreferencesDialog", "Mouse Behaviour", 0, QApplication::UnicodeUTF8));
        guessMoveCheck->setText(QApplication::translate("PreferencesDialog", "&Guess move on left-click", 0, QApplication::UnicodeUTF8));
        guessNextMove->setText(QApplication::translate("PreferencesDialog", "Next guess on right click", 0, QApplication::UnicodeUTF8));
        labelWheel->setText(QApplication::translate("PreferencesDialog", "Wheel sensitivity", 0, QApplication::UnicodeUTF8));
        labelFast->setText(QApplication::translate("PreferencesDialog", "Fast", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("PreferencesDialog", "Slow", 0, QApplication::UnicodeUTF8));
        boardThemeLabel->setText(QApplication::translate("PreferencesDialog", "Board &style:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PreferencesDialog", "&Colors:", 0, QApplication::UnicodeUTF8));
        boardFrameCheck->setText(QApplication::translate("PreferencesDialog", "&Draw frame between squares", 0, QApplication::UnicodeUTF8));
        pieceThemeLabel->setText(QApplication::translate("PreferencesDialog", "&Chess set:", 0, QApplication::UnicodeUTF8));
        hilightCurrentMove->setText(QApplication::translate("PreferencesDialog", "Highlight current move", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabGeneral), QApplication::translate("PreferencesDialog", "&Board", 0, QApplication::UnicodeUTF8));
        gbBoardOptions->setTitle(QApplication::translate("PreferencesDialog", "Autoplayer", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("PreferencesDialog", "Auto-Play Interval", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("PreferencesDialog", "Fast", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("PreferencesDialog", "Slow", 0, QApplication::UnicodeUTF8));
        cbSaveAndContinue->setText(QApplication::translate("PreferencesDialog", "Automatically save game and continue with next", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabAutoplay), QApplication::translate("PreferencesDialog", "Game", 0, QApplication::UnicodeUTF8));
        engineCommand->setPlaceholderText(QApplication::translate("PreferencesDialog", "Path to executable", 0, QApplication::UnicodeUTF8));
        engineDirectory->setPlaceholderText(QApplication::translate("PreferencesDialog", "Path where engine is started - usually place of books", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("PreferencesDialog", "Directory:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("PreferencesDialog", "Protocol:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("PreferencesDialog", "Name:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("PreferencesDialog", "Options:", 0, QApplication::UnicodeUTF8));
        commandButton->setText(QApplication::translate("PreferencesDialog", "...", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("PreferencesDialog", "Command:", 0, QApplication::UnicodeUTF8));
        engineOptions->setPlaceholderText(QApplication::translate("PreferencesDialog", "Command Line Options", 0, QApplication::UnicodeUTF8));
        engineProtocolUnknown->setText(QApplication::translate("PreferencesDialog", "Unknown", 0, QApplication::UnicodeUTF8));
        engineOptionMore->setText(QApplication::translate("PreferencesDialog", "Options", 0, QApplication::UnicodeUTF8));
        directoryButton->setText(QApplication::translate("PreferencesDialog", "...", 0, QApplication::UnicodeUTF8));
        engineName->setPlaceholderText(QApplication::translate("PreferencesDialog", "Arbitrary name to identify engine ", 0, QApplication::UnicodeUTF8));
        engineProtocolUSHI->setText(QApplication::translate("PreferencesDialog", "USI", 0, QApplication::UnicodeUTF8));
        engineDownButton->setText(QApplication::translate("PreferencesDialog", "Down", 0, QApplication::UnicodeUTF8));
        deleteEngineButton->setText(QApplication::translate("PreferencesDialog", "Remove", 0, QApplication::UnicodeUTF8));
        addEngineButton->setText(QApplication::translate("PreferencesDialog", "Add...", 0, QApplication::UnicodeUTF8));
        engineUpButton->setText(QApplication::translate("PreferencesDialog", "Up", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabEngines), QApplication::translate("PreferencesDialog", "&Engines", 0, QApplication::UnicodeUTF8));
        tablebaseCheck->setText(QApplication::translate("PreferencesDialog", "Use online tablebase servers", 0, QApplication::UnicodeUTF8));
        automaticECO->setText(QApplication::translate("PreferencesDialog", "Automatic classification with ECO Codes", 0, QApplication::UnicodeUTF8));
        useIndexFile->setText(QApplication::translate("PreferencesDialog", "Build index file", 0, QApplication::UnicodeUTF8));
        cbAutoCommitDB->setText(QApplication::translate("PreferencesDialog", "Commit Database after saving Game", 0, QApplication::UnicodeUTF8));
        limitSpin->setSuffix(QApplication::translate("PreferencesDialog", " MB", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("PreferencesDialog", "&Edit PGN files smaller than:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("PreferencesDialog", "Number of recent files:", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("PreferencesDialog", "Default Database Path:", 0, QApplication::UnicodeUTF8));
        defaultDataBasePath->setPlaceholderText(QApplication::translate("PreferencesDialog", "Default path for storing files", 0, QApplication::UnicodeUTF8));
        browsePathButton->setText(QApplication::translate("PreferencesDialog", "...", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabAdvanced), QApplication::translate("PreferencesDialog", "Database", 0, QApplication::UnicodeUTF8));
        labelGameTextFontSize->setText(QApplication::translate("PreferencesDialog", "&Game Text Font Size:", 0, QApplication::UnicodeUTF8));
        labelListFontSize->setText(QApplication::translate("PreferencesDialog", "List Font Size", 0, QApplication::UnicodeUTF8));
        verticalTabs->setText(QApplication::translate("PreferencesDialog", "Use vertical Tabs", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("PreferencesDialog", "Notation", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        notationNumeric->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        notationNumeric->setText(QApplication::translate("PreferencesDialog", "Numeric", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        notationAlgebraic->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        notationAlgebraic->setText(QApplication::translate("PreferencesDialog", "Algebraic", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tabFonts), QApplication::translate("PreferencesDialog", "Appearance", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("PreferencesDialog", "&Ok", 0, QApplication::UnicodeUTF8));
        applyButton->setText(QApplication::translate("PreferencesDialog", "&Apply", 0, QApplication::UnicodeUTF8));
        resetButton->setText(QApplication::translate("PreferencesDialog", "&Reset", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("PreferencesDialog", "&Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PreferencesDialog: public Ui_PreferencesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCES_H
