#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

class QStandardItemModel;
class SensorController;
class Sensor;

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
    void on_temperate_sensor_list_changed(QList<Sensor*>);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *m_Model;
    SensorController *m_Controller;
};

#endif // MAINWINDOW_H
