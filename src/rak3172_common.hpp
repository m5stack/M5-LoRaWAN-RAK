/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef _RAK3172_COMMON_HPP_
#define _RAK3172_COMMON_HPP_

#include <Arduino.h>

// #define RAK3172_DEBUG Serial  // This macro definition can be annotated without sending and receiving data prints
//          Define the serial port you want to use, e.g., Serial1 or Serial2
#if defined RAK3172_DEBUG
#define serialPrint(...)   RAK3172_DEBUG.print(__VA_ARGS__)
#define serialPrintln(...) RAK3172_DEBUG.println(__VA_ARGS__)
#define serialPrintf(...)  RAK3172_DEBUG.printf(__VA_ARGS__)
#define serialFlush()      RAK3172_DEBUG.flush()
#else
#endif

typedef enum {
    RAK3172_BPS_115200 = 0, /**< Baud rate of 115200 bps */
    RAK3172_BPS_9600,       /**< Baud rate of 9600 bps */
    RAK3172_BPS_4800,       /**< Baud rate of 4800 bps */
} rak3172_bps_t;

typedef enum {
    RAK3172_SLEEP_ONE = 1, /**< Low power mode level 1 */
    RAK3172_SLEEP_TWO,     /**< Low power mode level 2 */
} rak3172_sleep_t;

/**
 * @brief Encodes a given string into its hexadecimal representation.
 *
 * This function takes a string, converts each character to its hexadecimal
 * equivalent, and returns a new string containing the encoded result.
 *
 * @note
 * - The input string should not be empty.
 * - The function allocates a temporary buffer for the encoded string, which may
 *   lead to stack overflow if the input string is too long.
 * - Ensure that the resulting string is properly managed to avoid memory leaks.
 *
 * @param str The input string to be encoded.
 * @return A String object containing the hexadecimal representation of the input string.
 *
 */
String encodeMsg(String str);

/**
 * @brief Decodes a hexadecimal encoded string back to its original representation.
 *
 * This function takes a hexadecimal encoded string and converts it back to
 * the original string. If the input string's length is not even, it returns
 * the input string unchanged.
 *
 * @note
 * - The input string should be a valid hexadecimal representation (even-length).
 * - If the input string has an odd length, it will be returned as is without
 *   any decoding.
 * - Ensure that the resulting string is properly managed to avoid memory leaks.
 *
 * @param hexEncoded The hexadecimal encoded string to be decoded.
 * @return A String object containing the original representation of the input
 *         hexadecimal string, or the input string if its length is odd.
 */
String decodeMsg(String hexEncoded);

/**
 * @brief Converts a byte array to its hexadecimal string representation.
 *
 * This function takes a pointer to a byte array and its size, converting
 * each byte to a two-character hexadecimal string and concatenating them
 * into a single string.
 *
 * @note
 * - The resulting string will not have any delimiters between the hex values.
 * - Ensure that the input buffer is valid and the size is greater than zero.
 *
 * @param buf Pointer to the byte array to be converted.
 * @param size The number of bytes in the array.
 * @return A String object containing the hexadecimal representation of the byte array.
 */
String bytes2hex(const uint8_t* buf, size_t size);

/**
 * @brief Converts a hexadecimal string to a long integer.
 *
 * This function takes a hexadecimal string, validates that its length is even,
 * and converts the string to a long integer. If the string length is odd,
 * the function returns false to indicate an error.
 *
 * @note
 * - The input string should be a valid hexadecimal string with an even number of characters.
 * - If the string length is odd, the function will return false as an error code.
 * - The function assumes the hexadecimal string represents a value that fits within a `long` type.
 *
 * @param hex The hexadecimal string to be converted.
 * @return A `long` integer representation of the hexadecimal string, or false if the string length is odd.
 */
long hex2bin(String hex);

/**
 * @brief Converts a hexadecimal encoded string to a byte array.
 *
 * This function takes a hexadecimal encoded string and converts it into a
 * byte array. The function assumes that the provided buffer is large enough
 * to hold the resulting bytes. If the input string's length is odd, the
 * function does nothing.
 *
 * @note
 * - The input string should be a valid hexadecimal representation with an even length.
 * - Ensure that the provided buffer is allocated with sufficient size to hold the output.
 *
 * @param hexEncoded The hexadecimal encoded string to be converted.
 * @param buf Pointer to the byte array where the result will be stored.
 * @param size The size of the provided buffer.
 */
void hex2bytes(String hexEncoded, uint8_t* buf, size_t size);

/**
 * @brief Validates if the provided string matches a specified length and contains only hexadecimal characters.
 *
 * This function checks if the length of the input string matches the expected length
 * and verifies that all characters in the string are valid hexadecimal characters (0-9, A-F, a-f).
 *
 * @note This function can be used to ensure that strings representing identifiers,
 *       keys, or other data conform to the requirements of being exactly a certain length
 *       and consisting solely of valid hexadecimal digits. This is particularly useful in
 *       applications where such formats are required (e.g., cryptographic keys, hardware IDs).
 *
 * @param key The string to be validated.
 * @param len The expected length of the string.
 * @return true if the string matches the expected length and contains only hexadecimal characters;
 *         false otherwise.
 */
bool checkString(const String& key, size_t len);

class RAK3172 {
protected:
    HardwareSerial* _serial;
    int _tx_pin;
    int _rx_pin;
    SemaphoreHandle_t _serial_mutex;

public:
    /**
     * @brief Initializes the RAK3172 module with the specified serial communication parameters.
     *
     * This function sets up the serial communication (RX, TX pins and baud rate)
     * for the RAK3172 module and establishes a mutex for synchronizing serial
     * communication. The function also sends an "AT" command to verify communication
     * with the module.
     *
     * @note
     * - The function assumes that the serial interface is properly connected and
     *   that the RX and TX pins are correctly specified.
     * - The mutex is created and immediately released after initialization.
     * - The function sends an "AT" command to test the connectivity with the RAK3172 module.
     *
     * @param serial A pointer to the `HardwareSerial` object to be used for communication.
     * @param rx The RX pin number for serial communication.
     * @param tx The TX pin number for serial communication.
     * @param baudRate The baud rate to be used for serial communication. This can
     *        be one of the values defined in the `rak3172_bps_t` enum.
     * @return `true` if the initialization and communication test were successful,
     *         `false` otherwise.
     */
    bool init(HardwareSerial* serial = &Serial2, int rx = 16, int tx = 17, rak3172_bps_t baudRate = RAK3172_BPS_115200);

    /**
     * @brief Sends a command to the RAK3172 module and waits for a response.
     *
     * This function sends a specified command string to the RAK3172 module over
     * the serial interface and reads the response. It uses a mutex to ensure
     * thread-safe access to the serial communication. The function checks if the
     * response contains "OK" to determine if the command was successful.
     *
     * @note
     * - The function assumes that the serial interface has been properly initialized
     *   before calling this function.
     * - The mutex is acquired before sending the command and released after reading
     *   the response.
     *
     * @param cmd The command string to be sent to the RAK3172 module.
     * @return `true` if the command was sent successfully and the response contains
     *         "OK", `false` otherwise.
     */
    bool sendCommand(String cmd);

    /**
     * @brief Sets the baud rate for communication with the RAK3172 module.
     *
     * This function sends an AT command to the RAK3172 module to set the baud rate. If the baud rate change is
     * successful, it updates the serial connection's baud rate.
     *
     * @note The baud rate change will only be applied if the AT command is successfully sent and acknowledged by the
     * module.
     * @param baudRate The desired baud rate from the available options.
     *                 - `RAK3172_BPS_115200` for 115200 bps
     *                 - `RAK3172_BPS_9600` for 9600 bps
     *                 - `RAK3172_BPS_4800` for 4800 bps
     *
     * @return true if the baud rate was successfully set, false if the operation failed.
     */
    bool setBaudRate(rak3172_bps_t baudRate);

    /**
     * @brief Enables or disables low power mode (LPM) for the RAK3172 module.
     *
     * This function sends an AT command to the RAK3172 module to enable or disable low power mode.
     * When LPM is enabled, the device automatically enters sleep mode after sending the AT command,
     * eliminating the need to send the AT+SLEEP command separately.
     *
     * @note Enabling low power mode will help conserve battery life during periods of inactivity.
     *
     * @param enable A boolean value indicating whether to enable or disable low power mode.
     *               - `true` to enable low power mode.
     *               - `false` to disable low power mode.
     *
     * @return true if the command to set low power mode was successfully sent, false otherwise.
     */
    bool setLPM(bool enable);

    /**
     * @brief Sets the low power mode (LPM) level for the RAK3172 module.
     *
     * This function sends an AT command to configure the low power mode level of the RAK3172 module.
     * The available levels optimize power consumption differently.
     *
     * - Stop1 mode allows both UART1 and UART2 to wake the device from LPM.
     * - Stop2 mode is more optimized for low current consumption but does not allow UART1 to wake the device.
     *
     * @note Choosing the appropriate LPM level is crucial for balancing power consumption and wake-up capabilities.
     *
     * @param level The desired low power mode level to set.
     *              - `RAK3172_SLEEP_ONE` for low power mode level 1 (Stop1).
     *              - `RAK3172_SLEEP_TWO` for low power mode level 2 (Stop2).
     *
     * @return true if the command to set the LPM level was successfully sent, false otherwise.
     */
    bool setLPMLevel(rak3172_sleep_t level);

    /**
     * @brief Enables sleep mode for the RAK3172 module.
     *
     * This function sends an AT command to enable sleep mode.
     * If called with no parameters using `AT+SLEEP`, the module will enter continuous sleep mode without a timeout.
     * If a timeout is provided, the module will automatically wake up after the specified duration.
     *
     * @note Using sleep mode can effectively conserve battery power, making it suitable for scenarios with long periods
     * of inactivity.
     *
     * @param time_ms The duration for which the module should sleep (in milliseconds).
     *                - If set to 0, continuous sleep mode is enabled.
     *
     * @return true if the sleep command was successfully sent; false otherwise.
     */
    bool sleep(uint32_t time_ms);

    /**
     * @brief Sets an alias for the device.
     *
     * This function sends the command `AT+ALIAS=` followed by the provided alias
     * to the RAK3172 module, allowing the user to set a user-friendly name for the device.
     *
     * @note This command is useful for identifying the device with a user-friendly name,
     *       making it easier to manage multiple devices. Ensure the alias does not exceed
     *       16 characters.
     *
     * @param alias A string representing the desired alias for the device.
     *              The alias must be a string of up to 16 characters.
     * @return true if the command was successfully sent and acknowledged;
     *         false otherwise.
     */
    bool setDeviceAlias(String alias);

    /**
     * @brief Sends a command to the RAK3172 module and retrieves the response.
     *
     * This function takes a command string, sends it to the RAK3172 module via the serial interface,
     * and reads the response. It uses a mutex to ensure thread safety during the communication.
     *
     * @note The function looks for the first '=' character in the response to extract the relevant data.
     *       Debug information can be printed if the RAK3172_DEBUG flag is defined.
     *
     * @param cmd The command string to be sent to the RAK3172 module.
     *
     * @return A string containing the data extracted from the response.
     *         If the response does not contain valid data, an empty string is returned.
     */
    String getCommand(String cmd);

    /**
     * @brief Retrieves the firmware version of the RAK3172 module.
     *
     * This function sends the command `AT+VER=?` to the RAK3172 module
     * and returns the response, which contains the firmware version information.
     * @note This command is useful for checking the current firmware version
     *       of the RAK3172 module for compatibility and debugging purposes.
     * @return A string containing the firmware version.
     *         If the command fails or the response is invalid, an empty string may be returned.
     */
    String getVersion();

    /**
     * @brief Retrieves the equipment (serial) number of the RAK3172 module.
     *
     * This function sends the command `AT+SN=?` to the RAK3172 module
     * and returns the response, which contains the device's serial number.
     *
     * @note This command is useful for identifying the specific device
     *
     *       and for inventory or tracking purposes.
     * @return A string containing the equipment (serial) number.
     *         If the command fails or the response is invalid, an empty string may be returned.
     */
    String getEquipmentNumber();

    /**
     * @brief Retrieves the baud rate of the RAK3172 module.
     *
     * This function sends the command `AT+BAUD=?` to the RAK3172 module
     * and returns the response, which contains the current baud rate setting.
     *
     * @note This command is useful for checking the current baud rate configuration
     *       of the RAK3172 module for communication purposes.
     *
     * @return A string containing the baud rate.
     *         If the command fails or the response is invalid, an empty string may be returned.
     */
    String getBaudRate();

    /**
     * @brief Retrieves the battery voltage of the RAK3172 module.
     *
     * This function sends the command `AT+BAT=?` to the RAK3172 module
     * and returns the response, which contains the current battery voltage information.
     *
     * @note This command is useful for monitoring the battery status
     *       of the RAK3172 module to ensure proper operation.
     *
     * @return A string containing the battery voltage.
     *         If the command fails or the response is invalid, an empty string may be returned.
     */
    String getBAT();

    /**
     * @brief Retrieves the build date and time of the firmware for the RAK3172 module.
     *
     * This function sends the command `AT+BUILDTIME=?` to the RAK3172 module
     * and returns the response, which contains the firmware's build date and time information.
     *
     * @note This command is useful for verifying the firmware version and
     *       for debugging purposes to ensure the correct firmware is in use.
     *
     * @return A string containing the build date and time of the firmware.
     *         If the command fails or the response is invalid, an empty string may be returned.
     */
    String getBuildTime();

    /**
     * @brief Retrieves the repository information of the firmware for the RAK3172 module.
     *
     * This function sends the command `AT+REPOINFO=?` to the RAK3172 module
     * and returns the response, which contains information about the firmware's repository.
     *
     * @note This command is useful for identifying the source repository of the firmware,
     *       which can be helpful for debugging and tracking the firmware's origin.
     *
     * @return A string containing the repository information of the firmware.
     *         If the command fails or the response is invalid, an empty string may be returned.
     */
    String getRepository();

    /**
     * @brief Retrieves the AT command interface (CLI) version of the RAK3172 module.
     *
     * This function sends the command `AT+CLIVER=?` to the RAK3172 module
     * and returns the response, which contains the version information of the AT command interface (CLI).
     *
     * @note This command is useful for checking the version of the AT command interface
     *       to ensure compatibility with the module's firmware and for troubleshooting.
     *
     * @return A string containing the AT command interface (CLI) version.
     *         If the command fails or the response is invalid, an empty string may be returned.
     */
    String getCLIVersion();

    /**
     * @brief Retrieves the RUI API version of the RAK3172 module.
     *
     * This function sends the command `AT+APIVER=?` to the RAK3172 module
     * and returns the response, which contains the version information of the RUI API.
     *
     * @note This command is useful for checking the version of the RUI API
     *       to ensure compatibility with the module's firmware and for development purposes.
     *
     * @return A string containing the RUI API version.
     *         If the command fails or the response is invalid, an empty string may be returned.
     */
    String getAPIVersion();

    /**
     * @brief Retrieves the hardware model information of the RAK3172 module.
     *
     * This function sends the command `AT+HWMODEL=?` to the RAK3172 module
     * and returns the response, which contains the hardware model information.
     *
     * @note This command is useful for identifying the hardware model of the RAK3172 module,
     *       which can be helpful for versioning, debugging, and hardware compatibility checks.
     *
     * @return A string containing the hardware model.
     *         If the command fails or the response is invalid, an empty string may be returned.
     */
    String getHWModel();

    /**
     * @brief Retrieves the hardware ID of the RAK3172 module.
     *
     * This function sends the command `AT+HWID=?` to the RAK3172 module
     * and returns the response, which contains the hardware ID information.
     *
     * @note This command is useful for identifying the specific hardware ID of the RAK3172 module,
     *       which can be important for tracking, inventory, and compatibility purposes.
     *
     * @return A string containing the hardware ID.
     *         If the command fails or the response is invalid, an empty string may be returned.
     */
    String getHWId();

    /**
     * @brief Allows the user to set an alias for the device.
     *
     * This function sends the command `AT+ALIAS=?` to the RAK3172 module
     * and returns the response, which allows the user to configure a device alias.
     *
     * @note This command is useful for identifying the device with a user-friendly name,
     *       making it easier to manage multiple devices.
     * @return A string containing the device alias.
     *         If the command fails or the response is invalid, an empty string may be returned.
     */
    String getDeviceAlias();

    /**
     * @brief Retrieves the system voltage of the RAK3172 module.
     *
     * This function sends the command `AT+SYSV=?` to the RAK3172 module
     * and returns the response, which contains the system's current voltage information.
     *
     * @note This command is useful for monitoring the system's voltage level,
     *       which can be important for battery-powered devices to ensure proper operation.
     *
     * @return A string containing the system voltage.
     *         If the command fails or the response is invalid, an empty string may be returned.
     *
     */
    String getSysbat();
};

#endif
