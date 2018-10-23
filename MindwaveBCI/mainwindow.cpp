#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Windows.h"

#include <thread>

#include <QPainter>
#include <QPaintEvent>
#include <QThread>
#include <QTimer>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::MainWindow)
{
//    ui->setupUi(this);
    this->window()->resize(2100, 800);
    setAutoFillBackground(false);
    m_t_start = std::chrono::high_resolution_clock::now();

    m_button = new QPushButton("start", this);
    m_button->move(300, 50);
    m_button_started = false;
    connect(m_button, SIGNAL (released()), this, SLOT (handleButton()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLabel() {
    status = new QLabel(this);
    QFont font("Helvetica", 20, QFont::Bold);
    status->setFont(font);
//    QPalette pal( status->palette() );
//    pal.setColor( QPalette::Active, QPalette::WindowText , Qt::white );
//    status->setPalette(pal);
    status->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    status->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    status->setText("first line\nsecond line");
    m_status_updater = new std::thread([](MindwaveConnector* mindwave_connector, QLabel* status) {
        while(true) {
            Sleep(500);
            int poor_signal = mindwave_connector->getValue(TG_DATA_POOR_SIGNAL);
            int attention = mindwave_connector->getValue(TG_DATA_ATTENTION);
            int meditation = mindwave_connector->getValue(TG_DATA_MEDITATION);

            std::string out_string = "POOR: " + std::to_string(poor_signal) +
            "\nATTENTION: " + std::to_string(attention) +
            "\nMEDITATION: " + std::to_string(meditation);
            status->setText(QString::fromStdString(out_string));
        }
    }, this->mindwave_connector, status);
}

void MainWindow::update() {
    auto t_current = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration<double, std::milli>(t_current - m_t_start).count();
    bool need_repaint = false;
    if (80 <= elapsed && elapsed < 100) {
        need_repaint |= turnRed(true);
        need_repaint |= turnBlue(false);
    } else if (100 <= elapsed && elapsed < 180) {
        need_repaint |= turnRed(false);
        need_repaint |= turnBlue(false);
    } else if (180 <= elapsed && elapsed < 200) {
        need_repaint |= turnRed(true);
        need_repaint |= turnBlue(true);
    } else if (200 <= elapsed){
        need_repaint |= turnRed(false);
        need_repaint |= turnBlue(false);
        m_t_start = t_current;
    }
    if (need_repaint) {
        repaint();
    }
    QTimer::singleShot(1, this, SLOT(update()));
}

void MainWindow::handleButton() {
    if (!m_button_started) {
        mindwave_connector->recordStart();
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
    painter.fillRect(event->rect(), QBrush(QColor(255, 255, 255)));
    if (m_blue_show)
    {
        QRect rect(100, 200, 400, 400);
        painter.setBrush (Qt::blue);
        painter.drawRect (rect);
    }
    if (m_red_show) {
        QRect rect(1400, 200, 400, 400);
        painter.setBrush (Qt::red);
        painter.drawRect (rect);
    }
    painter.end();
}
