#pragma once

#include <mosquittopp.h>
#include <vector>
#include <string>

class fd_mosquitto : public mosqpp::mosquittopp
{
private:
    typedef mosqpp::mosquittopp baseclass;

private:
    fd_mosquitto() = delete;

public:
    fd_mosquitto(std::vector<std::string> &);
    ~fd_mosquitto();

    int connect();

private:
    void on_connect(int) override;
    void on_subscribe(int, int, const int *) override;
    void on_message(const struct mosquitto_message *) override;

private:
    std::vector<std::string> m_topics;
};
