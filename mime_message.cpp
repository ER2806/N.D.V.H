#include "mime_message.h"

using namespace Mail;

MimeMessage::MimeMessage() {
    //clearAll();
    setDate(QDateTime::currentDateTime());
    mainBoundary = getBoundary();
}

bool MimeMessage::setSender(const QString email, const QString &name) {
    if (!emailIsValid(email))
        return false;
    if (name.trimmed().isEmpty())
        from = "\r\nFrom: " + email;
    else
        from = "\r\nFrom: " + headerEncoder(name.trimmed()) + " <" + email + ">";
    s_from = "<" + email + ">";
    return true;
}

bool MimeMessage::addRcpt(const QString email, const QString &name) {
    if (!emailIsValid(email))
        return false;
    if (to.isEmpty()) to = "\r\nTo: ";
    if (name.trimmed().isEmpty()) {
        if (to.indexOf('@') == -1) {
            to += email;
        }
        else {
            to += ",\r\n    " + email;
        }
    }
    else {
      if (to.indexOf('@') == -1) {
          to += headerEncoder(name.trimmed()) + " <" + email + ">";
      }
      else {
          to += ",\r\n    " + headerEncoder(name.trimmed()) + " <" + email + ">";
      }
    }
    rcptLst.append("<" + email + ">");
    return true;
}

QStringList *MimeMessage::rcptList() {
    return &rcptLst;
}

const QString MimeMessage::getFromEmail() {
    return s_from;
}

const QString MimeMessage::getMimeText() {
    if (from.isEmpty() || to.isEmpty())
        return "";

    QString output = "MIME-Version: 1.0";
    //output.append(messageId);
    //output.append(userAgent);
    output.append(mDate);
    output.append(from);
    output.append(to);
    output.append(subject);

    output.append("\r\nContent-Type: multipart/mixed;");
    output.append("\r\n  boundary=" + mainBoundary + "\r\n");

    output.append(plainText);
    //output.append(html);
    //output.append(multipart);
    output.append(attach);

    output.append("\r\n--" + mainBoundary + "--");
    output.append("\r\n");
    return output;
}

void MimeMessage::setSubject(const QString &subj) {
    subject = "\r\nSubject: " + headerEncoder(subj.trimmed());
}

void MimeMessage::setDate(const QDateTime dt) {
  mDate = "\r\nDate: " + dateToRFC2822(&dt);
}

void MimeMessage::addPlainTextPart(const QString &plain_text){
    if (plain_text.isEmpty()) {
        return;
    }
    plainText.append("\r\n--"+mainBoundary);
    plainText.append("\r\nContent-Type: text/plain; charset=UTF-8");
    plainText.append("\r\nContent-Transfer-Encoding: base64\r\n\r\n");
    plainText.append(formatter(str2base64(plain_text)));
}

bool MimeMessage::addAttach(const QString &filename) {
    if (filename.trimmed().isEmpty()) {
        return false;
    }
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        return false;
    }
    QString fn = headerEncoder(QFileInfo(f).fileName());
    QByteArray ba = f.readAll();
    f.close();
    addAttach(&ba,fn);
    return true;
}

void MimeMessage::addAttach(QByteArray *ba, const QString &name) {
    attach.append("\r\n--" + mainBoundary);
    attach.append("\r\nContent-Type: application/octet-stream;");
    attach.append("\r\n  name=\"" + name + "\"");
    attach.append("\r\nContent-Transfer-Encoding: base64");
    attach.append("\r\nContent-Disposition: attachment;");
    attach.append("\r\n  filename=\"" + name + "\"");
    attach.append("\r\n\r\n");
    attach.append(formatter(ba->toBase64()));
}

const QString MimeMessage::headerEncoder(const QString &str) {
  return "=?utf-8?B?" + str2base64(str) + "?=";
}

const QString MimeMessage::formatter(const QString &str)
{
  int pos = 0;
  int cnt = FORMAT_CNT;
  QString result = "";
  while(pos < str.length()-1)
  {
    result.append(str.mid(pos,cnt) + "\r\n");
    pos += cnt;
    if(pos + FORMAT_CNT < str.length()-1)
      cnt = FORMAT_CNT;
    else
      cnt = str.length() - pos;
  }
  return result;
}

const QString MimeMessage::dateToRFC2822(const QDateTime *dt) {
    QString buf;

    QLocale l = QLocale(QLocale::English);
    QString ddd = l.dayName(dt->date().dayOfWeek(), QLocale::ShortFormat);
    QString MMM = l.monthName(dt->date().month(), QLocale::ShortFormat);
    buf = ddd + ", " + dt->toString("d ") + MMM + dt->toString(" yyyy hh:mm:ss ");
    int utcOffset = dt->utcOffset();
    if (dt->timeSpec() == Qt::LocalTime) {
        QDateTime utc = dt->toUTC();
        utc.setTimeSpec(dt->timeSpec());
        utcOffset = utc.secsTo(*dt);
    }
    const int offset = qAbs(utcOffset);
    buf += QLatin1Char((offset == utcOffset) ? '+' : '-');
    const int hour = offset / 3600;
    if (hour < 10) {
        buf += QLatin1Char('0');
    }
    buf += QString::number(hour);
    const int min = (offset - (hour * 3600)) / 60;
    if (min < 10) {
        buf += QLatin1Char('0');
    }
    buf += QString::number(min);
    return buf;
}

const QString MimeMessage::getBoundary() {
    QCryptographicHash h(QCryptographicHash::Md5);
    QString dt = QDateTime::currentDateTime().toString("yyyyddMMhhmmsszzz");
    QString recursive_boundary = dt + lastBoundary;
    QByteArray data = recursive_boundary.toLatin1();
    h.addData(data);
    lastBoundary = h.result().toHex();
    return lastBoundary;
}

bool MimeMessage::emailIsValid(const QString &email) {
    QRegExp rx(EMAIL_REGEXP);
    return (rx.indexIn(email) != -1);
}

const QString MimeMessage::str2base64(const QString &str) {
  return str.toUtf8().toBase64();
}
