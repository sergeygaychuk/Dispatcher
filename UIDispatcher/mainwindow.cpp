#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "temperaturesensor.h"
#include "temperaturesensorcontroller.h"

#include <QtNetwork/QTcpSocket>
#include <QtGui/QMessageBox>
#include <QStandardItemModel>

class TempeartureSensorAdapter : public QList<QStandardItem*> {
public:
    TempeartureSensorAdapter(TemperatureSensor *aSensor) {
        this->append(new QStandardItem(aSensor->name()));
        this->append(new QStandardItem(QString::number(aSensor->temperature())));
    }
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_Model(new QStandardItemModel),
    m_Controller(new TemperatureSensorController(this))
{
    ui->setupUi(this);

    m_Model->setHorizontalHeaderLabels(QStringList() << "Sensor name" << "Temperature");
    ui->sensorList->setModel(m_Model);
    connect(m_Controller, SIGNAL(listReady(QList<TemperatureSensor*>)), this, SLOT(on_temperate_sensor_list_changed(QList<TemperatureSensor*>)));
    m_Controller->startRead();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_temperate_sensor_list_changed(QList<TemperatureSensor *> aList) {
    m_Model->removeRows(0, m_Model->rowCount());
    for (int i = 0; i < aList.length(); ++ i) {
        m_Model->insertRow(i, TempeartureSensorAdapter(aList.at(i)));
    }
}
