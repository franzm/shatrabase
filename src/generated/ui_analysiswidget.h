/********************************************************************************
** Form generated from reading UI file 'analysiswidget.ui'
**
** Created: Wed Feb 5 19:08:10 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANALYSISWIDGET_H
#define UI_ANALYSISWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AnalysisWidget
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label;
    QSpinBox *vpcount;
    QComboBox *engineList;
    QPushButton *analyzeButton;
    QWidget *boardView;
    QTextBrowser *variationText;

    void setupUi(QWidget *AnalysisWidget)
    {
        if (AnalysisWidget->objectName().isEmpty())
            AnalysisWidget->setObjectName(QString::fromUtf8("AnalysisWidget"));
        AnalysisWidget->resize(446, 268);
        AnalysisWidget->setMinimumSize(QSize(150, 80));
        vboxLayout = new QVBoxLayout(AnalysisWidget);
        vboxLayout->setSpacing(3);
        vboxLayout->setContentsMargins(3, 3, 3, 3);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setContentsMargins(0, 0, 0, 0);
#endif
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        label = new QLabel(AnalysisWidget);
        label->setObjectName(QString::fromUtf8("label"));

        hboxLayout->addWidget(label);

        vpcount = new QSpinBox(AnalysisWidget);
        vpcount->setObjectName(QString::fromUtf8("vpcount"));
        vpcount->setMinimum(1);
        vpcount->setMaximum(7);

        hboxLayout->addWidget(vpcount);

        engineList = new QComboBox(AnalysisWidget);
        engineList->setObjectName(QString::fromUtf8("engineList"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(engineList->sizePolicy().hasHeightForWidth());
        engineList->setSizePolicy(sizePolicy);

        hboxLayout->addWidget(engineList);

        analyzeButton = new QPushButton(AnalysisWidget);
        analyzeButton->setObjectName(QString::fromUtf8("analyzeButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(analyzeButton->sizePolicy().hasHeightForWidth());
        analyzeButton->setSizePolicy(sizePolicy1);
        analyzeButton->setCheckable(true);

        hboxLayout->addWidget(analyzeButton);


        vboxLayout->addLayout(hboxLayout);

        boardView = new QWidget(AnalysisWidget);
        boardView->setObjectName(QString::fromUtf8("boardView"));

        vboxLayout->addWidget(boardView);

        variationText = new QTextBrowser(AnalysisWidget);
        variationText->setObjectName(QString::fromUtf8("variationText"));
        variationText->setOpenLinks(false);

        vboxLayout->addWidget(variationText);

#ifndef QT_NO_SHORTCUT
        label->setBuddy(vpcount);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(vpcount, engineList);
        QWidget::setTabOrder(engineList, analyzeButton);
        QWidget::setTabOrder(analyzeButton, variationText);

        retranslateUi(AnalysisWidget);

        QMetaObject::connectSlotsByName(AnalysisWidget);
    } // setupUi

    void retranslateUi(QWidget *AnalysisWidget)
    {
        AnalysisWidget->setWindowTitle(QApplication::translate("AnalysisWidget", "Analysis", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AnalysisWidget", "&Lines:", 0, QApplication::UnicodeUTF8));
        analyzeButton->setText(QApplication::translate("AnalysisWidget", "Analyze", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AnalysisWidget: public Ui_AnalysisWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANALYSISWIDGET_H
