#pragma once

#include "food-disp-module/food_disp_shared.h"

class motor {
private:
    motor(const motor &) = delete;
    motor (const motor &&) = delete;
    motor & operator=(const motor &) = delete;
    motor & operator=(const motor &&) = delete;

public:
    motor(const int &, const uint64_t &);
    ~motor();

    int open_device(const std::string &);
    void cleanup();
    int initialize_channel();
    int apply_state();
    void period(uint64_t &);
    void duty_cycle(uint64_t &);
    void state(bool);

private:
    int m_fd;
    struct pwm_config m_control;
};
