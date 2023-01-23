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
#include <QtWidgets/QLabel>
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
    QVBoxLayout *verticalLayout_2;
    QGroupBox *connectGroup;
    QHBoxLayout *horizontalLayout;
    QLineEdit *hostLineEdit;
    QSpinBox *portSpinBox;
    QPushButton *conectBtn;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_8;
    QGroupBox *talkGroup;
    QGridLayout *gridLayout_10;
    QLabel *gameLabel;
    QLabel *guessLabel;
    QLineEdit *msgLineEdit;
    QTextEdit *gameTextEdit;
    QPushButton *sendBtn;
    QGroupBox *roomGroup;
    QGridLayout *gridLayout_12;
    QLabel *roomlabel;
    QTextEdit *roomTextEdit;
    QPushButton *startGameBtn;

    void setupUi(QWidget *MyWidget)
    {
        if (MyWidget->objectName().isEmpty())
            MyWidget->setObjectName(QString::fromUtf8("MyWidget"));
        MyWidget->resize(901, 471);
        verticalLayout_2 = new QVBoxLayout(MyWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
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


        verticalLayout_2->addWidget(connectGroup);

        groupBox = new QGroupBox(MyWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_8 = new QHBoxLayout(groupBox);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        talkGroup = new QGroupBox(groupBox);
        talkGroup->setObjectName(QString::fromUtf8("talkGroup"));
        talkGroup->setEnabled(false);
        talkGroup->setMaximumSize(QSize(437, 16777215));
        gridLayout_10 = new QGridLayout(talkGroup);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        gameLabel = new QLabel(talkGroup);
        gameLabel->setObjectName(QString::fromUtf8("gameLabel"));

        gridLayout_10->addWidget(gameLabel, 0, 0, 1, 1);

        guessLabel = new QLabel(talkGroup);
        guessLabel->setObjectName(QString::fromUtf8("guessLabel"));

        gridLayout_10->addWidget(guessLabel, 2, 0, 1, 1);

        msgLineEdit = new QLineEdit(talkGroup);
        msgLineEdit->setObjectName(QString::fromUtf8("msgLineEdit"));

        gridLayout_10->addWidget(msgLineEdit, 3, 0, 1, 1);

        gameTextEdit = new QTextEdit(talkGroup);
        gameTextEdit->setObjectName(QString::fromUtf8("gameTextEdit"));
        gameTextEdit->setReadOnly(true);

        gridLayout_10->addWidget(gameTextEdit, 1, 0, 1, 1);

        sendBtn = new QPushButton(talkGroup);
        sendBtn->setObjectName(QString::fromUtf8("sendBtn"));

        gridLayout_10->addWidget(sendBtn, 4, 0, 1, 1);


        horizontalLayout_8->addWidget(talkGroup);

        roomGroup = new QGroupBox(groupBox);
        roomGroup->setObjectName(QString::fromUtf8("roomGroup"));
        gridLayout_12 = new QGridLayout(roomGroup);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        gridLayout_12->setContentsMargins(9, 9, 9, 9);
        roomlabel = new QLabel(roomGroup);
        roomlabel->setObjectName(QString::fromUtf8("roomlabel"));

        gridLayout_12->addWidget(roomlabel, 0, 0, 1, 1);

        roomTextEdit = new QTextEdit(roomGroup);
        roomTextEdit->setObjectName(QString::fromUtf8("roomTextEdit"));

        gridLayout_12->addWidget(roomTextEdit, 1, 0, 1, 1);

        startGameBtn = new QPushButton(roomGroup);
        startGameBtn->setObjectName(QString::fromUtf8("startGameBtn"));

        gridLayout_12->addWidget(startGameBtn, 2, 0, 1, 1);


        horizontalLayout_8->addWidget(roomGroup);


        verticalLayout_2->addWidget(groupBox);

        QWidget::setTabOrder(portSpinBox, hostLineEdit);
        QWidget::setTabOrder(hostLineEdit, conectBtn);

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
        gameLabel->setText(QCoreApplication::translate("MyWidget", "Game", nullptr));
        guessLabel->setText(QCoreApplication::translate("MyWidget", "Guess", nullptr));
        sendBtn->setText(QCoreApplication::translate("MyWidget", "send", nullptr));
        roomlabel->setText(QCoreApplication::translate("MyWidget", "Room", nullptr));
        startGameBtn->setText(QCoreApplication::translate("MyWidget", "StartGame", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MyWidget: public Ui_MyWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYWIDGET_H
