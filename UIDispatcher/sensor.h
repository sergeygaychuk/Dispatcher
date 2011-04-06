#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include <QObject>
#include <QDateTime>

class Sensor : public QObject
{
    Q_OBJECT
protected:
    explicit Sensor(QObject *parent = 0);

public:
    QString address();
    int type();
    QDateTime date();
    QString state();
protected:
    void setAddress(const QString& aAddress);
    void setType(int aType);
    void setDate(const QDateTime& aDate);
    void setState(const QString& aState);
private:
    QString m_Address;
    int m_Type;
    QDateTime m_Date;
    QString m_State;
};

#endif // TEMPERATURESENSOR_H
