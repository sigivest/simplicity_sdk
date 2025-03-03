/***************************************************************************//**
 * @file
 * @brief APIs and defines for the Gas Proxy Function plugin.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef SILABS_GAS_PROXY_FUNCTION_H
#define SILABS_GAS_PROXY_FUNCTION_H

#include "gas-proxy-function-config.h"

#define fieldLength(field) \
  (sl_zigbee_af_current_command()->bufLen - (field - sl_zigbee_af_current_command()->buffer));

/**
 * @defgroup gas-proxy-function Gas Proxy Function
 * @ingroup component
 * @brief API and Callbacks for the Gas Proxy Function Component
 *
 * Silicon Labs implementation of the Gas Proxy Function (GPF).
 * This component receives and processes GBZ Top Off Messages.
 * NOTE: This component uses malloc and should only be used in a non-SoC setup.
 *
 */

/**
 * @addtogroup gas-proxy-function
 * @{
 */

/**
 * @name Handlers
 * @{
 */

sl_status_t sl_zigbee_af_gas_proxy_function_non_tap_off_message_handler(uint8_t * gbzCommands,
                                                                        uint16_t  gbzCommandsLength,
                                                                        uint16_t  messageCode);

sl_status_t sl_zigbee_af_gas_proxy_function_tap_off_message_handler(uint8_t * gbzCommand,
                                                                    uint16_t  gbzCommandLength,
                                                                    uint8_t * gbzCommandResponse,
                                                                    uint16_t  gbzCommandResponseLength,
                                                                    uint16_t  messageCode);

/** @} */ // end of name Handlers

#ifndef DOXYGEN_SHOULD_SKIP_THIS
typedef enum {
  SL_ZIGBEE_AF_GPF_ZCL_COMMAND_PERMISSION_ALLOWED,
  SL_ZIGBEE_AF_GPF_ZCL_COMMAND_PERMISSION_NOT_ALLOWED,
  SL_ZIGBEE_AF_GPF_ZCL_COMMAND_PERMISSION_IGNORED,
} sl_zigbee_af_gpf_zcl_command_permission_t;

typedef enum {
  SL_ZIGBEE_AF_GPF_MESSAGE_TYPE_TOM,
  SL_ZIGBEE_AF_GPF_MESSAGE_TYPE_NON_TOM,
} sl_zigbee_af_g_p_f_message_type_t;

uint32_t sli_zigbee_af_gas_proxy_function_get_gbz_start_time(void);
void sli_zigbee_af_gas_proxy_function_alert(uint16_t alertCode,
                                            sl_zigbee_af_cluster_command_t *cmd,
                                            uint16_t messageCode);
#endif //DOXYGEN_SHOULD_SKIP_THIS

/**
 * @name API
 * @{
 */

/**
 * @brief Ignore future commands.
 * Per GBCS v0.8.1, section 10.3.4.2 Step 12/13, it is inferred that per
 * elemental ZCL command, future commands ( startTime greater than now ) will
 * be discarded and the application will be notified.
 *
 * @return true - if future dated current command are ignored, false - otherwise.
 */
bool sl_zigbee_af_gas_proxy_function_ignore_future_command(uint32_t startTime);
/** @} */ // end of name API

/**
 * @name Callbacks
 * @{
 */

/**
 * @defgroup gas_proxy_cb Gas Proxy Function
 * @ingroup af_callback
 * @brief Callbacks for Gas Proxy Function Component
 *
 */

/**
 * @addtogroup gas_proxy_cb
 * @{
 */

/** @brief Notify the application that the Non Tap Off Message (Non-TOM) has been
 * handled by the mirror endpoint.
 *
 * This callback is invoked when the the Non
 * Tap Off Message have been processed. The first argument is a pointer to the
 * GBZ commands response buffer. The application is responsible for freeing
 * this buffer.
 *
 * @param gbzCommandsResponse   Ver.: always
 * @param gbzCommandsResponseLength   Ver.: always
 */
void sl_zigbee_af_gas_proxy_function_non_tap_off_message_handler_completed_cb(uint8_t *gbzCommandsResponse,
                                                                              uint16_t gbzCommandsResponseLength);
/** @brief Notify the application of an Alert that should be sent to the WAN.
 *
 * The second argument is a pointer to the BGZ alert buffer. The application is
 * responsible for freeing this buffer.
 *
 * @param alertCode The 16 bit alert code as defined by GBCS Ver.: always
 * @param gbzAlert Buffer containing the GBZ formatted Alert Ver.: always
 * @param gbzAlertLength The length in octets of the GBZ formatted Alert
 * Ver.: always
 */
void sl_zigbee_af_gas_proxy_function_alert_w_a_n_cb(uint16_t alertCode,
                                                    uint8_t *gbzAlert,
                                                    uint16_t gbzAlertLength);
/** @brief Notify the application that an elemental ZCL command, embedded within the
 * Tap Off Message (TOM), has been ignored by the GPF as it is a future dated
 * command.
 *
 *  The application can use the callback to store the discarded
 * command for later retrieval. When the future dated command becomes active on
 * the GSME, it will inform the CommsHub about the activation via an Alert
 * message. The CommsHub should be able to correlate the matching messages and
 * pass the previously stored ZCL commands to the TOM message handler for
 * processing.
 *
 * @param gpfMessage   Ver.: always
 * @param zclClusterCommand   Ver.: always
 */
void sl_zigbee_af_gas_proxy_function_tap_off_message_future_command_ignored_cb(const sl_zigbee_af_gpf_message_t * gpfMessage,
                                                                               const sl_zigbee_af_cluster_command_t * zclClusterCommand);
/** @brief Query the application regarding the processing of the current GBCS-specific
 * command.
 *
 * Depending on the vendor-specific information, such as the Tenancy
 * attribute in the Device Management Cluster, the callback's return value
 * determines whether the current command gets processed or not. With a return
 * value of true, the plugin will process the message. Otherwise, the plugin
 * will reject the message.
 *
 * @param gpfMessage   Ver.: always
 * @param zclClusterCommand   Ver.: always
 */
bool sl_zigbee_af_gas_proxy_function_data_log_access_request_cb(const sl_zigbee_af_gpf_message_t * gpfMessage,
                                                                const sl_zigbee_af_cluster_command_t * zclClusterCommand);
/** @brief Validate the incoming ZCL command.
 *
 * Query the application to check if a specific ZCL command should be processed
 * given the UseCase # and ZCL command payload information. This function
 * should capture the behavior detailed by the GBCS Use Case documentation.
 * See sl_zigbee_af_gpf_zcl_command_permission_t enum for specific return data
 * types.
 *
 * @param cmd   Ver.: always
 * @param messageCode   Ver.: always
 */
sl_zigbee_af_gpf_zcl_command_permission_t sl_zigbee_af_gas_proxy_function_validate_incoming_zcl_command_cb(const sl_zigbee_af_cluster_command_t * cmd,
                                                                                                           uint16_t messageCode);
/** @brief Called when the GPF receives a PublishSeasons command with
 * an unknown week ID in one of the seasons.
 *
 * The callback contains a mask,
 * unknownWeekIdSeasonsMask, with bits set corresponding to season entries that
 * were not published because of an unknown week ID reference.
 *
 * @param issuerCalendarId The calendar ID associated with these seasons.
 * Ver.: always
 * @param seasonEntries The season entries for this publish command.
 * Ver.: always
 * @param seasonEntriesLength The length of the season entries. Ver.: always
 * @param unknownWeekIdSeasonsMask A mask where bit N indicates that the Nth
 * season entry contains an unknown week ID reference. Ver.: always
 */
void sl_zigbee_af_gas_proxy_function_unknown_season_week_id_cb(uint32_t issuerCalendarId,
                                                               uint8_t *seasonEntries,
                                                               uint8_t seasonEntriesLength,
                                                               uint8_t unknownWeekIdSeasonsMask);
/** @brief Called when the GPF receives a PublishSpecialDays command
 * with an unknown day ID for one of the special days.
 *
 * The callback contains a
 * mask, unknownDayIdMask, with bits set corresponding to special day entries
 * that were not published because of an unknown day ID reference.
 *
 * @param issuerCalendarId The calendar ID associated with these special days.
 * Ver.: always
 * @param specialDayEntries The special day entries for this publish command.
 * Ver.: always
 * @param specialDayEntriesLength The length of the special day entries.
 * Ver.: always
 * @param unknownDayIdMask A mask where bit N indicates that the Nth special
 * day entry contains an unknown day ID reference. Ver.: always
 */
void sl_zigbee_af_gas_proxy_function_unknown_special_days_day_id_cb(uint32_t issuerCalendarId,
                                                                    uint8_t *specialDayEntries,
                                                                    uint8_t specialDayEntriesLength,
                                                                    uint8_t unknownDayIdMask);
/** @} */ // end of gas_proxy_cb
/** @} */ // end of name Callbacks
#endif // #ifndef _GAS_PROXY_FUNCTION_H_
