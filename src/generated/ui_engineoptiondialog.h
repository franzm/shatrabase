/********************************************************************************
** Form generated from reading UI file 'engineoptiondialog.ui'
**
** Created: Wed Feb 5 19:08:10 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENGINEOPTIONDIALOG_H
#define UI_ENGINEOPTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include "engineoptionlist.h"

QT_BEGIN_NAMESPACE

class Ui_EngineOptionDialog
{
public:
    QVBoxLayout *verticalLayout;
    EngineOptionList *tableView;
    QHBoxLayout *horizontalLayout;
    QPushButton *resetButton;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *EngineOptionDialog)
    {
        if (EngineOptionDialog->objectName().isEmpty())
            EngineOptionDialog->setObjectName(QString::fromUtf8("EngineOptionDialog"));
        EngineOptionDialog->resize(598, 349);
        verticalLayout = new QVBoxLayout(EngineOptionDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tableView = new EngineOptionList(EngineOptionDialog);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        verticalLayout->addWidget(tableView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        resetButton = new QPushButton(EngineOptionDialog);
        resetButton->setObjectName(QString::fromUtf8("resetButton"));

        horizontalLayout->addWidget(resetButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        buttonBox = new QDialogButtonBox(EngineOptionDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout->addWidget(buttonBox);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(EngineOptionDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), EngineOptionDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), EngineOptionDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(EngineOptionDialog);
    } // setupUi

    void retranslateUi(QDialog *EngineOptionDialog)
    {
        EngineOptionDialog->setWindowTitle(QApplication::translate("EngineOptionDialog", "Engine Options", 0, QApplication::UnicodeUTF8));
        resetButton->setText(QApplication::translate("EngineOptionDialog", "Reset", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EngineOptionDialog: public Ui_EngineOptionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENGINEOPTIONDIALOG_H
