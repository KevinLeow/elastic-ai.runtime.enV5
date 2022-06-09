#define SOURCE_FILE "MAIN"
// src headers
#include "QueueWrapper.h"
#include "TaskWrapper.h"
#include "common.h"
#include "MQTTBroker.h"
#include "Network.h"
// external headers
#include "hardware/watchdog.h"
#include "pico/bootrom.h"
#include "pico/stdlib.h"

_Noreturn void mainTask(void) {
    while (true) {
        PRINT("Hello, World!")
        TaskSleep(5000);
    }
}

_Noreturn void enterBootModeTask(void) {
    while (true) {
        if (getchar_timeout_us(10) == 'r' || !stdio_usb_connected()) {
            MQTT_Broker_Disconnect(true);
            reset_usb_boot(0, 0);
        }
        watchdog_update();
        TaskSleep(1000);
    }
}

void init(void) {
    // Did we crash last time -> reboot into boot rom mode
    if (watchdog_enable_caused_reboot()) {
        reset_usb_boot(0, 0);
    }
    // init usb, queue and watchdog
    stdio_init_all();
    while ((!stdio_usb_connected())) {} // waits for usb connections

    while (!Network_init());
    CreateQueue();

    watchdog_enable(2000, 1);
}

int main() {
    init();

    RegisterTask(enterBootModeTask, "enterBootModeTask");
    RegisterTask((TaskCodeFunc) mainTask, "mainTask");
    StartScheduler();
}
