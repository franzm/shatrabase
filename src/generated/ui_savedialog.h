/********************************************************************************
** Form generated from reading UI file 'savedialog.ui'
**
** Created: Wed Feb 5 19:08:10 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVEDIALOG_H
#define UI_SAVEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTimeEdit>

QT_BEGIN_NAMESPACE

class Ui_SaveDialog
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *buttonDiscardChanges;
    QPushButton *buttonCancel;
    QPushButton *buttonOk;
    QGroupBox *groupBox;
    QGridLayout *gridLayout1;
    QLineEdit *blackEloEdit;
    QLabel *textLabel2_2;
    QLabel *textLabel2;
    QLineEdit *whiteEloEdit;
    QLabel *textLabel1;
    QLineEdit *whiteEdit;
    QLineEdit *blackEdit;
    QLabel *textLabel1_2;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_3;
    QGridLayout *gridlayout4;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QTimeEdit *whiteStartTime;
    QSpacerItem *horizontalSpacer;
    QTimeEdit *blackStartTime;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QLineEdit *timeControl;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout2;
    QLineEdit *eventDateEdit;
    QLineEdit *siteEdit;
    QLabel *textLabel2_5;
    QLineEdit *eventEdit;
    QLabel *textLabel1_2_2;
    QLabel *textLabel1_2_3;
    QGroupBox *gameGroupBox;
    QGridLayout *gridLayout3;
    QLineEdit *roundEdit;
    QLabel *textLabel2_3;
    QLabel *textLabel2_4;
    QLineEdit *dateEdit;
    QSpacerItem *spacerItem1;
    QHBoxLayout *hboxLayout1;
    QLabel *label;
    QRadioButton *result1Button;
    QRadioButton *result5Button;
    QRadioButton *result0Button;
    QRadioButton *resultNoneButton;
    QSpacerItem *spacerItem2;
    QFrame *line;

    void setupUi(QDialog *SaveDialog)
    {
        if (SaveDialog->objectName().isEmpty())
            SaveDialog->setObjectName(QString::fromUtf8("SaveDialog"));
        SaveDialog->resize(541, 503);
        SaveDialog->setSizeGripEnabled(true);
        gridLayout = new QGridLayout(SaveDialog);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacerItem = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        buttonDiscardChanges = new QPushButton(SaveDialog);
        buttonDiscardChanges->setObjectName(QString::fromUtf8("buttonDiscardChanges"));

        hboxLayout->addWidget(buttonDiscardChanges);

        buttonCancel = new QPushButton(SaveDialog);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout->addWidget(buttonCancel);

        buttonOk = new QPushButton(SaveDialog);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout->addWidget(buttonOk);


        gridLayout->addLayout(hboxLayout, 6, 0, 1, 1);

        groupBox = new QGroupBox(SaveDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout1 = new QGridLayout(groupBox);
        gridLayout1->setSpacing(6);
        gridLayout1->setContentsMargins(11, 11, 11, 11);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        blackEloEdit = new QLineEdit(groupBox);
        blackEloEdit->setObjectName(QString::fromUtf8("blackEloEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(blackEloEdit->sizePolicy().hasHeightForWidth());
        blackEloEdit->setSizePolicy(sizePolicy);
        blackEloEdit->setMaximumSize(QSize(80, 32767));

        gridLayout1->addWidget(blackEloEdit, 1, 3, 1, 1);

        textLabel2_2 = new QLabel(groupBox);
        textLabel2_2->setObjectName(QString::fromUtf8("textLabel2_2"));

        gridLayout1->addWidget(textLabel2_2, 1, 2, 1, 1);

        textLabel2 = new QLabel(groupBox);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));

        gridLayout1->addWidget(textLabel2, 0, 2, 1, 1);

        whiteEloEdit = new QLineEdit(groupBox);
        whiteEloEdit->setObjectName(QString::fromUtf8("whiteEloEdit"));
        sizePolicy.setHeightForWidth(whiteEloEdit->sizePolicy().hasHeightForWidth());
        whiteEloEdit->setSizePolicy(sizePolicy);
        whiteEloEdit->setMaximumSize(QSize(80, 32767));

        gridLayout1->addWidget(whiteEloEdit, 0, 3, 1, 1);

        textLabel1 = new QLabel(groupBox);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));

        gridLayout1->addWidget(textLabel1, 0, 0, 1, 1);

        whiteEdit = new QLineEdit(groupBox);
        whiteEdit->setObjectName(QString::fromUtf8("whiteEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(whiteEdit->sizePolicy().hasHeightForWidth());
        whiteEdit->setSizePolicy(sizePolicy1);

        gridLayout1->addWidget(whiteEdit, 0, 1, 1, 1);

        blackEdit = new QLineEdit(groupBox);
        blackEdit->setObjectName(QString::fromUtf8("blackEdit"));
        sizePolicy1.setHeightForWidth(blackEdit->sizePolicy().hasHeightForWidth());
        blackEdit->setSizePolicy(sizePolicy1);

        gridLayout1->addWidget(blackEdit, 1, 1, 1, 1);

        textLabel1_2 = new QLabel(groupBox);
        textLabel1_2->setObjectName(QString::fromUtf8("textLabel1_2"));

        gridLayout1->addWidget(textLabel1_2, 1, 0, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 4, 0, 1, 1);

        groupBox_3 = new QGroupBox(SaveDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridlayout4 = new QGridLayout(groupBox_3);
        gridlayout4->setSpacing(6);
        gridlayout4->setContentsMargins(11, 11, 11, 11);
        gridlayout4->setObjectName(QString::fromUtf8("gridlayout4"));
        gridlayout4->setVerticalSpacing(6);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_2);

        whiteStartTime = new QTimeEdit(groupBox_3);
        whiteStartTime->setObjectName(QString::fromUtf8("whiteStartTime"));

        horizontalLayout->addWidget(whiteStartTime);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        blackStartTime = new QTimeEdit(groupBox_3);
        blackStartTime->setObjectName(QString::fromUtf8("blackStartTime"));
        blackStartTime->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(blackStartTime);

        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_3);


        gridlayout4->addLayout(horizontalLayout, 5, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_2->addWidget(label_4);

        timeControl = new QLineEdit(groupBox_3);
        timeControl->setObjectName(QString::fromUtf8("timeControl"));

        horizontalLayout_2->addWidget(timeControl);


        gridlayout4->addLayout(horizontalLayout_2, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_3, 3, 0, 1, 1);

        groupBox_2 = new QGroupBox(SaveDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout2 = new QGridLayout(groupBox_2);
        gridLayout2->setSpacing(6);
        gridLayout2->setContentsMargins(11, 11, 11, 11);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        eventDateEdit = new QLineEdit(groupBox_2);
        eventDateEdit->setObjectName(QString::fromUtf8("eventDateEdit"));
        sizePolicy.setHeightForWidth(eventDateEdit->sizePolicy().hasHeightForWidth());
        eventDateEdit->setSizePolicy(sizePolicy);
        eventDateEdit->setMaximumSize(QSize(160, 32767));

        gridLayout2->addWidget(eventDateEdit, 1, 3, 1, 1);

        siteEdit = new QLineEdit(groupBox_2);
        siteEdit->setObjectName(QString::fromUtf8("siteEdit"));
        sizePolicy1.setHeightForWidth(siteEdit->sizePolicy().hasHeightForWidth());
        siteEdit->setSizePolicy(sizePolicy1);

        gridLayout2->addWidget(siteEdit, 1, 1, 1, 1);

        textLabel2_5 = new QLabel(groupBox_2);
        textLabel2_5->setObjectName(QString::fromUtf8("textLabel2_5"));

        gridLayout2->addWidget(textLabel2_5, 1, 2, 1, 1);

        eventEdit = new QLineEdit(groupBox_2);
        eventEdit->setObjectName(QString::fromUtf8("eventEdit"));
        sizePolicy1.setHeightForWidth(eventEdit->sizePolicy().hasHeightForWidth());
        eventEdit->setSizePolicy(sizePolicy1);

        gridLayout2->addWidget(eventEdit, 0, 1, 1, 3);

        textLabel1_2_2 = new QLabel(groupBox_2);
        textLabel1_2_2->setObjectName(QString::fromUtf8("textLabel1_2_2"));

        gridLayout2->addWidget(textLabel1_2_2, 1, 0, 1, 1);

        textLabel1_2_3 = new QLabel(groupBox_2);
        textLabel1_2_3->setObjectName(QString::fromUtf8("textLabel1_2_3"));

        gridLayout2->addWidget(textLabel1_2_3, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox_2, 1, 0, 1, 1);

        gameGroupBox = new QGroupBox(SaveDialog);
        gameGroupBox->setObjectName(QString::fromUtf8("gameGroupBox"));
        gridLayout3 = new QGridLayout(gameGroupBox);
        gridLayout3->setSpacing(6);
        gridLayout3->setContentsMargins(11, 11, 11, 11);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        roundEdit = new QLineEdit(gameGroupBox);
        roundEdit->setObjectName(QString::fromUtf8("roundEdit"));
        sizePolicy.setHeightForWidth(roundEdit->sizePolicy().hasHeightForWidth());
        roundEdit->setSizePolicy(sizePolicy);
        roundEdit->setMaximumSize(QSize(80, 32767));

        gridLayout3->addWidget(roundEdit, 0, 1, 1, 1);

        textLabel2_3 = new QLabel(gameGroupBox);
        textLabel2_3->setObjectName(QString::fromUtf8("textLabel2_3"));

        gridLayout3->addWidget(textLabel2_3, 0, 0, 1, 1);

        textLabel2_4 = new QLabel(gameGroupBox);
        textLabel2_4->setObjectName(QString::fromUtf8("textLabel2_4"));

        gridLayout3->addWidget(textLabel2_4, 0, 3, 1, 1);

        dateEdit = new QLineEdit(gameGroupBox);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));
        sizePolicy.setHeightForWidth(dateEdit->sizePolicy().hasHeightForWidth());
        dateEdit->setSizePolicy(sizePolicy);
        dateEdit->setMaximumSize(QSize(160, 32767));

        gridLayout3->addWidget(dateEdit, 0, 4, 1, 1);

        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout3->addItem(spacerItem1, 0, 2, 1, 1);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        label = new QLabel(gameGroupBox);
        label->setObjectName(QString::fromUtf8("label"));

        hboxLayout1->addWidget(label);

        result1Button = new QRadioButton(gameGroupBox);
        result1Button->setObjectName(QString::fromUtf8("result1Button"));

        hboxLayout1->addWidget(result1Button);

        result5Button = new QRadioButton(gameGroupBox);
        result5Button->setObjectName(QString::fromUtf8("result5Button"));

        hboxLayout1->addWidget(result5Button);

        result0Button = new QRadioButton(gameGroupBox);
        result0Button->setObjectName(QString::fromUtf8("result0Button"));

        hboxLayout1->addWidget(result0Button);

        resultNoneButton = new QRadioButton(gameGroupBox);
        resultNoneButton->setObjectName(QString::fromUtf8("resultNoneButton"));
        resultNoneButton->setChecked(true);

        hboxLayout1->addWidget(resultNoneButton);

        spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem2);


        gridLayout3->addLayout(hboxLayout1, 1, 0, 1, 4);


        gridLayout->addWidget(gameGroupBox, 2, 0, 1, 1);

        line = new QFrame(SaveDialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 5, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        textLabel1->setBuddy(whiteEdit);
        textLabel1_2->setBuddy(blackEdit);
        textLabel2_5->setBuddy(eventDateEdit);
        textLabel1_2_2->setBuddy(siteEdit);
        textLabel1_2_3->setBuddy(eventEdit);
        textLabel2_3->setBuddy(roundEdit);
        textLabel2_4->setBuddy(dateEdit);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(whiteEdit, whiteEloEdit);
        QWidget::setTabOrder(whiteEloEdit, blackEdit);
        QWidget::setTabOrder(blackEdit, blackEloEdit);
        QWidget::setTabOrder(blackEloEdit, eventEdit);
        QWidget::setTabOrder(eventEdit, siteEdit);
        QWidget::setTabOrder(siteEdit, eventDateEdit);
        QWidget::setTabOrder(eventDateEdit, roundEdit);
        QWidget::setTabOrder(roundEdit, dateEdit);
        QWidget::setTabOrder(dateEdit, result1Button);
        QWidget::setTabOrder(result1Button, result5Button);
        QWidget::setTabOrder(result5Button, result0Button);
        QWidget::setTabOrder(result0Button, resultNoneButton);
        QWidget::setTabOrder(resultNoneButton, buttonCancel);

        retranslateUi(SaveDialog);
        QObject::connect(buttonOk, SIGNAL(clicked()), SaveDialog, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), SaveDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(SaveDialog);
    } // setupUi

    void retranslateUi(QDialog *SaveDialog)
    {
        SaveDialog->setWindowTitle(QApplication::translate("SaveDialog", "Save Game", 0, QApplication::UnicodeUTF8));
        buttonDiscardChanges->setText(QApplication::translate("SaveDialog", "Discard Changes", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("SaveDialog", "&Cancel", 0, QApplication::UnicodeUTF8));
        buttonOk->setText(QApplication::translate("SaveDialog", "&OK", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("SaveDialog", "Players", 0, QApplication::UnicodeUTF8));
        textLabel2_2->setText(QApplication::translate("SaveDialog", "Elo:", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("SaveDialog", "Elo:", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("SaveDialog", "&White:", 0, QApplication::UnicodeUTF8));
        textLabel1_2->setText(QApplication::translate("SaveDialog", "&Black:", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("SaveDialog", "Time Control", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("SaveDialog", "White Start Time", 0, QApplication::UnicodeUTF8));
        whiteStartTime->setDisplayFormat(QApplication::translate("SaveDialog", "H:mm:ss", 0, QApplication::UnicodeUTF8));
        blackStartTime->setDisplayFormat(QApplication::translate("SaveDialog", "H:mm:ss", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("SaveDialog", "Black Start Time", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("SaveDialog", "Time Control", 0, QApplication::UnicodeUTF8));
        timeControl->setPlaceholderText(QApplication::translate("SaveDialog", "e.g. 40/7200:3600 for 2h/40 moves + 1h", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("SaveDialog", "Tournament", 0, QApplication::UnicodeUTF8));
        textLabel2_5->setText(QApplication::translate("SaveDialog", "E&vent date:", 0, QApplication::UnicodeUTF8));
        textLabel1_2_2->setText(QApplication::translate("SaveDialog", "&Site:", 0, QApplication::UnicodeUTF8));
        textLabel1_2_3->setText(QApplication::translate("SaveDialog", "&Event:", 0, QApplication::UnicodeUTF8));
        gameGroupBox->setTitle(QApplication::translate("SaveDialog", "Game Information", 0, QApplication::UnicodeUTF8));
        textLabel2_3->setText(QApplication::translate("SaveDialog", "&Round:", 0, QApplication::UnicodeUTF8));
        textLabel2_4->setText(QApplication::translate("SaveDialog", "&Date:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SaveDialog", "Result:", 0, QApplication::UnicodeUTF8));
        result1Button->setText(QApplication::translate("SaveDialog", "1-0", 0, QApplication::UnicodeUTF8));
        result5Button->setText(QApplication::translate("SaveDialog", "1/2-1/2", 0, QApplication::UnicodeUTF8));
        result0Button->setText(QApplication::translate("SaveDialog", "0-1", 0, QApplication::UnicodeUTF8));
        resultNoneButton->setText(QApplication::translate("SaveDialog", "*", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SaveDialog: public Ui_SaveDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEDIALOG_H
