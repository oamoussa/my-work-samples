# ============================================
# UART Console Configuration - CRITICAL
# ============================================
CONFIG_SERIAL=y
CONFIG_UART_INTERRUPT_DRIVEN=y
CONFIG_CONSOLE=y
CONFIG_UART_CONSOLE=y
CONFIG_STDOUT_CONSOLE=y
CONFIG_PRINTK=y

# Enable early console BEFORE main()
CONFIG_EARLY_CONSOLE=y

# Show boot banner
CONFIG_BOOT_BANNER=y
CONFIG_BOOT_DELAY=0

# ============================================
# Logging Configuration
# ============================================
CONFIG_LOG=y
CONFIG_LOG_DEFAULT_LEVEL=3
CONFIG_LOG_BACKEND_UART=y
CONFIG_LOG_PRINTK=y

# ============================================
# Shell Configuration
# ============================================
CONFIG_SHELL=y
CONFIG_SHELL_BACKEND_SERIAL=y
CONFIG_SHELL_BACKEND_SERIAL_INIT_PRIORITY=50
CONFIG_SHELL_PROMPT_UART="uart:~$ "
CONFIG_SHELL_STACK_SIZE=4096
CONFIG_SHELL_CMD_BUFF_SIZE=256
CONFIG_SHELL_STATS=y
CONFIG_SHELL_HELP=y

# ============================================
# Stack Sizes
# ============================================
CONFIG_MAIN_STACK_SIZE=4096
CONFIG_SYSTEM_WORKQUEUE_STACK_SIZE=4096
CONFIG_ISR_STACK_SIZE=2048

# ============================================
# GPIO Configuration
# ============================================
CONFIG_GPIO=y

# ============================================
# I2C Configuration
# ============================================
CONFIG_I2C=y
CONFIG_I2C_TARGET=y
CONFIG_I2C_SHELL=y

# ============================================
# Pin Control
# ============================================
CONFIG_PINCTRL=y

# ============================================
# Memory Configuration
# ============================================
CONFIG_HEAP_MEM_POOL_SIZE=2048

# ============================================
# Optional: UART Line Control
# ============================================
CONFIG_UART_LINE_CTRL=y

# ============================================
# Optional: Console Handler
# ============================================
CONFIG_CONSOLE_HANDLER=y