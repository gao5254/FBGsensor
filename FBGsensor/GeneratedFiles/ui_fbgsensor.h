/********************************************************************************
** Form generated from reading UI file 'fbgsensor.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FBGSENSOR_H
#define UI_FBGSENSOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FBGsensorClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FBGsensorClass)
    {
        if (FBGsensorClass->objectName().isEmpty())
            FBGsensorClass->setObjectName(QStringLiteral("FBGsensorClass"));
        FBGsensorClass->resize(600, 400);
        menuBar = new QMenuBar(FBGsensorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        FBGsensorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FBGsensorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FBGsensorClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(FBGsensorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        FBGsensorClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(FBGsensorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FBGsensorClass->setStatusBar(statusBar);

        retranslateUi(FBGsensorClass);

        QMetaObject::connectSlotsByName(FBGsensorClass);
    } // setupUi

    void retranslateUi(QMainWindow *FBGsensorClass)
    {
        FBGsensorClass->setWindowTitle(QApplication::translate("FBGsensorClass", "FBGsensor", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FBGsensorClass: public Ui_FBGsensorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FBGSENSOR_H
