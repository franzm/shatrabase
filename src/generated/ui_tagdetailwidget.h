/********************************************************************************
** Form generated from reading UI file 'tagdetailwidget.ui'
**
** Created: Wed Feb 5 19:08:10 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TAGDETAILWIDGET_H
#define UI_TAGDETAILWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TagDetailWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *filterEdit;
    QListView *tagList;
    QPushButton *filterDatabase;
    QVBoxLayout *verticalLayout_2;
    QTextBrowser *detailText;

    void setupUi(QWidget *TagDetailWidget)
    {
        if (TagDetailWidget->objectName().isEmpty())
            TagDetailWidget->setObjectName(QString::fromUtf8("TagDetailWidget"));
        TagDetailWidget->resize(400, 300);
        horizontalLayout = new QHBoxLayout(TagDetailWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label = new QLabel(TagDetailWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        filterEdit = new QLineEdit(TagDetailWidget);
        filterEdit->setObjectName(QString::fromUtf8("filterEdit"));

        horizontalLayout_2->addWidget(filterEdit);


        verticalLayout->addLayout(horizontalLayout_2);

        tagList = new QListView(TagDetailWidget);
        tagList->setObjectName(QString::fromUtf8("tagList"));
        tagList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tagList->setAlternatingRowColors(true);

        verticalLayout->addWidget(tagList);

        filterDatabase = new QPushButton(TagDetailWidget);
        filterDatabase->setObjectName(QString::fromUtf8("filterDatabase"));

        verticalLayout->addWidget(filterDatabase);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        detailText = new QTextBrowser(TagDetailWidget);
        detailText->setObjectName(QString::fromUtf8("detailText"));

        verticalLayout_2->addWidget(detailText);


        horizontalLayout->addLayout(verticalLayout_2);

#ifndef QT_NO_SHORTCUT
        label->setBuddy(filterEdit);
#endif // QT_NO_SHORTCUT

        retranslateUi(TagDetailWidget);

        QMetaObject::connectSlotsByName(TagDetailWidget);
    } // setupUi

    void retranslateUi(QWidget *TagDetailWidget)
    {
        TagDetailWidget->setWindowTitle(QApplication::translate("TagDetailWidget", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TagDetailWidget", "&Filter:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        tagList->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        filterDatabase->setText(QApplication::translate("TagDetailWidget", "Filter Game List!", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TagDetailWidget: public Ui_TagDetailWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TAGDETAILWIDGET_H
