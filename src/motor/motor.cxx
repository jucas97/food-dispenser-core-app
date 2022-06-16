#include <iostream>
#include <fcntl.h>
#include <unistd.h>

#include <sys/ioctl.h>

#include "motor.h"
#include "food_disp_types.h"

motor::motor(const int &pwm_channel, const uint64_t &frequency)
    : m_fd(-1)
    , m_control()
{
    m_control = {
        .period = frequency,
        .duty_cycle = 0,
        .channel = pwm_channel,
        .enabled = false,
    };
}

motor::~motor()
{
    // Resoures cleanup
    if (m_control.enabled) {
        m_control.enabled = false;
        apply_state();
    }
    cleanup();
}

int motor::open_device(const std::string &device)
{
    if ((m_fd = open(device.c_str(), O_RDWR)) == -1) {
        std::cout << "failure opening device " << std::endl;
        return NO_DEVICE;
    }

    return NO_ERROR;
}

int motor::initialize_channel()
{
    int ret;

    if (m_fd == -1) {
        std::cout << "NO FD" << std::endl;
        return NO_PWM_CHANNEL;
    }

    if ((ret = ioctl(m_fd, FOOD_DISP_IOCTL_PWM_CHANNEL, &m_control.channel)) != 0) {
        std::cout << "Error, return code: " << ret << std::endl;
        return IOCTL_ERROR;
    }

    return NO_ERROR;
}

int motor::apply_state()
{
    int ret;

    if ((ret = ioctl(m_fd, FOOD_DISP_IOCTL_PWM_APPLY_STATE, &m_control)) != 0) {
        std::cout << "Error, return code: " << ret << std::endl;
        return IOCTL_ERROR;
    }

    return NO_ERROR;
}

void motor::period(uint64_t &frequency)
{
    m_control.period = frequency;
}

void motor::duty_cycle(uint64_t &duty_cycle)
{
    m_control.duty_cycle = duty_cycle;
}

void motor::state(bool enabled)
{
    m_control.enabled = enabled;
}

void motor::cleanup()
{
    if (m_fd != -1) {
        close(m_fd);
        m_fd = -1;
    }
}
