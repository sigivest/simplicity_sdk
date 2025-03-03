/***************************************************************************//**
 * @file
 * @brief Definitions for the Smart Energy Registration plugin.
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

#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#endif
#include "smart-energy-registration-config.h"
#if (SL_ZIGBEE_AF_PLUGIN_SMART_ENERGY_REGISTRATION_ESI_REDISCOVERY == 1)
#define ESI_REDISCOVERY
#endif
#if (SL_ZIGBEE_AF_PLUGIN_SMART_ENERGY_REGISTRATION_ALLOW_NON_ESI_TIME_SERVERS == 1)
#define ALLOW_NON_ESI_TIME_SERVERS
#endif

/**
 * @defgroup smart-energy-registration  Smart Energy Registration
 * @ingroup component
 * @brief API and Callbacks for the Smart Energy Registration Component
 *
 * Silicon Labs implementation of Smart Energy Registration.
 * The component performs key establishment with the trust center when
 * the node joins a network.  If the application implements Messaging, Price,
 * or DRLC client clusters, the component also discovers ESIs on the network
 * and performs partner link key exchange and binding with each ESI it discovers.
 * ESI discovery requires the ESI Management component, in which the number of
 * supported ESIs can be configured. The frequency with which ESIs are
 * rediscovered is configurable. These options are ignored if the
 * application does not implement Messaging, Price, or DRLC client clusters.
 * Registration is not required for trust centers, so this component should be
 * disabled if the device is acting as the trust center.
 *
 */

/**
 * @addtogroup smart-energy-registration
 * @{
 */

#ifndef SL_ZIGBEE_AF_PLUGIN_SMART_ENERGY_REGISTRATION_ERROR_LIMIT
#define SL_ZIGBEE_AF_PLUGIN_SMART_ENERGY_REGISTRATION_ERROR_LIMIT 3
#endif //SL_ZIGBEE_AF_PLUGIN_SMART_ENERGY_REGISTRATION_ERROR_LIMIT

#define SL_ZIGBEE_AF_PLUGIN_SMART_ENERGY_REGISTRATION_DELAY_INITIAL    (MILLISECOND_TICKS_PER_SECOND << 2)
#define SL_ZIGBEE_AF_PLUGIN_SMART_ENERGY_REGISTRATION_DELAY_RETRY      (MILLISECOND_TICKS_PER_SECOND << 5)
#define SL_ZIGBEE_AF_PLUGIN_SMART_ENERGY_REGISTRATION_DELAY_BRIEF      (MILLISECOND_TICKS_PER_SECOND << 3)
#define SL_ZIGBEE_AF_PLUGIN_SMART_ENERGY_REGISTRATION_DELAY_RESUME     (MILLISECOND_TICKS_PER_SECOND >> 1)
#define SL_ZIGBEE_AF_PLUGIN_SMART_ENERGY_REGISTRATION_DELAY_TRANSITION (MILLISECOND_TICKS_PER_SECOND >> 2)
#define SL_ZIGBEE_AF_PLUGIN_SMART_ENERGY_REGISTRATION_DELAY_NONE       0

#ifdef SL_CATALOG_ZIGBEE_SMART_ENERGY_REGISTRATION_PRESENT

// ESI discovery and binding is only required if the device implements certain
// Smart Energy clusters.  If it doesn't implement these clusters (e.g., it is
// an ESI), the ESI discovery and binding process can be skipped altogether.  If
// discovery is required, it shall be repeated on a period of no more than once
// every three hours and no less than once every 24 hours, according to section
// 5.4.9.2 of 105638r09.
  #if (defined(ZCL_USING_DEMAND_RESPONSE_LOAD_CONTROL_CLUSTER_CLIENT) \
  || defined(ZCL_USING_MESSAGING_CLUSTER_CLIENT)                      \
  || defined(ZCL_USING_PRICE_CLUSTER_CLIENT))
    #define SL_ZIGBEE_AF_PLUGIN_SMART_ENERGY_REGISTRATION_ESI_DISCOVERY_REQUIRED

// The plugin option for the discovery period is specified in hours and is
// converted here to milliseconds.  If rediscovery is not enabled, the delay
// is not defined, and the plugin won't schedule an event to rediscover ESIs.
    #ifdef ESI_REDISCOVERY
      #define SL_ZIGBEE_AF_PLUGIN_SMART_ENERGY_REGISTRATION_DELAY_PERIOD \
  (SL_ZIGBEE_AF_PLUGIN_SMART_ENERGY_REGISTRATION_ESI_DISCOVERY_PERIOD * MILLISECOND_TICKS_PER_HOUR)
extern uint32_t sli_zigbee_af_smart_energy_registration_discovery_period;
    #endif
  #endif

// ReadAttributesResponse callback is used to determine the time source.
// It is only used if SE registration is enabled and then only if one of these
// two options are enabled.
  #if defined(SL_ZIGBEE_AF_PLUGIN_SMART_ENERGY_REGISTRATION_ESI_DISCOVERY_REQUIRED) \
  || defined(ALLOW_NON_ESI_TIME_SERVERS)
    #define SL_ZIGBEE_AF_PLUGIN_SMART_ENERGY_REGISTRATION_TIME_SOURCE_REQUIRED
void sli_zigbee_af_smart_energy_registration_read_attributes_response_callback(uint8_t *buffer,
                                                                               uint16_t bufLen);
  #endif

#endif // SL_CATALOG_ZIGBEE_SMART_ENERGY_REGISTRATION_PRESENT

/** @} */ // end of smart-energy-registration

uint8_t sli_zigbee_af_smart_energy_registration_trust_center_key_establishment_endpoint(void);
