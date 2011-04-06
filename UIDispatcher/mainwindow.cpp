#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sensor.h"
#include "sensorcontroller.h"

#include <QtNetwork/QTcpSocket>
#include <QtGui/QMessageBox>
#include <QStandardItemModel>

#include "sqliteprotocol.h"

class SensorAdapter : public QList<QStandardItem*> {
public:
    SensorAdapter(Sensor *aSensor) {
        this->append(new QStandardItem(aSensor->address()));
        this->append(new QStandardItem(QString(aSensor->type() == SP_DEVICE_TYPE_TEMPERATURE_SENSOR ? "temperature" : "relay")));
        this->append(new QStandardItem(aSensor->date().toString()));
        this->append(new QStandardItem(aSensor->state()));
    }
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_Model(new QStandardItemModel),
    m_Controller(new SensorController(this))
{
    ui->setupUi(this);

    m_Model->setHorizontalHeaderLabels(QStringList() << "Address" << "Type" << "Date" << "State");
    ui->sensorList->setModel(m_Model);
    connect(m_Controller, SIGNAL(listReady(QList<Sensor*>)), this, SLOT(on_temperate_sensor_list_changed(QList<Sensor*>)));
    m_Controller->startRead();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_temperate_sensor_list_changed(QList<Sensor *> aList) {
    m_Model->removeRows(0, m_Model->rowCount());
    for (int i = 0; i < aList.length(); ++ i) {
        m_Model->insertRow(i, SensorAdapter(aList.at(i)));
    }
}
