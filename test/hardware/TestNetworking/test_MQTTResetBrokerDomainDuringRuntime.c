#define SOURCE_FILE "RESET-BROKER-TEST"

#include "hardware/watchdog.h"
#include "pico/bootrom.h"
#include "pico/stdlib.h"

#include "MQTTBroker.h"
#include "QueueWrapper.h"
#include "TaskWrapper.h"
#include "Network.h"
#include "protocol.h"

void enterBootModeTask(void);

void init(void);

void _Noreturn mainTask(void) {
    while (NetworkStatus.WIFIStatus == NOT_CONNECTED) {
        Network_ConnectToNetworkPlain("PUT_YOUR_SSID_HERE", "PASSWORD");
    }

    while (true) {
        MQTT_Broker_ConnectToBroker("PUT_BROKER_IP_HERE", "1883", "embedded_dev_bro",
                                    "embedded_device_bro"); //Free broker
        publishData("testENv5Pub", "data");
        TaskSleep(5000);
        MQTT_Broker_setBrokerDomain("second_device_with_longer_name");
    }
}

int main() {

    init();

    RegisterTask(enterBootModeTask, "enterBootModeTask");
    RegisterTask(mainTask, "mainTask");
//    RegisterTask((TaskCodeFunc) mainTask, 0);
    StartScheduler();
}

void init(void) {
    stdio_init_all();
    while ((!stdio_usb_connected())) {}

    // Did we crash last time -> reboot into boot rom mode
    if (watchdog_enable_caused_reboot()) {
        reset_usb_boot(0, 0);
    }
    while (!Network_init());//
    // init usb, queue and watchdog

    CreateQueue();
    watchdog_enable(2000, 1);
}

void _Noreturn enterBootModeTask(void) {
    while (true) {
        if (getchar_timeout_us(10) == 'r' || !stdio_usb_connected()) {
            MQTT_Broker_Disconnect(true);//
            reset_usb_boot(0, 0);
        }
        watchdog_update();
        TaskSleep(1000);
    }
}
