#ifndef TEMPERATURESENSORCONTROLLER_H
#define TEMPERATURESENSORCONTROLLER_H

#include <QObject>
#include <QList>

class TemperatureSensor;

class TemperatureSensorController : public QObject
{
    Q_OBJECT
public:
    explicit TemperatureSensorController(QObject *parent = 0);

    void startRead();
signals:
    void listReady(QList<TemperatureSensor*> aList);
protected slots:
    void on_timer();
};

#endif // TEMPERATURESENSORCONTROLLER_H
