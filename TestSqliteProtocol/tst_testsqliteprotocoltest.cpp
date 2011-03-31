#include <QtCore/QString>
#include <QtTest/QtTest>

#include "sqliteprotocol.h"

class TestSqliteProtocolTest : public QObject
{
    Q_OBJECT

public:
    TestSqliteProtocolTest();

private Q_SLOTS:
    void testRetrieveDeviceList();
    void testRetrieveDeviceData();
};

TestSqliteProtocolTest::TestSqliteProtocolTest()
{
}

void TestSqliteProtocolTest::testRetrieveDeviceList()
{
    sp_device_list *list = 0;
    int length = sp_get_device_list(&list);
    QVERIFY2(length == 2, "Device list length is not equal to 2");
    QVERIFY2(list, "Device list is null");

    sp_device *device = sp_get_device_from_list(list, -1);
    QVERIFY2(!device, "Wrong device in -1 position");
    device = sp_get_device_from_list(list, 2);
    QVERIFY2(!device, "Wrong device in 2 position");

    device = sp_get_device_from_list(list, 0);
    QVERIFY2(device, "Wrong device in 0 position");
    QVERIFY2(!strcmp(device->m_Address, "AAFF1234"), "Wrong device address");
    QVERIFY2(device->m_Type == SP_DEVICE_TYPE_TEMPERATURE_SENSOR, "Wrong device type");

    device = sp_get_device_from_list(list, 1);
    QVERIFY2(device, "Wrong device in 1 position");
    QVERIFY2(!strcmp(device->m_Address, "1234AAFF"), "Wrong device address");
    QVERIFY2(device->m_Type == SP_DEVICE_TYPE_RELAY, "Wrong device type");

    sp_close_device_list(list);
}

void TestSqliteProtocolTest::testRetrieveDeviceData()
{
    sp_device_list *list = 0;
    int length = sp_get_device_list(&list);
    QVERIFY2(length == 2, "Device list length is not equal to 2");
    QVERIFY2(list, "Device list is null");

    sp_device *device = sp_get_device_from_list(list, 0);
    QVERIFY2(device, "Wrong device in 0 position");
    QVERIFY2(!strcmp(device->m_Address, "AAFF1234"), "Wrong device address");
    QVERIFY2(device->m_Type == SP_DEVICE_TYPE_TEMPERATURE_SENSOR, "Wrong device type");

    sp_device_data *aTemperatureData = 0;
    aTemperatureData = sp_get_device_last_data(device);
    QVERIFY2(aTemperatureData, "Temperature data is null");
    sp_device_data_temperature_sensor *aTempSensor = (sp_device_data_temperature_sensor*)aTemperatureData;
    QVERIFY2(aTempSensor->m_Temperature == 37.2, "Temperature sensor temp is not equal to 37.2");
    sp_free_device_data(aTemperatureData);

    device = sp_get_device_from_list(list, 1);
    QVERIFY2(device, "Wrong device in 1 position");
    QVERIFY2(!strcmp(device->m_Address, "1234AAFF"), "Wrong device address");
    QVERIFY2(device->m_Type == SP_DEVICE_TYPE_RELAY, "Wrong device type");

    sp_device_data *aRelayData = 0;
    aRelayData = sp_get_device_last_data(device);
    QVERIFY2(aRelayData, "Relay data is null");
    sp_device_data_relay *aRelay = (sp_device_data_relay*)aRelayData;
    QVERIFY2(aRelay->m_State == SP_DEVICE_RELAY_ON, "Relay status is not ON");
    sp_free_device_data(aRelayData);

    sp_close_device_list(list);
}

QTEST_APPLESS_MAIN(TestSqliteProtocolTest);

#include "tst_testsqliteprotocoltest.moc"
