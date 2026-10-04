#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>

static const struct device *const led_sensor = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    if (!device_is_ready(led_sensor)) {
        shell_error(sh, "Device %s not ready", led_sensor->name);
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(led_sensor);
    if (ret < 0) {
        shell_error(sh, "sensor_sample_fetch failed: %d", ret);
        return ret;
    }

    shell_print(sh, "Sample fetched (LED ON)");
    return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    struct sensor_value val;

    if (!device_is_ready(led_sensor)) {
        shell_error(sh, "Device %s not ready", led_sensor->name);
        return -ENODEV;
    }

    int ret = sensor_channel_get(led_sensor, SENSOR_CHAN_AMBIENT_TEMP, &val);
    if (ret < 0) {
        shell_error(sh, "sensor_channel_get failed: %d", ret);
        return ret;
    }

    shell_print(sh, "Value: %d.%06d", val.val1, val.val2);
    return 0;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    shell_print(sh, "Device name: %s", led_sensor->name);
    shell_print(sh, "Ready:       %s", device_is_ready(led_sensor) ? "yes" : "no");
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(our_driver_subcmd,
    SHELL_CMD(fetch, NULL, "Fetch a sample (sensor_sample_fetch).", cmd_sensor_fetch),
    SHELL_CMD(read,  NULL, "Read channel (sensor_channel_get).",    cmd_sensor_read),
    SHELL_CMD(info,  NULL, "Print device name and ready state.",    cmd_sensor_info),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &our_driver_subcmd, "LED sensor driver commands", NULL);