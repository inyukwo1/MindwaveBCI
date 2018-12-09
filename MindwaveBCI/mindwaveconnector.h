#ifndef MINDWAVECONNECTOR_H
#define MINDWAVECONNECTOR_H

#include "thinkgear.h"
#include "Windows.h"
#include "client.h"

#include <ctime>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>

#define COM_PORT_NAME "\\\\.\\COM5"
#define TG_BAUD_RATE TG_BAUD_57600
#define PACKET_FETCH_RATE 1000

class MindwaveConnector: public QObject {
    Q_OBJECT
public:

    MindwaveConnector() {
        m_connection_id = 0;
        m_recording = false;
        m_record_finish = false;

        m_recent_poor = 0;
        m_recent_attention = 0;
        m_recent_meditation = 0;

        m_recording_pos = 0;
        sendmode = false;

        getConnectionId();
        connect();
    }

    ~MindwaveConnector() {
        delete m_packet_printer;
    }

    int getValue(int data_type) {
        if (data_type == TG_DATA_POOR_SIGNAL) {
            return m_recent_poor;
        } else if (data_type == TG_DATA_ATTENTION) {
            return m_recent_attention;
        } else if (data_type == TG_DATA_MEDITATION) {
            return m_recent_meditation;
        }
        return -1;
    }

    void recordStart() {
    }

    bool recording() {
        return m_recording;
    }

    void recordEnd() {
        m_record_finish = true;
        m_file_out.close();
        std::cout << "END" << std::endl;
    }

    void recordingLeft() {
        m_recording = true;
        m_recording_pos = 0;
    }

    void recordingMiddle() {
        m_recording = true;
        m_recording_pos = 1;
    }

    void recordingRight() {
        m_recording = true;
        m_recording_pos = 2;
    }

    void recordingRest() {
        m_recording = false;
    }

    int recordingPos() {
        return m_recording_pos;
    }


    void printPackets(std::string file_name) {
        m_packet_printer = new std::thread([this, file_name](int connection_id){

            m_file_out.open(file_name);

            std::cout << "START" << std::endl;
            int i = 0;
            int threshold = 5;
            int sum[8] = {0, 0, 0, 0, 0, 0, 0, 0};
            std::vector<int> observed_values;
            while (true) {
                Sleep(0.5);
                bool raw_assertion = false;
                if (m_recording != true) {
                    i = 0;
                }
                if (m_record_finish) {
                    break;
                }

                int packetread = TG_ReadPackets(connection_id, 1);
                if (packetread != 1)
                    continue;

                if (TG_GetValueStatus(connection_id, TG_DATA_RAW) != 0) {
                    int value = (int) TG_GetValue(connection_id, TG_DATA_RAW);
                    observed_values.push_back(value);
                    if (this->recording()) {
                        m_file_out << value << " ";
                    }
                }
                if (TG_GetValueStatus(connection_id, TG_DATA_POOR_SIGNAL) != 0) {
                    m_recent_poor = (int) TG_GetValue(connection_id, TG_DATA_POOR_SIGNAL);
                }
                if (TG_GetValueStatus(connection_id, TG_DATA_ATTENTION) != 0) {
                    m_recent_attention = (int) TG_GetValue(connection_id, TG_DATA_ATTENTION);
                    if (m_recent_attention >= threshold)
                        i++;
                }
                if (TG_GetValueStatus(connection_id, TG_DATA_MEDITATION) != 0) {
                    m_recent_meditation = (int) TG_GetValue(connection_id, TG_DATA_MEDITATION);
                }

                if (TG_GetValueStatus(connection_id, TG_DATA_DELTA) != 0) {
                    int value = (int) TG_GetValue(connection_id, TG_DATA_DELTA);
                    observed_values.push_back(value);
                    raw_assertion = true;
                    if (this->recording()) {
                        m_file_out << value << " ";
                        sum[0] += value;
                    }
                }
                if (TG_GetValueStatus(connection_id, TG_DATA_THETA) != 0) {
                    int value = (int) TG_GetValue(connection_id, TG_DATA_THETA);
                    observed_values.push_back(value);
                    assert(raw_assertion == true);
                    if (this->recording()) {
                        m_file_out << value << " ";
                        sum[1] += value;
                    }
                } else {
                    assert(raw_assertion == false);
                }
                if (TG_GetValueStatus(connection_id, TG_DATA_ALPHA1) != 0) {
                    int value = (int) TG_GetValue(connection_id, TG_DATA_ALPHA1);
                    observed_values.push_back(value);
                    assert(raw_assertion == true);
                    if (this->recording()) {
                        m_file_out <<value << " ";
                        sum[2] += value;
                    }
                } else {
                    assert(raw_assertion == false);
                }
                if (TG_GetValueStatus(connection_id, TG_DATA_ALPHA2) != 0) {
                    int value = (int) TG_GetValue(connection_id, TG_DATA_ALPHA2);
                    observed_values.push_back(value);
                    assert(raw_assertion == true);
                    if (this->recording()) {
                        m_file_out << value << " ";
                        sum[3] += value;
                    }
                } else {
                    assert(raw_assertion == false);
                }
                if (TG_GetValueStatus(connection_id, TG_DATA_BETA1) != 0) {
                    int value = (int) TG_GetValue(connection_id, TG_DATA_BETA1);
                    observed_values.push_back(value);
                    assert(raw_assertion == true);
                    if (this->recording()) {
                        m_file_out << value << " ";
                        sum[4] += value;
                    }
                } else {
                    assert(raw_assertion == false);
                }
                if (TG_GetValueStatus(connection_id, TG_DATA_BETA2) != 0) {
                    int value = (int) TG_GetValue(connection_id, TG_DATA_BETA2);
                    observed_values.push_back(value);
                    assert(raw_assertion == true);
                    if (this->recording()) {
                        m_file_out << value << " ";
                        sum[5] += value;
                    }
                } else {
                    assert(raw_assertion == false);
                }
                if (TG_GetValueStatus(connection_id, TG_DATA_GAMMA1) != 0) {
                    int value = (int) TG_GetValue(connection_id, TG_DATA_GAMMA1);
                    observed_values.push_back(value);
                    assert(raw_assertion == true);
                    if (this->recording()) {
                        m_file_out << value << " ";
                        sum[6] += value;
                    }
                } else {
                    assert(raw_assertion == false);
                }
                if (TG_GetValueStatus(connection_id, TG_DATA_GAMMA2) != 0) {
                    int value = (int) TG_GetValue(connection_id, TG_DATA_GAMMA2);
                    assert(raw_assertion == true);
                    observed_values.push_back(value);
                    if (observed_values.size() == 520 && sendmode) {
                        // TODO if this takes a lot of time, we have to think..
                       emit toServer(observed_values);
                    }
                    observed_values.resize(0);
                    if (this->recording()) {
                        m_file_out << value << " " << m_recording_pos << "\n";
                        sum[7] += value;
                    }
                } else {
                    assert(raw_assertion == false);
                }
            }
        }, m_connection_id);
    }

private:
    void getConnectionId() {
        m_connection_id = TG_GetNewConnectionId();
        if (m_connection_id < 0) {
            std::cerr << "ERROR: TG_GetNewConnectionId() returned " << m_connection_id << std::endl;
            return;
        }
        std::cout << "Successfully get connect id " << m_connection_id << std::endl;
    }

    void connect() {
        int err_code = 0;
        err_code = TG_Connect(m_connection_id,
                              COM_PORT_NAME,
                              TG_BAUD_RATE,
                              TG_STREAM_PACKETS);
        if (err_code < 0) {
            std::cerr << "ERROR: TG_Connect() returned " << err_code << std::endl;
            return;
        }
        std::cout << "Successfully connected" << std::endl;
    }

    int m_connection_id;
    int m_recording_pos;
    std::thread* m_packet_printer;

    std::ofstream m_file_out;
    bool m_recording;
    bool m_record_finish;

    int m_recent_poor;
    int m_recent_attention;
    int m_recent_meditation;

public:
    bool sendmode;

signals:
    void toServer(std::vector<int> array);
};

#endif // MINDWAVECONNECTOR_H
