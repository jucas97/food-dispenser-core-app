#pragma once

enum class steps {
    PARSE_CONFIG = 0,
    CONNECT_MQTT,
    SETUP_MOTOR,
    LOOP_MQTT,
};

enum return_code : int {
    NO_ERROR = 0,
    NO_DEVICE = 1,
    NO_PWM_CHANNEL = 2,
    IOCTL_ERROR = 3
};
