/********************************************************************************
** Form generated from reading UI file 'commentdialog.ui'
**
** Created: Wed Feb 5 19:08:10 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMMENTDIALOG_H
#define UI_COMMENTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QRadioButton>
#include <QtGui/QTimeEdit>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include "plaintextedit.h"

QT_BEGIN_NAMESPACE

class Ui_CommentDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    PlainTextEdit *textEdit;
    QVBoxLayout *verticalLayout_2;
    QToolButton *clearText;
    QHBoxLayout *horizontalLayout;
    QRadioButton *egtTime;
    QRadioButton *clkTime;
    QTimeEdit *timeEdit;
    QToolButton *clearTime;
    QFrame *line;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CommentDialog)
    {
        if (CommentDialog->objectName().isEmpty())
            CommentDialog->setObjectName(QString::fromUtf8("CommentDialog"));
        CommentDialog->resize(440, 221);
        CommentDialog->setMinimumSize(QSize(0, 1));
        verticalLayout = new QVBoxLayout(CommentDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 12, -1, -1);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        textEdit = new PlainTextEdit(CommentDialog);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setTabChangesFocus(true);

        horizontalLayout_2->addWidget(textEdit);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        clearText = new QToolButton(CommentDialog);
        clearText->setObjectName(QString::fromUtf8("clearText"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/text_clear.png"), QSize(), QIcon::Normal, QIcon::Off);
        clearText->setIcon(icon);

        verticalLayout_2->addWidget(clearText);


        horizontalLayout_2->addLayout(verticalLayout_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        egtTime = new QRadioButton(CommentDialog);
        egtTime->setObjectName(QString::fromUtf8("egtTime"));

        horizontalLayout->addWidget(egtTime);

        clkTime = new QRadioButton(CommentDialog);
        clkTime->setObjectName(QString::fromUtf8("clkTime"));
        clkTime->setChecked(true);

        horizontalLayout->addWidget(clkTime);

        timeEdit = new QTimeEdit(CommentDialog);
        timeEdit->setObjectName(QString::fromUtf8("timeEdit"));
        timeEdit->setButtonSymbols(QAbstractSpinBox::UpDownArrows);

        horizontalLayout->addWidget(timeEdit);

        clearTime = new QToolButton(CommentDialog);
        clearTime->setObjectName(QString::fromUtf8("clearTime"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/time_clear.png"), QSize(), QIcon::Normal, QIcon::On);
        clearTime->setIcon(icon1);

        horizontalLayout->addWidget(clearTime);


        verticalLayout->addLayout(horizontalLayout);

        line = new QFrame(CommentDialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        buttonBox = new QDialogButtonBox(CommentDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(CommentDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CommentDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CommentDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(CommentDialog);
    } // setupUi

    void retranslateUi(QDialog *CommentDialog)
    {
        CommentDialog->setWindowTitle(QApplication::translate("CommentDialog", "Edit Comment", 0, QApplication::UnicodeUTF8));
        clearText->setText(QString());
        egtTime->setText(QApplication::translate("CommentDialog", "Elapsed Time", 0, QApplication::UnicodeUTF8));
        clkTime->setText(QApplication::translate("CommentDialog", "Clock Time", 0, QApplication::UnicodeUTF8));
        timeEdit->setDisplayFormat(QApplication::translate("CommentDialog", "H:mm:ss", 0, QApplication::UnicodeUTF8));
        clearTime->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CommentDialog: public Ui_CommentDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMENTDIALOG_H
