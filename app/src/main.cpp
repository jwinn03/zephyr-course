#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

#define SLEEP_TIME_MS CONFIG_APP_HEARTBEAT_PERIOD_MS

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

namespace {
    void test() {
        const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
        struct sensor_value val;
        int ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
        LOG_INF("Channel ret %d", ret);
    }
}

int main(void)
{
    test();

    const struct device *led_sensor = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

    while (1) {
        sensor_sample_fetch(led_sensor);
        LOG_INF("LED state: ON");
        k_msleep(SLEEP_TIME_MS);

        struct sensor_value val;
        sensor_channel_get(led_sensor, SENSOR_CHAN_AMBIENT_TEMP, &val);
        LOG_INF("LED state: OFF");
        k_msleep(SLEEP_TIME_MS);
    }
    return 0;
}
