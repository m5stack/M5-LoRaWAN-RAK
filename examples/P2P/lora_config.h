#include "Arduino.h"

#define FREQ_868M 0
#define FREQ_915M 1
#define FREQ_923M 0
#define FREQ_470M 0

#if FREQ_868M
#define PRODUCT_NAME "UNIT LoRaWAN2 RAK 868"

#define LORA_FREQ     868E6
#define LORA_FREQ_STR "868M"

#elif FREQ_915M
#define PRODUCT_NAME  "UNIT LoRaWAN2 RAK 915"
#define LORA_FREQ     915E6
#define LORA_FREQ_STR "915M"

#elif FREQ_923M
#define PRODUCT_NAME  "UNIT LoRaWAN2 RAK 923"
#define LORA_FREQ     923E6
#define LORA_FREQ_STR "923M"

#elif FREQ_470M
#define PRODUCT_NAME  "UNIT LoRaWAN RAK 470"
#define LORA_FREQ     470E6
#define LORA_FREQ_STR "470M"

#endif
