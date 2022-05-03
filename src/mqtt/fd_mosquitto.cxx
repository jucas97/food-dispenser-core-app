#include <iostream>
#include <cstdio>

#include "fd_mosquitto.h"

fd_mosquitto::fd_mosquitto(std::vector<std::string> &topics, const std::string &host)
    : m_topics(topics)
    , m_host(host)
{
    mosqpp::lib_init();
}

fd_mosquitto::~fd_mosquitto()
{
    baseclass::disconnect();
    mosqpp::lib_cleanup();
}

int fd_mosquitto::connect()
{

    return baseclass::connect(m_host.c_str());
}

void fd_mosquitto::on_connect(int rc)
{
    if (rc != MOSQ_ERR_SUCCESS) {
        std::cout << "Connection failure" << std::endl;
        return;
    }

    for (auto topic: m_topics) {
        if (mosqpp::mosquittopp::subscribe(NULL, topic.c_str()) != MOSQ_ERR_SUCCESS)
            std::cout << "Failed to subscribe to topic: " << topic.c_str() << std::endl;
    }
}

void fd_mosquitto::on_subscribe(int mid, int qos_count, const int *qos_list)
{
    std::cout << "Subscribed" << std::endl;
}

void fd_mosquitto::on_message(const struct mosquitto_message *message)
{
    std::cout << "Received message for topic " << message->topic << std::endl;
}
