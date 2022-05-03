#include <iostream>
#include <cstdio>

#include "fd_mosquitto.h"
#include "c_crypt.h"

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

int fd_mosquitto::connect(std::vector<int> &user, std::vector<int> &pw)
{
    if (!user.empty() && !pw.empty()) {
        char u_plain[user.size()+1], pw_plain[pw.size()+1];
        int u_crpt[user.size()], pw_crpt[pw.size()];

        std::copy(user.begin(), user.end(), u_crpt);
        std::copy(pw.begin(), pw.end(), pw_crpt);

        c_decrypt(u_crpt, user.size(), u_plain);
        c_decrypt(pw_crpt, pw.size(), pw_plain);
        baseclass::username_pw_set(u_plain, pw_plain);
    }

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
