/********************************************************************************
** Form generated from reading UI file 'mywidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYWIDGET_H
#define UI_MYWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *connectGroup;
    QHBoxLayout *horizontalLayout;
    QLineEdit *hostLineEdit;
    QSpinBox *portSpinBox;
    QPushButton *conectBtn;
    QGroupBox *talkGroup;
    QGridLayout *gridLayout;
    QTextEdit *msgsTextEdit;
    QLineEdit *msgLineEdit;
    QPushButton *sendBtn;

    void setupUi(QWidget *MyWidget)
    {
        if (MyWidget->objectName().isEmpty())
            MyWidget->setObjectName(QString::fromUtf8("MyWidget"));
        MyWidget->resize(455, 433);
        verticalLayout = new QVBoxLayout(MyWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        connectGroup = new QGroupBox(MyWidget);
        connectGroup->setObjectName(QString::fromUtf8("connectGroup"));
        horizontalLayout = new QHBoxLayout(connectGroup);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        hostLineEdit = new QLineEdit(connectGroup);
        hostLineEdit->setObjectName(QString::fromUtf8("hostLineEdit"));

        horizontalLayout->addWidget(hostLineEdit);

        portSpinBox = new QSpinBox(connectGroup);
        portSpinBox->setObjectName(QString::fromUtf8("portSpinBox"));
        portSpinBox->setButtonSymbols(QAbstractSpinBox::PlusMinus);
        portSpinBox->setMinimum(1);
        portSpinBox->setMaximum(65535);
        portSpinBox->setValue(2000);

        horizontalLayout->addWidget(portSpinBox);

        conectBtn = new QPushButton(connectGroup);
        conectBtn->setObjectName(QString::fromUtf8("conectBtn"));

        horizontalLayout->addWidget(conectBtn);


        verticalLayout->addWidget(connectGroup);

        talkGroup = new QGroupBox(MyWidget);
        talkGroup->setObjectName(QString::fromUtf8("talkGroup"));
        talkGroup->setEnabled(false);
        gridLayout = new QGridLayout(talkGroup);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        msgsTextEdit = new QTextEdit(talkGroup);
        msgsTextEdit->setObjectName(QString::fromUtf8("msgsTextEdit"));
        msgsTextEdit->setReadOnly(true);

        gridLayout->addWidget(msgsTextEdit, 0, 0, 1, 2);

        msgLineEdit = new QLineEdit(talkGroup);
        msgLineEdit->setObjectName(QString::fromUtf8("msgLineEdit"));

        gridLayout->addWidget(msgLineEdit, 1, 0, 1, 1);

        sendBtn = new QPushButton(talkGroup);
        sendBtn->setObjectName(QString::fromUtf8("sendBtn"));

        gridLayout->addWidget(sendBtn, 1, 1, 1, 1);


        verticalLayout->addWidget(talkGroup);

        QWidget::setTabOrder(portSpinBox, hostLineEdit);
        QWidget::setTabOrder(hostLineEdit, conectBtn);
        QWidget::setTabOrder(conectBtn, msgLineEdit);
        QWidget::setTabOrder(msgLineEdit, sendBtn);
        QWidget::setTabOrder(sendBtn, msgsTextEdit);

        retranslateUi(MyWidget);

        QMetaObject::connectSlotsByName(MyWidget);
    } // setupUi

    void retranslateUi(QWidget *MyWidget)
    {
        MyWidget->setWindowTitle(QCoreApplication::translate("MyWidget", "Simple TCP client", nullptr));
        connectGroup->setTitle(QString());
        hostLineEdit->setText(QCoreApplication::translate("MyWidget", "localhost", nullptr));
        conectBtn->setText(QCoreApplication::translate("MyWidget", "connect", nullptr));
        talkGroup->setTitle(QString());
        sendBtn->setText(QCoreApplication::translate("MyWidget", "send", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MyWidget: public Ui_MyWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYWIDGET_H
