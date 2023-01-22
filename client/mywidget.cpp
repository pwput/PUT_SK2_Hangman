#include "mywidget.h"
#include "ui_mywidget.h"

#include <QMessageBox>

MyWidget::MyWidget(QWidget *parent) : QWidget(parent), ui(new Ui::MyWidget) {
    ui->setupUi(this);

    connect(ui->conectBtn, &QPushButton::clicked, this, &MyWidget::connectBtnHit);
    connect(ui->hostLineEdit, &QLineEdit::returnPressed, ui->conectBtn, &QPushButton::click);

    connect(ui->sendBtn, &QPushButton::clicked, this, &MyWidget::sendBtnHit);
    connect(ui->msgLineEdit, &QLineEdit::returnPressed, ui->sendBtn, &QPushButton::click);
}

MyWidget::~MyWidget() {
    if(sock)
        sock->close();
    delete ui;
}

void MyWidget::connectBtnHit(){
    ui->connectGroup->setEnabled(false);
    ui->msgsTextEdit->append("<b>Connecting to " + ui->hostLineEdit->text() + ":" + QString::number(ui->portSpinBox->value())+"</b>");
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
        ui->msgsTextEdit->append("<b>Connect timed out</b>");
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
}

void MyWidget::socketDisconnected(){
    ui->msgsTextEdit->append("<b>Disconnected</b>");
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
    ui->msgsTextEdit->append("<b>Socket error: "+sock->errorString()+"</b>");
    ui->talkGroup->setEnabled(false);
    ui->connectGroup->setEnabled(true);
}

void MyWidget::socketReadable(){
    QByteArray ba = sock->readAll();
    ui->msgsTextEdit->append(QString::fromUtf8(ba).trimmed());
    ui->msgsTextEdit->setAlignment(Qt::AlignLeft);
}

void MyWidget::sendBtnHit(){
    auto txt = ui->msgLineEdit->text().trimmed();
    if(txt.isEmpty())
        return;
    sock->write((txt+'\n').toUtf8());

    ui->msgsTextEdit->append("<span style=\"color: blue\">"+txt+"</span>");
    ui->msgsTextEdit->setAlignment(Qt::AlignRight);

    ui->msgLineEdit->clear();
    ui->msgLineEdit->setFocus();
}
