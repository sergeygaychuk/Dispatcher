#include "temperaturesensor.h"

TemperatureSensor::TemperatureSensor(QString aName, QObject *parent)
    : QObject(parent)
    , m_Name(aName)
    , m_Temperature(0.00)
{
}

QString TemperatureSensor::name() {
    return m_Name;
}

qreal TemperatureSensor::temperature() {
    return m_Temperature;
}

void TemperatureSensor::setTemperature(qreal aTemperature) {
    m_Temperature = aTemperature;
}
