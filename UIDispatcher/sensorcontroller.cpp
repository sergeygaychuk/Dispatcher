#include "sensorcontroller.h"
#include "sensor.h"

#include "sqliteprotocol.h"

#include <QTimer>

class SensorWrapper : public Sensor {
public:
    SensorWrapper(sp_device *aDevice, QObject *aParent)
        : Sensor(aParent), m_Device(aDevice) {
        setAddress(QString(m_Device->m_Address));
        setType(m_Device->m_Type);
        setState(QString(""));
        setDate(QDateTime::currentDateTime());
    }
    ~SensorWrapper() {
        if (m_Device)
            sp_device_free(m_Device);
    }
public:
    void update() {
        sp_device_data *data = sp_device_get_last_data(m_Device);
        if (data) {
            setState(QString(data->m_State));
            setDate(QDateTime::fromTime_t((uint)data->m_Date));
            sp_device_data_free(data);
        }
    }
private:
    sp_device *m_Device;
};

SensorController::SensorController(QObject *parent) :
    QObject(parent),
    m_NeedUpdateList(0)
{
}

int enumerate_callback(void *aUserData, sp_device *aDevice) {
    if (aUserData && aDevice) {
        QList<Sensor*> *list = (QList<Sensor*> *)aUserData;
        list->append(new SensorWrapper(aDevice, 0) );
        return 0;
    }
    return -1;
}

void SensorController::startRead() {
    if ((m_NeedUpdateList % 2) == 0) {
        m_List.clear();
        m_NeedUpdateList = 0;
        sp_devices_enumerate(&enumerate_callback, (void *)&m_List);
    }
    if (!m_List.empty()) {
        for (int i = 0; i < m_List.length(); ++ i) {
            ((SensorWrapper *)m_List[i])->update();
        }
        emit listReady(m_List);
    }
    ++ m_NeedUpdateList;
    QTimer::singleShot(500, this, SLOT(on_timer()));
}

void SensorController::on_timer() {
    startRead();
}
