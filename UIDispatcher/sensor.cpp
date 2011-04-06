#include "sensor.h"

#include "sqliteprotocol.h"

Sensor::Sensor(QObject *parent)
    : QObject(parent)
    , m_Type(SP_DEVICE_TYPE_UNKNOWN)
{
}

QString Sensor::address() {
    return m_Address;
}

int Sensor::type() {
    return m_Type;
}

QDateTime Sensor::date() {
    return m_Date;
}

QString Sensor::state() {
    return m_State;
}

void Sensor::setAddress(const QString& aAddress) {
    m_Address = aAddress;
}

void Sensor::setType(int aType) {
    m_Type = aType;
}

void Sensor::setDate(const QDateTime& aDate) {
    m_Date = aDate;
}

void Sensor::setState(const QString& aState) {
    m_State = aState;
}
