/***************************************************************************//**
 * @file
 * @brief
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include "sl_cli.h"
#include "sl_zigbee_debug_print.h"
#ifdef EZSP_HOST
#include "app/util/ezsp/ezsp-protocol.h"
#include "app/util/ezsp/ezsp.h"
#ifdef RAIL_SIM_EZSP
#include "af_main.h"
#else //!RAIL_SIM_EZSP
#include "app/framework/include/af-types.h"
#endif //RAIL_SIM_EZSP
#else //!EZSP_HOST
#include "sl_rail_util_ieee802154_phy_select.h"
#endif //EZSP_HOST

#define PHY_COUNT ((sizeof(phyNames) / sizeof(phyNames[0])) - 1)
static const char * const phyNames[] = {
  "IEEE802154_2P4_MODE_DEFAULT",
  "IEEE802154_2P4_MODE_ANT_DIV",
  "IEEE802154_2P4_MODE_COEX",
  "IEEE802154_2P4_MODE_ANT_DIV_COEX",
  "IEEE802154_2P4_MODE_FEM",
  "IEEE802154_2P4_MODE_ANT_DIV_FEM",
  "IEEE802154_2P4_MODE_COEX_FEM",
  "IEEE802154_2P4_MODE_ANT_DIV_COEX_FEM",
  "INVALID_PHY_SELECTION"
};

//-----------------------------------------------------------------------------
// Get Active Radio PHY
// Console Command : "plugin phy-select get-active-phy"
// Console Response: "Active Radio PHY:<Active Radio PHY>"
void sl_util_af_get_active_radio_phy(sl_cli_command_arg_t *arguments)
{
  (void)arguments;
  uint8_t activePhy;
#ifdef EZSP_HOST
  uint8_t valueLength = sizeof(activePhy);
  sl_zigbee_ezsp_get_value(SL_ZIGBEE_EZSP_VALUE_ACTIVE_RADIO_CONFIG, &valueLength, &activePhy);
#else //!EZSP_HOST
  activePhy = (uint8_t)sl_rail_util_ieee802154_get_active_radio_config();
#endif //EZSP_HOST

  if (activePhy >= PHY_COUNT) {
    activePhy = PHY_COUNT;
  }
  sl_zigbee_app_debug_print("Active Radio PHY:%s", phyNames[activePhy]);
}
