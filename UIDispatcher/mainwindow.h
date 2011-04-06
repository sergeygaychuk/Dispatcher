#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

class QStandardItemModel;
class TemperatureSensorController;
class TemperatureSensor;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void on_temperate_sensor_list_changed(QList<TemperatureSensor*> aList);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *m_Model;
    TemperatureSensorController *m_Controller;
};

#endif // MAINWINDOW_H
