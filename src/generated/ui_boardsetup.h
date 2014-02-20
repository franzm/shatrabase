/********************************************************************************
** Form generated from reading UI file 'boardsetup.ui'
**
** Created: Wed Feb 5 19:08:10 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOARDSETUP_H
#define UI_BOARDSETUP_H

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
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "boardview.h"

QT_BEGIN_NAMESPACE

class Ui_BoardSetupDialog
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    QLabel *spnLabel;
    QTabWidget *tabWidget;
    QWidget *boardTab;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    BoardView *boardView;
    QGridLayout *buttonLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label_4;
    QToolButton *toMoveButton;
    QLabel *label_3;
    QSpinBox *moveSpin;
    QSpacerItem *spacerItem;
    QPushButton *clearButton;
    QPushButton *resetButton;
    QWidget *advancedTab;
    QGridLayout *gridLayout1;
    QSpinBox *halfmoveSpin;
    QSpacerItem *spacerItem1;
    QSpacerItem *spacerItem2;
    QLabel *label;
    QGroupBox *groupBox;
    QGridLayout *gridLayout2;
    QCheckBox *bkCastleCheck;
    QCheckBox *wqCastleCheck;
    QCheckBox *wkCastleCheck;
    QCheckBox *bqCastleCheck;
    QComboBox *epCombo;
    QLabel *label_2;
    QSpacerItem *spacerItem3;
    QHBoxLayout *hboxLayout1;
    QPushButton *pasteButton;
    QPushButton *copyButton;
    QSpacerItem *spacerItem4;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *BoardSetupDialog)
    {
        if (BoardSetupDialog->objectName().isEmpty())
            BoardSetupDialog->setObjectName(QString::fromUtf8("BoardSetupDialog"));
        BoardSetupDialog->resize(510, 560);
        gridLayout = new QGridLayout(BoardSetupDialog);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox_2 = new QGroupBox(BoardSetupDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout = new QVBoxLayout(groupBox_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        spnLabel = new QLabel(groupBox_2);
        spnLabel->setObjectName(QString::fromUtf8("spnLabel"));
        spnLabel->setMinimumSize(QSize(400, 0));
        spnLabel->setFrameShape(QFrame::Panel);
        spnLabel->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(spnLabel);


        gridLayout->addWidget(groupBox_2, 3, 0, 1, 1);

        tabWidget = new QTabWidget(BoardSetupDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        boardTab = new QWidget();
        boardTab->setObjectName(QString::fromUtf8("boardTab"));
        verticalLayout_2 = new QVBoxLayout(boardTab);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        boardView = new BoardView(boardTab);
        boardView->setObjectName(QString::fromUtf8("boardView"));
        boardView->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(boardView->sizePolicy().hasHeightForWidth());
        boardView->setSizePolicy(sizePolicy);
        boardView->setAcceptDrops(true);

        horizontalLayout_3->addWidget(boardView);

        buttonLayout = new QGridLayout();
        buttonLayout->setSpacing(6);
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        buttonLayout->setSizeConstraint(QLayout::SetDefaultConstraint);

        horizontalLayout_3->addLayout(buttonLayout);

        horizontalLayout_3->setStretch(0, 4);
        horizontalLayout_3->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout_3);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        hboxLayout->setSizeConstraint(QLayout::SetFixedSize);
        label_4 = new QLabel(boardTab);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        hboxLayout->addWidget(label_4);

        toMoveButton = new QToolButton(boardTab);
        toMoveButton->setObjectName(QString::fromUtf8("toMoveButton"));

        hboxLayout->addWidget(toMoveButton);

        label_3 = new QLabel(boardTab);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        hboxLayout->addWidget(label_3);

        moveSpin = new QSpinBox(boardTab);
        moveSpin->setObjectName(QString::fromUtf8("moveSpin"));
        moveSpin->setMinimum(1);
        moveSpin->setMaximum(999);
        moveSpin->setValue(1);

        hboxLayout->addWidget(moveSpin);

        spacerItem = new QSpacerItem(61, 27, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        clearButton = new QPushButton(boardTab);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));

        hboxLayout->addWidget(clearButton);

        resetButton = new QPushButton(boardTab);
        resetButton->setObjectName(QString::fromUtf8("resetButton"));

        hboxLayout->addWidget(resetButton);


        verticalLayout_2->addLayout(hboxLayout);

        tabWidget->addTab(boardTab, QString());
        advancedTab = new QWidget();
        advancedTab->setObjectName(QString::fromUtf8("advancedTab"));
        gridLayout1 = new QGridLayout(advancedTab);
        gridLayout1->setSpacing(6);
        gridLayout1->setContentsMargins(11, 11, 11, 11);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        halfmoveSpin = new QSpinBox(advancedTab);
        halfmoveSpin->setObjectName(QString::fromUtf8("halfmoveSpin"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(halfmoveSpin->sizePolicy().hasHeightForWidth());
        halfmoveSpin->setSizePolicy(sizePolicy1);
        halfmoveSpin->setMinimum(0);
        halfmoveSpin->setMaximum(99);
        halfmoveSpin->setValue(0);

        gridLayout1->addWidget(halfmoveSpin, 2, 1, 1, 1);

        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout1->addItem(spacerItem1, 2, 2, 1, 1);

        spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout1->addItem(spacerItem2, 1, 2, 1, 1);

        label = new QLabel(advancedTab);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout1->addWidget(label, 1, 0, 1, 1);

        groupBox = new QGroupBox(advancedTab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout2 = new QGridLayout(groupBox);
        gridLayout2->setSpacing(6);
        gridLayout2->setContentsMargins(11, 11, 11, 11);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        bkCastleCheck = new QCheckBox(groupBox);
        bkCastleCheck->setObjectName(QString::fromUtf8("bkCastleCheck"));

        gridLayout2->addWidget(bkCastleCheck, 0, 1, 1, 1);

        wqCastleCheck = new QCheckBox(groupBox);
        wqCastleCheck->setObjectName(QString::fromUtf8("wqCastleCheck"));

        gridLayout2->addWidget(wqCastleCheck, 1, 0, 1, 1);

        wkCastleCheck = new QCheckBox(groupBox);
        wkCastleCheck->setObjectName(QString::fromUtf8("wkCastleCheck"));

        gridLayout2->addWidget(wkCastleCheck, 0, 0, 1, 1);

        bqCastleCheck = new QCheckBox(groupBox);
        bqCastleCheck->setObjectName(QString::fromUtf8("bqCastleCheck"));

        gridLayout2->addWidget(bqCastleCheck, 1, 1, 1, 1);


        gridLayout1->addWidget(groupBox, 0, 0, 1, 3);

        epCombo = new QComboBox(advancedTab);
        epCombo->setObjectName(QString::fromUtf8("epCombo"));

        gridLayout1->addWidget(epCombo, 1, 1, 1, 1);

        label_2 = new QLabel(advancedTab);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout1->addWidget(label_2, 2, 0, 1, 1);

        spacerItem3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout1->addItem(spacerItem3, 3, 1, 1, 1);

        tabWidget->addTab(advancedTab, QString());

        gridLayout->addWidget(tabWidget, 2, 0, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        pasteButton = new QPushButton(BoardSetupDialog);
        pasteButton->setObjectName(QString::fromUtf8("pasteButton"));

        hboxLayout1->addWidget(pasteButton);

        copyButton = new QPushButton(BoardSetupDialog);
        copyButton->setObjectName(QString::fromUtf8("copyButton"));

        hboxLayout1->addWidget(copyButton);

        spacerItem4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem4);

        okButton = new QPushButton(BoardSetupDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        hboxLayout1->addWidget(okButton);

        cancelButton = new QPushButton(BoardSetupDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout1->addWidget(cancelButton);


        gridLayout->addLayout(hboxLayout1, 6, 0, 1, 1);

        QWidget::setTabOrder(wkCastleCheck, bkCastleCheck);
        QWidget::setTabOrder(bkCastleCheck, wqCastleCheck);
        QWidget::setTabOrder(wqCastleCheck, bqCastleCheck);
        QWidget::setTabOrder(bqCastleCheck, halfmoveSpin);
        QWidget::setTabOrder(halfmoveSpin, moveSpin);
        QWidget::setTabOrder(moveSpin, okButton);
        QWidget::setTabOrder(okButton, cancelButton);

        retranslateUi(BoardSetupDialog);

        tabWidget->setCurrentIndex(0);
        epCombo->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(BoardSetupDialog);
    } // setupUi

    void retranslateUi(QDialog *BoardSetupDialog)
    {
        BoardSetupDialog->setWindowTitle(QApplication::translate("BoardSetupDialog", "Setup position", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("BoardSetupDialog", "FEN", 0, QApplication::UnicodeUTF8));
        spnLabel->setText(QString());
        label_4->setText(QApplication::translate("BoardSetupDialog", "Side to move:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("BoardSetupDialog", "Move:", 0, QApplication::UnicodeUTF8));
        clearButton->setText(QApplication::translate("BoardSetupDialog", "Clear", 0, QApplication::UnicodeUTF8));
        resetButton->setText(QApplication::translate("BoardSetupDialog", "&Reset", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(boardTab), QApplication::translate("BoardSetupDialog", "Board", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("BoardSetupDialog", "En passant file:", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("BoardSetupDialog", "Castling rights", 0, QApplication::UnicodeUTF8));
        bkCastleCheck->setText(QApplication::translate("BoardSetupDialog", "Black O-O", 0, QApplication::UnicodeUTF8));
        wqCastleCheck->setText(QApplication::translate("BoardSetupDialog", "White O-O-O", 0, QApplication::UnicodeUTF8));
        wkCastleCheck->setText(QApplication::translate("BoardSetupDialog", "White O-O", 0, QApplication::UnicodeUTF8));
        bqCastleCheck->setText(QApplication::translate("BoardSetupDialog", "Black O-O-O", 0, QApplication::UnicodeUTF8));
        epCombo->clear();
        epCombo->insertItems(0, QStringList()
         << QApplication::translate("BoardSetupDialog", "-", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("BoardSetupDialog", "a", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("BoardSetupDialog", "b", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("BoardSetupDialog", "c", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("BoardSetupDialog", "d", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("BoardSetupDialog", "e", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("BoardSetupDialog", "f", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("BoardSetupDialog", "g", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("BoardSetupDialog", "h", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("BoardSetupDialog", "Halfmove clock:", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(advancedTab), QApplication::translate("BoardSetupDialog", "&Advanced", 0, QApplication::UnicodeUTF8));
        pasteButton->setText(QApplication::translate("BoardSetupDialog", "Paste FEN", 0, QApplication::UnicodeUTF8));
        copyButton->setText(QApplication::translate("BoardSetupDialog", "Copy FEN", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("BoardSetupDialog", "&Ok", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("BoardSetupDialog", "&Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BoardSetupDialog: public Ui_BoardSetupDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOARDSETUP_H
