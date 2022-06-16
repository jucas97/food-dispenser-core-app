#pragma once

#include <memory>

#include "food_disp_types.h"
#include "mqtt/fd_mosquitto.h"

class food_disp {
private:
    class config {
    private:
        config(const config &) = delete;
        config(const config &&) = delete;
        config & operator=(const config &) = delete;
        config & operator=(const config &&) = delete;

    public:
        config()
            : m_host("localhost")
            , m_pw()
        {}
        ~config() = default;

        void parse();

    public:
        std::string m_host;
        std::vector<int> m_user;
        std::vector<int> m_pw;
    };

private:
    food_disp(const food_disp &) = delete;
    food_disp(const food_disp &&) = delete;
    food_disp& operator= (const food_disp &) = delete;
    food_disp& operator= (const food_disp &&) = delete;

public:
    food_disp();
    ~food_disp();

    int state_machine(steps step);
    void mqtt_message_callback(uint64_t);

private:
    config m_config;
    std::shared_ptr<fd_mosquitto> m_mosq;
};