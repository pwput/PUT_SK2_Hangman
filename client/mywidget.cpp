#include "mywidget.h"
#include "ui_mywidget.h"

#include "../config.h"
#include "../Message.h"

#include <QMessageBox>
#include <iostream>

Message stringToMessage(QString string){
    cout<<"recived:"<<endl;
    cout<<string.toStdString()<<endl;
    cout<<"end"<<endl;
    if (string.contains(QString::fromStdString(Commands[START_GAME]))){
        auto mid = string.mid(std::string(Commands[START_GAME]).length());
        auto std = mid.toStdString();
        return {Commands[START_GAME],std};
    }
    if (string.contains(QString::fromStdString(Commands[SCOREBOARD]))){
        auto mid = string.mid(std::string(Commands[SCOREBOARD]).length());
        auto std = mid.toStdString();
        return {Commands[SCOREBOARD],std};
    }

    if (string.contains(QString::fromStdString(Commands[SEND_LETTER]))) {
        auto mid = string.mid(std::string(Commands[SEND_LETTER]).length());
        auto std = mid.toStdString();
        return {Commands[SEND_LETTER], std};
    }

    if (string.contains(QString::fromStdString(Commands[END_GAME]))){
        auto mid = string.mid(std::string(Commands[END_GAME]).length());
        auto std = mid.toStdString();
        return {Commands[END_GAME],std};
    }
    return  {"",""};
}


MyWidget::MyWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MyWidget) {
    ui->setupUi(this);

    connect(ui->conectBtn, &QPushButton::clicked, this, &MyWidget::connectBtnHit);
    connect(ui->hostLineEdit, &QLineEdit::returnPressed, ui->conectBtn, &QPushButton::click);

    connect(ui->sendBtn, &QPushButton::clicked, this, &MyWidget::sendBtnHit);
    connect(ui->msgLineEdit, &QLineEdit::returnPressed, ui->sendBtn, &QPushButton::click);

    connect(ui->startGameBtn,&QPushButton::clicked,this, &MyWidget::startGameBTnHit);
}

MyWidget::~MyWidget() {
    if(sock)
        sock->close();
    delete ui;
}

void MyWidget::connectBtnHit(){
    ui->connectGroup->setEnabled(false);
    ui->gameTextEdit->append("<b>Connecting to " + ui->hostLineEdit->text() + ":" + QString::number(ui->portSpinBox->value())+"</b>");
    if(sock)
        delete sock;
    sock = new QTcpSocket(this);
    connTimeoutTimer = new QTimer(this);
    connTimeoutTimer->setSingleShot(true);
    connect(connTimeoutTimer, &QTimer::timeout, [&]{
        sock->abort();
        sock->deleteLater();
        sock = nullptr;
        connTimeoutTimer->deleteLater();
        connTimeoutTimer=nullptr;
        ui->connectGroup->setEnabled(true);
        ui->gameTextEdit->append("<b>Connect timed out</b>");
        QMessageBox::critical(this, "Error", "Connect timed out");
    });

    connect(sock, &QTcpSocket::connected, this, &MyWidget::socketConnected);
    connect(sock, &QTcpSocket::disconnected, this, &MyWidget::socketDisconnected);
    connect(sock, &QTcpSocket::errorOccurred, this, &MyWidget::socketError);
    connect(sock, &QTcpSocket::readyRead, this, &MyWidget::socketReadable);

    sock->connectToHost(ui->hostLineEdit->text(), ui->portSpinBox->value());
    connTimeoutTimer->start(3000);
}

void MyWidget::socketConnected(){
    connTimeoutTimer->stop();
    connTimeoutTimer->deleteLater();
    connTimeoutTimer=nullptr;
    ui->talkGroup->setEnabled(true);
    ui->gameTextEdit->setEnabled(true);
}

void MyWidget::socketDisconnected(){
    ui->gameTextEdit->append("<b>Disconnected</b>");
    ui->talkGroup->setEnabled(false);
    ui->connectGroup->setEnabled(true);
}

void MyWidget::socketError(QTcpSocket::SocketError err){
    if(err == QTcpSocket::RemoteHostClosedError)
        return;
    if(connTimeoutTimer){
        connTimeoutTimer->stop();
        connTimeoutTimer->deleteLater();
        connTimeoutTimer=nullptr;
    }
    QMessageBox::critical(this, "Error", sock->errorString());
    ui->gameTextEdit->append("<b>Socket error: "+sock->errorString()+"</b>");
    ui->talkGroup->setEnabled(false);
    ui->roomGroup->setEnabled(false);
    ui->connectGroup->setEnabled(true);
}

void MyWidget::socketReadable(){

    QByteArray ba = sock->readAll();

    auto str = QString::fromUtf8(ba).trimmed();

    auto n = str.toStdString();

    bool end = false;

    while(!n.empty()){

        string read;
        char c;
        do {
            c = n[0];
            read += c;
            n.erase(0,1);
            end = (c=='\000' || c=='\n');
        } while (!end);

    auto message = stringToMessage(QString::fromStdString(read));

    if (message.command.empty())
        return;

    if (message.command == Commands[START_GAME]){
        startGame(message.content);
    }

    if (message.command == Commands[SCOREBOARD]){
        setScoreBoard(message.content);
    }

    if (message.command == Commands[END_GAME]){
        endGame(message.content);
    }

    if (message.command == Commands[SEND_LETTER]){
        setWord(message.content);
    }
}
}


void MyWidget::sendBtnHit(){
    auto txt = ui->msgLineEdit->text().trimmed();
    if(txt.isEmpty())
        return;
    if(txt.length()!= 1)
        return;
    auto au = (int) txt.toStdString()[0];
    auto ch = to_string(au);
    auto mess = Message(Commands[SEND_LETTER],ch).toSend();
    sock->write(QString::fromStdString(mess).toUtf8());
    ui->msgLineEdit->clear();
    ui->msgLineEdit->setFocus();
}

void MyWidget::startGameBTnHit() {
    auto message = QString::fromStdString(Message(Commands[START_GAME], to_string(1)).toSend());
    sock ->write(message.toUtf8());
}

void MyWidget::startGame(std::string basicString) {
    if (basicString.find(to_string(RESOLUT_SUCCESS)) != string::npos){
        ui->roomGroup->setEnabled(false);
        ui->talkGroup->setEnabled(true);
    }
}



void MyWidget::setScoreBoard(std::string basicString) {
    ui->roomTextEdit->clear();
    ui->roomTextEdit->append("name\tscore\tlives\tstate");
    ui->roomTextEdit->append(convertScoreBoard(basicString));

}

void MyWidget::endGame(std::string basicString) {
    ui->talkGroup->setEnabled(false);
    ui->roomGroup->setEnabled(true);
}

QString MyWidget::convertScoreBoard(std::string basicString) {
    auto res = QString::fromStdString(basicString);
    res.replace(";","\n");
    res.replace(',','\t');
    return res;
}

void MyWidget::setWord(std::string basicString) {
    ui->gameTextEdit->clear();
    ui->gameTextEdit->append(QString::fromStdString(basicString));
}
