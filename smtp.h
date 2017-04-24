#ifndef SMTP_HPP
#define SMTP_HPP

#include "mime_message.h"
#include <QTcpSocket>
#include <QSslSocket>

namespace Mail {

class SMTP
{
public:
    SMTP(QString host, quint16 port = 25, bool ssl = false);
    ~SMTP();

    void setLogin(const QString &login);
    void setPass(const QString &pass);
    void setName(const QString &name);
    void setConnectionTimeout(int timeout);

    QString getLog();

    bool send(MimeMessage *message);

protected:
    bool tcpConnect();
    void tcpDisconnect();
    bool checkRespCode(int code);
    bool sendCmd(const QString &cmd, int respCode);
    bool waitForResponse(bool isMessage = false);
    bool sendText(const QString &text, bool isMessage = false);
    void writeLog(const QString &str);

    QTcpSocket *socket;
    MimeMessage *message;
    QString smtp_host;
    quint16 smtp_port;
    QString smtp_user;
    QString smtp_pass;
    QString smtp_name;
    QString responseText;
    QString log;
    bool ssl;
    int responseCode;
    int connectionTimeout;
    int commandRespTimeout;
    int messageRespTimeout;
    int writeTimeout;
};

}

#endif // SMTP_HPP
