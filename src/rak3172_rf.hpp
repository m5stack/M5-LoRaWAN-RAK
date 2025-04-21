/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef _RAK3172_RF_HPP_
#define _RAK3172_RF_HPP_

#include "rak3172_common.hpp"

class RAK3172RF : public RAK3172 {
public:
    /**
     * @brief Starts the RF RSSI (Received Signal Strength Indicator) test.
     *
     * This command allows the user to initiate an RF RSSI test, which measures
     * the strength of the received signal from the LoRaWAN module.
     *
     * @note The function sends the "AT+TRSSI?" command to the module to start the test.
     *
     * @return true if the RSSI test command was successfully sent;
     *         false if the command failed.
     */
    bool startRssi();

    /**
     * @brief Starts the radio frequency (RF) test mode.
     *
     * This command allows the user to initiate a radio frequency test by enabling
     * the tone transmission in LoRaWAN communication.
     *
     * @note The function sends the "AT+TTONE" command to start the RF test mode,
     * where the module will generate a tone for testing purposes.
     *
     * @return true if the RF test mode command was successfully sent;
     *         false if the command failed.
     */
    bool startRadioFrequency();

    /**
     * @brief Sets the number of data packets to send for the RF transmission test.
     *
     * This command allows the user to specify the number of data packets to be sent
     * during the RF transmission test. The valid range for the number of packets is
     * from 0 to (2^32 - 1).
     *
     * @note The function sends the command "AT+TTX=<num>", where <num> is the
     * specified number of packets to send.
     *
     * @param num The number of data packets to send, within the range of 0 to
     *            (2^32 - 1).
     *
     * @return true if the command to set the number of packets was successfully sent;
     *         false if the command failed.
     */
    bool startSend(uint16_t num);

    /**
     * @brief Sets the number of data packets to receive for the RF reception test.
     *
     * This command allows the user to specify the number of data packets to be received
     * during the RF reception test.
     *
     * @note The function sends the command "AT+TRX=<num>", where <num> is the
     * specified number of packets to receive.
     *
     * @param num The number of data packets to receive.
     *
     * @return true if the command to set the number of packets was successfully sent;
     *         false if the command failed.
     */
    bool startRecv(uint16_t num);

    /**
     * @brief Configures the LoRa® RF test parameters for transmission.
     *
     * This command allows the user to access and configure various RF test parameters
     * for LoRa® communication. It is used for RF certification testing, with default
     * values set to "868000000:14:0:7:1:0:0:1:4:25000:0:0".
     *
     * @note The parameters are as follows:
     * 1. Frequency (long)
     * 2. Power (int)
     * 3. Bandwidth (int):
     *    - LoRa mode (kHz): 0=125, 1=250, 2=500, 3=7.8, 4=10.4, 5=15.63, 6=20.83, 7=31.25, 8=41.67, 9=62.5;
     *    - FSK mode (Hz): 4800-467000
     * 4. Spreading Factor (int): 5-12
     * 5. Coding Rate (int): 1=4/5, 2=4/6, 3=4/7, 4=4/8
     * 6. LNA State (int): Not implemented
     * 7. PA Boost State (int): Not implemented
     * 8. Modulation (int): 0=FSK, 1=LoRa
     * 9. Payload Length (int)
     * 10. Frequency Deviation (uint16_t): Only for FSK, 600 - 200000 Hz
     * 11. LowDRopt (int): Not implemented
     * 12. BT Product (int): Not implemented
     *
     * @param freq The frequency to set (in Hz).
     * @param pwr The power level to set (in dBm).
     * @param bw The bandwidth parameter to set.
     * @param sf The spreading factor to set.
     * @param cr The coding rate to set.
     * @param mode The modulation mode to set (0 for FSK, 1 for LoRa).
     * @param prlen The payload length to set.
     * @param fsk The frequency deviation for FSK (in Hz).
     *
     * @return true if the configuration command was successfully sent;
     *         false if the command failed.
     */
    bool setConfig(long freq, int pwr, int bw, int sf, int cr, int mode, int prlen, uint16_t fsk = 0);

    /**
     * @brief Starts the RF transmission frequency hopping test.
     *
     * This command initiates a frequency hopping test for RF transmission,
     * starting from Fstart to Fstop with a specified step size of Fdelta.
     *
     * @note The function sends the command "AT+TTH=<Fstart>:<Fstop>:<Fdelta>:<len>",
     * where <Fstart> is the starting frequency, <Fstop> is the stopping frequency,
     * <Fdelta> is the frequency step size, and <len> is the length of the test.
     *
     * @param Fstart The starting frequency for the hopping test (in Hz).
     * @param Fstop The stopping frequency for the hopping test (in Hz).
     * @param Fdelta The frequency step size for hopping (in Hz).
     * @param len The length of the frequency hopping test.
     *
     * @return true if the frequency hopping command was successfully sent;
     *         false if the command failed.
     */
    bool setSendFrequencyHopping(uint32_t Fstart, uint32_t Fstop, uint16_t Fdelta, int len);

    /**
     * @brief Stops the ongoing RF test.
     *
     * This command is used to halt any RF testing that is currently in progress.
     *
     * @note The function sends the command "AT+TOFF?" to stop the RF test.
     *
     * @return true if the stop command was successfully sent;
     *         false if the command failed.
     */
    bool stop();

    /**
     * @brief Retrieves the current LoRaWAN® certification mode of the module.
     *
     * This command allows the user to obtain the current operating mode of the
     * module in relation to LoRaWAN® certification. The modes are defined as follows:
     * - 0: Normal mode
     * - 1: Certification mode
     *
     * @note The function sends the command "AT+CERTIF?" to query the current mode.
     *
     * @return true if the command to retrieve the certification mode was successfully sent;
     *         false if the command failed.
     */
    bool loraCertification();

    /**
     * @brief Enables continuous RF transmission with configurable frequency, power, and duration.
     *
     * This command initiates continuous RF transmission with the specified frequency,
     * transmission power, and duration. It also allows the user to obtain detailed
     * information about the RF transmission.
     *
     * @note The function sends the command "AT+CW=<freq>:<power>:<time>", where:
     * - <freq>:
     *   - RAK3172(L) requires a low frequency range of 150000000 - 600000000 Hz.
     *   - RAK3172(H) requires a high frequency range of 600000000 - 960000000 Hz.
     * - <power>: Transmission power in dBm (5 - 22 dBm).
     * - <time>: Duration of transmission in milliseconds (0 - 65535 ms).
     *
     * @param freq The frequency to use for RF transmission (in Hz).
     * @param pwr The transmission power to use (in dBm).
     * @param time The duration for which to transmit (in milliseconds).
     *
     * @return true if the continuous RF transmission command was successfully sent;
     *         false if the command failed.
     */
    bool RFcontinuity(long freq, int pwr, int time);

    /**
     * @brief Configures the random sequence RF Tx frequency hopping test.
     *
     * This command allows the user to configure a frequency hopping test where
     * the frequencies are selected randomly within a specified range. The user
     * can set the starting and stopping frequencies, the frequency interval,
     * and the number of packets to be transmitted.
     *
     * @note The function sends the command "AT+TRTH=<Fstart>:<Fstop>:<Fdelta>:<len>", where:
     * - <Fstart>: The starting frequency of the hopping test (in Hz).
     * - <Fstop>: The stopping frequency of the hopping test (in Hz).
     * - <Fdelta>: The frequency interval or step size (in Hz).
     * - <len>: The number of packets to be transmitted during the test.
     *
     * @param Fstart The starting frequency for the hopping test (in Hz).
     * @param Fstop The stopping frequency for the hopping test (in Hz).
     * @param Fdelta The frequency interval for hopping (in Hz).
     * @param len The number of packets to be transmitted.
     *
     * @return true if the random frequency hopping command was successfully sent;
     *         false if the command failed.
     */
    bool setRandomFrequencyHopping(uint32_t Fstart, uint32_t Fstop, uint16_t Fdelta, int len);
};

#endif