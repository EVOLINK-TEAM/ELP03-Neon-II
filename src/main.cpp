#include "esp_sleep.h"
#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "RTClib.h"
#include "evlk_hv5523.h"

using namespace _EVLK_SHIFTREG_;

#define PIN_KEY1 12
#define PIN_KEY2 14
#define PIN_ADC 34
#define PIN_CHG 34
#define PIN_INT1 35
#define PIN_INT2 32
#define PIN_RGB 13
#define PIN_RGBU 15
#define PIN_RGBD 2
#define PIN_SDA 21
#define PIN_SCL 22
#define PIN_EN_LIGHT 26
#define PIN_EN_VH 27
#define PIN_HV_BL 4
#define PIN_HV_DIN 5
#define PIN_HV_LE 18
#define PIN_HV_CLK 23

#define conf_tube_max_light 255
#define conf_rgb_max_light 255
#define conf_rgbt_max_light 255

#define mode 1

enum PWR_LEVEL {
    PWR_LOW,
    PWR_MID,
    PWR_HIGH,
    PWR_FULL,
};
PWR_LEVEL sysPwrLevel        = PWR_FULL;
const uint8_t tubeTohv5523[] = {13, 12, 5,  10, 8,  7,  6,  11, 15, 14, 9,  4,   // 10=L, 11=R;
                                20, 19, 24, 17, 27, 26, 25, 18, 22, 21, 28, 23}; // 22=L, 23=R;

// #define delay_t(ms) vTaskDelay(pdMS_TO_TICKS(ms));
#define delay_t(ms) delay(ms)

hv5523 HV5523(PIN_HV_DIN, PIN_HV_LE, PIN_HV_CLK, 1);
Adafruit_NeoPixel rgb(2, PIN_RGB, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel rgbt(1, PIN_RGBD, NEO_RGB + NEO_KHZ800);
RTC_DS3231 rtc;

void enablePINS() {
    digitalWrite(PIN_EN_LIGHT, LOW);
    digitalWrite(PIN_EN_VH, HIGH);
    digitalWrite(PIN_HV_BL, HIGH);
}
void disablePINS() {
    digitalWrite(PIN_EN_LIGHT, HIGH);
    digitalWrite(PIN_EN_VH, LOW);
    digitalWrite(PIN_HV_BL, LOW);
    // pinMode(PIN_EN_LIGHT, INPUT);
    // pinMode(PIN_EN_VH, INPUT);
}
void SystemSleepInit() { esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_KEY2, LOW); }
void SystemPowerDown() {
    HV5523.~hv5523();
    rgb.~Adafruit_NeoPixel();
    disablePINS();
    // while (1)
    //     ;

    SystemSleepInit();
    esp_deep_sleep_start();
}

double magicPwrFunction(double vot) {
    if (vot <= 2.5)
        return 0;
    if (vot <= 3.68f)
        return 5 * vot - 12.5;
    return -263.2 * vot * vot + 2249.2 * vot - 4705.2;
}
double adc(uint32_t adcValue) { return (adcValue / 4095.0) * 2 * 3.3; }
PWR_LEVEL getPwrLevel(uint32_t pin) {
    analogReadResolution(12);
    // pinMode(pin, INPUT_ANALOG);
    double per = magicPwrFunction(adc(analogRead(pin)));
    if (per >= 95)
        return PWR_FULL;
    if (per >= 70)
        return PWR_HIGH;
    if (per >= 30)
        return PWR_MID;
    return PWR_LOW;
};

void rtc_init() {
    rtc.disable32K();
    rtc.disableAlarm(1);
    rtc.disableAlarm(2);
    rtc.writeSqwPinMode(DS3231_OFF);

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void setTubeLight(uint8_t val) { analogWrite(PIN_HV_BL, val); }
void printNUM(bool tube, uint8_t num, bool l = false, bool r = false) {
    uint8_t mask[4] = {tube ? 255 : 0, tube ? 255 : 0, tube ? 0 : 255, tube ? 0 : 255};
    HV5523.digitalWrite(mask, 4, 0);
    for (int i = 0; i < 4; i++)
        mask[i] = 0;

    auto masknum = [tube, &mask](int num) {
        num = (tubeTohv5523[tube ? num + 12 : num]) - 1;
        mask[3 - (num / 8)] |= 1 << (num % 8);
    };

    masknum(num);
    if (l)
        masknum(10);
    if (r)
        masknum(11);
    HV5523.digitalWrite(mask, 4, 1);
};
void printNUM(uint8_t num, bool l1 = false, bool r1 = false, bool l2 = false, bool r2 = false) {
    uint8_t ten = num / 10;
    num         = num % 10;
    printNUM(false, ten, l1, r1);
    printNUM(true, num, l2, r2);
};

// ###################################################################
void printTime(void *) {
    DateTime now = rtc.now();
    printNUM(now.hour(), true, false, false, false);
    delay_t(2000);
    printNUM(now.minute(), false, true, false, false);
    delay_t(2000);
    for (int i = 0; i < 5; i++) {
        now = rtc.now();
        printNUM(now.second(), false, false, true, true);
        delay_t(1000);
    }
    SystemPowerDown();
}

void setup() {
    pinMode(PIN_EN_LIGHT, OUTPUT);
    pinMode(PIN_EN_VH, OUTPUT);
    pinMode(PIN_HV_BL, OUTPUT);
    pinMode(PIN_KEY1, INPUT);
    pinMode(PIN_KEY2, INPUT_PULLUP);

    enablePINS();

    Wire.begin(PIN_SDA, PIN_SCL);
    HV5523.Begin();
    rgb.begin();
    rgbt.begin();
    rtc.begin();

    if (rtc.lostPower())
        rtc_init();

    sysPwrLevel               = getPwrLevel(PIN_ADC);
    const uint32_t color_blue = Adafruit_NeoPixel::Color(0, 100, 255);
    const uint32_t color_red  = Adafruit_NeoPixel::Color(200, 0, 10);
    uint32_t color            = Adafruit_NeoPixel::Color(0, 0, 0);

    if (sysPwrLevel > PWR_LOW) {
        rgb.setPixelColor(0, color_red);
        rgb.setPixelColor(1, color_red);
        if (sysPwrLevel == PWR_MID)
            color = color_red;
        if (sysPwrLevel >= PWR_HIGH)
            color = color_blue;
    }

    rgbt.setPixelColor(0, color);
    for (int i = 0; i < 255; i++) { // start rgb
        rgb.show();
        rgbt.show();
    }

#if mode == 1 // 正常
    printTime(NULL);
#elif mode == 2
    delay(5000);
    Serial.begin(9600);
    Serial.println("please input iso8601(2000-01-01T00:00:00):");
    while (!Serial.available())
        delay(1);
    String iso8601date = Serial.readString();
    DateTime now(iso8601date.c_str());
    Serial.println("push the button!");
    while (digitalRead(PIN_KEY2))
        delay(1);
    rtc.adjust(now);
    Serial.println("OK!");
    SystemPowerDown();
#endif
}

void loop() {}
