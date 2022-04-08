#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "mqtt/fd_mosquitto.h"
//#include "food_disp_shared.h"

int main(int argc, char *argv[])
{
    /**
    int fd, ret, pwm_channel = 2;

    if ((fd = open("/dev/food_disp_chr", O_RDWR)) == -1) {
        std::cout<< "Failed to open file" << std::endl;
        return -1;
    }

    ret = ioctl(fd, FOOD_DISP_IOCTL_PWM_CHANNEL, pwm_channel);
    std::cout << "Return value " << ret << std::endl;
    close(fd);
    */
    int rc;
    std::shared_ptr<fd_mosquitto> mosq;
    std::vector<std::string> topics{"test"};

    mosq = std::make_shared<fd_mosquitto>(topics);
    rc = mosq->connect();
    if (rc != MOSQ_ERR_SUCCESS) {
        std::cout << "Failed to connect RC: " << rc << std::endl;
        return -1;
    }

    mosq->loop_forever();

    return 0;
}
