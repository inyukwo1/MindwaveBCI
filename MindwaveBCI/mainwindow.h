#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mindwaveconnector.h"
#include "Windows.h"
#include "client.h"

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QLabel>
#include <QPushButton>
#include <vector>

namespace Ui {
class MainWindow;
}
class MainWindow : public QOpenGLWidget
{
    Q_OBJECT
public:
    MindwaveConnector* mindwave_connector;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setLabel();
    Client* client;
    bool savemode;

public slots:
    void updateSelf();
    void handleButton();
protected:
    void paintEvent(QPaintEvent* event);
private:
    bool turnRed(bool on);
    bool turnBlue(bool on);

    Ui::MainWindow *ui;
    QLabel* status;
    QLabel* zero_status;
    QLabel* two_status;
    std::thread* m_status_updater;

    QPushButton* m_button;
    bool m_button_started;

    bool m_blue_show;
    bool m_red_show;
    double m_t_start;

    std::vector<int> show_list;
    //debug
    double m_t_just_before;
};
#endif // MAINWINDOW_H
