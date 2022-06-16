#pragma once

#include <vector>
#include <string>
#include <functional>

#include <mosquittopp.h>

class fd_mosquitto : public mosqpp::mosquittopp
{
private:
    typedef mosqpp::mosquittopp baseclass;
    typedef std::function<void(uint64_t)> message_callback_t;

private:
    fd_mosquitto() = delete;

public:
    fd_mosquitto(std::vector<std::string> &, const std::string &, message_callback_t callback);
    ~fd_mosquitto();

    int connect(std::vector<int> &, std::vector<int> &);

private:
    bool safe_alpha_num_conversion(const char *);

    void on_connect(int) override;
    void on_subscribe(int, int, const int *) override;
    void on_message(const struct mosquitto_message *) override;

private:
    std::string m_host;
    std::vector<std::string> m_topics;
    message_callback_t m_callback;
};
