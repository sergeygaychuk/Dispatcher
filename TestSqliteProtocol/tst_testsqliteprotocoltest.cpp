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

QTEST_APPLESS_MAIN(TestSqliteProtocolTest);

#include "tst_testsqliteprotocoltest.moc"
