/********************************************************************************
** Form generated from reading UI file 'quicksearch.ui'
**
** Created: Wed Feb 5 19:08:10 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUICKSEARCH_H
#define UI_QUICKSEARCH_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_QuickSearchDialog
{
public:
    QGridLayout *gridLayout;
    QComboBox *modeCombo;
    QComboBox *tagCombo;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QLineEdit *valueEdit;
    QLabel *label_2;
    QLabel *label;
    QLabel *label_3;

    void setupUi(QDialog *QuickSearchDialog)
    {
        if (QuickSearchDialog->objectName().isEmpty())
            QuickSearchDialog->setObjectName(QString::fromUtf8("QuickSearchDialog"));
        QuickSearchDialog->resize(392, 148);
        gridLayout = new QGridLayout(QuickSearchDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        modeCombo = new QComboBox(QuickSearchDialog);
        modeCombo->setObjectName(QString::fromUtf8("modeCombo"));

        gridLayout->addWidget(modeCombo, 2, 1, 1, 1);

        tagCombo = new QComboBox(QuickSearchDialog);
        tagCombo->setObjectName(QString::fromUtf8("tagCombo"));

        gridLayout->addWidget(tagCombo, 0, 1, 1, 1);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(QuickSearchDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setDefault(true);

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(QuickSearchDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout->addWidget(cancelButton);


        gridLayout->addLayout(hboxLayout, 3, 0, 1, 2);

        valueEdit = new QLineEdit(QuickSearchDialog);
        valueEdit->setObjectName(QString::fromUtf8("valueEdit"));

        gridLayout->addWidget(valueEdit, 1, 1, 1, 1);

        label_2 = new QLabel(QuickSearchDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(QuickSearchDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(QuickSearchDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        label_2->setBuddy(valueEdit);
        label->setBuddy(tagCombo);
        label_3->setBuddy(modeCombo);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(tagCombo, valueEdit);
        QWidget::setTabOrder(valueEdit, modeCombo);
        QWidget::setTabOrder(modeCombo, okButton);
        QWidget::setTabOrder(okButton, cancelButton);

        retranslateUi(QuickSearchDialog);

        QMetaObject::connectSlotsByName(QuickSearchDialog);
    } // setupUi

    void retranslateUi(QDialog *QuickSearchDialog)
    {
        QuickSearchDialog->setWindowTitle(QApplication::translate("QuickSearchDialog", "Quick Tag Search", 0, QApplication::UnicodeUTF8));
        tagCombo->clear();
        tagCombo->insertItems(0, QStringList()
         << QApplication::translate("QuickSearchDialog", "Game Number", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QuickSearchDialog", "White", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QuickSearchDialog", "White Elo", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QuickSearchDialog", "Black", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QuickSearchDialog", "Black Elo", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QuickSearchDialog", "Event", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QuickSearchDialog", "Site", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QuickSearchDialog", "Round", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QuickSearchDialog", "Date", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QuickSearchDialog", "Result", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QuickSearchDialog", "ECO", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QuickSearchDialog", "Number of Moves", 0, QApplication::UnicodeUTF8)
        );
        okButton->setText(QApplication::translate("QuickSearchDialog", "&Find", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("QuickSearchDialog", "&Cancel", 0, QApplication::UnicodeUTF8));
        valueEdit->setPlaceholderText(QApplication::translate("QuickSearchDialog", "Value or Range like 2000-2300", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("QuickSearchDialog", "&Value:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("QuickSearchDialog", "&Tag:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("QuickSearchDialog", "&Mode:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QuickSearchDialog: public Ui_QuickSearchDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUICKSEARCH_H
