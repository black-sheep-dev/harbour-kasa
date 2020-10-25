#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#define     TPLINK_CIPHER_START_KEY         171
#define     TPLINK_DEFAULT_PORT             9999

#include <QObject>

#include <QAbstractSocket>
#include <QHash>
#include <QJsonObject>
#include <QQueue>
#include <QUdpSocket>

class ApiInterface : public QObject
{
    Q_OBJECT

public:
    explicit ApiInterface(QObject *parent = nullptr);
    ~ApiInterface() override;

signals:
    void connectionError(const QString &hostname);
    void replyAvailable(const QString &hostname,
                        const QString &topic,
                        const QString &cmd,
                        const QJsonObject &payload = QJsonObject());

public slots:
    void searchDevices();
    void sendRequest(const QString &hostname, const QByteArray &payload);
    void sendRequest(const QString &hostname,
                     const QString &topic,
                     const QString &cmd,
                     const QJsonObject &payload = QJsonObject());

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError error);
    void onReply();

    void parseDatagram();

private:
    void startSending(const QString &hostname);

    // encryption
    QByteArray decrypt(const QByteArray &data);
    QByteArray encrypt(const QByteArray &data);

    QHash<QString, bool> m_isSending;
    QHash<QString, QQueue<QByteArray> *> m_queues;

    QUdpSocket *m_udpSocket;
};

#endif // APIINTERFACE_H
