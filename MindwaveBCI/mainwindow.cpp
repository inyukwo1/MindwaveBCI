#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Windows.h"

#include <thread>
#include <string>
#include <cstdlib>
#include <random>

#include <QPainter>
#include <QPaintEvent>
#include <QThread>
#include <QTimer>
#include <QString>

int SAMPLING_TIME = 10;
int REST_TIME = 3;

double get_wall_time(){
    LARGE_INTEGER time,freq;
    if (!QueryPerformanceFrequency(&freq)){
        //  Handle error
        return 0;
    }
    if (!QueryPerformanceCounter(&time)){
        //  Handle error
        return 0;
    }
    return (double)time.QuadPart / freq.QuadPart;
}

MainWindow::MainWindow(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::MainWindow)
{
//    ui->setupUi(this);
    srand(time(NULL));
    client = NULL;
    this->window()->resize(2100, 800);
    setAutoFillBackground(false);
    m_t_start = get_wall_time();
    m_t_just_before = get_wall_time();

    m_button = new QPushButton("start", this);
    m_button->move(300, 50);
    m_button_started = false;

    for (int i = 0; i < 100; i++) {
        show_list.push_back(rand() % 3);
    }
    connect(m_button, SIGNAL (released()), this, SLOT (handleButton()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLabel() {
    status = new QLabel(this);
    QFont font("times", 24, 3);
    status->setFont(font);
    QPalette pal( status->palette() );
    pal.setColor( QPalette::Active, QPalette::WindowText , Qt::white );
    status->setPalette(pal);
    status->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    status->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    status->setGeometry(QRect(810, 0, 150, 50));

    zero_status = new QLabel(this);
    zero_status->setFont(font);
    zero_status->setPalette(pal);
    zero_status->setAlignment(Qt::AlignCenter);
    zero_status->setGeometry(QRect(100, 105, 48, 30));

    two_status = new QLabel(this);
    two_status->setFont(font);
    two_status->setPalette(pal);
    two_status->setAlignment(Qt::AlignCenter);
    two_status->setGeometry(QRect(1770, 887, 48, 30));

    m_status_updater = new std::thread([this](MindwaveConnector* mindwave_connector, QLabel* status) {
        while(true) {
            Sleep(500);
            int poor_signal = mindwave_connector->getValue(TG_DATA_POOR_SIGNAL);
            int attention = mindwave_connector->getValue(TG_DATA_ATTENTION);
            int meditation = mindwave_connector->getValue(TG_DATA_MEDITATION);

            std::string out_string = "POOR: " + std::to_string(poor_signal);
            status->setText(QString::fromStdString(out_string));
        }
    }, this->mindwave_connector, status);
}

void MainWindow::updateSelf() {
    if (!m_button_started) {
        QTimer::singleShot(1, this, SLOT(updateSelf()));
        return;
    }

    double t_current = get_wall_time();
    double elapsed = t_current - m_t_start;


    bool need_repaint = false;

    int period = elapsed / (SAMPLING_TIME + REST_TIME);
    m_button->setText(std::to_string(period).c_str());
    if (period == 21) {
        printf("DONEDONEDONEDONE");
        mindwave_connector->recordEnd();
        return;
    }
    if (show_list[period] == 0) {
        mindwave_connector->recordingLeft();
    }
    else if (show_list[period] == 1) {
        mindwave_connector->recordingMiddle();
    } else {
        mindwave_connector->recordingRight();
    }
    if ((static_cast<int>(elapsed) % (SAMPLING_TIME + REST_TIME)) < REST_TIME) {
        mindwave_connector->recordingRest();
//        turnRed(true);
//        turnBlue(true);
//        repaint();
//        QTimer::singleShot(1, this, SLOT(update()));
//        return;
    }

    // 12hz
    int cnt = (int) (elapsed * 600) % 50;
    if (0 <= cnt && cnt < 20) {
        need_repaint |= turnBlue(true);
    } else if (20 <= cnt) {
        need_repaint |= turnBlue(false);
    }
    // 10Hz
    int cnt2 = (int) (elapsed * 600) % 60;
    if (0 <= cnt2 && cnt2 < 20) {
        need_repaint |= turnRed(true);
    } else if (20 <= cnt2) {
        need_repaint |= turnRed(false);
    }
    if (need_repaint) {
        update();
    }
    QTimer::singleShot(1, this, SLOT(updateSelf()));
}

void MainWindow::handleButton() {
    if (!m_button_started) {
        mindwave_connector->recordStart();
        m_t_start = get_wall_time();
        m_button->setText("END");
        m_button_started = true;
    } else {
        m_button->setText("DONE");
        mindwave_connector->recordEnd();
    }
}

bool MainWindow::turnRed(bool on) {
    if (on != m_red_show) {
        m_red_show = on;
        return true;
    }
    return false;
}

bool MainWindow::turnBlue(bool on) {
    if (on != m_blue_show) {
        m_blue_show = on;
        return true;
    }
    return false;
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    painter.fillRect(event->rect(), QBrush(QColor(0, 0, 0)));

    int attention =  mindwave_connector->getValue(TG_DATA_ATTENTION);
    zero_status->setText(QString::fromStdString(std::to_string((int)(client->last_result[0] * 100))));
    two_status->setText(QString::fromStdString( std::to_string((int)(client->last_result[2] * 100))));
    if (savemode) {
        if (mindwave_connector->recordingPos() == 0) {
            QRect rect(275, 520, 50, 50);
            painter.setBrush(Qt::yellow);
            painter.drawRect(rect);
        } else if (mindwave_connector->recordingPos() == 1) {
            QRect rect(975, 520, 50, 50);
            painter.setBrush(Qt::yellow);
            painter.drawRect(rect);
        } else {
            QRect rect(1575, 520, 50, 50);
            painter.setBrush(Qt::yellow);
            painter.drawRect(rect);
        }
    } else {
        if (client->last_result[0] > 0.67) {
            QRect rect(0, 0, 250, 250);
            painter.setBrush(Qt::yellow);
            painter.drawRect(rect);
        } else if (client->last_result[2] > 0.9) {
            QRect rect(1675, 765, 250, 250);
            painter.setBrush(Qt::yellow);
            painter.drawRect(rect);
        } else {
            QRect rect(975, 520, 50, 50);
            painter.setBrush(Qt::yellow);
            painter.drawRect(rect);
        }
    }
    if (m_blue_show)
    {
        QRect rect(25, 25, 200, 200);
        painter.setBrush (Qt::red);
        painter.drawRect (rect);
    }
    if (m_red_show) {
        QRect rect(1695, 790, 200, 200);
        painter.setBrush (Qt::blue);
        painter.drawRect (rect);
    }

    painter.end();
}
