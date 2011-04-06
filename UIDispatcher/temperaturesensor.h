#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include <QObject>

class TemperatureSensor : public QObject
{
    Q_OBJECT
protected:
    explicit TemperatureSensor(QString aName, QObject *parent = 0);

public:
    QString name();
    qreal temperature();
protected:
    void setTemperature(qreal aTemperature);
private:
    QString m_Name;
    qreal m_Temperature;
};

#endif // TEMPERATURESENSOR_H
