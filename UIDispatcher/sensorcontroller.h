#ifndef TEMPERATURESENSORCONTROLLER_H
#define TEMPERATURESENSORCONTROLLER_H

#include <QObject>
#include <QList>

class Sensor;

class SensorController : public QObject
{
    Q_OBJECT
public:
    explicit SensorController(QObject *parent = 0);

    void startRead();
signals:
    void listReady(QList<Sensor*> aList);
protected slots:
    void on_timer();
private:
    int m_NeedUpdateList;
    QList<Sensor*> m_List;
};

#endif // TEMPERATURESENSORCONTROLLER_H
