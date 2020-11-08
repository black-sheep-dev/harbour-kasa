#include "apiinterface.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QDateTime>
#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QStandardPaths>
#include <QTcpSocket>
#include <QTextStream>

ApiInterface::ApiInterface(QObject *parent) :
    QObject(parent),
    m_udpSocket(new QUdpSocket(this))
{
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &ApiInterface::parseDatagram);
    m_udpSocket->bind(QHostAddress::Broadcast, TPLINK_DEFAULT_PORT);
}

ApiInterface::~ApiInterface()
{
    qDeleteAll(m_queues.values().begin(), m_queues.values().end());
}

bool ApiInterface::debug() const
{
    return m_debug;
}

void ApiInterface::searchDevices()
{
    QByteArray data;

    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << QByteArray("{\"system\":{\"get_sysinfo\":{}}}");

    m_udpSocket->writeDatagram(encrypt(data), QHostAddress::Broadcast, TPLINK_DEFAULT_PORT);
}

void ApiInterface::sendRequest(const QString &hostname, const QByteArray &payload)
{
    if (hostname.isEmpty() || payload.isEmpty())
        return;

    if (!m_queues.keys().contains(hostname))
        m_queues.insert(hostname, new QQueue<QByteArray>());

    // encrypt data and append to queue
    QByteArray data;

    QDataStream out(&data, QIODevice::WriteOnly);
    out << encrypt(payload);

    m_queues.value(hostname)->enqueue(data);

    startSending(hostname);
}

void ApiInterface::sendRequest(const QString &hostname,
                               const QString &topic,
                               const QString &cmd,
                               const QJsonObject &payload)
{
    QJsonObject request;

    QJsonObject obj;
    obj.insert(cmd, payload);

    request.insert(topic, obj);

    sendRequest(hostname, QJsonDocument(request).toJson(QJsonDocument::Compact));
}

void ApiInterface::setDebug(bool debug)
{
    if (m_debug == debug)
        return;

    m_debug = debug;
    emit debugChanged(m_debug);
}

void ApiInterface::onConnected()
{
    auto *socket = qobject_cast<QTcpSocket *>(sender());

    if (!socket)
        return;

    const QByteArray payload = m_queues.value(socket->peerName())->dequeue();

    if (payload.isEmpty()) {
        socket->disconnectFromHost();
        return;
    }

    socket->write(payload);
}

void ApiInterface::onDisconnected()
{
    auto *socket = qobject_cast<QTcpSocket *>(sender());

    if (!socket)
        return;

    socket->deleteLater();
}

void ApiInterface::onError(QAbstractSocket::SocketError error)
{
#ifdef QT_NO_DEBUG
    Q_UNUSED(error)
#endif

    auto *socket = qobject_cast<QTcpSocket *>(sender());

    if (!socket)
        return;

#ifdef QT_DEBUG
    qDebug() << "CONNECTION ERROR";
    qDebug() << socket->peerName();
    qDebug() << error;
#endif

    // clear queue
    m_queues.value(socket->peerName())->clear();

    emit connectionError(socket->peerName());

    socket->deleteLater();
}

void ApiInterface::onReply()
{
    auto *socket = qobject_cast<QTcpSocket *>(sender());

    if (!socket)
        return;

    const QString hostname = socket->peerName();

    // read data
    QByteArray data = socket->readAll();

    QDataStream stream(&data, QIODevice::ReadOnly);

    QByteArray raw;
    stream >> raw;

    // close socket
    socket->disconnectFromHost();

    // decrypt and parse
    QJsonParseError parseError{};

    const QJsonDocument doc = QJsonDocument::fromJson(decrypt(raw), &parseError);
    const QJsonObject response = doc.object();

    if (parseError.error > 0)
        return;

    if (response.isEmpty())
        return;

    // log data
    if (m_debug) {
        logData(hostname, doc);
    }

    // parse data
    const QString topic = response.keys().first();

    const QJsonObject obj = response.value(topic).toObject();
    if (obj.isEmpty())
        return;

    const QString cmd = obj.keys().first();

    emit replyAvailable(hostname, topic, cmd, obj.value(cmd).toObject());
}

void ApiInterface::parseDatagram()
{
    QByteArray datagram;

    while (m_udpSocket->hasPendingDatagrams()) {
        datagram.resize(int(m_udpSocket->pendingDatagramSize()));
        m_udpSocket->readDatagram(datagram.data(), datagram.size());

        QDataStream stream(&datagram, QIODevice::ReadOnly);

        QByteArray raw;
        stream >> raw;
    }

}

void ApiInterface::logData(const QString &hostname, const QJsonDocument &doc)
{
    const QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + APP_TARGET + "/";

    // create folder
    QDir().mkpath(path);

    QFile file(path + hostname + QStringLiteral(".txt"));

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        return;

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << QString(doc.toJson(QJsonDocument::Indented)) << ",";

    file.close();
}

void ApiInterface::startSending(const QString &hostname)
{
    // connect to socket
    auto *socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &ApiInterface::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &ApiInterface::onDisconnected);
    connect(socket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            this, &ApiInterface::onError);

    connect(socket, &QTcpSocket::readyRead, this, &ApiInterface::onReply);

    socket->connectToHost(hostname, TPLINK_DEFAULT_PORT);
}

QByteArray ApiInterface::decrypt(const QByteArray &data)
{
    if (data.isNull() || data.isEmpty())
        return QByteArray();

    QByteArray out;

    int key = TPLINK_CIPHER_START_KEY;

    for (int idx = 0; idx < data.length(); ++idx) {
        int a = key ^ static_cast<int>(data[idx]);
        key = static_cast<int>(data[idx]);
        out.append(static_cast<char>(a));
    }

    return out;
}

QByteArray ApiInterface::encrypt(const QByteArray &data)
{
    if (data.isNull() || data.isEmpty())
        return QByteArray();

    QByteArray out;

    int key = TPLINK_CIPHER_START_KEY;

    for (int idx = 0; idx < data.length(); ++idx) {
        int a = key ^ static_cast<int>(data[idx]);
        key = a;
        out.append(static_cast<char>(a));
    }

    return out;
}
