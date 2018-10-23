#ifndef MINDWAVECONNECTOR_H
#define MINDWAVECONNECTOR_H

#include "thinkgear.h"
#include "Windows.h"

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>

#define COM_PORT_NAME "\\\\.\\COM3"
#define TG_BAUD_RATE TG_BAUD_57600
#define PACKET_FETCH_RATE 1000

class MindwaveConnector {
public:

    MindwaveConnector() {
        m_connection_id = 0;
        m_recording = false;
        m_record_finish = false;

        m_recent_poor = 0;
        m_recent_attention = 0;
        m_recent_meditation = 0;

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
        m_recording = true;
    }

    bool recording() {
        return m_recording;
    }

    void recordEnd() {
        m_record_finish = true;
        m_file_out.close();
        std::cout << "END" << std::endl;
    }

    void printPackets(std::string file_name) {
        m_packet_printer = new std::thread([this, file_name](int connection_id){

            m_file_out.open(file_name);

            std::cout << "START" << std::endl;
            while (true) {
                TG_ReadPackets(connection_id, -1);
                if (TG_GetValueStatus(connection_id, TG_DATA_RAW) != 0) {
                    int value = (int) TG_GetValue(connection_id, TG_DATA_RAW);
                    if (this->recording()) {
                        m_file_out << value << "\n";
                    }
                }
                if (TG_GetValueStatus(connection_id, TG_DATA_POOR_SIGNAL) != 0) {
                    m_recent_poor = (int) TG_GetValue(connection_id, TG_DATA_POOR_SIGNAL);
                }
                if (TG_GetValueStatus(connection_id, TG_DATA_ATTENTION) != 0) {
                    m_recent_attention = (int) TG_GetValue(connection_id, TG_DATA_ATTENTION);
                }
                if (TG_GetValueStatus(connection_id, TG_DATA_MEDITATION) != 0) {
                    m_recent_meditation = (int) TG_GetValue(connection_id, TG_DATA_MEDITATION);
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
    std::thread* m_packet_printer;

    std::ofstream m_file_out;
    bool m_recording;
    bool m_record_finish;

    int m_recent_poor;
    int m_recent_attention;
    int m_recent_meditation;
};

#endif // MINDWAVECONNECTOR_H
