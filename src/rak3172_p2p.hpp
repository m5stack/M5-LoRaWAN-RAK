/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef _RAK3172_P2P_HPP_
#define _RAK3172_P2P_HPP_

#include <Arduino.h>
#include "Stream.h"
#include <vector>
#include "rak3172_common.hpp"

/**
 * @brief Structure representing a point-to-point (P2P) frame.
 *
 * This structure is used to hold the information of a P2P frame, including
 * the received signal strength indicator (RSSI), signal-to-noise ratio (SNR),
 * the length of the payload, and the payload data itself.
 */
typedef struct {
    int rssi;          /**< Received Signal Strength Indicator (RSSI) value.*/
    int snr;           /**< Signal-to-Noise Ratio (SNR) value.*/
    int len;           /**< Length of the payload data.*/
    char payload[500]; /**< Payload data (up to 500 bytes).*/
} p2p_frame_t;

/**
 * @brief Enumeration representing the modes of point-to-point (P2P) communication.
 *
 * This enumeration defines the different operational modes for P2P communication:
 * - P2P_TX_MODE: Transmit mode only.
 * - P2P_RX_MODE: Receive mode only.
 * - P2P_TX_RX_MODE: Both transmit and receive mode.
 */
typedef enum {
    P2P_TX_MODE = 0, /**< Transmit mode only.*/
    P2P_RX_MODE,     /**< Receive mode only.*/
    P2P_TX_RX_MODE   /**< Both transmit and receive mode.*/
} p2p_mode_t;

class RAK3172P2P : public RAK3172 {
public:
    /**
     * @brief Initializes the RAK3172 P2P module.
     *
     * This function sets up the RAK3172 P2P module by initializing the
     * serial interface, configuring the RX and TX pins, and setting
     * the baud rate. It also restarts the module and configures it to
     * operate in a specific network mode.
     *
     * @note
     * - The function first calls the base class initialization method.
     * - A delay of 100 milliseconds is introduced after restarting the module
     *   to ensure it is ready to receive commands.
     * - The function sends the "AT" command to check the module's response
     *   and sets the network mode to P2P using the "AT+NWM=0" command.
     *
     * @param serial Pointer to the HardwareSerial instance used for communication.
     * @param rx The RX pin number to be used for serial communication.
     * @param tx The TX pin number to be used for serial communication.
     * @param baudRate The baud rate for serial communication, specified as rak3172_bps_t.
     *
     * @return true if the initialization commands were successfully sent;
     *         false if any command failed or the initialization was unsuccessful.
     */
    bool init(HardwareSerial* serial = &Serial2, int rx = 16, int tx = 17, rak3172_bps_t baudRate = RAK3172_BPS_115200);

    /**
     * @brief Restarts the RAK3172 P2P module.
     *
     * This function sends the "ATZ" command to the module, which triggers
     * a restart of the RAK3172 P2P module.
     *
     * @note The "ATZ" command is a standard reset command for the module.
     * It resets the module to its default state, and a brief delay may be required
     * after calling this function to allow the module to fully restart.
     *
     * @return true if the restart command was successfully sent;
     *         false if the command failed.
     */
    bool restart();

    /**
     * @brief Parses a received P2P frame and extracts relevant information.
     *
     * This function processes a string frame received from the RAK3172 P2P module,
     * extracting the RSSI, SNR, and payload data. The extracted information is
     * stored in a `p2p_frame_t` structure and added to the internal frames vector.
     *
     * @note
     * - The function looks for the "+EVT:RXP2P:" prefix to identify valid P2P frames.
     * - It extracts the RSSI and SNR values from the frame, as well as the payload.
     * - The payload is decoded using the `decodeMsg` function before being stored.
     * - The resulting `p2p_frame_t` structure is pushed into the `_frames` vector.
     *
     * @param frame The string representation of the received P2P frame to be parsed.
     *
     * @return void This function does not return a value.
     */
    void parse(String frame);

    /**
     * @brief Updates the state of the RAK3172 P2P module by reading incoming data.
     *
     * This function checks for incoming data from the RAK3172 P2P module.
     * It uses a mutex to ensure thread-safe access to the serial interface.
     * The function reads a line of data and processes it based on specific event indicators.
     *
     * @note
     * - The function waits indefinitely for access to the serial interface using a semaphore.
     * - It reads data until a newline character is encountered.
     * - If the received data contains the "+EVT:RXP2P" event, it checks for errors and
     *   processes the data if no errors are found.
     * - If the received data indicates that the transmission is complete with
     *   "+EVT:TXP2P DONE", it can be handled accordingly (currently no action is taken).
     *
     * @return void This function does not return a value.
     */
    void update();

    /**
     * @brief Configures the P2P mode parameters for the RAK3172 module.
     *
     * This function sends a command to configure various parameters for the
     * point-to-point (P2P) mode of the RAK3172 module. It allows setting
     * frequency, spreading factor, bandwidth, coding rate, preamble length,
     * and transmission power.
     *
     * @note
     * - Default parameter values are: "868000000:7:125:0:8:14".
     * - Frequency range: 150000000 - 960000000 Hz.
     * - Spreading Factor (SF): 6, 7, 8, 9, 10, 11, 12.
     * - Bandwidth (BW): 125, 250, 500 kHz.
     * - Coding Rate (CR): 4/5=0, 4/6=1, 4/7=2, 4/8=3.
     * - Preamble Length: 2 - 65535 symbols.
     * - Transmission Power: 5 - 22 dBm.
     *
     * @param freq The frequency to be set for P2P communication (in Hz).
     * @param sf The spreading factor to be used for P2P communication.
     * @param bw The bandwidth for the P2P connection (in kHz).
     * @param cr The coding rate represented as an integer.
     * @param prlen The preamble length for the transmission.
     * @param pwr The transmission power level (in dBm).
     *
     * @return true if the configuration command was successfully sent;
     *         false if the command failed.
     */
    bool config(long freq, int sf, int bw, int cr, int prlen, int pwr);

    /**
     * @brief Sets the operation mode for the RAK3172 P2P module (TX, RX, or TX/RX mode).
     *
     * This function configures the RAK3172 P2P module to operate in one of the
     * following modes:
     * - **P2P_TX_MODE**: Configures the module to operate in transmit mode.
     * - **P2P_RX_MODE**: Configures the module to operate in receive mode with a specified timeout.
     * - **P2P_TX_RX_MODE**: Configures the module to operate in both transmit and receive modes.
     *
     * @note
     * - In **P2P_TX_MODE**, the "AT+PRECV=0" command is sent to stop the module from listening for P2P LoRa packets.
     * - In **P2P_RX_MODE**, the "AT+PRECV=<timeout>" command is used, where the `timeout` value determines how long the
     * module listens for packets. If the `timeout` is set to 0, the device stops listening for P2P LoRa packets and
     * switches to TX mode.
     * - In **P2P_TX_RX_MODE**, the "AT+PRECV=65533" command is used to enable both transmitting and receiving.
     * - The function updates the internal mode (`_mode`) after successfully setting the mode.
     *
     * @param mode The desired mode for the module. One of the following:
     *   - `P2P_TX_MODE`: Transmit mode only.
     *   - `P2P_RX_MODE`: Receive mode only, with a specified timeout.
     *   - `P2P_TX_RX_MODE`: Both transmit and receive mode.
     * @param timeout The timeout (in seconds) for the receive mode. Only used if `mode` is `P2P_RX_MODE`. A value of 0
     * stops the module from receiving.
     *
     * @return true if the mode change command was successfully sent;
     *         false if the command failed.
     */
    bool setMode(p2p_mode_t mode, time_t timeout = 65535);

    /**
     * @brief Sets the frequency for the P2P mode of the RAK3172 module.
     *
     * This function allows the user to configure the frequency for the
     * point-to-point (P2P) mode of the RAK3172 module. The frequency is
     * specified in Hertz (Hz).
     *
     * @note
     * - The command "AT+PFREQ=<freq>" is sent to set the desired frequency.
     * - Ensure that the frequency is within the supported range for P2P communication.
     *
     * @param freq The frequency to be set for P2P communication (in Hz).
     *
     * @return true if the frequency configuration command was successfully sent;
     *         false if the command failed.
     */
    bool setFreq(long freq);

    /**
     * @brief Sets the spreading factor for the P2P mode of the RAK3172 module.
     *
     * This function allows the user to configure the spreading factor (SF)
     * for the point-to-point (P2P) mode of the RAK3172 module. The spreading
     * factor determines the data rate and range of the communication.
     *
     * @note
     * - The spreading factor is constrained to the range of 6 to 12.
     * - The command "AT+PSF=<sf>" is sent to set the desired spreading factor.
     * - Valid values for the spreading factor are between 5 and 12.
     *
     * @param sf The spreading factor to be set for P2P communication (must be between 6 and 12).
     *
     * @return true if the spreading factor configuration command was successfully sent;
     *         false if the command failed.
     */
    bool setSpreadingFactor(uint8_t sf);

    /**
     * @brief Sets the bandwidth for the P2P mode of the RAK3172 module.
     *
     * This function allows the user to configure the bandwidth for the
     * point-to-point (P2P) mode of the RAK3172 module. The bandwidth
     * affects the data rate and range of the communication.
     *
     * @note
     * - The bandwidth is constrained to the values of 125, 250, or 500 kHz.
     * - The command "AT+PBW=<bw>" is sent to set the desired bandwidth.
     *
     * @param bw The bandwidth to be set for P2P communication (must be one of 125, 250, or 500).
     *
     * @return true if the bandwidth configuration command was successfully sent;
     *         false if the command failed.
     */
    bool setBandWidth(int bw);

    /**
     * @brief Sets the coding rate for the P2P mode of the RAK3172 module.
     *
     * This function allows the user to configure the coding rate for the
     * point-to-point (P2P) mode of the RAK3172 module. The coding rate
     * affects the error correction capability of the communication.
     *
     * @note
     * - The coding rate options are as follows:
     *   - 4/5: 0
     *   - 4/6: 1
     *   - 4/7: 2
     *   - 4/8: 3
     * - The coding rate is constrained to values between 0 and 3.
     * - The command "AT+PCR=<cr>" is sent to set the desired coding rate.
     *
     * @param cr The coding rate to be set for P2P communication (must be one of 0, 1, 2, or 3).
     *
     * @return true if the coding rate configuration command was successfully sent;
     *         false if the command failed.
     */
    bool setCodingRate(int8_t cr);

    /**
     * @brief Sets the output power for the P2P mode of the RAK3172 module.
     *
     * This function allows the user to configure the output power for the
     * point-to-point (P2P) mode of the RAK3172 module. The output power
     * affects the transmission range and signal strength.
     *
     * @note
     * - The output power is constrained to values between 5 and 22.
     * - The command "AT+PTP=<power>" is sent to set the desired output power.
     *
     * @param power The output power to be set for P2P communication (must be between 5 and 22).
     *
     * @return true if the output power configuration command was successfully sent;
     *         false if the command failed.
     */
    bool setOutPower(uint8_t power);

    /**
     * @brief Sets the preamble length for the P2P mode of the RAK3172 module.
     *
     * This function allows the user to configure the preamble length for the
     * point-to-point (P2P) mode of the RAK3172 module. The preamble length
     * affects the synchronization of the receiver and the robustness of the
     * communication.
     *
     * @note
     * - The preamble length is constrained to values between 2 and 65535.
     * - The command "AT+PPL=<preambleLength>" is sent to set the desired preamble length.
     *
     * @param preambleLength The preamble length to be set for P2P communication (must be between 2 and 65535).
     *
     * @return true if the preamble length configuration command was successfully sent;
     *         false if the command failed.
     */
    bool setPreambleLength(uint16_t preambleLength);

    /**
     * @brief Sets the synchronization word for the P2P mode of the RAK3172 module.
     *
     * This function allows the user to configure the synchronization word
     * for the point-to-point (P2P) mode of the RAK3172 module. The syncword
     * is used for frame synchronization and can help in filtering out
     * unwanted signals.
     *
     * @note
     * - The syncword must be in the range of 0x0000 to 0xFFFF (0 to 65535).
     * - The command "AT+SYNCWORD=<sync>" is sent to set the desired synchronization word.
     * - Leading zeros are added to ensure the syncword is represented as a 4-character string.
     *
     * @param sync The synchronization word to be set for P2P communication (must be between 0 and 65535).
     *
     * @return true if the syncword configuration command was successfully sent;
     *         false if the command failed.
     */
    bool setSyncword(int sync);

    /**
     * @brief Enables or disables encryption for the P2P mode of the RAK3172 module.
     *
     * This function allows the user to configure the encryption setting for
     * the point-to-point (P2P) mode of the RAK3172 module. Enabling encryption
     * enhances the security of the communication.
     *
     * @note
     * - This command enables or disables encryption in P2P mode.
     * - The command "AT+ENCRY=<en>" is sent to set the desired encryption state.
     *
     * @param en A boolean value indicating whether to enable (true) or disable (false) encryption.
     *
     * @return true if the encryption configuration command was successfully sent;
     *         false if the command failed.
     */
    bool setEncipher(bool en);

    /**
     * @brief Sets the encryption key for the P2P mode of the RAK3172 module.
     *
     * This function allows the user to configure the encryption key that will
     * be used to encrypt the data being sent and received in the point-to-point
     * (P2P) mode of the RAK3172 module. A valid key is essential for secure
     * communication.
     *
     * @note
     * - The key must be exactly 16 characters long.
     * - The command "AT+ENCKEY=<key>" is sent to set the desired encryption key.
     *
     * @param key The encryption key to be set for P2P communication (must be 16 characters).
     *
     * @return true if the encryption key configuration command was successfully sent;
     *         false if the command failed or if the key is invalid.
     */
    bool setEncryptionKey(const String& key);

    /**
     * @brief Sets the password state for the P2P mode of the RAK3172 module.
     *
     * This function allows the user to configure the password protection
     * state for the point-to-point (P2P) mode of the RAK3172 module.
     * Enabling the password state enhances the security of the communication.
     *
     * @note
     * - This command allows the user to set the password state in P2P mode.
     * - The command "AT+PCRYPT=<en>" is sent to set the desired password state.
     *
     * @param en A boolean value indicating whether to enable (true) or disable (false) password protection.
     *
     * @return true if the password state configuration command was successfully sent;
     *         false if the command failed.
     */
    bool setPasswordState(bool en);

    /**
     * @brief Sets the encryption and decryption key for the P2P mode of the RAK3172 module.
     *
     * This function allows the user to configure the encryption and decryption
     * key that will be used in the point-to-point (P2P) mode of the RAK3172 module.
     * A valid key is essential for secure communication.
     *
     * @note
     * - The key must be exactly 8 characters long.
     * - This command allows the user to access both encryption and decryption keys in P2P mode.
     * - The command "AT+PKEY=<key>" is sent to set the desired encryption and decryption key.
     *
     * @param key The encryption and decryption key to be set for P2P communication (must be 8 characters).
     *
     * @return true if the key configuration command was successfully sent;
     *         false if the command failed or if the key is invalid.
     */
    bool setEncryptionDecryptionKey(const String& key);

    /**
     * @brief Sets the encryption initialization vector (IV) for the P2P mode of the RAK3172 module.
     *
     * This function allows the user to configure the encryption initialization
     * vector (IV) that will be used in the point-to-point (P2P) mode of the
     * RAK3172 module. The IV is essential for ensuring the security of the
     * encryption process.
     *
     * @note
     * - The IV must be exactly 16 characters long and should be provided in
     *   hexadecimal format.
     * - This command allows the user to get or set the encryption IV in P2P mode.
     * - The command "AT+CRYPIV=<key>" is sent to set the desired encryption IV.
     *
     * @param key The encryption IV to be set for P2P communication (must be 16 characters in hexadecimal).
     *
     * @return true if the IV configuration command was successfully sent;
     *         false if the command failed or if the IV is invalid.
     */
    bool setEncryptionIV(const String& key);

    /**
     * @brief Sets the frequency deviation for the FSK modem in P2P mode of the RAK3172 module.
     *
     * This function allows the user to configure the frequency deviation for
     * the Frequency Shift Keying (FSK) modem used in the point-to-point (P2P)
     * mode of the RAK3172 module. Frequency deviation affects the modulation
     * characteristics and can influence communication reliability.
     *
     * @note
     * - The frequency deviation must be in the range of 600 to 200000 Hz.
     * - The command "AT+PFDEV=<freq>" is sent to set the desired frequency deviation.
     *
     * @param freq The frequency deviation to be set for P2P FSK communication (must be between 600 and 200000 Hz).
     *
     * @return true if the frequency deviation configuration command was successfully sent;
     *         false if the command failed.
     */
    bool setFSKFrequencyDeviation(uint16_t freq);

    /**
     * @brief Sets the bit rate for the FSK modem in P2P mode of the RAK3172 module.
     *
     * This function allows the user to configure the bit rate for the
     * Frequency Shift Keying (FSK) modem used in the point-to-point (P2P)
     * mode of the RAK3172 module. The bit rate affects the data transmission
     * speed and overall communication performance.
     *
     * @note
     * - The bit rate must be in the range of 600 b/s to 300000 b/s.
     * - The command "AT+PBR=<rate>" is sent to set the desired bit rate.
     *
     * @param rate The bit rate to be set for P2P FSK communication (must be between 600 and 300000 b/s).
     *
     * @return true if the bit rate configuration command was successfully sent;
     *         false if the command failed.
     */
    bool setFSKrate(uint16_t rate);

    /**
     * @brief Sends a string message in hex-encoded format using the P2P mode of the RAK3172 module.
     *
     * This function allows the user to send a message as a string, which is first
     * hex-encoded before being transmitted using the "AT+PSEND" command in P2P mode.
     * The function returns the length of the original string if the message was
     * successfully sent, or 0 if the command failed.
     *
     * @note
     * - The message is encoded in hexadecimal format before being sent.
     * - The command "AT+PSEND=<hexEncodedMessage>" is used to send the message.
     *
     * @param str The null-terminated string to be sent in P2P mode.
     *
     * @return The length of the input string if the message was successfully sent;
     *         0 if the send operation failed.
     */
    size_t print(const char* str);

    /**
     * @brief Sends a byte buffer as a hex-encoded string using the P2P mode of the RAK3172 module.
     *
     * This function allows the user to send a buffer of bytes, which is first
     * converted to a hex-encoded string before being transmitted using the
     * "AT+PSEND" command in P2P mode. The input buffer must be between 2 to 500
     * bytes in length, and the resulting hex string must consist of an even number
     * of characters representing 1 to 256 hexadecimal values.
     *
     * @note
     * - The length of the input buffer must be between 2 and 500 bytes.
     * - The hex-encoded string must consist of characters 0-9, a-f, A-F, and must
     *   have an even length.
     * - The command "AT+PSEND=<hexEncodedMessage>" is used to send the message.
     *
     * @param buf A pointer to the byte buffer to be sent in P2P mode.
     * @param size The size of the byte buffer (must be between 2 and 500).
     *
     * @return The number of bytes sent if the message was successfully sent;
     *0 if the send operation failed.
     */
    size_t write(const uint8_t* buf, size_t size);

    /**
     * @brief Returns the number of available frames in the P2P buffer.
     *
     * This function returns the number of frames that are currently available
     * for processing in the P2P buffer. It helps to check how many frames are
     * queued and ready to be retrieved.
     *
     * @note
     * - This function checks the size of the `_frames` container, which holds
     *   the incoming frames.
     *
     * @return The number of available frames in the buffer.
     */
    int available();

    /**
     * @brief Reads and returns the available frames from the P2P buffer.
     *
     * This function retrieves all the frames currently stored in the P2P buffer
     * and returns them as a vector. It allows the user to access the frames
     * that have been received and are ready for processing.
     *
     * @note
     * - The returned vector contains all frames stored in the `_frames` container.
     * - The user should ensure to process the frames appropriately after reading.
     *
     * @return A vector containing the available frames from the P2P buffer.
     */
    std::vector<p2p_frame_t> read();

    /**
     * @brief Clears the P2P buffer and flushes the serial output.
     *
     * This function flushes the serial output buffer to ensure that all
     * pending data is sent. It also clears the P2P frame buffer, removing
     * all stored frames. This is useful for resetting the state of the
     * communication and discarding any frames that are no longer needed.
     *
     * @note
     * - After calling this function, all frames in the `_frames` container
     *   will be lost.
     * - Ensure that any necessary frames are processed before calling this
     *   function.
     *
     * @return void
     */
    void flush();

    /**
     * @brief Retrieves the current frequency setting for the P2P mode.
     *
     * This function sends the "AT+PFREQ=?" command to the RAK3172 module to
     * query the current frequency setting used in P2P mode. It returns the
     * frequency value as a string.
     *
     * @note
     * - The returned string will contain the frequency setting returned by
     *   the device in response to the query.
     * - If the command fails or returns an error, the returned string may
     *   be empty or contain an error message.
     *
     * @return A string representing the current frequency setting in P2P mode.
     */
    String getFreq();

    /**
     * @brief Retrieves the current spreading factor setting for the P2P mode.
     *
     * This function sends the "AT+PSF=?" command to the RAK3172 module to
     * query the current spreading factor setting used in P2P mode. It returns
     * the spreading factor value as a string.
     *
     * @note
     * - The returned string will contain the spreading factor setting returned
     *   by the device in response to the query.
     * - If the command fails or returns an error, the returned string may
     *   be empty or contain an error message.
     *
     * @return A string representing the current spreading factor setting in P2P mode.
     */
    String getSpreadingFactor();

    /**
     * @brief Retrieves the current bandwidth setting for the P2P mode.
     *
     * This function sends the "AT+PBW=?" command to the RAK3172 module to query
     * the current bandwidth setting used in P2P mode. It then interprets the
     * result and returns the corresponding bandwidth value as a string.
     *
     * @note
     * - The function assumes that the returned data is a numerical string that
     *   represents a bandwidth setting. It translates this to the corresponding
     *   bandwidth value.
     * - If the returned value does not match expected values ("0", "1", "2"),
     *   the function will return "error".
     * - The possible bandwidth values are:
     *   - "0" corresponds to 125 kHz
     *   - "1" corresponds to 250 kHz
     *   - "2" corresponds to 500 kHz
     *
     * @return A string representing the current bandwidth setting:
     *         - "125" for 125 kHz
     *         - "250" for 250 kHz
     *         - "500" for 500 kHz
     *         - "error" if an unrecognized value is returned.
     */
    String getBandwidth();

    /**
     * @brief Retrieves the current coding rate setting for the P2P mode.
     *
     * This function sends the "AT+PCR=?" command to the RAK3172 module to query
     * the current coding rate setting used in P2P mode. It then interprets the
     * returned value and returns the corresponding coding rate as a string.
     *
     * @note
     * - The function assumes that the returned data is a numerical string that
     *   represents a coding rate setting. It maps this to the corresponding
     *   coding rate value.
     * - If the returned value does not match any of the expected values ("0",
     *   "1", "2", "3"), the function will return "error".
     * - The possible coding rates are:
     *   - "0" corresponds to 4/5
     *   - "1" corresponds to 4/6
     *   - "2" corresponds to 4/7
     *   - "3" corresponds to 4/8
     *
     * @return A string representing the current coding rate:
     *         - "4/5" for coding rate 4/5
     *         - "4/6" for coding rate 4/6
     *         - "4/7" for coding rate 4/7
     *         - "4/8" for coding rate 4/8
     *         - "error" if the returned value is not recognized.
     */
    String getCodingRate();

    /**
     * @brief Retrieves the current transmission power setting for the P2P mode.
     *
     * This function sends the "AT+PTP=?" command to the RAK3172 module to query
     * the current transmission power (output power) setting in P2P mode.
     * It returns the output power value as a string.
     *
     * @note
     * - The returned string contains the transmission power value returned
     *   by the device in response to the query.
     * - If the command fails or the module returns an error, the returned
     *   string may be empty or contain an error message.
     *
     * @return A string representing the current transmission power setting.
     *         The exact value depends on the module's configuration.
     */
    String getOutPower();

    /**
     * @brief Retrieves the current syncword setting for the P2P mode.
     *
     * This function sends the "AT+SYNCWORD=?" command to the RAK3172 module to query
     * the current syncword used in P2P (Point-to-Point) mode. The syncword is
     * typically a 16-bit value ranging from 0x0000 to 0xFFFF.
     *
     * @note
     * - The returned string contains the syncword value in response to the query.
     * - The syncword is used to identify valid packets and synchronize communication
     *   between devices in P2P mode.
     * - If the command fails or the module returns an error, the returned string
     *   may be empty or contain an error message.
     *
     * @return A string representing the current syncword setting in hexadecimal format,
     *         ranging from "0000" to "FFFF".
     */
    String getSyncword();

    /**
     * @brief Retrieves the current encryption status for the P2P mode.
     *
     * This function sends the "AT+ENCRY=?" command to the RAK3172 module to query
     * the current encryption status used in P2P (Point-to-Point) mode.
     * The encryption status indicates whether the communication is encrypted or not.
     *
     * @note
     * - The returned string contains the encryption status in response to the query.
     * - If the command fails or the module returns an error, the returned string
     *   may be empty or contain an error message.
     *
     * @return A string representing the current encryption status, which may indicate
     *         whether encryption is enabled or disabled.
     */
    String getEncipher();

    /**
     * @brief Retrieves the current encryption key for the P2P mode.
     *
     * This function sends the "AT+ENCKEY=?" command to the RAK3172 module to query
     * the current encryption key used in P2P (Point-to-Point) mode. The encryption
     * key is used to secure communications between devices.
     *
     * @note
     * - The returned string contains the encryption key in response to the query.
     * - The format of the encryption key may vary based on the configuration.
     * - If the command fails or the module returns an error, the returned string
     *   may be empty or contain an error message.
     *
     * @return A string representing the current encryption key for P2P communication.
     */
    String getEncryptionKey();

    /**
     * @brief Retrieves the current password state for the P2P mode.
     *
     * This function sends the "AT+PCRYPT=?" command to the RAK3172 module to query
     * the current password state in P2P (Point-to-Point) mode. The password state
     * indicates whether a password is set or if the communication is password-protected.
     *
     * @note
     * - The returned string contains the password state in response to the query.
     * - The password state determines whether the P2P communication requires a password
     *   for access or not.
     * - If the command fails or the module returns an error, the returned string
     *   may be empty or contain an error message.
     *
     * @return A string representing the current password state for P2P communication.
     *         The string may indicate whether a password is enabled or not.
     */
    String getPasswordState();

    /**
     * @brief Retrieves the current encryption and decryption key for the P2P mode.
     *
     * This function sends the "AT+PKEY=?" command to the RAK3172 module to query
     * the current encryption and decryption key used in P2P (Point-to-Point) mode.
     * This key is essential for securing communications between devices.
     *
     * @note
     * - The returned string contains the encryption and decryption key in response
     *   to the query.
     * - The format of the key may vary based on the configuration and security settings.
     * - If the command fails or the module returns an error, the returned string
     *   may be empty or contain an error message.
     *
     * @return A string representing the current encryption and decryption key for
     *         P2P communication.
     */
    String getEncryptionDecryptionKey();

    /**
     * @brief Retrieves the current encryption IV (Initialization Vector) for the P2P mode.
     *
     * This function sends the "AT+CRYPIV=?" command to the RAK3172 module to query
     * the current encryption IV used in P2P (Point-to-Point) mode. The IV is a
     * 16-byte hexadecimal value that is essential for secure encryption processes.
     *
     * @note
     * - The returned string contains the encryption IV in hexadecimal format.
     * - The IV is used to ensure that the same plaintext encrypts to different ciphertexts
     *   each time, enhancing security.
     * - If the command fails or the module returns an error, the returned string
     *   may be empty or contain an error message.
     *
     * @return A string representing the current encryption IV for P2P communication,
     *         formatted as a 16-byte hexadecimal value.
     */
    String getEncryptionIV();

    /**
     * @brief Retrieves the current FSK (Frequency Shift Keying) bit rate for the P2P mode.
     *
     * This function sends the "AT+PBR=?" command to the RAK3172 module to query
     * the current FSK modulation bit rate used in P2P (Point-to-Point) mode.
     * The bit rate defines the speed at which data is transmitted, and it can range
     * from 600 bps to 300,000 bps.
     *
     * @note
     * - The returned string contains the FSK bit rate in response to the query.
     * - The valid range for the bit rate is from 600 bps to 300,000 bps.
     * - If the command fails or the module returns an error, the returned string
     *   may be empty or contain an error message.
     *
     * @return A string representing the current FSK bit rate for P2P communication,
     *         within the range of 600 bps to 300,000 bps.
     */
    String getFSKrate();

    /**
     * @brief Retrieves the current FSK (Frequency Shift Keying) frequency deviation for the P2P mode.
     *
     * This function sends the "AT+PFDEV=?" command to the RAK3172 module to query
     * the current frequency deviation used in FSK modulation for P2P (Point-to-Point) mode.
     * Frequency deviation is the amount by which the carrier frequency is shifted during modulation,
     * and it can range from 600 Hz to 200,000 Hz.
     *
     * @note
     * - The returned string contains the FSK frequency deviation in response to the query.
     * - The valid range for the frequency deviation is from 600 Hz to 200,000 Hz.
     * - If the command fails or the module returns an error, the returned string
     *   may be empty or contain an error message.
     *
     * @return A string representing the current FSK frequency deviation for P2P communication,
     *         within the range of 600 Hz to 200,000 Hz.
     */
    String getFSKFrequencyDeviation();

private:
    /**
     * @brief A vector to hold multiple P2P frames.
     *
     * This variable stores a collection of P2P frames, allowing for the
     * management and processing of multiple frames in point-to-point communication.
     */
    std::vector<p2p_frame_t> _frames;

    /**
     * @brief Current mode of point-to-point (P2P) communication.
     *
     * This variable holds the current operational mode for P2P communication,
     * which can be set to transmit, receive, or both modes.
     */
    p2p_mode_t _mode;
};

#endif
