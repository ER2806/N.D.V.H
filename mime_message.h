#ifndef MIME_MESSAGE_HPP
#define MIME_MESSAGE_HPP

#include <QStringList>
#include <QDebug>
#include <QDate>
#include <QFileInfo>
#include <QCryptographicHash>

#define FORMAT_CNT 76
#define EMAIL_REGEXP "^[-a-z0-9!#$%&'*+/=?^_`{|}~]+(?:\\.[-a-z0-9!#$%&'*+/=?^_`{|}~]+)"\
  "*@(?:[a-z0-9]([-a-z0-9]{0,61}[a-z0-9])?\\.)*([a-z0-9]([-a-z0-9]{0,61}[a-z0-9])?\\.)[a-z][a-z]+$"

namespace Mail {

class MimeMessage
{
public:
    MimeMessage();

    bool setSender(const QString email, const QString &name); // format ("test@test.com", "Jon Smitt")
    bool addRcpt(const QString email, const QString &name);   // format ("test@test.com", "Jon Smitt")

    void setSubject(const QString &subj);
    void setDate(const QDateTime dt);

    void addPlainTextPart(const QString &plain_text);
    bool addAttach(const QString &filename);
    void addAttach(QByteArray *ba, const QString &name);

    QStringList *rcptList();
    const QString getFromEmail();

    const QString getMimeText();

protected:
    const QString str2base64(const QString &str);
    const QString headerEncoder(const QString &str);
    const QString formatter(const QString &str);
    const QString dateToRFC2822(const QDateTime *dt);
    const QString getBoundary();
    bool emailIsValid(const QString &email);

    /* required fields */
    QString from;
    QString to;
    QString subject;
    /* other fields */
    //QString messageId;
    //QString userAgent;
    QString mDate;

    QString plainText;
    QString attach;
    //QString multipart;
    QString s_from;

    QString lastBoundary;
    QString mainBoundary;

    QStringList rcptLst;
};

}


#endif // MIME_MESSAGE_HPP
