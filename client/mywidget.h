#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTimer>

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();

protected:
    QTcpSocket * sock {nullptr};
    QTimer * connTimeoutTimer{nullptr};
    void startGameBTnHit();
    void connectBtnHit();
    void socketConnected();
    void socketDisconnected();
    void socketError(QTcpSocket::SocketError);
    void socketReadable();
    void sendBtnHit();

private:
    Ui::MyWidget * ui;


    void startGame(std::string basicString);

    void setScoreBoard(std::string basicString);

    void endGame(std::string basicString);

    QString convertScoreBoard(std::string basicString);

    void setWord(std::string basicString);
};

#endif // MYWIDGET_H
