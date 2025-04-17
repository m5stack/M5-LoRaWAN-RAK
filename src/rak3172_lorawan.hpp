/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef _RAK3172_LoRaWAN_HPP_
#define _RAK3172_LoRaWAN_HPP_

#include <Arduino.h>
#include "Stream.h"
#include <vector>
#include "rak3172_common.hpp"

/**
 * @def EU433
 * @brief Europe 433 MHz frequency band
 *
 * This represents the LoRaWAN frequency plan for the European 433 MHz band.
 */
#define EU433 "0"

/**
 * @def CN470
 * @brief China 470 MHz frequency band
 *
 * This represents the LoRaWAN frequency plan for the Chinese 470 MHz band.
 */
#define CN470 "1"

/**
 * @def RU864
 * @brief Russia 864 MHz frequency band
 *
 * This represents the LoRaWAN frequency plan for the Russian 864 MHz band.
 */
#define RU864 "2"

/**
 * @def IN865
 * @brief India 865 MHz frequency band
 *
 * This represents the LoRaWAN frequency plan for the Indian 865 MHz band.
 */
#define IN865 "3"

/**
 * @def EU868
 * @brief Europe 868 MHz frequency band
 *
 * This represents the LoRaWAN frequency plan for the European 868 MHz band.
 */
#define EU868 "4"

/**
 * @def US915
 * @brief United States 915 MHz frequency band
 *
 * This represents the LoRaWAN frequency plan for the United States 915 MHz band.
 */
#define US915 "5"

/**
 * @def AU915
 * @brief Australia 915 MHz frequency band
 *
 * This represents the LoRaWAN frequency plan for the Australian 915 MHz band.
 */
#define AU915 "6"

/**
 * @def KR920
 * @brief South Korea 920 MHz frequency band
 *
 * This represents the LoRaWAN frequency plan for the South Korean 920 MHz band.
 */
#define KR920 "7"

/**
 * @def AS923_1
 * @brief Asia 923 MHz frequency band (Region 1)
 *
 * This represents the LoRaWAN frequency plan for Asia in the 923 MHz band, Region 1.
 */
#define AS923_1 "8"

/**
 * @def AS923_1_JP_LBT
 * @brief Asia 923 MHz frequency band (Region 1 with Japan LBT)
 *
 * This represents the specific LoRaWAN frequency plan for Asia 923 MHz band with Japan's Listen Before Talk (LBT)
 * requirement.
 */
#define AS923_1_JP_LBT "8-1-JP"

/**
 * @def AS923_2
 * @brief Asia 923 MHz frequency band (Region 2)
 *
 * This represents the LoRaWAN frequency plan for Asia in the 923 MHz band, Region 2.
 */
#define AS923_2 "8-2"

/**
 * @def AS923_3
 * @brief Asia 923 MHz frequency band (Region 3)
 *
 * This represents the LoRaWAN frequency plan for Asia in the 923 MHz band, Region 3.
 */
#define AS923_3 "8-3"

/**
 * @def AS923_4
 * @brief Asia 923 MHz frequency band (Region 4)
 *
 * This represents the LoRaWAN frequency plan for Asia in the 923 MHz band, Region 4.
 */
#define AS923_4 "8-4"

/**
 * @brief Enumeration for LoRaWAN join modes.
 */
typedef enum {
    OTAA = 0, /**< Over-The-Air Activation */
    ABP       /**< Activation By Personalization */
} lorawan_join_mode_t;

/**
 * @brief Enumeration for LoRaWAN device classes.
 */
typedef enum {
    CLASS_A = 0, /**< Class A: Bi-directional, allows downlink only after an uplink transmission */
    CLASS_B,     /**< Class B: Bi-directional, allows periodic downlink windows */
    CLASS_C      /**< Class C: Bi-directional, allows continuous listening for downlinks */
} lorawan_dev_class_t;

/**
 * @brief Enumeration for link check modes in LoRaWAN.
 */
typedef enum {
    DIS_LINKCHECK = 0, /**< Disable link check requests */
    ONCE_LINKCHECK,    /**< Send link check request once */
    ALLWAYS_LINKCHECK  /**< Send link check requests continuously */
} lorawan_linkcheck_t;

/**
 * @brief Enumeration for LoRaWAN error codes.
 */
typedef enum {
    ERROR = 0 /**< Generic error code */
} lorawan_error_t;

/**
 * @brief Structure representing LoRaWAN received frame information.
 */
typedef struct {
    int rssi;          /**< Received Signal Strength Indicator (RSSI) in dBm */
    int snr;           /**< Signal-to-Noise Ratio (SNR) in dB */
    int len;           /**< Length of the payload */
    int port;          /**< Port number for the message */
    bool unicast;      /**< Indicates if the message is a unicast (true) or broadcast (false) */
    char payload[500]; /**< Payload data received (up to 500 bytes) */
} lorawan_frame_t;

class RAK3172LoRaWAN : public RAK3172 {
public:
    /**
     * @brief Initializes the RAK3172 LoRaWAN module.
     *
     * This function sets up the hardware serial interface and configures the
     * RAK3172 module to operate on the LoRaWAN network. It sends the necessary
     * commands to switch the module to the appropriate mode.
     *
     * @note This function calls the base class initialization and includes
     *       a delay to allow for hardware stabilization before sending commands.
     *
     * @param serial Pointer to the HardwareSerial object used for communication.
     * @param rx The RX pin number for serial communication.
     * @param tx The TX pin number for serial communication.
     * @param baudRate The desired baud rate for serial communication.
     *
     * @return True if the initialization commands were successfully sent;
     *         false otherwise.
     */
    bool init(HardwareSerial* serial = &Serial2, int rx = 16, int tx = 17, rak3172_bps_t baudRate = RAK3172_BPS_115200);

    /**
     * @brief Sets the global application identifier (AppEUI) for the RAK3172 LoRaWAN module.
     *
     * This command allows users to access and configure the global application
     * identifier (AppEUI) for the device. The AppEUI must be a string of
     * exactly 8 characters in length.
     *
     * @note The function checks the validity of the input string before
     *       attempting to send the command to the device.
     *
     * @param identifier A constant reference to a String representing
     *                   the 8-character application identifier.
     *
     * @return True if the command to set the application identifier was
     *         successfully sent; false if the identifier is invalid.
     */
    bool setApplicationIdentifier(const String& identifier);

    /**
     * @brief Sets the application key (AppKey) for the RAK3172 LoRaWAN module.
     *
     * This command allows users to access and configure the application
     * key (AppKey) for the device. The AppKey must be a string of
     * exactly 16 characters in length.
     *
     * @note The function checks the validity of the input string before
     *       attempting to send the command to the device.
     *
     * @param key A constant reference to a String representing
     *            the 16-character application key.
     *
     * @return True if the command to set the application key was
     *         successfully sent; false if the key is invalid.
     */
    bool setApplicationKey(const String& key);

    /**
     * @brief Sets the application session key (AppSKey) for the RAK3172 LoRaWAN module.
     *
     * This command allows users to configure the application session
     * key (AppSKey) for the device. The AppSKey must be a string of
     * exactly 16 characters in length.
     *
     * @note The function checks the validity of the input string before
     *       attempting to send the command to the device.
     *
     * @param key A constant reference to a String representing
     *            the 16-character application session key.
     *
     * @return True if the command to set the application session key was
     *         successfully sent; false if the key is invalid.
     */
    bool setApplicationSessionKey(const String& key);

    /**
     * @brief Sets the network session key (NwkSKey) for the RAK3172 LoRaWAN module.
     *
     * This command allows users to configure the network session
     * key (NwkSKey) for the device. The NwkSKey must be a string of
     * exactly 16 characters in length.
     *
     * @note The function checks the validity of the input string before
     *       attempting to send the command to the device.
     *
     * @param key A constant reference to a String representing
     *            the 16-character network session key.
     *
     * @return True if the command to set the network session key was
     *         successfully sent; false if the key is invalid.
     */
    bool setNetworkSessionKey(const String& key);

    /**
     * @brief Sets the device address (DevAddr) for the RAK3172 LoRaWAN module.
     *
     * This command allows users to configure the device address
     * (DevAddr) for the device. The DevAddr must be a string of
     * exactly 4 characters in length.
     *
     * @note The function checks the validity of the input string before
     *       attempting to send the command to the device.
     *
     * @param addr A constant reference to a String representing
     *             the 4-character device address.
     *
     * @return True if the command to set the device address was
     *         successfully sent; false if the address is invalid.
     */
    bool setDevAddr(const String& addr);

    /**
     * @brief Sets the global end-device identifier (DevEUI) for the RAK3172 LoRaWAN module.
     *
     * This command allows users to configure the global end-device
     * identifier (DevEUI) for the device. The DevEUI must be a string of
     * exactly 8 characters in length.
     *
     * @note The function checks the validity of the input string before
     *       attempting to send the command to the device.
     *
     * @param eui A constant reference to a String representing
     *            the 8-character global end-device identifier.
     *
     * @return True if the command to set the DevEUI was
     *         successfully sent; false if the EUI is invalid.
     */
    bool setDevEUI(const String& eui);

    /**
     * @brief Sets the frequency band and channel mask for the RAK3172 LoRaWAN module.
     *
     * This function sets the frequency band corresponding to a specific
     * region using the command AT+BAND. Additionally, it configures the
     * device's channels by setting a hexadecimal channel mask using
     * the command AT+MASK.
     *
     * @note Make sure that the frequency band and channel mask correspond to
     *       the regulations of your operating region. Incorrect settings may
     *       lead to communication issues or non-compliance with local laws.
     * @note **This function is only applicable to the following frequency bands:**
     *       - `US915`
     *       - `AU915`
     *       - `CN470`
     *       Other frequency bands may require different configuration methods.
     *
     * @param band A String representing the frequency band number
     *             associated with the desired region.
     * @param channel_mask A String representing the hexadecimal channel mask
     *                     used to configure the device's channels.
     *
     * @return True if both commands to set the band and channel mask
     *         were successfully sent; false otherwise.
     */
    bool setBAND(String band, String channel_mask="");

    /**
     * @brief Configures the device for Over-The-Air Activation (OTAA) for the RAK3172 LoRaWAN module.
     *
     * This function sets the device's join mode to OTAA and configures
     * the necessary parameters: DevEUI, AppEUI, and AppKey.
     * It sends the following commands:
     * - AT+NJM=1: Sets the join mode to OTAA.
     * - AT+DEVEUI=<deveui>: Sets the device identifier.
     * - AT+APPEUI=<appeui>: Sets the application identifier.
     * - AT+APPKEY=<appkey>: Sets the application key.
     *
     * @note Ensure that all parameters (DevEUI, AppEUI, AppKey) are set correctly
     *       and conform to the specifications to avoid issues during activation.
     *       Invalid values may prevent the device from joining the network.
     *
     * @param deveui A String representing the global end-device identifier
     *               (DevEUI) needed for OTAA.
     * @param appeui A String representing the global application identifier
     *               (AppEUI) required for OTAA.
     * @param appkey A String representing the application key (AppKey) used for
     *                secure communication in OTAA.
     *
     * @return True if all commands to configure OTAA were successfully sent;
     *         false otherwise.
     */
    bool setOTAA(String deveui, String appeui, String appkey);

    /**
     * @brief Configures the device for Activation By Personalization (ABP) for the RAK3172 LoRaWAN module.
     *
     * This function sets the device's join mode to ABP and configures
     * the necessary parameters: DevAddr, NwkSKey, and AppSKey.
     * It sends the following commands:
     * - AT+NJM=0: Sets the join mode to ABP.
     * - AT+DEVADDR=<devaddr>: Sets the device address (DevAddr).
     * - AT+NWKSKEY=<nwkskey>: Sets the network session key (NwkSKey).
     * - AT+APPSKEY=<appskey>: Sets the application session key (AppSKey).
     *
     * @note Ensure that all parameters (DevAddr, NwkSKey, AppSKey) are valid
     *       and correctly formatted. Incorrect values may lead to failure in
     *       device activation or secure communication issues.
     *
     * @param devaddr A String representing the device address (DevAddr)
     *                 needed for ABP.
     * @param nwkskey A String representing the network session key (NwkSKey)
     *                 used for communication in ABP.
     * @param appskey A String representing the application session key (AppSKey)
     *                 required for secure communication in ABP.
     *
     * @return True if all commands to configure ABP were successfully sent;
     *         false otherwise.
     */
    bool setABP(String devaddr, String nwkskey, String appskey);

    /**
     * @brief Configures multicast settings by adding a new multicast group and its parameters.
     *
     * This function constructs a command to add a new multicast group
     * along with associated parameters such as device address, network session key,
     * application session key, frequency, data rate, and periodicity.
     * It sends the command using AT+ADDMULC.
     *
     * @note Ensure that the provided parameters conform to the specifications
     *       and requirements of your network. Incorrect values may lead to
     *       communication issues or failure to join the multicast group.
     *
     * @param mode A String representing the multicast mode (e.g., "0" for
     *             standard multicast, "1" for other modes).
     * @param devaddr A String representing the device address (DevAddr)
     *                 for the new multicast group.
     * @param nwkskey A String representing the network session key (NwkSKey)
     *                 used for communication in the multicast group.
     * @param appskey A String representing the application session key (AppSKey)
     *                 required for secure communication in the multicast group.
     * @param freq A uint32_t representing the frequency to be used for the
     *             multicast communication.
     * @param dataRate A uint8_t representing the data rate to be used for
     *                  the multicast communication.
     * @param periodicity A uint8_t representing the periodicity of the multicast
     *                    messages.
     *
     * @return The constructed command string to add the new multicast group
     *         and its parameters. This should then be sent to the device
     *         for execution.
     */
    bool setADDMulc(String mode, String devaddr, String nwkskey, String appskey, uint32_t freq, uint8_t dataRate,
                    uint8_t periodicity);

    /**
     * @brief Deletes a configured multicast group using its device address.
     *
     * This command allows users to remove a previously configured
     * multicast group identified by its device address.
     * It sends the command using AT+RMVMULC.
     *
     * @note Ensure that the device address provided is correct and corresponds
     *       to an existing multicast group. Attempting to delete a non-existent
     *       group may result in an error or no action being taken.
     *
     * @param devaddr A String representing the device address (DevAddr)
     *                 of the multicast group to be removed.
     *
     * @return The constructed command string to delete the specified
     *         multicast group. This should then be sent to the device
     *         for execution.
     */
    bool detelRmvmulc(String devaddr);

    /**
     * @brief Configures the link check settings for the RAK3172 LoRaWAN module.
     *
     * This command allows users to access and configure the device's
     * network link status. The following link check modes can be set:
     * - 0 (DIS_LINKCHECK): Disables link checking.
     * - 1 (ONCE_LINKCHECK): Executes a link check on the next valid
     *   uplink payload.
     * - 2 (ALLWAYS_LINKCHECK): The module will automatically execute
     *   a link check after each data upload.
     *
     * @note Ensure that the selected link check mode aligns with your
     *       application requirements. For instance, enabling frequent
     *       link checks may increase network traffic and power consumption,
     *       while disabling them may hinder the monitoring of network reliability.
     *
     * @param mode The desired link check mode, specified as a
     *             lorawan_linkcheck_t enumeration value.
     *
     * @return True if the command to set the link check mode was
     *         successfully sent; false if an invalid mode was provided.
     */
    bool setLinkCheck(lorawan_linkcheck_t mode);

    /**
     * @brief Configures Adaptive Data Rate (ADR) settings for the RAK3172 LoRaWAN module.
     *
     * This command allows users to enable or disable the Adaptive Data Rate
     * feature. The following options are available:
     * - false: Disable ADR (off).
     * - true: Enable ADR (on).
     *
     * @note Enabling ADR allows the device to optimize its data rate
     *       and transmission power based on the network conditions, which can
     *       extend battery life and improve communication reliability.
     *       However, disabling ADR may be beneficial in environments with
     *       stable conditions or when a fixed data rate is preferred.
     *
     * @param adr A boolean value indicating whether to enable (true)
     *            or disable (false) the Adaptive Data Rate.
     *
     * @return True if the command to set the ADR mode was successfully sent;
     *         false if there was an error in sending the command.
     */
    bool setADR(bool adr);

    /**
     * @brief Configures the Duty Cycle Switch (DCS) settings for the RAK3172 LoRaWAN module.
     *
     * This command allows users to enable or disable the duty cycle feature.
     * The following options are available:
     * - false: Disable duty cycle (off).
     * - true: Enable duty cycle (on).
     *
     * @note Enabling the duty cycle feature helps adhere to regulatory
     *       requirements by limiting the amount of time the device can transmit,
     *       which can prevent interference with other devices. However, disabling
     *       it may allow for more frequent transmissions, beneficial in scenarios
     *       where rapid updates are necessary, but could risk interference
     *       and may not comply with local regulations.
     *
     * @param dcs A boolean value indicating whether to enable (true)
     *            or disable (false) the duty cycle.
     *
     * @return True if the command to set the duty cycle mode was successfully sent;
     *         false if there was an error in sending the command.
     */
    bool setDCS(bool dcs);

    /**
     * @brief Configures the Listen Before Talk (LBT) settings for the RAK3172 LoRaWAN module.
     *
     * This command allows users to enable or disable the "Listen Before Talk"
     * feature in LoRaWAN. The following options are available:
     * - false: Disable LBT.
     * - true: Enable LBT.
     *
     * @note Enabling the Listen Before Talk feature helps to minimize
     *       the risk of collisions on the communication channel by ensuring that
     *       the device listens for ongoing transmissions before attempting to transmit
     *       its own data. This can improve overall network reliability and efficiency,
     *       but may introduce some latency before the device starts transmitting.
     *
     * @param lb A boolean value indicating whether to enable (true)
     *           or disable (false) the Listen Before Talk feature.
     *
     * @return True if the command to set the LBT mode was successfully sent;
     *         false if there was an error in sending the command.
     */
    bool setLBT(bool lb);

    /**
     * @brief Configures the Data Rate (DR) settings for the RAK3172 LoRaWAN module.
     *
     * This command allows users to access and configure the data rate for
     * various frequency plans.  The valid data rates depend on the region:
     *
     * - For EU433 / RU864 / IN865 / EU868 / CN470 / KR920:
     *   Valid data rates are 0-5 (DR0-DR5).
     * - For AS923:
     *   Valid data rates are 2-5 (DR2-DR5).
     * - For US915:
     *   Valid data rates are 0-4 (DR0-DR4).
     * - For AU915:
     *   Valid data rates are 2-6 (DR2-DR6).
     * - For AU915 / LA915:
     *   Valid data rates are 0-6 (DR0-DR6).
     *
     * @note When selecting a data rate, consider the trade-off between range and
     *       power consumption. Lower data rates can provide longer ranges but may
     *       increase transmission time, while higher data rates reduce transmission
     *       time but may limit range. Ensure that the selected data rate is within
     *       the valid range for the specific frequency plan being used to avoid errors
     *       during communication.
     *
     * @param dr A uint8_t value representing the desired data rate
     *           to be set for the module.
     *
     * @return True if the command to set the data rate was successfully sent;
     *         false if there was an error in sending the command.
     */
    bool setDR(uint8_t dr);

    /**
     * @brief Configures the Transmission Power (TXP) settings for the RAK3172 LoRaWAN module.
     *
     * This command allows users to access and configure the transmission
     * power of the module. The default transmission power is 0. The valid
     * power levels depend on the frequency plan:
     *
     * - For EU868 / RU864 / KR920 / AS923 / CN470:
     *   Valid transmission power values are in the range of 0-7.
     * - For US915 / AU915:
     *   Valid transmission power values are in the range of 0-14.
     * - For EU433:
     *   Valid transmission power values are in the range of 0-5.
     * - For IN865:
     *   Valid transmission power values are in the range of 0-10.
     *
     * @note When configuring the transmission power, keep in mind that higher
     *       power levels can increase the communication range but may also lead
     *       to increased power consumption and potential interference with other
     *       devices. It’s essential to select the appropriate power level based on
     *       your specific application requirements and regulatory constraints for
     *       your operating region.
     *
     * @param power A uint8_t value representing the desired transmission
     *              power level to be set for the module.
     *
     * @return True if the command to set the transmission power was successfully sent;
     *         false if there was an error in sending the command.
     */
    bool setOutPower(uint8_t power);

    /**
     * @brief Configures the Join Receive Window 1 Delay (JN1DL) for the RAK3172 LoRaWAN module.
     *
     * This command allows users to access and configure the delay for
     * the first receive window after a join request. The delay value is
     * applicable only when the device is in Over-The-Air Activation (OTAA) mode.
     * Note that every time AT+JN1DL is updated, the AT+JN2DL will also be automatically updated.
     *
     * Valid delay values range from 1 to 14 seconds.
     *
     * @note The Join Receive Window 1 Delay determines how long the device
     *       waits after sending a join request before it listens for a response
     *       from the network on the first receive window. Adjusting this delay
     *       can impact the overall joining process; shorter delays may lead to
     *       quicker responses, while longer delays can help conserve battery life
     *       by allowing the device to enter low-power modes. Ensure that the
     *       delay is appropriate for your application requirements and network conditions.
     *
     * @param sec A uint8_t value representing the desired delay in seconds
     *            for the first receive window.
     *
     * @return True if the command to set the Join RX1 Delay was successfully sent;
     *         false if the device is not in OTAA mode or if an error occurred.
     */
    bool setJoinRX1Delay(uint8_t sec);

    /**
     * @brief Configures the Join Receive Window 2 Delay (JN2DL) for the RAK3172 LoRaWAN module.
     *
     * This command allows users to access and configure the delay for
     * the second receive window after a join request. The delay value is
     * applicable only when the device is in Over-The-Air Activation (OTAA) mode.
     * Note that every time AT+JN2DL is updated, the AT+JN1DL will also be automatically updated.
     *
     * Valid delay values range from 1 to 14 seconds.
     *
     * @note The Join Receive Window 2 Delay determines how long the device
     *       waits after sending a join request before it listens for a response
     *       from the network on the second receive window. Adjusting this delay
     *       can impact the overall joining process; shorter delays may lead to
     *       quicker responses, while longer delays can help conserve battery life
     *       by allowing the device to enter low-power modes. Ensure that the
     *       delay is appropriate for your application requirements and network conditions.
     *
     * @param sec A uint8_t value representing the desired delay in seconds
     *            for the second receive window.
     *
     * @return True if the command to set the Join RX2 Delay was successfully sent;
     *         false if the device is not in OTAA mode or if an error occurred.
     */
    bool setJoinRX2Delay(uint8_t sec);

    /**
     * @brief Configures the Receive Window 1 Delay (RX1DL) for the RAK3172 LoRaWAN module.
     *
     * This command allows users to access and configure the delay for
     * the first receive window. The delay value is applicable when the device
     * is listening for downlink messages. Note that every time AT+RX1DL is updated,
     * the AT+RX2DL will also be automatically updated.
     *
     * Valid delay values range from 1 to 15 seconds.
     *
     * @note The Receive Window 1 Delay determines how long the device waits
     *       after sending an uplink message before it listens for a response
     *       from the network on the first receive window. Adjusting this delay
     *       can influence the timing of downlink messages, so it's important to set
     *       it according to the specific requirements of your application and
     *       the behavior of the LoRaWAN network.
     *
     * @param sec A uint8_t value representing the desired delay in seconds
     *            for the first receive window.
     *
     * @return True if the command to set the RX1 Delay was successfully sent;
     *         false if an error occurred during the command execution.
     */
    bool setRX1Delay(uint8_t sec);

    /**
     * @brief Configures the Receive Window 2 Delay (RX2DL) for the RAK3172 LoRaWAN module.
     *
     * This command allows users to access and configure the delay for
     * the second receive window. The delay value is applicable when the device
     * is listening for downlink messages on the second receive window. Note that
     * every time AT+RX2DL is updated, the AT+RX1DL will also be automatically updated.
     *
     * Valid delay values range from 2 to 16 seconds.
     *
     * @note The Receive Window 2 Delay determines how long the device waits
     *       after sending an uplink message before it listens for a response
     *       from the network on the second receive window. Adjusting this delay
     *       can influence the timing of downlink messages, so it's important to set
     *       it according to the specific requirements of your application and
     *       the behavior of the LoRaWAN network.
     *
     * @param sec A uint8_t value representing the desired delay in seconds
     *            for the second receive window.
     *
     * @return True if the command to set the RX2 Delay was successfully sent;
     *         false if an error occurred during the command execution.
     */
    bool setRX2Delay(uint8_t sec);

    /**
     * @brief Configures the Receive Window 2 Data Rate (RX2DR) for the RAK3172 LoRaWAN module.
     *
     * This command allows users to access and configure the data rate for
     * the second receive window. The data rate is applicable when the device
     * is listening for downlink messages on the second receive window.
     *
     * Valid data rates depend on the regional parameters:
     * - For EU433 / RU864 / IN865 / EU868 / CN470 / KR920: Data rates range from 0 to 5 (DR0-DR5).
     * - For AS923: Data rates range from 2 to 5 (DR2-DR5).
     * - For US915 / AU915 / LA915: Data rates range from 8 to 13 (DR8-DR13).
     *
     * @note The Receive Window 2 Data Rate determines how data is transmitted
     *       during downlink communications. Adjusting this parameter can affect
     *       the communication range and reliability based on environmental factors.
     *       Ensure that the data rate selected is compatible with your network's
     *       configuration and requirements.
     *
     * @param dr A uint8_t value representing the desired data rate for the
     *           second receive window.
     *
     * @return True if the command to set the RX2 Data Rate was successfully sent;
     *         false if an error occurred during the command execution.
     */
    bool setRX2DR(uint8_t dr);

    /**
     * @brief Configures the confirmation method for uplink messages in the RAK3172 LoRaWAN module.
     *
     * This command allows users to access and configure the acknowledgment
     * type for the device's payload. When enabled, the device will expect
     * a confirmation (ACK) from the network after sending an uplink message.
     *
     * @note Enabling the confirmation method ensures that the device receives
     *       feedback from the server regarding the successful delivery of
     *       the uplink messages. However, it may introduce some latency
     *       in communication as the device waits for the acknowledgment.
     *       Consider the requirements of your application when configuring this setting.
     *
     * @param confirm A boolean value indicating whether to enable (true) or
     *                disable (false) acknowledgment for uplink messages.
     *
     * @return True if the command to set the confirmation method was successfully sent;
     *         false if an error occurred during the command execution.
     */
    bool setComfirm(bool comfirm);

    /**
     * @brief Configures the LoRaWAN® node operating mode for the RAK3172 module.
     *
     * This command allows users to access and configure the operational
     * class of the LoRaWAN® node. The device can operate in one of three classes:
     * - Class A: Bi-directional end-devices that allow for both uplink and downlink
     *   communications but only listen for downlink messages after an uplink
     *   transmission.
     * - Class B: Similar to Class A, but with scheduled receive slots for downlink
     *   messages, allowing for more predictable communication patterns.
     * - Class C: Continuously listening for downlink messages except when transmitting
     *   uplink messages, which may lead to higher power consumption.
     *
     * @note Selecting the appropriate operating mode is crucial for optimizing
     *       power consumption and communication efficiency based on application
     *       requirements. Class A is suitable for battery-powered devices,
     *       while Class C is useful for applications needing low-latency downlink
     *       communication at the cost of increased power usage.
     *
     * @param mode The desired LoRaWAN® operating mode, specified as a value
     *             of type `lorawan_dev_class_t`.
     *
     * @return True if the command to set the operating mode was successfully sent;
     *         false if an invalid mode was specified or an error occurred during
     *         the command execution.
     */
    bool setMode(lorawan_dev_class_t mode);

    /**
     * @brief Sends a join request to the LoRa® network for OTAA (Over-The-Air Activation).
     *
     * This command initiates a request to join the LoRa® network. It allows users
     * to enable or disable the joining process, configure automatic joining on boot,
     * set a retry interval for connection attempts, and specify the number of retry
     * attempts.
     *
     * The command format is: AT+JOIN=Param1:Param2:Param3:Param4
     * - Param1 (Join Command):
     *   - 1: Initiate joining the network
     *   - 0: Stop joining
     *
     * - Param2 (Automatic Join Configuration):
     *   - 1: Enable automatic joining on boot
     *   - 0: Disable automatic joining (default is 0)
     *
     * - Param3 (Retry Interval):
     *   - Specifies the time interval between retry attempts in seconds;
     *     valid range is 7 to 255 seconds (default is 8 seconds).
     *
     * - Param4 (Join Retry Count):
     *   - Specifies the maximum number of join retry attempts;
     *     valid range is 0 to 255 (default is 0).
     *
     * @note Ensure that the device is configured correctly for OTAA mode before
     *       invoking this command. Proper configurations for network credentials
     *       are necessary for a successful join operation.
     *
     * @param enable A boolean value indicating whether to initiate or stop joining.
     * @param boot_auto_join A boolean value indicating whether to automatically
     *                       join on boot.
     * @param retry_interval A uint8_t value representing the time interval
     *                       between retry attempts (7-255 seconds).
     * @param retry_times A uint8_t value representing the maximum number of
     *                    retry attempts (0-255).
     *
     * @return True if the command to send the join request was successfully sent;
     *         false if the device is not in OTAA mode or an error occurred
     *         during command execution.
     */
    bool join(bool enable = true, bool boot_auto_join = false, uint8_t retry_interval = 10, uint8_t retry_times = 8);

    /**
     * @brief Sets the retransmission count for confirmed packets.
     *
     * This function sends a command to the LoRaWAN module to set the
     * retransmission count (RETY) for confirmed data packets. The
     * retransmission count determines how many times a packet will be
     * resent if it is not acknowledged by the recipient, enhancing the
     * reliability of communication.
     *
     * The valid range for the retransmission count is from 0 to 7, with
     * a default value of 0, indicating that no retransmissions will occur
     * if a packet is not acknowledged.
     *
     * The function constructs the command `AT+RETY=<num>`, where `<num>`
     * is the desired number of retransmissions, and sends it to the
     * LoRaWAN module. It returns a boolean indicating whether the command
     * was successfully sent and processed by the module.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to set the retransmission count.
     *
     * @param num The desired number of retransmissions (0-7).
     * @return `true` if the command was successfully sent and acknowledged;
     *         `false` otherwise.
     */
    bool setRetransmission(uint8_t num);

    /**
     * @brief Sends data over a specified port using the RAK3172 LoRaWAN module.
     *
     * This command allows users to send payload data to the LoRa® network
     * over a dedicated port. The payload is encoded in hexadecimal format before
     * transmission.
     *
     * The command format is: AT+SEND=<port>:<payload>
     * - <port>: A decimal integer representing the designated port number,
     *            valid range is 1 to 233.
     * - <payload>: A string containing the data to be sent, which must:
     *               - Be between 2 and 500 characters in length.
     *               - Have an even number of digits.
     *               - Contain only valid hexadecimal characters (0-9, a-f, A-F),
     *                 representing 1 to 256 hexadecimal values.
     *
     * @note Ensure that the data being sent meets the specified criteria for
     *       length and character validity to avoid transmission errors.
     *       Additionally, consider the application requirements when selecting
     *       the appropriate port for communication.
     *
     * @param data The payload string to be sent, which will be converted to
     *             hexadecimal format.
     * @param port An integer indicating the port number on which to send the data.
     *
     * @return The length of the original data if the command was successfully sent;
     *         0 if there was an error during the command execution.
     */
    size_t send(String data, int port = 1);

    /**
     * @brief Sends binary data over a specified port using the RAK3172 LoRaWAN module.
     *
     * This command allows users to send raw byte data to the LoRa® network
     * over a dedicated port. The binary data is converted into a hexadecimal
     * representation before transmission.
     *
     * The command format is: AT+SEND=<port>:<payload>
     * - <port>: A decimal integer representing the designated port number,
     *            valid range is 1 to 233.
     * - <payload>: A string containing the hex-encoded bytes to be sent,
     *               where each byte is represented by two hexadecimal digits.
     *
     * @note Ensure that the binary data being sent is properly formatted,
     *       as it will be encoded into hexadecimal. This function is useful
     *       for sending non-text data, such as sensor readings or configuration
     *       settings that are represented in binary form.
     *
     * @param buf A pointer to the binary data (byte array) to be sent.
     * @param size The size of the binary data in bytes.
     * @param port An integer indicating the port number on which to send the data.
     *
     * @return The size of the original data if the command was successfully sent;
     *         0 if there was an error during the command execution.
     */
    size_t send(const uint8_t* buf, size_t size, int port = 1);

    /**
     * @brief Parses a received LoRaWAN frame and extracts relevant information.
     *
     * This function processes a received LoRaWAN frame string that contains information
     * about a received message, including metrics such as RSSI, SNR, and whether
     * the message is unicast. It extracts these values and stores them in a
     * structured format for further processing.
     *
     * The expected frame format is: +EVT:RX_<type>:<rssi>:<snr>:<type>:<port>:<payload>
     * - <type>: Typically indicates the type of message (e.g., UNICAST).
     * - <rssi>: Received Signal Strength Indicator, an integer value.
     * - <snr>: Signal-to-Noise Ratio, an integer value.
     * - <port>: A port number associated with the message, an integer.
     * - <payload>: The actual data sent, which should be decoded from its
     *              encoded form.
     *
     * @note This function assumes that the input frame adheres to the expected
     *       format. If the frame does not conform to the specified structure,
     *       the parsing may result in undefined behavior. Ensure that the frame
     *       is properly formatted before calling this function.
     *
     * @param frame A String containing the received frame data to be parsed.
     */
    void parse(String frame);

    /**
     * @brief Updates and processes incoming data from the LoRaWAN serial interface.
     *
     * This function is designed to periodically check for new data from the LoRaWAN
     * module and handle various events based on the received response. It reads data
     * from the serial interface, processes specific event messages (such as join status,
     * transmission status, and received messages), and invokes corresponding callback
     * functions if they are set.
     *
     * The function reads a line of data terminated by a newline character and checks
     * for specific event keywords. It processes the following events:
     * - **+EVT:JOINED**: Indicates that the device has successfully joined the LoRaWAN network.
     * - **+EVT:JOIN_FAILED**: Indicates that the device failed to join the network.
     * - **+EVT:TX_DONE**: Indicates that a transmission has been completed.
     * - **+EVT:RX_**: Indicates that a received message is available, which is then parsed.
     *
     * If a corresponding event is found, the appropriate callback function (if set) is invoked:
     * - `_onJoin`: Called with `true` if the device successfully joined the network, `false` if it failed.
     * - `_onSend`: Placeholder for any send completion handler (currently commented out).
     * - `parse`: Processes the received frame (if a "RX_" event is detected).
     *
     * @note This function requires a mutex (_serial_mutex) to safely access the serial
     *       interface and ensure that other tasks or processes do not interfere during
     *       the data read operation. This is particularly important in multi-threaded
     *       or multi-core environments (e.g., FreeRTOS).
     *
     * @note The event handling sections (such as +EVT:TX_DONE) are placeholders
     *       for future implementations, where actions may be taken upon completion
     *       of a transmission, or additional event types may be added.
     */
    void update();

    /**
     * @brief Registers a callback function to handle received LoRaWAN frames.
     *
     * This function allows users to register a callback function that will be invoked
     * whenever a new LoRaWAN frame is received. The callback function should accept a
     * `lorawan_frame_t` structure, which contains details about the received frame,
     * such as RSSI, SNR, port, and the payload data.
     *
     * The registered callback is stored and will be called whenever a frame is received
     * and successfully parsed by the system (typically during the processing of an
     * event like `+EVT:RX_`).
     *
     * @note The callback function should be designed to handle the incoming
     *       `lorawan_frame_t` structure, which contains information about the
     *       received frame, including the payload, signal quality, and other metadata.
     *       Make sure the callback function performs necessary actions, such as
     *       data processing or triggering other application logic, based on the
     *       frame content.
     *
     * @param callback A pointer to the callback function that will be invoked when
     *                 a frame is received. The callback should have the following signature:
     *                 `void callback(lorawan_frame_t frame);`
     * @return `true` if the callback function was successfully registered,
     *         always returns `true` as the callback registration does not fail.
     */
    bool onReceive(void (*callback)(lorawan_frame_t));

    /**
     * @brief Registers a callback function to handle transmission completion events.
     *
     * This function allows users to register a callback function that will be invoked
     * whenever a transmission to the LoRaWAN network is completed. This is useful for
     * notifying the application that the data has been sent successfully or to trigger
     * subsequent actions based on the transmission status.
     *
     * The registered callback is stored and will be called after a successful
     * transmission event, typically indicated by an event like `+EVT:TX_DONE`.
     *
     * @note The callback function should be designed to perform any necessary actions
     *       after a transmission is completed, such as updating the application state,
     *       logging the event, or preparing for the next transmission.
     *       Currently, the implementation does not check for the validity of the callback.
     *
     * @param callback A pointer to the callback function that will be invoked upon
     *                 successful transmission. The callback should have the following signature:
     *                 `void callback();`
     * @return `true` if the callback function was successfully registered,
     *         always returns `true` as the callback registration does not fail.
     */
    bool onSend(void (*callback)());

    /**
     * @brief Registers a callback function to handle join status events.
     *
     * This function allows users to register a callback function that will be invoked
     * whenever the device successfully joins or fails to join the LoRaWAN network.
     * This is useful for notifying the application of the join status and taking
     * appropriate actions based on the result.
     *
     * The registered callback is stored and will be called with a boolean parameter
     * indicating the success of the join operation:
     * - `true`: The device has successfully joined the network.
     * - `false`: The device failed to join the network.
     *
     * @note The callback function should be designed to handle the join status
     *       appropriately, such as updating the application state, logging the event,
     *       or initiating further actions based on whether the join was successful or not.
     *
     * @param callback A pointer to the callback function that will be invoked upon
     *                 join status change. The callback should have the following signature:
     *                 `void callback(bool success);`
     * @return `true` if the callback function was successfully registered,
     *         always returns `true` as the callback registration does not fail.
     */
    bool onJoin(void (*callback)(bool));

    /**
     * @brief Registers a callback function to handle error events.
     *
     * This function allows users to register a callback function that will be invoked
     * whenever an error occurs in the LoRaWAN communication process. This is useful for
     * notifying the application of errors and taking appropriate actions based on the
     * error type or message.
     *
     * The registered callback is stored and will be called with a character pointer
     * that points to a string describing the error.
     *
     * @note The callback function should be designed to handle the error message
     *       appropriately, such as logging the error, notifying the user, or
     *       attempting to recover from the error condition.
     *
     * @param callback A pointer to the callback function that will be invoked upon
     *                 an error event. The callback should have the following signature:
     *                 `void callback(char *errorMessage);`
     * @return `true` if the callback function was successfully registered,
     *         always returns `true` as the callback registration does not fail.
     */
    bool onError(void (*callback)(char*));

    /**
     * @brief Returns the number of available LoRaWAN frames in the buffer.
     *
     * This function checks how many frames are currently stored in the internal
     * buffer (e.g., `_frames`), indicating the number of received LoRaWAN frames
     * that are available for processing. This is useful for determining if there
     * are frames to be processed or if the application can safely attempt to read
     * the next frame.
     *
     * The function returns the size of the internal buffer, which contains the
     * received frames that have not yet been processed or consumed.
     *
     * @return The number of available frames in the internal buffer.
     */
    int available();

    /**
     * @brief Reads and returns the available LoRaWAN frames from the buffer.
     *
     * This function retrieves all the currently stored LoRaWAN frames from the
     * internal buffer (e.g., `_frames`) and returns them as a vector. This is
     * useful for processing multiple frames at once, allowing the application
     * to handle all received frames that have not yet been processed.
     *
     * The returned vector contains `lorawan_frame_t` structures, each representing
     * a received frame with its associated data (e.g., payload, metadata).
     *
     * @note After calling this function, the application may need to clear or
     *       process the frames to avoid re-reading the same data in subsequent
     *       calls. The function does not modify the internal buffer; it simply
     *       returns a copy of the frames.
     *
     * @return A vector of `lorawan_frame_t` containing all available frames
     *         in the internal buffer.
     */
    std::vector<lorawan_frame_t> read();

    /**
     * @brief Clears the internal frame buffer and flushes the serial output.
     *
     * This function performs two main actions:
     * - It calls the `flush()` method on the serial interface, ensuring that
     *   any data waiting to be transmitted through the serial interface is
     *   fully transmitted (i.e., it waits for the transmission buffer to clear).
     * - It clears the internal frame buffer (e.g., `_frames`), removing any
     *   stored LoRaWAN frames that have not yet been processed.
     *
     * This is useful in scenarios where you want to reset the state of the
     * LoRaWAN communication, ensuring that no old frames are left in the buffer
     * and that any pending serial transmission is completed.
     *
     * @note After calling this function, any frames that were in the buffer
     *       will be lost, so ensure that frames are processed or saved
     *       before calling `flush()`.
     * @note The serial flush only applies if the `_serial` object is properly
     *       initialized and represents an active serial interface.
     */
    void flush();

    /**
     * @brief Retrieves the global application identifier (AppEUI).
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * global application identifier (AppEUI). The AppEUI is a unique identifier
     * used to identify the application in the LoRaWAN network.
     *
     * The function constructs and sends the command `AT+APPEUI=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current application identifier configured in the device.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the identifier.
     *
     * @return A `String` containing the global application identifier (AppEUI)
     *         returned by the LoRaWAN module.
     */
    String getApplicationIdentifier();

    /**
     * @brief Retrieves the application key (AppKey).
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * application key (AppKey). The AppKey is a unique key used for
     * securing communication between the device and the LoRaWAN network.
     *
     * The function constructs and sends the command `AT+APPKEY=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current application key configured in the device.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the key.
     *
     * @return A `String` containing the application key (AppKey)
     *         returned by the LoRaWAN module.
     */
    String getApplicationKey();

    /**
     * @brief Retrieves the application session key (AppSKey).
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * application session key (AppSKey). The AppSKey is a key used for
     * encrypting and decrypting the application payload in LoRaWAN communication.
     *
     * The function constructs and sends the command `AT+APPSKEY?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current application session key configured in the device.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the key.
     *
     * @return A `String` containing the application session key (AppSKey)
     *         returned by the LoRaWAN module.
     */
    String getApplicationSessionKey();

    /**
     * @brief Retrieves the network session key (NwkSKey).
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * network session key (NwkSKey). The NwkSKey is a key used for securing
     * the network layer of communication in LoRaWAN, ensuring the integrity
     * and authenticity of the data between the device and the network server.
     *
     * The function constructs and sends the command `AT+NWKSKEY=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current network session key configured in the device.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the key.
     *
     * @return A `String` containing the network session key (NwkSKey)
     *         returned by the LoRaWAN module.
     */
    String getNetworkSessionKey();

    /**
     * @brief Retrieves the network identifier (NetID).
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * network identifier (NetID). The NetID is a unique identifier for the
     * LoRaWAN network to which the device is connected, allowing for
     * differentiation between multiple networks.
     *
     * The function constructs and sends the command `AT+NETID=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current network identifier configured in the device.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the identifier.
     *
     * @return A `String` containing the network identifier (NetID)
     *         returned by the LoRaWAN module.
     */
    String getNetworkId();

    /**
     * @brief Retrieves the device address (DevAddr).
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * device address (DevAddr). The DevAddr is a unique identifier assigned
     * to the device within the LoRaWAN network, allowing the network server
     * to identify and communicate with the device.
     *
     * The function constructs and sends the command `AT+DEVADDR=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current device address configured in the device.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the address.
     *
     * @return A `String` containing the device address (DevAddr)
     *         returned by the LoRaWAN module.
     */
    String getDevAddr();

    /*
     * @brief Retrieves the global device identifier (DevEUI).
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * global unique identifier for the device (DevEUI). The DevEUI is a
     * unique identifier assigned to each device, which is used to identify
     * the device globally in the LoRaWAN network.
     *
     * The function constructs and sends the command `AT+DEVEUI=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current device identifier (DevEUI) configured in the device.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the identifier.
     *
     * @return A `String` containing the global device identifier (DevEUI)
     *         returned by the LoRaWAN module.
     */
    String getDevEUI();

    /**
     * @brief Retrieves the Adaptive Data Rate (ADR) status.
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * current status of the Adaptive Data Rate (ADR) feature. ADR is a
     * mechanism used in LoRaWAN networks to adjust the data rate and transmission
     * power of a device based on the network conditions, optimizing power consumption
     * and network capacity.
     *
     * The function constructs and sends the command `AT+ADR=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current ADR setting configured on the device.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the ADR status.
     *
     * @return A `String` containing the ADR status returned by the LoRaWAN module.
     *         The response typically indicates whether ADR is enabled or disabled.
     */
    String getADR();

    /**
     * @brief Retrieves the Duty Cycle Switch (DCS) status.
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * current status of the Duty Cycle Switch (DCS). The DCS is a feature
     * that controls the duty cycle of the device's transmissions, helping
     * to manage the amount of time the device can transmit over a given
     * period. This is important for compliance with regulatory requirements
     * and to minimize interference in the network.
     *
     * The function constructs and sends the command `AT+DCS=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current DCS setting configured on the device.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the DCS status.
     *
     * @return A `String` containing the DCS status returned by the LoRaWAN module.
     *         The response typically indicates whether the duty cycle switch is
     *         enabled or disabled.
     */
    String getDCS();

    /**
     * @brief Retrieves the duty cycle time.
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * duty cycle time configured for the device. The duty cycle time determines
     * the amount of time the device is allowed to transmit within a specific
     * period, in compliance with regional regulatory requirements.
     *
     * The command `AT+DUTYTIME=?` is used to query the duty cycle time, which is
     * applicable to devices operating in the EU868, RU864, and EU433 frequency bands.
     * The returned value is given in seconds, representing the allowed transmission
     * time over a given period.
     *
     * If the device is operating in a frequency band other than EU868, RU864, or EU433,
     * the command will return `0`, indicating that the duty cycle time is not applicable
     * for that band.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the duty cycle time.
     *
     * @return A `String` containing the duty cycle time (in seconds) for the device,
     *         or `0` if the frequency band does not support duty cycle time configuration.
     */
    String getDutyTime();

    /**
     * @brief Retrieves the current data rate (DR) setting.
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * current data rate (DR) configured for the device. The data rate
     * determines the speed at which data is transmitted over the LoRaWAN
     * network, impacting both the range and the power consumption of the
     * device.
     *
     * The function constructs and sends the command `AT+DR?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current data rate setting configured on the device.
     *
     * The valid data rate values depend on the frequency band:
     * - EU433 / RU864 / IN865 / EU868 / CN470 / KR920: DR values range from 0 to 5 (DR0-DR5).
     * - AS923: DR values range from 2 to 5 (DR2-DR5).
     * - US915: DR values range from 0 to 4 (DR0-DR4).
     * - AU915: DR values range from 2 to 6 (DR2-DR6).
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the data rate.
     *
     * @return A `String` containing the current data rate (DR) returned by
     *         the LoRaWAN module. The response typically indicates the
     *         data rate value being used for transmissions.
     */
    String getDR();

    /**
     * @brief Retrieves the RX1 window join delay.
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * RX1 window join delay (JN1DL), which is the delay between the end of
     * the transmission of a join request and the start of the reception of
     * the join accept message in the RX1 window. This delay is used during
     * the process of network joining, ensuring proper synchronization between
     * the device and the network.
     *
     * The function constructs and sends the command `AT+JN1DL=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current RX1 window join delay setting on the device.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the RX1 window join delay.
     *
     * @return A `String` containing the RX1 join delay returned by the
     *         LoRaWAN module. The response represents the delay time in
     *         milliseconds.
     */
    String getJoinRX1Delay();

    /**
     * @brief Retrieves the RX2 window join delay.
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * RX2 window join delay (JN2DL), which is the delay between the end of
     * the transmission of a join request and the start of the reception of
     * the join accept message in the RX2 window. The RX2 window is the second
     * receive window used during the join procedure, following RX1. This delay
     * ensures proper synchronization and timing for receiving the join accept
     * message.
     *
     * The function constructs and sends the command `AT+JN2DL=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current RX2 window join delay setting on the device.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the RX2 window join delay.
     *
     * @return A `String` containing the RX2 join delay returned by the
     *         LoRaWAN module. The response represents the delay time in
     *         milliseconds.
     */
    String getJoinRX2Delay();

    /**
     * @brief Retrieves the RX1 window delay.
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * RX1 window delay (RX1DL), which is the delay between the end of
     * the transmission of a message and the start of the reception of
     * that message in the RX1 window. This delay is crucial for ensuring
     * proper timing and synchronization when receiving messages from the
     * network.
     *
     * The function constructs and sends the command `AT+RX1DL=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current RX1 window delay setting on the device.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the RX1 window delay.
     *
     * @return A `String` containing the RX1 window delay returned by the
     *         LoRaWAN module. The response represents the delay time in
     *         milliseconds.
     */
    String getRX1Delay();

    /**
     * @brief Retrieves the RX2 window delay.
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * RX2 window delay (RX2DL), which is the delay between the end of
     * the transmission of a message and the start of the reception of
     * that message in the RX2 window. This delay is important for ensuring
     * proper timing and synchronization when receiving messages from the
     * network, particularly in scenarios where the RX1 window may not be
     * utilized effectively or if the device needs to listen on RX2.
     *
     * The function constructs and sends the command `AT+RX2DL=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current RX2 window delay setting on the device.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the RX2 window delay.
     *
     * @return A `String` containing the RX2 window delay returned by the
     *         LoRaWAN module. The response represents the delay time in
     *         milliseconds.
     */
    String getRX2Delay();

    /**
     * @brief Retrieves the RX2 window data rate.
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * data rate (DR) setting for the RX2 window (RX2DR). The RX2 window
     * is used for receiving messages from the network, and the data rate
     * determines the speed at which data is transmitted. The appropriate
     * data rate can affect the range and reliability of the communication.
     *
     * The function constructs and sends the command `AT+RX2DR=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current RX2 window data rate setting on the device.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the RX2 window data rate.
     *
     * @return A `String` containing the RX2 data rate returned by the
     *         LoRaWAN module. The response indicates the data rate value
     *         that can be used for transmissions in the RX2 window.
     */
    String getRX2DR();

    /**
     * @brief Retrieves the RX2 window frequency.
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * frequency (FQ) setting for the RX2 window (RX2FQ). The RX2 window is
     * the second receive window used by LoRaWAN devices to receive messages
     * from the network. The frequency setting determines the operating
     * frequency of the RX2 window, which is crucial for ensuring proper
     * communication with the network.
     *
     * The function constructs and sends the command `AT+RX2FQ=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current RX2 window frequency setting on the device.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the RX2 window frequency.
     *
     * @return A `String` containing the RX2 frequency returned by the
     *         LoRaWAN module. The response represents the frequency value
     *         used for receiving messages in the RX2 window.
     */
    String getRX2Freq();

    /**
     * @brief Retrieves the transmission output power.
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * current transmission power setting (TXP). The transmission power
     * determines how strong the signal is when the device transmits data,
     * which can affect range and communication reliability.
     *
     * The function constructs and sends the command `AT+TXP=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current output power setting on the device.
     *
     * The range of valid transmission power values depends on the region:
     * - EU868 / RU864 / KR920 / AS923 / CN470: 0 to 7
     * - US915 / AU915: 0 to 14
     * - EU433: 0 to 5
     * - IN865: 0 to 10
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the transmission power setting.
     *
     * @return A `String` containing the transmission output power returned by
     *         the LoRaWAN module. The response indicates the current power
     *         level used for transmissions.
     */
    String getOutPower();

    /**
     * @brief Retrieves the retransmission count for confirmed packets.
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * current retransmission setting (RETY) for confirmed data packets.
     * The retransmission count determines how many times a packet will be
     * resent if it is not acknowledged by the recipient, which can enhance
     * reliability in communication.
     *
     * The function constructs and sends the command `AT+RETY=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current retransmission count setting on the device.
     *
     * The valid range for the retransmission count is from 0 to 7, with
     * a default value of 0, indicating that no retransmissions will occur
     * if a packet is not acknowledged.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the retransmission setting.
     *
     * @return A `String` containing the retransmission count returned by the
     *         LoRaWAN module. The response indicates how many times the device
     *         will attempt to resend a confirmed packet.
     */
    String getRetransmission();

    /**
     * @brief Retrieves the channel mask configuration.
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * current channel mask setting (MASK). The channel mask determines which
     * channels are enabled or disabled for communication in specific regions.
     * It is applicable only for US915, AU915, LA915, and CN470 frequency plans.
     *
     * The function constructs and sends the command `AT+MASK=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the current channel mask configuration on the device.
     *
     * Default values for the channel mask based on the region are:
     * - US915 / AU915 / LA915: 01FF
     * - CN470: 0FFF
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the channel mask setting.
     *
     * @return A `String` containing the channel mask returned by the
     *         LoRaWAN module. The response indicates which channels are
     *         currently enabled for transmission.
     */
    String getChannelMask();

    /**
     * @brief Retrieves the band number corresponding to the region.
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * band number (BAND) that corresponds to the current regional frequency plan.
     * The band number helps identify the specific frequency plan in use, which
     * determines the frequency range and channel configurations for communication.
     *
     * The function constructs and sends the command `AT+BAND=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the band number assigned to the device's region.
     *
     * The region-specific band numbers are as follows:
     * - 0: EU433
     * - 1: CN470
     * - 2: RU864
     * - 3: IN865
     * - 4: EU868
     * - 5: US915
     * - 6: AU915
     * - 7: KR920
     * - 8: AS923-1
     * - 9: AS923-2
     * - 10: AS923-3
     * - 11: AS923-4
     * - 12: LA915
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the band number.
     *
     * @return A `String` containing the band number returned by the
     *         LoRaWAN module. The response indicates the region-specific
     *         frequency plan being used.
     */
    String getBAND();

    /**
     * @brief Retrieves the link check configuration for the device.
     *
     * This function sends a command to the LoRaWAN module to access the
     * current network link check setting (LINKCHECK). The link check
     * determines how the device monitors its connection to the network,
     * which can be important for maintaining reliable communication.
     *
     * The function constructs and sends the command `AT+LINKCHECK=?` to
     * the module, and returns the response as a `String`. This allows users
     * to access the current link check configuration on the device.
     *
     * The possible settings for the link check are:
     * - 0: Link check disabled
     * - 1: Perform a link check on the next valid uplink payload
     * - 2: The module will automatically perform a link check after every
     *      data upload
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the link check setting.
     *
     * @return A `String` containing the current link check configuration
     *         returned by the LoRaWAN module. The response indicates how
     *         link checking is being handled by the device.
     */
    String getLinkCheck();

    /**
     * @brief Retrieves the multicast group configuration.
     *
     * This function sends a command to the LoRaWAN module to retrieve the
     * current multicast group settings. Multicast allows a device to send
     * messages to a group of devices rather than a single one, improving
     * communication efficiency in scenarios where multiple devices need to
     * receive the same data.
     *
     * The function constructs and sends the command `AT+LSTMULC=?` to the
     * module, and returns the response as a `String`. This allows users to
     * access the currently configured multicast group information on the device.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the multicast group information.
     *
     * @return A `String` containing the multicast group configuration returned
     *         by the LoRaWAN module. The response provides details about the
     *         multicast groups that are currently configured.
     */
    String getLstMulc();

    /**
     * @brief Retrieves the current activation state of the device.
     *
     * This function sends a command to the LoRaWAN module to access the
     * current network state (NJS) of the device. This allows users to
     * determine whether the device has successfully joined the LoRaWAN
     * network and is currently active.
     *
     * The function constructs and sends the command `AT+NJS=?` to the
     * module, and returns the response as a `String`. This provides users
     * with information about the device's connection status to the network.
     *
     * @note Ensure that the LoRaWAN module is properly initialized and connected
     *       before calling this function, as it relies on communication with
     *       the module to retrieve the network state.
     *
     * @return A `String` containing the current network state returned by
     *         the LoRaWAN module. The response indicates whether the device
     *         is active and has joined the LoRaWAN network.
     */
    String getNetworkState();

private:
    /**
     * @brief A vector holding received LoRaWAN frames.
     */
    std::vector<lorawan_frame_t> _frames;

    /**
     * @brief Current device class mode.
     */
    lorawan_dev_class_t _class_mode;

    /**
     * @brief Current join mode (OTAA or ABP).
     */
    lorawan_join_mode_t _join_mode;

    /**
     * @brief Indicates if the device has successfully joined the network.
     */
    bool _is_joined;

    /**
     * @brief Indicates if data confirmation is enabled.
     */
    bool _data_comfirm;

    /**
     * @brief Callback function invoked when a frame is received.
     *
     * @param frame The received LoRaWAN frame.
     */
    void (*_onReceive)(lorawan_frame_t);

    /**
     * @brief Callback function invoked when a frame is sent.
     */
    void (*_onSend)();

    /**
     * @brief Callback function invoked when the device joins the network.
     *
     * @param success Indicates whether the join was successful.
     */
    void (*_onJoin)(bool);

    /**
     * @brief Callback function invoked on error occurrence.
     *
     * @param errorMsg Pointer to the error message string.
     */
    void (*_onError)(char*);
};

#endif
