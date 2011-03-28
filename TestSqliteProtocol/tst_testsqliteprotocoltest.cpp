#include <QtCore/QString>
#include <QtTest/QtTest>

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
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(TestSqliteProtocolTest);

#include "tst_testsqliteprotocoltest.moc"
