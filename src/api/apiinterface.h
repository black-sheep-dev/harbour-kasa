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

    Q_PROPERTY(bool debug READ debug WRITE setDebug NOTIFY debugChanged)

public:
    explicit ApiInterface(QObject *parent = nullptr);
    ~ApiInterface() override;

    // properties
    bool debug() const;

signals:
    void connectionError(const QString &hostname);
    void replyAvailable(const QString &hostname,
                        const QString &topic,
                        const QString &cmd,
                        const QJsonObject &payload = QJsonObject());

    void debugChanged(bool debug);

public slots:
    void searchDevices();
    void sendRequest(const QString &hostname, const QByteArray &payload);
    void sendRequest(const QString &hostname,
                     const QString &topic,
                     const QString &cmd,
                     const QJsonObject &payload = QJsonObject());

    // properties
    void setDebug(bool debug);

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError error);
    void onReply();

    void parseDatagram();

private:
    void logData(const QString &hostname, const QJsonDocument &doc);
    void startSending(const QString &hostname);

    // encryption
    QByteArray decrypt(const QByteArray &data);
    QByteArray encrypt(const QByteArray &data);

    QHash<QString, bool> m_isSending;
    QHash<QString, QQueue<QByteArray> *> m_queues;

    QUdpSocket *m_udpSocket;

    // properties
    bool m_debug;
};

#endif // APIINTERFACE_H
