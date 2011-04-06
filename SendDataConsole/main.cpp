#include <QtCore/QCoreApplication>
#include <QtCore/QTime>
#include <QtCore/QThread>
#include <QtCore/QUuid>

#include "sqliteprotocol.h"

#include <stdio.h>

class QThreadI : public QThread {
public:
    static void sleep_i() {
        QThread::sleep(1);
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    sp_devices_clear();
    sp_device *devices[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int current = 0;
    int pos = 0;
    while (pos < 50) {
        if (0 == (pos % 5)) {
            sp_device *device = 0;
            while (!device) {
                device = sp_device_add(QUuid::createUuid().toString().toAscii(), (qrand() % 2) + 1);
                if (!device) QThreadI::sleep_i();
                else printf("Add device: %s - %d\n", device->m_Address, device->m_Type);
            }
            devices[current] = device;
            ++ current;
        }
        for (int i = 0; i < 10; ++ i) {
            if (devices[i]) {
                QDateTime dt = QDateTime::currentDateTime();
                QString state = "";
                if (SP_DEVICE_TYPE_TEMPERATURE_SENSOR == devices[i]->m_Type) {
                    state = QString::number(qrand() % 50) + QString(".") + QString::number(qrand() % 100);
                } else {
                    state = (qrand() % 2) == 0 ? "off" : "on";
                }
                sp_device_add_data(devices[i], (int)dt.toTime_t(), state.toAscii());
                printf("Add data: %d\n", (int)dt.toTime_t());
            }
        }
        QThreadI::sleep_i();
        ++ pos;

        printf("POSITION %ld\n", pos);
    }
    printf("End of execution\n");
    for (int i = 0; i < 10; ++ i) {
        if (devices[i]) {
            sp_device_free(devices[i]);
        }
    }
    return a.exec();
}
