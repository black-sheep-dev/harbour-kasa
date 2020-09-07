#ifndef KASACONTROL_H
#define KASACONTROL_H

#include <QObject>

class KasaControl : public QObject
{
    Q_OBJECT
public:
    explicit KasaControl(QObject *parent = nullptr);

signals:

public slots:
    Q_INVOKABLE void initialize();

private:
};

#endif // KASACONTROL_H
