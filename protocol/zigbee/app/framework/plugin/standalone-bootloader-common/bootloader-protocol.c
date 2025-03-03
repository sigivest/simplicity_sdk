/***************************************************************************//**
 * @file
 * @brief This file defines the proprietary Ember standalone bootloader messages.
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

#include "app/framework/include/af.h"
#include "bootloader-protocol.h"

//------------------------------------------------------------------------------
// Globals

//------------------------------------------------------------------------------
// Functions

bool sl_zigbee_af_standalone_bootloader_common_check_incoming_message(uint8_t length,
                                                                      uint8_t* message)
{
  // This is assumed to be an Ember bootload message.  In other words,
  // the MAC data payload.
  if (length < BOOTLOAD_MESSAGE_OVERHEAD) {
    bootloadPrintln("Error: Got short bootload message, length: %d", length);
    return false;
  }

  if (message[OFFSET_VERSION] != BOOTLOAD_PROTOCOL_VERSION) {
    bootloadPrintln("Error: Protocol version in bootload message (%d) does not match mine (%d).",
                    message[OFFSET_VERSION],
                    BOOTLOAD_PROTOCOL_VERSION);
    return false;
  }

  return true;
}

sl_status_t sl_zigbee_af_standalone_bootloader_common_send_message(bool isBroadcast,
                                                                   sl_802154_long_addr_t targetEui,
                                                                   uint8_t length,
                                                                   uint8_t* message)
{
  sl_status_t status = sli_zigbee_af_send_bootload_message(isBroadcast,
                                                           targetEui,
                                                           length,
                                                           message);
  if (SL_STATUS_OK != status) {
    bootloadPrintln("Failed to send bootload message type: 0x%X, status: 0x%X",
                    message[1],
                    status);
  }
  return status;
}

uint8_t sl_zigbee_af_standalone_bootloader_common_make_header(uint8_t *message, uint8_t type)
{
  //common header values
  message[0] = BOOTLOAD_PROTOCOL_VERSION;
  message[1] = type;

  // for XMODEM_QUERY and XMODEM_EOT messages, this represents the end of the
  // header.  However, for XMODEM_QRESP, XMODEM_SOH, XMODEM_ACK, XMODEM_NAK
  // messages, there are additional values that need to be added.
  // Note that the application will not have to handle creation of
  // over the air XMODEM_ACK and XMODEM_NAK since these are all handled by
  // the bootloader on the target node.

  return 2;
}

// Make sure we have a NULL delemiter and ignore 0xFF characters
void sli_zigbee_af_standalone_bootloader_common_print_hardware_tag(uint8_t* text)
{
  uint8_t hardwareTagString[SL_ZIGBEE_AF_STANDALONE_BOOTLOADER_HARDWARE_TAG_LENGTH + 1];
  uint8_t i;
  memset(hardwareTagString, 0, SL_ZIGBEE_AF_STANDALONE_BOOTLOADER_HARDWARE_TAG_LENGTH + 1);
  for (i = 0; i < SL_ZIGBEE_AF_STANDALONE_BOOTLOADER_HARDWARE_TAG_LENGTH; i++) {
    if (text[i] == 0xFF) {
      // Last
      i = SL_ZIGBEE_AF_STANDALONE_BOOTLOADER_HARDWARE_TAG_LENGTH;
    } else {
      hardwareTagString[i] = text[i];
    }
  }
  bootloadPrintln("%s", hardwareTagString);
}
