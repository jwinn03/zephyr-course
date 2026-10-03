#ifndef OUR_DRIVER_H_
#define OUR_DRIVER_H_

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

int our_driver_increment_counter(const struct device *dev, int amount);

#ifdef __cplusplus
}
#endif

#endif /* OUR_DRIVER_H_ */