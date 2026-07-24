
#include <zephyr/kernel.h>

#include <zephyr/device.h>

#include <zephyr/drivers/gpio.h>

#include <zephyr/drivers/i2c.h>

#include <zephyr/shell/shell.h>

#include <stdlib.h>

#include <string.h>



/* --- Device Bindings --- */

static const struct device * const gpio0 = DEVICE_DT_GET(DT_NODELABEL(gpio0));

static const struct device * const gpio1 = DEVICE_DT_GET(DT_NODELABEL(gpio1));



/* --- GPIO Pin Definitions --- */

struct pin_config {

    const char *name;

    const struct device *port;

    uint8_t pin_num;

    gpio_flags_t flags;

};



static const struct pin_config pins[] = {

    // GPIO Port 0

    {"USB_SEL0",    gpio0, 1,  GPIO_OUTPUT_INACTIVE},

    {"USB_PWR01",   gpio0, 7,  GPIO_OUTPUT_INACTIVE},

    {"USB_PWR02",   gpio0, 8,  GPIO_OUTPUT_INACTIVE},

    {"USB_SEL1",    gpio0, 13, GPIO_OUTPUT_INACTIVE},

    {"USB_SEL2",    gpio0, 14, GPIO_OUTPUT_INACTIVE},

    {"USB_SEL3",    gpio0, 17, GPIO_OUTPUT_INACTIVE},

    {"USB_PWR11",   gpio0, 18, GPIO_OUTPUT_INACTIVE},

    {"USB_PWR12",   gpio0, 21, GPIO_OUTPUT_INACTIVE},

    {"USB_PWR21",   gpio0, 22, GPIO_OUTPUT_INACTIVE},

    {"USB_PWR22",   gpio0, 23, GPIO_OUTPUT_INACTIVE},

    {"USB_SEL4",    gpio0, 24, GPIO_OUTPUT_INACTIVE},

    {"MCU_LED",     gpio0, 25, GPIO_OUTPUT_INACTIVE},

    {"DEBUG1",      gpio0, 26, GPIO_OUTPUT_INACTIVE},

    {"DEBUG2",      gpio0, 27, GPIO_OUTPUT_INACTIVE},

    {"USB_PWR31",   gpio0, 29, GPIO_OUTPUT_INACTIVE},

    {"USB_PWR32",   gpio0, 30, GPIO_OUTPUT_INACTIVE},



    // GPIO Port 1

    {"USB_PWR41",   gpio1, 0,  GPIO_OUTPUT_INACTIVE},

    {"USB_PWR42",   gpio1, 1,  GPIO_OUTPUT_INACTIVE},

    {"USB_SEL5",    gpio1, 2,  GPIO_OUTPUT_INACTIVE},

    {"DPEQ_RST",    gpio1, 3,  GPIO_OUTPUT_ACTIVE},

    {"USB_PWR51",   gpio1, 10, GPIO_OUTPUT_INACTIVE},

    {"USB_PWR52",   gpio1, 11, GPIO_OUTPUT_INACTIVE},

    {"USB_PWR61",   gpio1, 12, GPIO_OUTPUT_INACTIVE},

    {"USB_PWR62",   gpio1, 13, GPIO_OUTPUT_INACTIVE},

    {"USB_PWR71",   gpio1, 15, GPIO_OUTPUT_INACTIVE},

    {"USB_PWR72",   gpio1, 16, GPIO_OUTPUT_INACTIVE},

    {"USB_SEL6",    gpio1, 20, GPIO_OUTPUT_INACTIVE},

    {"USB_SEL7",    gpio1, 21, GPIO_OUTPUT_INACTIVE},

    {"DP_SEL0",     gpio1, 22, GPIO_OUTPUT_INACTIVE},

    {"DP_SEL1",     gpio1, 23, GPIO_OUTPUT_INACTIVE},

    {"DP_SEL2",     gpio1, 24, GPIO_OUTPUT_INACTIVE},

    {"DP_SEL3",     gpio1, 25, GPIO_OUTPUT_INACTIVE},

    {"DP_SEL4",     gpio1, 26, GPIO_OUTPUT_INACTIVE},

    {"DP_SEL5",     gpio1, 27, GPIO_OUTPUT_INACTIVE},

    {"DP_SEL6",     gpio1, 28, GPIO_OUTPUT_INACTIVE},

    {"DP_SEL7",     gpio1, 29, GPIO_OUTPUT_INACTIVE},

};







static int cmd_dp_switch_with_training(const struct shell *shell, size_t argc, char **argv)

{

    if (argc != 2) {

        shell_error(shell, "Usage: dp_switch_training <0-7>");

        return -EINVAL;

    }

    

    int dp_port = atoi(argv[1]);

    if (dp_port < 0 || dp_port > 7) {

        shell_error(shell, "DP port must be 0-7");

        return -EINVAL;

    }

    

    shell_print(shell, "Switching to DP port %d with link training support...", dp_port);

   

    

    // 1. Disable current DP selection (causes HPD low)

    const char* dp_pins[] = {"DP_SEL0", "DP_SEL1", "DP_SEL2", "DP_SEL3", 

                            "DP_SEL4", "DP_SEL5", "DP_SEL6", "DP_SEL7"};

    /*

    for (int i = 0; i < 8; i++) {

        for (int j = 0; j < ARRAY_SIZE(pins); j++) {

            if (strcmp(dp_pins[i], pins[j].name) == 0) {

                gpio_pin_set(pins[j].port, pins[j].pin_num, 0);

                break;

            }

        }

    }

    */

    // 2. Wait for HPD low period (minimum 100ms for proper disconnect)

    //shell_print(shell, "Waiting for HPD low period...");

    //k_sleep(K_MSEC(100));

    

    /*

    // 3. Reset DP equalizer if needed

    for (int i = 0; i < ARRAY_SIZE(pins); i++) {

        if (strcmp("DPEQ_RST", pins[i].name) == 0) {

            gpio_pin_set(pins[i].port, pins[i].pin_num, 0);

            k_sleep(K_MSEC(1));

            gpio_pin_set(pins[i].port, pins[i].pin_num, 1);

            k_sleep(K_MSEC(10));

            break;

        }

    }

    */

    /*

    // Correct reset sequence for SN65DP141RLRJ

    for (int i = 0; i < ARRAY_SIZE(pins); i++) {

        if (strcmp("DPEQ_RST", pins[i].name) == 0) {

            gpio_pin_set(pins[i].port, pins[i].pin_num, 0); // Assert reset (LOW)

            k_busy_wait(10);                               // ~10μs delay (more than 1μs min)

            gpio_pin_set(pins[i].port, pins[i].pin_num, 1); // Release reset (HIGH)

        // No additional delay needed - chip is ready immediately

            break;

        }

    }

    */

    //gpio_pin_set(DPEQ_RST_port, DPEQ_RST, 1); // Set pin HIGH

    

    // 4. Enable new DP port (causes HPD high)

    for (int i = 0; i < ARRAY_SIZE(pins); i++) {

        if (strcmp(dp_pins[dp_port], pins[i].name) == 0) {

            gpio_pin_set(pins[i].port, pins[i].pin_num, 1);

            break;

        }

    }

    

    // 5. Wait for link training to complete (typically 100-500ms)

    shell_print(shell, "Allowing time for link training...");

    k_sleep(K_MSEC(500));

    

    shell_print(shell, "DP switch to port %d complete", dp_port);

    return 0;

}


/* Register the command so the shell (and compiler) knows it's being used */

SHELL_CMD_REGISTER(dp_switch_training, NULL, "Switch DP port with training (0-7)", cmd_dp_switch_with_training);


/* --- Shell Commands --- */

static int cmd_gpio_set(const struct shell *shell, size_t argc, char **argv)

{

    // Usage: set <name> <0|1>

    if (argc != 3) {

        shell_error(shell, "Usage: set <pin_name> <0|1>");

        return -EINVAL;

    }



    const char *name = argv[1];

    int value = atoi(argv[2]);



    if (value != 0 && value != 1) {

        shell_error(shell, "Value must be 0 or 1");

        return -EINVAL;

    }



    for (int i = 0; i < ARRAY_SIZE(pins); i++) {

        if (strcmp(name, pins[i].name) == 0) {

            int ret = gpio_pin_set(pins[i].port, pins[i].pin_num, value);

            if (ret < 0) {

                shell_error(shell, "Failed to set pin %s: %d", name, ret);

                return ret;

            }

            shell_print(shell, "Set %s to %d", name, value);

            return 0;

        }

    }



    shell_error(shell, "Pin with name '%s' not found.", name);

    return -EINVAL;

}



static int cmd_list_pins(const struct shell *shell, size_t argc, char **argv)

{

    shell_print(shell, "Available GPIO pins:");

    shell_print(shell, "--------------------");

    for (int i = 0; i < ARRAY_SIZE(pins); i++) {

        int port_num = (pins[i].port == gpio0) ? 0 : 1;

        shell_print(shell, "  Name: %-12s (GPIO%d, Pin %2d)", pins[i].name, port_num, pins[i].pin_num);

    }

    return 0;

}



/*

static int cmd_dp_switch(const struct shell *shell, size_t argc, char **argv)

{

    if (argc != 2) {

        shell_error(shell, "Usage: dp_switch <0-7>");

        return -EINVAL;

    }

    

    int dp_port = atoi(argv[1]);

    if (dp_port < 0 || dp_port > 7) {

        shell_error(shell, "DP port must be 0-7");

        return -EINVAL;

    }

    

    // First, disable all DP selection pins

    const char* dp_pins[] = {"DP_SEL0", "DP_SEL1", "DP_SEL2", "DP_SEL3", 

                            "DP_SEL4", "DP_SEL5", "DP_SEL6", "DP_SEL7"};

    

    for (int i = 0; i < 8; i++) {

        for (int j = 0; j < ARRAY_SIZE(pins); j++) {

            if (strcmp(dp_pins[i], pins[j].name) == 0) {

                gpio_pin_set(pins[j].port, pins[j].pin_num, 0);

                break;

            }

        }

    }

    

    // Small delay to allow signals to settle

    k_sleep(K_MSEC(10));

    

    // Now enable the selected DP port

    for (int i = 0; i < ARRAY_SIZE(pins); i++) {

        if (strcmp(dp_pins[dp_port], pins[i].name) == 0) {

            gpio_pin_set(pins[i].port, pins[i].pin_num, 1);

            shell_print(shell, "Switched to DisplayPort %d", dp_port);

            return 0;

        }

    }

    

    shell_error(shell, "DP port %d not found", dp_port);

    return -EINVAL;

}

*/


/* Register shell commands */

SHELL_STATIC_SUBCMD_SET_CREATE(sub_gpio,

    SHELL_CMD_ARG(set, NULL, "Set a GPIO pin's value. Usage: set <name> <0|1>", cmd_gpio_set, 3, 0),

    SHELL_CMD_ARG(list, NULL, "List all configured GPIO pins", cmd_list_pins, 1, 0),

    SHELL_SUBCMD_SET_END

);

SHELL_CMD_REGISTER(gpio, &sub_gpio, "GPIO control commands", NULL);


int main(void) {

    printk("Zeus_300d System Ready\n");



    if (!device_is_ready(gpio0) || !device_is_ready(gpio1)) {

        printk("Error: GPIO controllers not ready\n");

        return -ENODEV;

    }



    // This loop uses the 'pins' array, resolving the compiler error

    for (int i = 0; i < ARRAY_SIZE(pins); i++) {

        int ret = gpio_pin_configure(pins[i].port, pins[i].pin_num, pins[i].flags);

        if (ret < 0) {

            printk("Error configuring %s: %d\n", pins[i].name, ret);

        }

    }



    printk("Successfully configured %d GPIO pins\n", ARRAY_SIZE(pins));



    while (1) {

        // Toggle MCU_LED to show the board is alive

        gpio_pin_toggle(gpio0, 25); 

        k_sleep(K_MSEC(1000));

    }

    return 0;

}



