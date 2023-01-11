#define SOURCE_FILE "MAIN"
// src headers
#include "MQTTBroker.h"
#include "Network.h"
#include "QueueWrapper.h"
#include "TaskWrapper.h"
#include "common.h"
#include "esp.h"
#include "network_configuration.h"
// external headers
#include "hardware/watchdog.h"
#include "pico/bootrom.h"
#include "pico/stdlib.h"
#include "protocol.h"
#include <malloc.h>
#include <string.h>
#include<stdlib.h>

float temperature= 14.0;
int delay=0;

//publishing temperature
void publishTemperature(float temperature) {
    char buffer[2];
    sprintf(buffer, "%.1f", temperature);
    char *data = malloc(strlen(buffer) + strlen(" Degree"));
    strcpy(data, buffer);
    strcat(data, " Degree");
    publishData("Temperature", data);
    free(data);
}

//subscribe message received
void deliver(Posting posting) {

    PRINT("Received Data: %s", posting.data)
    delay = atoi(posting.data);
    PRINT("%d", delay)
}

_Noreturn void mainTask(void) {
    network_TryToConnectToNetworkUntilSuccessful(NetworkCredentials);
    mqtt_connectToBrokerUntilSuccessful(MQTTHost, "eip://uni-due.de/es", "enV5");
    subscribeForData("sub", "Delay", (Subscriber){.deliver = deliver});
    while (delay == 0){
        PRINT("Waiting...")
        TaskSleep(1000);
    }

    while (true) {
        publishTemperature(temperature);
        TaskSleep(delay);
    }

}


// Goes into bootloader mod load when 'r' is pressed
_Noreturn void enterBootModeTask(void) {
    while (true) {
        if (getchar_timeout_us(10) == 'r' || !stdio_usb_connected()) {
            reset_usb_boot(0, 0);
        }
        // Watchdog update needs to be performed frequent, otherwise the device
        // will crash
        watchdog_update();
        TaskSleep(1000);
    }
}

void init(void) {
    // First check if we crash last time -> reboot into boot rom mode
    if (watchdog_enable_caused_reboot()) {
        reset_usb_boot(0, 0);
    }
    // init usb, queue and watchdog
    stdio_init_all();
    // waits for usb connection, REMOVE to continue without waiting for
    // connection
    while ((!stdio_usb_connected()))
        ;
    // Checks connection to ESP and initializes
    esp_Init();
    // Create FreeRTOS task queue
    CreateQueue();
    // enables watchdog to check for reboots
    watchdog_enable(2000, 1);
}

int main() {
    init();
    RegisterTask(enterBootModeTask, "enterBootModeTask");
    RegisterTask(mainTask, "mainTask");
    // Starts FreeRTOS tasks
    StartScheduler();
}
