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
    QVERIFY2(length == 0, "Device list length is not equal to 0");
    QVERIFY2(list, "Device list is null");

    sp_add_device_to_list(list, "AAFF1234", SP_DEVICE_TYPE_TEMPERATURE_SENSOR);
    sp_add_device_to_list(list, "1234AAFF", SP_DEVICE_TYPE_RELAY);

    length = sp_get_device_list_length(list);
    QVERIFY2(length == 2, "Device list length is not equal to 2");

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
    sp_add_device_to_list(list, "AAFF1234", SP_DEVICE_TYPE_TEMPERATURE_SENSOR);
    sp_add_device_to_list(list, "1234AAFF", SP_DEVICE_TYPE_RELAY);

    length = sp_get_device_list_length(list);
    QVERIFY2(length == 2, "Device list length is not equal to 2");

    sp_device *device = sp_get_device_from_list(list, 0);
    QVERIFY2(device, "Wrong device in 0 position");
    QVERIFY2(!strcmp(device->m_Address, "AAFF1234"), "Wrong device address");
    QVERIFY2(device->m_Type == SP_DEVICE_TYPE_TEMPERATURE_SENSOR, "Wrong device type");

    sp_device_add_data(device, 109, "37.2");
    sp_device_data *aTemperatureData = sp_get_device_last_data(device);
    QVERIFY2(aTemperatureData, "Temperature data is null");
    QVERIFY2(!strcmp(aTemperatureData->m_State, "37.2"), "Temperature sensor temp is not equal to 37.2");
    QVERIFY2(aTemperatureData->m_Date == 109, "Temperature sensor date is not equal to 109");

    sp_device_add_data(device, 110, "37.6");
    aTemperatureData = sp_get_device_last_data(device);
    QVERIFY2(aTemperatureData, "Temperature data is null");
    QVERIFY2(!strcmp(aTemperatureData->m_State, "37.6"), "Temperature sensor temp is not equal to 37.6");
    QVERIFY2(aTemperatureData->m_Date == 110, "Temperature sensor date is not equal to 110");

    device = sp_get_device_from_list(list, 1);
    QVERIFY2(device, "Wrong device in 1 position");
    QVERIFY2(!strcmp(device->m_Address, "1234AAFF"), "Wrong device address");
    QVERIFY2(device->m_Type == SP_DEVICE_TYPE_RELAY, "Wrong device type");

    sp_device_add_data(device, 111, "on");
    sp_device_data *aRelayData = sp_get_device_last_data(device);
    QVERIFY2(aRelayData, "Relay data is null");
    QVERIFY2(!strcmp(aRelayData->m_State, "on"), "Relay is not on");
    QVERIFY2(aRelayData->m_Date == 111, "Relay date is not equal to 109");

    sp_close_device_list(list);
}

QTEST_APPLESS_MAIN(TestSqliteProtocolTest);

#include "tst_testsqliteprotocoltest.moc"
