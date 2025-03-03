/***************************************************************************//**
 * @brief RAIL Configuration
 * @details
 *   WARNING: Auto-Generated Radio Config Header  -  DO NOT EDIT
 *   Radio Configurator Version: 2304.4.4
 *   RAIL Adapter Version: 2.4.33
 *   RAIL Compatibility: 2.x
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef __SL_RAIL_BLE_CONFIG_38M4HZ_H__
#define __SL_RAIL_BLE_CONFIG_38M4HZ_H__

#include <stdint.h>
#include "rail_types.h"

extern const uint32_t sl_rail_ble_phy_1Mbps_viterbi_38M4Hz_modemConfigBase[];

extern const uint32_t sl_rail_ble_phy_1Mbps_viterbi_38M4Hz_0_37_modemConfig[];
extern const uint32_t sl_rail_ble_phy_2Mbps_viterbi_38M4Hz_0_37_modemConfig[];
extern const uint32_t sl_rail_ble_phy_2Mbps_aox_38M4Hz_0_34_modemConfig[];
extern const uint32_t sl_rail_ble_phy_125kbps_38M4Hz_0_37_modemConfig[];
extern const uint32_t sl_rail_ble_phy_500kbps_38M4Hz_0_37_modemConfig[];
extern const uint32_t sl_rail_ble_phy_simulscan_38M4Hz_0_37_modemConfig[];

#define RAIL0_SL_RAIL_BLE_PHY_1MBPS_VITERBI_38M4HZ_PHY_BLUETOOTH_1M_AOX_PROD
#define RAIL0_SL_RAIL_BLE_PHY_1MBPS_VITERBI_38M4HZ_PROFILE_BASE
extern const RAIL_ChannelConfig_t sl_rail_ble_phy_1Mbps_viterbi_38M4Hz_channelConfig;
extern const RAIL_ChannelConfigEntry_t sl_rail_ble_phy_1Mbps_viterbi_38M4Hz_channels[];

#define RAIL0_SL_RAIL_BLE_PHY_2MBPS_VITERBI_38M4HZ_PHY_BLUETOOTH_2M_38M4HZ_PROD
#define RAIL0_SL_RAIL_BLE_PHY_2MBPS_VITERBI_38M4HZ_PROFILE_BASE
extern const RAIL_ChannelConfig_t sl_rail_ble_phy_2Mbps_viterbi_38M4Hz_channelConfig;
extern const RAIL_ChannelConfigEntry_t sl_rail_ble_phy_2Mbps_viterbi_38M4Hz_channels[];

#define RAIL0_SL_RAIL_BLE_PHY_2MBPS_AOX_38M4HZ_PHY_BLUETOOTH_2M_38M4HZ_AOX_PROD
#define RAIL0_SL_RAIL_BLE_PHY_2MBPS_AOX_38M4HZ_PROFILE_BASE
extern const RAIL_ChannelConfig_t sl_rail_ble_phy_2Mbps_aox_38M4Hz_channelConfig;
extern const RAIL_ChannelConfigEntry_t sl_rail_ble_phy_2Mbps_aox_38M4Hz_channels[];

#define RAIL0_SL_RAIL_BLE_PHY_125KBPS_38M4HZ_PHY_BLUETOOTH_LR_125K_PROD
#define RAIL0_SL_RAIL_BLE_PHY_125KBPS_38M4HZ_PROFILE_BASE
extern const RAIL_ChannelConfig_t sl_rail_ble_phy_125kbps_38M4Hz_channelConfig;
extern const RAIL_ChannelConfigEntry_t sl_rail_ble_phy_125kbps_38M4Hz_channels[];

#define RAIL0_SL_RAIL_BLE_PHY_500KBPS_38M4HZ_PHY_BLUETOOTH_LR_500K_PROD
#define RAIL0_SL_RAIL_BLE_PHY_500KBPS_38M4HZ_PROFILE_BASE
extern const RAIL_ChannelConfig_t sl_rail_ble_phy_500kbps_38M4Hz_channelConfig;
extern const RAIL_ChannelConfigEntry_t sl_rail_ble_phy_500kbps_38M4Hz_channels[];

#define RAIL0_SL_RAIL_BLE_PHY_SIMULSCAN_38M4HZ_PHY_BLUETOOTH_1M_CONCURRENT_PROD
#define RAIL0_SL_RAIL_BLE_PHY_SIMULSCAN_38M4HZ_PROFILE_BASE
extern const RAIL_ChannelConfig_t sl_rail_ble_phy_simulscan_38M4Hz_channelConfig;
extern const RAIL_ChannelConfigEntry_t sl_rail_ble_phy_simulscan_38M4Hz_channels[];

#endif // __SL_RAIL_BLE_CONFIG_38M4HZ_H__
