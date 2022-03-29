#include <iostream>

#include <sys/ioctl.h>

#include <fcntl.h>
#include <unistd.h>

#include "food_disp_shared.h"

int main(int argc, char *argv[])
{
    int fd, ret, pwm_channel = 2;

    if ((fd = open("/dev/food_disp_chr", O_RDWR)) == -1) {
        std::cout<< "Failed to open file" << std::endl;
        return -1;
    }

    ret = ioctl(fd, FOOD_DISP_IOCTL_PWM_CHANNEL, pwm_channel);
    std::cout << "Return value " << ret << std::endl;
    close(fd);

    return 0;
}
