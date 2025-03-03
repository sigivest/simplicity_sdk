/***************************************************************************//**
 * @file pro_compliance_stack_interface_internal_def.h
 * @brief internal names for 'pro_compliance_stack_interface' declarations
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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
// automatically generated from pro_compliance_stack_interface.h.  Do not manually edit
#ifndef PRO_COMPLIANCE_STACK_INTERFACE_INTERNAL_DEF_H
#define PRO_COMPLIANCE_STACK_INTERFACE_INTERNAL_DEF_H

#include "stack/include/pro_compliance_stack_interface.h"

// Command Indirection

sl_802154_short_addr_t sli_mac_stack_find_child_short_id(sl_802154_long_addr_t eui64);

uint64_t sli_mac_stack_get_child_info_flags(uint8_t childIndex);

void sli_mac_stack_indirect_purge(uint8_t nwk_index);

void sli_mac_stack_kickstart(uint8_t mac_index);

bool sli_mac_stack_lower_mac_radio_is_on(uint8_t mac_index);

void sli_mac_stack_lower_mac_radio_sleep(void);

void sli_mac_stack_lower_mac_radio_wakeup(void);

sl_status_t sli_mac_stack_lower_mac_set_radio_channel(uint8_t mac_index,
                                                      uint8_t channel);

sl_status_t sli_mac_stack_lower_mac_set_radio_idle_mode(uint8_t mac_index,
                                                        uint8_t mode);

sl_status_t sli_mac_stack_lower_mac_set_radio_power(uint8_t mac_index,
                                                    int8_t power);

void sli_mac_stack_set_coordinator(bool isCoordinator);

sl_status_t sli_mac_stack_test_associate_command(sl_802154_short_addr_t parentId,
                                                 uint16_t panId);

void sli_mac_stack_test_send_mac_command(uint8_t macCommandLength,
                                         uint8_t *macCommand);

sl_status_t sli_mac_stack_test_set_nwk_radio_params_channel(uint8_t channel);

sl_status_t sli_mac_stack_test_set_nwk_radio_params_eui(uint8_t network_index,
                                                        sl_802154_long_addr_t eui);

sl_status_t sli_mac_stack_test_set_tx_power(int8_t power);

void sli_zigbee_stack_set_end_device_poll_timeout(uint8_t timeout);

sl_status_t sli_zigbee_stack_set_packet_validate_library_state(uint16_t state);

void sli_zigbee_stack_set_pan_id(uint16_t panId);

void sli_zigbee_stack_set_pan_id_conflict_report(boolean set_value,
                                                 uint8_t nwk_index);

bool sli_zigbee_stack_test_aps_key_in_sync(sl_802154_long_addr_t eui64);

sl_status_t sli_zigbee_stack_test_aps_key_set_sync_status(sl_802154_long_addr_t eui64,
                                                          bool setSync);

sl_status_t sli_zigbee_stack_test_custom_send_security_challenge_request(sl_802154_short_addr_t destShort,
                                                                         sl_zigbee_sec_man_context_t *context,
                                                                         uint8_t cmdoptions);

sl_status_t sli_zigbee_stack_test_ieee_address_request_to_target(sl_802154_short_addr_t discoveryNodeId,
                                                                 bool reportKids,
                                                                 uint8_t childStartIndex,
                                                                 uint8_t sourceEndpoint,
                                                                 sl_zigbee_aps_option_t options,
                                                                 sl_802154_short_addr_t targetNodeIdOfRequest);

void sli_zigbee_stack_test_join_list_add(uint8_t command,
                                         uint8_t *eui64List,
                                         uint8_t counts);

void sli_zigbee_stack_test_join_list_request(uint8_t startIndex);

bool sli_zigbee_stack_test_network_send_command(sl_802154_short_addr_t destination,
                                                uint8_t *commandFrame,
                                                uint8_t length,
                                                bool tryToInsertLongDest,
                                                sl_802154_long_addr_t destinationEui);

void sli_zigbee_stack_test_perform_raw_active_scan(uint32_t scanChannels,
                                                   uint8_t scanDuration);

void sli_zigbee_stack_test_reset_frame_counter(uint8_t mask);

bool sli_zigbee_stack_test_send_device_update(uint16_t newShortId,
                                              sl_802154_long_addr_t newLongId,
                                              bool apsEncryption,
                                              uint8_t deviceStatus);

sl_status_t sli_zigbee_stack_test_send_leave_request_command(uint16_t destId,
                                                             sl_802154_long_addr_t destEui);

bool sli_zigbee_stack_test_send_link_key(sl_802154_short_addr_t targetNodeId,
                                         sl_802154_long_addr_t targetEui64,
                                         uint8_t keyType,
                                         sl_zigbee_key_data_t *key,
                                         bool useApsEncryption);

void sli_zigbee_stack_test_send_network_rejoin_command(uint8_t cmd_id,
                                                       sl_802154_long_addr_t longId,
                                                       sl_802154_short_addr_t oldShortId,
                                                       sl_802154_short_addr_t newShortId,
                                                       bool useNwkSecurity,
                                                       uint8_t status,
                                                       bool reallySend);

void sli_zigbee_stack_test_send_network_timeout_request(uint8_t requestedTimeoutValue);

void sli_zigbee_stack_test_send_our_end_device_announcement(void);

sl_status_t sli_zigbee_stack_test_send_remove_device_command(uint16_t destId,
                                                             sl_802154_long_addr_t destEui,
                                                             sl_802154_long_addr_t deviceToRemoveEui,
                                                             bool sendNonEncrypted);

bool sli_zigbee_stack_test_send_report_or_update(uint8_t command,
                                                 uint8_t updateId,
                                                 uint16_t panId);

void sli_zigbee_stack_test_send_route_error_payload(sl_802154_short_addr_t destination,
                                                    sl_802154_short_addr_t target,
                                                    uint8_t errorCode,
                                                    uint8_t *payload,
                                                    uint8_t payload_len);

void sli_zigbee_stack_test_send_route_error_payload_no_network_encryption(sl_802154_short_addr_t destination,
                                                                          sl_802154_short_addr_t target,
                                                                          uint8_t errorCode,
                                                                          uint8_t *payload,
                                                                          uint8_t payload_len);

void sli_zigbee_stack_test_send_timeout_request(void);

bool sli_zigbee_stack_test_send_update_device_command(uint16_t newDeviceShortId,
                                                      sl_802154_long_addr_t newDeviceLongId,
                                                      uint32_t apsEncryption,
                                                      uint8_t deviceStatus);

void sli_zigbee_stack_test_set_network_tokens(uint8_t stackProfile,
                                              uint8_t nodeType,
                                              uint8_t channel,
                                              int8_t power,
                                              uint16_t nodeId,
                                              uint16_t panId,
                                              sl_802154_long_addr_t extendedPanId);

void sli_zigbee_stack_test_spoof_device_announcement(uint16_t shortId,
                                                     uint8_t *sourceEUI64,
                                                     sl_802154_long_addr_t deviceAnnounceEui,
                                                     uint8_t capabilities);

sl_status_t sli_zigbee_stack_test_zdo_generate_clear_all_bindings_req(sl_802154_short_addr_t destination,
                                                                      bool encrypt,
                                                                      uint8_t *eui64_list,
                                                                      uint8_t counts);

sl_status_t sli_zigbee_stack_test_zdo_generate_get_authentication_level_req(sl_802154_short_addr_t dest,
                                                                            sl_zigbee_aps_option_t aps_options,
                                                                            sl_802154_long_addr_t target);

sl_status_t sli_zigbee_stack_test_zdo_generate_security_decommission_req(sl_802154_short_addr_t destination,
                                                                         bool encrypt,
                                                                         uint8_t *eui64_list,
                                                                         uint8_t counts);

sl_status_t sli_zigbee_stack_zigbee_remove_child(sl_802154_short_addr_t childId,
                                                 uint8_t options);

#endif // PRO_COMPLIANCE_STACK_INTERFACE_INTERNAL_DEF_H
