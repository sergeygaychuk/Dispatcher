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
private:
    static int enumerate_callback(void *aUserData, sp_device *aDevice);
private:
    static sp_device *m_Devices[2];
};

sp_device * TestSqliteProtocolTest::m_Devices[2] = {NULL, NULL};

TestSqliteProtocolTest::TestSqliteProtocolTest()
{
}

#define QVERIFY2_R(statement, description, r) \
do {\
    if (statement) {\
        if (!QTest::qVerify(true, #statement, (description), __FILE__, __LINE__))\
            return r;\
    } else {\
        if (!QTest::qVerify(false, #statement, (description), __FILE__, __LINE__))\
            return r;\
    }\
} while (0)

int TestSqliteProtocolTest::enumerate_callback(void *aUserData, sp_device *aDevice) {
    int *current = (int *)aUserData;
    if (current && aDevice) {
        switch (*current) {
        case 0:
            QVERIFY2_R(aDevice, "Wrong device in 0 position", -1);
            QVERIFY2_R(!strcmp(aDevice->m_Address, "AAFF1234"), "Wrong device address", -1);
            QVERIFY2_R(aDevice->m_Type == SP_DEVICE_TYPE_TEMPERATURE_SENSOR, "Wrong device type", -1);
            break;
        case 1:
            QVERIFY2_R(aDevice, "Wrong device in 1 position", -1);
            QVERIFY2_R(!strcmp(aDevice->m_Address, "1234AAFF"), "Wrong device address", -1);
            QVERIFY2_R(aDevice->m_Type == SP_DEVICE_TYPE_RELAY, "Wrong device type", -1);
            break;
        }
        TestSqliteProtocolTest::m_Devices[*current] = aDevice;
        *current = *current + 1;
        return 0;
    }
    return -1;
}

void TestSqliteProtocolTest::testRetrieveDeviceList()
{
    int iCurrent = 0;
    int length = 0;
    sp_devices_clear();
    length = sp_devices_length();
    QVERIFY2(length == 0, "Device list length is not equal to 0");

    sp_device_add("AAFF1234", SP_DEVICE_TYPE_TEMPERATURE_SENSOR);
    length = sp_devices_length();
    QVERIFY2(length == 1, "Device list length is not equal to 1");

    sp_device_add("1234AAFF", SP_DEVICE_TYPE_RELAY);
    length = sp_devices_length();
    QVERIFY2(length == 2, "Device list length is not equal to 2");

    sp_devices_enumerate(&TestSqliteProtocolTest::enumerate_callback, (void *)&iCurrent);
}

void TestSqliteProtocolTest::testRetrieveDeviceData()
{
    QVERIFY2(m_Devices[0], "Empty first element in array");
    QVERIFY2(m_Devices[1], "Empty second element in array");

    {
      sp_device_data *aTemperatureData = 0;
      sp_device_data *aRelayData = 0;

      sp_device_add_data(m_Devices[0], 109, "37.2");
      aTemperatureData = sp_device_get_last_data(m_Devices[0]);
      QVERIFY2(aTemperatureData, "Temperature data is null");
      QVERIFY2(!strcmp(aTemperatureData->m_State, "37.2"), "Temperature sensor temp is not equal to 37.2");
      QVERIFY2(aTemperatureData->m_Date == 109, "Temperature sensor date is not equal to 109");
      sp_device_data_free(aTemperatureData);

      sp_device_add_data(m_Devices[0], 110, "37.6");
      aTemperatureData = sp_device_get_last_data(m_Devices[0]);
      QVERIFY2(aTemperatureData, "Temperature data is null");
      QVERIFY2(!strcmp(aTemperatureData->m_State, "37.6"), "Temperature sensor temp is not equal to 37.6");
      QVERIFY2(aTemperatureData->m_Date == 110, "Temperature sensor date is not equal to 110");
      sp_device_data_free(aTemperatureData);

      sp_device_add_data(m_Devices[1], 111, "on");
      aRelayData = sp_device_get_last_data(m_Devices[1]);
      QVERIFY2(aRelayData, "Relay data is null");
      QVERIFY2(!strcmp(aRelayData->m_State, "on"), "Relay is not on");
      QVERIFY2(aRelayData->m_Date == 111, "Relay date is not equal to 109");
      sp_device_data_free(aRelayData);
    }
}

QTEST_APPLESS_MAIN(TestSqliteProtocolTest);

#include "tst_testsqliteprotocoltest.moc"
