#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>

#include "fd_mosquitto.h"
#include "c_crypt.h"

/* duty cycle in ns + end of string char */
static constexpr int PAYLOAD_MAX_SIZE = 8;

fd_mosquitto::fd_mosquitto(std::vector<std::string> &topics, const std::string &host, message_callback_t callback)
    : m_topics(topics)
    , m_host(host)
    , m_callback(callback)
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

bool fd_mosquitto::safe_alpha_num_conversion(const char *message)
{
    std::string message_str{message};

    std::string::const_iterator message_it = find_if(message_str.begin(), message_str.end(), [](auto c){
        return (bool)std::isalpha(c);
    });

    return message_it == message_str.end();
}

void fd_mosquitto::on_message(const struct mosquitto_message *message)
{
    // Topic to be checked
    char buffer[PAYLOAD_MAX_SIZE];

    if (message->payload == nullptr) {
        std::cout << "Empty payload" << std::endl;
        return;
    }

    memset(static_cast<void *>(&buffer), 0, PAYLOAD_MAX_SIZE);
    memcpy(static_cast<void *>(&buffer), message->payload, PAYLOAD_MAX_SIZE);

    if (safe_alpha_num_conversion(buffer)) {
        uint64_t value = strtoul(&buffer[0], NULL, 10);
        m_callback(value);
    }
}
