#include "temperaturesensorcontroller.h"
#include "temperaturesensor.h"

#include "sqliteprotocol.h"

#include <QTimer>

class TemperatureSensorWrapper : public TemperatureSensor {
public:
    TemperatureSensorWrapper(sp_device *aDevice, QObject *aParent)
        : TemperatureSensor(aDevice->m_Address, aParent),
        m_Device(aDevice) {
    }
    ~TemperatureSensorWrapper() {
        if (m_Device)
            sp_device_free(m_Device);
    }
public:
    void update() {
        sp_device_data *data = sp_device_get_last_data(m_Device);
        if (data) {
            setTemperature(QString(data->m_State).toFloat());
            sp_device_data_free(data);
        } else {
            setTemperature(0.0);
        }
    }
private:
    sp_device *m_Device;
};

TemperatureSensorController::TemperatureSensorController(QObject *parent) :
    QObject(parent)
{
    QTimer::singleShot(500, this, SLOT(on_timer()));
}

int enumerate_callback(void *aUserData, sp_device *aDevice) {
    if (aUserData && aDevice) {
        if (SP_DEVICE_TYPE_TEMPERATURE_SENSOR == aDevice->m_Type) {
            QList<TemperatureSensor*> *list = (QList<TemperatureSensor*> *)aUserData;
            list->append(new TemperatureSensorWrapper(aDevice, 0) );
        } else {
            sp_device_free(aDevice);
        }
    }
    return -1;
}

void TemperatureSensorController::startRead() {
    QList<TemperatureSensor*> list;
    sp_devices_enumerate(&enumerate_callback, (void *)&list);
    if (!list.empty()) {
        for (int i = 0; i < list.length(); ++ i) {
            ((TemperatureSensorWrapper *)list[i])->update();
        }
    }
    emit listReady(list);
    QTimer::singleShot(500, this, SLOT(on_timer()));
}

void TemperatureSensorController::on_timer() {
    startRead();
}
