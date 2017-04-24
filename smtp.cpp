#include "smtp.h"

using namespace Mail;

SMTP::SMTP(QString host, quint16 port, bool ssl){
    smtp_host = host;
    smtp_port = port;
    this->ssl = ssl;

    connectionTimeout = 5000;
    commandRespTimeout = 2000;
    messageRespTimeout = -1;
    writeTimeout = 1000;
    smtp_name = "localhost";

    if (ssl) {
        socket = new QSslSocket();
    }
    else {
        socket = new QTcpSocket();
    }
}

SMTP::~SMTP() {
    delete socket;
}

void SMTP::setLogin(const QString &login) {
    smtp_user = login.trimmed();
}

void SMTP::setPass(const QString &pass) {
    smtp_pass = pass;
}

void SMTP::setName(const QString &name) {
    smtp_name = name;
}

void SMTP::setConnectionTimeout(int timeout) {
    connectionTimeout = timeout;
}

QString SMTP::getLog() {
    return log;
}

bool SMTP::send(MimeMessage *message) {
    log.clear();
    int mSize = message->getMimeText().toLatin1().size();
    /* calc timeout for slowly 56 kbps connection */
    messageRespTimeout = (mSize / (56000/8)) * 1000;

    if (!tcpConnect()) return false;
    if (!waitForResponse()) return false;
    if (!checkRespCode(220)) return false;
    if (!sendCmd("EHLO " + smtp_name,250)) return false;
    if (!smtp_user.isEmpty() || smtp_pass.isEmpty()) {
        if (!sendCmd("AUTH LOGIN",334)) return false;
        if (!sendCmd(smtp_user.toUtf8().toBase64(),334)) return false;
        if (!sendCmd(smtp_pass.toUtf8().toBase64(),235)) return false;
    }
    if (!sendCmd("MAIL FROM: " + message->getFromEmail(),250)) return false;
    for (int i=0; i < message->rcptList()->count(); ++i) {
        if (!sendCmd("RCPT TO: " + message->rcptList()->at(i),250)) return false;
    }
    if (!sendCmd("DATA",354)) return false;
    sendText(message->getMimeText(),true);
    sendText(".");
    if (!waitForResponse(true)) return false;
    if (!checkRespCode(250)) return false;
    if (!sendCmd("QUIT",221)) return false;

    tcpDisconnect();
    return true;
}

bool SMTP::tcpConnect() {
    writeLog("Start session");
    if (ssl) {
        ((QSslSocket*)socket)->connectToHostEncrypted(smtp_host, smtp_port);
    }
    else {
        ((QTcpSocket*)socket)->connectToHost(smtp_host, smtp_port);
    }
    if (!socket->waitForConnected(connectionTimeout)) {
        writeLog("Error connecting to "+smtp_host+QString(":%1").arg(smtp_port));
        writeLog("Connection timeout");
        return false;
    }
    return true;
}

void SMTP::tcpDisconnect() {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
    }
    writeLog("Stop session");
}

bool SMTP::checkRespCode(int code) {
    if (code != responseCode) {
        writeLog("Error: bad response code");
        tcpDisconnect();
        return false;
    }
    return true;
}

bool SMTP::sendCmd(const QString &cmd, int respCode) {
    sendText(cmd);
    if (!waitForResponse(false)) return false;
    if (!checkRespCode(respCode)) return false;
    return true;
}

bool SMTP::waitForResponse(bool isMessage) {
    int tm = isMessage ? messageRespTimeout : commandRespTimeout;
    if (!socket->waitForReadyRead(tm)) {
        if (isMessage) {
            writeLog("Error: Message response timeout");
        }
        else {
            writeLog("Error: Command response timeout");
        }
        tcpDisconnect();
        return false;
    }
    while (socket->canReadLine()) {
        responseText = socket->readLine();
        responseCode = responseText.left(3).toInt();
        writeLog("S <= " + responseText);
    }
    return true;
}

bool SMTP::sendText(const QString &text, bool isMessage) {
    socket->write(text.toLatin1() + "\r\n");
    if (!socket->waitForBytesWritten(writeTimeout)) {
        return false;
    }
    if (!isMessage) {
        writeLog("C => " + text.trimmed());
    }
    else {
        writeLog("C => <message>");
    }
    return true;
}

void SMTP::writeLog(const QString &str) {
    QString timestamp = QDateTime::currentDateTime().toString("[yyyy.MM.dd hh.mm.ss.zzz] ");
    log.append(timestamp + str.trimmed() + "\r\n");
}
