#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "our_driver.h"

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

struct our_driver_config {
    struct gpio_dt_spec led;
};

struct our_driver_data {
    bool led_on;
    uint32_t toggle_count;
};

static int sample_fetch_my_impl(const struct device *dev, enum sensor_channel chan) {
    const struct our_driver_config *config = dev->config;
    struct our_driver_data *data = dev->data;

    LOG_INF("Hello From Sample Fetch, channel %d", chan);

    if (gpio_pin_set_dt(&config->led, 1) < 0) {
        return -EIO;
    }
    data->led_on = true;
    return 0;
}

static int channel_get_my_impl(const struct device *dev,
                    enum sensor_channel chan,
                    struct sensor_value *val) {
    const struct our_driver_config *config = dev->config;
    struct our_driver_data *data = dev->data;

    LOG_INF("Hello From Channel Get, channel %d", chan);

    if (gpio_pin_set_dt(&config->led, 0) < 0) {
        return -EIO;
    }

    val->val1 = data->led_on ? 1 : 0;
    val->val2 = 0;
    data->led_on = false;
    return 0;
}

int our_driver_increment_counter(const struct device *dev, int amount) {
    struct our_driver_data *data = dev->data;

    data->toggle_count += amount;
    LOG_INF("Toggle count now %u", data->toggle_count);

    return 0;
}

static DEVICE_API(sensor, api_iomico_lecture) = {
    .sample_fetch = sample_fetch_my_impl,
    .channel_get = channel_get_my_impl,
};

// Init function
static int init(const struct device* dev) {
    const struct our_driver_config *config = dev->config;

    if (!gpio_is_ready_dt(&config->led)) {
        LOG_ERR("LED GPIO device not ready");
        return -ENODEV;
    }
    if (gpio_pin_configure_dt(&config->led, GPIO_OUTPUT_INACTIVE) < 0) {
        LOG_ERR("Failed to configure LED GPIO");
        return -EIO;
    }

    LOG_INF("Device Initialized");
    
    return 0;
}

static const struct our_driver_config config0 = {
    .led = GPIO_DT_SPEC_INST_GET(0, led_gpios),
};

static struct our_driver_data data0;

DEVICE_DT_INST_DEFINE(0, init, NULL, &data0, &config0, POST_KERNEL, 80, &api_iomico_lecture);