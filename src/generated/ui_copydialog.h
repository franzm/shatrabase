/********************************************************************************
** Form generated from reading UI file 'copydialog.ui'
**
** Created: Wed Feb 5 19:08:10 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COPYDIALOG_H
#define UI_COPYDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QRadioButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CopyDialog
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout;
    QRadioButton *singleButton;
    QRadioButton *filterButton;
    QRadioButton *allButton;
    QLabel *label;
    QListWidget *databaseList;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CopyDialog)
    {
        if (CopyDialog->objectName().isEmpty())
            CopyDialog->setObjectName(QString::fromUtf8("CopyDialog"));
        CopyDialog->resize(400, 342);
        gridLayout = new QGridLayout(CopyDialog);
#ifndef Q_OS_MAC
        gridLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        gridLayout->setContentsMargins(9, 9, 9, 9);
#endif
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox = new QGroupBox(CopyDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        vboxLayout = new QVBoxLayout(groupBox);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        singleButton = new QRadioButton(groupBox);
        singleButton->setObjectName(QString::fromUtf8("singleButton"));

        vboxLayout->addWidget(singleButton);

        filterButton = new QRadioButton(groupBox);
        filterButton->setObjectName(QString::fromUtf8("filterButton"));
        filterButton->setChecked(true);

        vboxLayout->addWidget(filterButton);

        allButton = new QRadioButton(groupBox);
        allButton->setObjectName(QString::fromUtf8("allButton"));

        vboxLayout->addWidget(allButton);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        label = new QLabel(CopyDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        databaseList = new QListWidget(CopyDialog);
        databaseList->setObjectName(QString::fromUtf8("databaseList"));

        gridLayout->addWidget(databaseList, 2, 0, 1, 1);

        buttonBox = new QDialogButtonBox(CopyDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 3, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        label->setBuddy(databaseList);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(singleButton, filterButton);
        QWidget::setTabOrder(filterButton, allButton);
        QWidget::setTabOrder(allButton, databaseList);
        QWidget::setTabOrder(databaseList, buttonBox);

        retranslateUi(CopyDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CopyDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CopyDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(CopyDialog);
    } // setupUi

    void retranslateUi(QDialog *CopyDialog)
    {
        CopyDialog->setWindowTitle(QApplication::translate("CopyDialog", "Copy Games", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CopyDialog", "Copy:", 0, QApplication::UnicodeUTF8));
        singleButton->setText(QApplication::translate("CopyDialog", "&Current game", 0, QApplication::UnicodeUTF8));
        filterButton->setText(QApplication::translate("CopyDialog", "Games in &filter", 0, QApplication::UnicodeUTF8));
        allButton->setText(QApplication::translate("CopyDialog", "&All games", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CopyDialog", "&To:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CopyDialog: public Ui_CopyDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COPYDIALOG_H
