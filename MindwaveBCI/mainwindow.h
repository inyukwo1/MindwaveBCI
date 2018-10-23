#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mindwaveconnector.h"

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QLabel>
#include <QPushButton>

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
public slots:
    void update();
    void handleButton();
protected:
    void paintEvent(QPaintEvent* event);
private:
    bool turnRed(bool on);
    bool turnBlue(bool on);

    Ui::MainWindow *ui;
    QLabel* status;
    std::thread* m_status_updater;

    QPushButton* m_button;
    bool m_button_started;

    bool m_blue_show;
    bool m_red_show;
    std::chrono::high_resolution_clock::time_point m_t_start;
};
#endif // MAINWINDOW_H
