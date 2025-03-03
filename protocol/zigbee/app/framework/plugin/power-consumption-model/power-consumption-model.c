/***************************************************************************//**
 * @file
 * @brief Routines for the Power Consumption Model plugin, which implements the
 *        Light Bulb Electrical Measurement server cluster.
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
#include "power-consumption-model.h"

#include "power-consumption-model-config.h"

//-----------------------------------------------------------------------------
// Plugin private macros
#define SIMPLE_METERING_DIVISOR 1000
#define SIMPLE_METERING_MULTIPLIER 1

#define WATT_TO_MILLI_WATT   1000
#define MILLI_SEC_TO_SECONDS 1000

// To preserve significant bits during calculations, we need to scale the power
// data during the integer math computations.
#define POWER_SCALE_FACTOR 1000

// 160 Watt Maximum
#define DEFAULT_POWER_RATING_MIILI_WATT                             \
  (SL_ZIGBEE_AF_PLUGIN_POWER_CONSUMPTION_MODEL_DEFAULT_POWER_RATING \
   * WATT_TO_MILLI_WATT)
#define SAMPLE_PERIOD_MILLI_SECOND                                       \
  ((SL_ZIGBEE_AF_PLUGIN_POWER_CONSUMPTION_MODEL_POLLING_TIME_INTERVAL_MS \
    * MILLISECOND_TICKS_PER_SECOND)                                      \
   / MILLI_SEC_TO_SECONDS)
#define OFF_POWER_CONSUMPTION_MILLI_WATT \
  SL_ZIGBEE_AF_PLUGIN_POWER_CONSUMPTION_MODEL_OFF_POWER_CONSUMPTION_MILLI_WATT
#define ON_POWER_CONSUMPTION_MILLI_WATT                             \
  (SL_ZIGBEE_AF_PLUGIN_POWER_CONSUMPTION_MODEL_ON_POWER_CONSUMPTION \
   * WATT_TO_MILLI_WATT)

#define SECONDS_PER_HOUR 3600
// Assuming that turning on point for level control is 13
#define LEVEL_CONTROL_AT_ON 13
#define LEVEL_CONTROL_AT_MAX 255   // maximum value for level control

// Formatting for instantaneous value and summation
// Three digit before and after the decimal point
#define CUMULATIVE_ENERGY_FORMAT 0x23
#define INSTANTANEOUS_POWER_FORMAT 0x23
// Unit of measurement for electrical is kWh and W
#define ELECTRICAL_UNIT_MEASUREMENT_KWH_AND_KW 0x00
// Electrical metering is defined to be 0
#define ELECTRICAL_METERING_DEVICE_TYPE 0x00
// Set 0 for no error status (good status)
#define METERING_DEVICE_GOOD_STATUS 0x00
// Set 0 for historical consumption Formatting
#define HISTORICAL_CONSUMPTION_FORMATTING 0x00

// definition to use for energy consumption and instant power attribute write
#define CUMULATIVE_ENERGY_CONSUMPTION_ATTRIBUTE_IN_BYTE 6
#define INSTANTANEOUS_POWER_DEMAND_ATTRIBUTE_IN_BYTE 3
#define SIZE_OF_24_BIT_ATTRUTE_IN_BYTE 3

#define SUCCESS_CODE 0
//-----------------------------------------------------------------------------
static uint8_t lightbulbEndpoint;   // light bulb is the only endpoint
static uint32_t totalEnergyConsumptionMilliWattSecond;

//-----------------------------------------------------------------------------
// Forward Declaration of private functions
// Conversion function from level to milli watt hour
static uint8_t readLevelControl(uint8_t deviceEndpoint);
static uint32_t computeInstantaneousPowerMilliWatt(uint8_t levelControl);
static uint32_t instantaneousPowerToMilliWattPerPeriod(uint32_t instantPowerMilliWatt,
                                                       uint16_t pollingTimeIntervalMs);
static uint32_t energyConsumptionWattHour(uint32_t energyConsumptionPerPeriod);
static void updateEnergyConsumptionDataAttribute(uint32_t energyConsumedWhr,
                                                 uint32_t instantPowerMilliWatt);
static void simpleMeteringClusterSetup(void);
static void refreshingEnergyConsumptionData(void);
static void lightbulbInit(void);

///////////////////////////////////////////////////////////////////////////////
// Plugin event handlers
// Event variable to create an event
static sl_zigbee_af_event_t readEvent;
static void readEventHandler(sl_zigbee_af_event_t * event);

//-----------------------------------------------------------------------------
// This function reads on_off and level control cluster and returns 0 when
// lightbulb is off and level control value when lightbulb is on
// @params:
//      deviceEndpoint - endpoint of the lightbulb
static uint8_t readLevelControl(uint8_t deviceEndpoint)
{
  bool lighbulbOn;
  if (sl_zigbee_af_contains_server(lightbulbEndpoint,
                                   ZCL_SIMPLE_METERING_CLUSTER_ID)) {
    sl_zigbee_af_read_server_attribute(deviceEndpoint,
                                       ZCL_ON_OFF_CLUSTER_ID,
                                       ZCL_ON_OFF_ATTRIBUTE_ID,
                                       (uint8_t *)&lighbulbOn,
                                       sizeof(bool));
    if (lighbulbOn) {
      uint8_t levelControl;
      sl_zigbee_af_read_server_attribute(deviceEndpoint,
                                         ZCL_LEVEL_CONTROL_CLUSTER_ID,
                                         ZCL_CURRENT_LEVEL_ATTRIBUTE_ID,
                                         (uint8_t *)&levelControl,
                                         sizeof(levelControl));
      return levelControl;
    }
  }
  return 0;
}

// This function calculates instantaneous power using level control and it
// returns value in micro watt.
// Note: This function uses custom formula to calculate instantaneous power.
//       If value of the level control attribute is below (13), then it is
//       assumed that lightbulb is off and returns constant power consumption
//       specified at app builder. Otherwise, it is a linear scale from
//       on-power-consumption to max-power (or default power rating).
// @params:
//      levelControl - level control of the light
static uint32_t computeInstantaneousPowerMilliWatt(uint8_t levelControl)
{
  int32_t instantPowerMilliWatt, scaler, shifter;
  int16_t levelControlCasted = (int16_t)levelControl;
  // If lightbulb level is greater than On threshold, find a scaler and shifter
  // to use to find instantaneous power; otherwise, use different formula to
  // find scaler and shifter below threshold
  // Formula use is Ax+B=y where x is level control and y is instant power
  // use two points to find A and B where A is scaler and B is shifter
  if (levelControl > LEVEL_CONTROL_AT_ON) {
    scaler = ((DEFAULT_POWER_RATING_MIILI_WATT
               - ON_POWER_CONSUMPTION_MILLI_WATT)
              * POWER_SCALE_FACTOR)
             / (LEVEL_CONTROL_AT_MAX
                - LEVEL_CONTROL_AT_ON);
    shifter = (((LEVEL_CONTROL_AT_MAX * ON_POWER_CONSUMPTION_MILLI_WATT)
                - LEVEL_CONTROL_AT_ON * DEFAULT_POWER_RATING_MIILI_WATT)
               * POWER_SCALE_FACTOR)
              / (LEVEL_CONTROL_AT_MAX
                 - LEVEL_CONTROL_AT_ON);
  } else {
    scaler = ((ON_POWER_CONSUMPTION_MILLI_WATT
               - OFF_POWER_CONSUMPTION_MILLI_WATT)
              * POWER_SCALE_FACTOR)
             / (LEVEL_CONTROL_AT_ON - 1);
    shifter = OFF_POWER_CONSUMPTION_MILLI_WATT * POWER_SCALE_FACTOR;
  }

  instantPowerMilliWatt = scaler * levelControlCasted + shifter;

  if (instantPowerMilliWatt < 0) {
    sl_zigbee_af_app_println("Calculation error");
    instantPowerMilliWatt = 0;
  }
  instantPowerMilliWatt = instantPowerMilliWatt / POWER_SCALE_FACTOR;

  return (uint32_t)instantPowerMilliWatt;
}

// This function calculates power consumption per polling period or energy
// consumed by lightbulb and it returns energy consumption in micro watt hour.
// @params:
//    instantPowerMicroWatt - Instantaneous power in micro watt
//    pollingTimeIntervalMs - polling period in micro second

static uint32_t instantaneousPowerToMilliWattPerPeriod(uint32_t instantPowerMilliWatt,
                                                       uint16_t pollingTimeIntervalMs)
{
  uint32_t milliWattSecond;

  milliWattSecond = (instantPowerMilliWatt * pollingTimeIntervalMs)
                    / MILLISECOND_TICKS_PER_SECOND;

  return milliWattSecond;
}

// This function caculates energy consumption with previous energy in micro watt
// and it returns energy consumption in watt hour
// @params:
//    energyConsumedPerPeriodMicroWattHour - energy consumption period period in
//                                            in micro watt hour
static uint32_t energyConsumptionWattHour(uint32_t energyConsumptionPerPeriod)
{
  uint32_t energyConsumptionWh;

  totalEnergyConsumptionMilliWattSecond += energyConsumptionPerPeriod;
  energyConsumptionWh = totalEnergyConsumptionMilliWattSecond
                        / (WATT_TO_MILLI_WATT * SECONDS_PER_HOUR);
  totalEnergyConsumptionMilliWattSecond =
    totalEnergyConsumptionMilliWattSecond
    % (WATT_TO_MILLI_WATT * SECONDS_PER_HOUR);

  return energyConsumptionWh;
}

// Updating instantaneous power and cumulative energy attribute
// @params:
//    energyConsumedWhr - energy consumed in watt hour
//    instantPowerMilliWatt - instantaneous power in milli watt
// 48 bit (6 byte array and read it from that) for cumulative energy
static void updateEnergyConsumptionDataAttribute(uint32_t energyConsumedWhr,
                                                 uint32_t instantPowerMilliWatt)
{
  uint8_t i;
  uint8_t
    previousCumulativeEnergy[CUMULATIVE_ENERGY_CONSUMPTION_ATTRIBUTE_IN_BYTE];
  uint64_t totalEnergyConsumption = (uint64_t)energyConsumedWhr;
  int32_t instantaneousDemandPowerMilliWatt = (int32_t)instantPowerMilliWatt;

  if (sl_zigbee_af_contains_server(lightbulbEndpoint,
                                   ZCL_SIMPLE_METERING_CLUSTER_ID)) {
    sl_zigbee_af_read_server_attribute(lightbulbEndpoint,
                                       ZCL_SIMPLE_METERING_CLUSTER_ID,
                                       ZCL_CURRENT_SUMMATION_DELIVERED_ATTRIBUTE_ID,
                                       (uint8_t *)(&previousCumulativeEnergy),
                                       CUMULATIVE_ENERGY_CONSUMPTION_ATTRIBUTE_IN_BYTE);
    for (i = 0; i < CUMULATIVE_ENERGY_CONSUMPTION_ATTRIBUTE_IN_BYTE; i++) {
      totalEnergyConsumption += (uint64_t)(previousCumulativeEnergy[i] << (i * 8));
    }

    sl_zigbee_af_write_server_attribute(lightbulbEndpoint,
                                        ZCL_SIMPLE_METERING_CLUSTER_ID,
                                        ZCL_CURRENT_SUMMATION_DELIVERED_ATTRIBUTE_ID,
                                        (uint8_t *)(&totalEnergyConsumption),
                                        ZCL_INT48U_ATTRIBUTE_TYPE);

    sl_zigbee_af_write_server_attribute(lightbulbEndpoint,
                                        ZCL_SIMPLE_METERING_CLUSTER_ID,
                                        ZCL_INSTANTANEOUS_DEMAND_ATTRIBUTE_ID,
                                        (uint8_t *)(&instantaneousDemandPowerMilliWatt),
                                        ZCL_INT24S_ATTRIBUTE_TYPE);
  }
}

// This function refreshes lightbulb data by the following pseudocode:
// 1. Read level control
// 2. Calculate instantaneous power
// 3. Calculate energy consumption per period
// 4. Calculate energy consumption with previous energy stored
// 5. Update lightbulb data
// @params:
static void refreshingEnergyConsumptionData(void)
{
  uint8_t levelControl;
  uint32_t energyConsumptionWh, energyConsumptionMilliWattSecond;
  uint32_t instantaneousPowerMilliWatt;

  levelControl = readLevelControl(lightbulbEndpoint);
  instantaneousPowerMilliWatt = computeInstantaneousPowerMilliWatt(
    levelControl);
  energyConsumptionMilliWattSecond = instantaneousPowerToMilliWattPerPeriod(
    instantaneousPowerMilliWatt,
    SAMPLE_PERIOD_MILLI_SECOND);
  energyConsumptionWh = energyConsumptionWattHour(energyConsumptionMilliWattSecond);
  updateEnergyConsumptionDataAttribute(energyConsumptionWh,
                                       instantaneousPowerMilliWatt);
}

//-----------------------------------------------------------------------------
// This function initialize lightbulb data
// @params:
static void lightbulbInit(void)
{
  lightbulbEndpoint = sl_zigbee_af_endpoint_from_index(0);
  totalEnergyConsumptionMilliWattSecond = 0;
}

#include "sl_cli.h"
void sl_zigbee_af_power_consumption_model_print_attributes_command(sl_cli_command_arg_t *arguments)
{
  sl_zigbee_af_power_consumption_model_print_attributes();
}

// This function read cumulative energy and instantaneous power and print it
void sl_zigbee_af_power_consumption_model_print_attributes(void)
{
  uint32_t totalEnergyConsumptionWh = 0;
  uint8_t i, cumulativeEnergy[CUMULATIVE_ENERGY_CONSUMPTION_ATTRIBUTE_IN_BYTE];
  uint32_t instantDemandPower = 0;
  uint8_t instantPowerMilliW[INSTANTANEOUS_POWER_DEMAND_ATTRIBUTE_IN_BYTE];

  if (sl_zigbee_af_contains_server(lightbulbEndpoint, ZCL_SIMPLE_METERING_CLUSTER_ID)
      == false) {
    sl_zigbee_af_app_println("Error:  need to include the simple metering cluster");
    return;
  }
  assert(SL_ZIGBEE_ZCL_STATUS_SUCCESS
         == sl_zigbee_af_read_server_attribute(lightbulbEndpoint,
                                               ZCL_SIMPLE_METERING_CLUSTER_ID,
                                               ZCL_CURRENT_SUMMATION_DELIVERED_ATTRIBUTE_ID,
                                               (uint8_t *)(&cumulativeEnergy),
                                               CUMULATIVE_ENERGY_CONSUMPTION_ATTRIBUTE_IN_BYTE));
  for (i = 0; i < CUMULATIVE_ENERGY_CONSUMPTION_ATTRIBUTE_IN_BYTE; i++) {
    totalEnergyConsumptionWh += (uint32_t)(cumulativeEnergy[i] << (i * 8));
  }
  sl_zigbee_af_app_println("Cumulative Energy: %d Wh", totalEnergyConsumptionWh);

  assert(SL_ZIGBEE_ZCL_STATUS_SUCCESS
         == sl_zigbee_af_read_server_attribute(lightbulbEndpoint,
                                               ZCL_SIMPLE_METERING_CLUSTER_ID,
                                               ZCL_INSTANTANEOUS_DEMAND_ATTRIBUTE_ID,
                                               (uint8_t *)(&instantPowerMilliW),
                                               INSTANTANEOUS_POWER_DEMAND_ATTRIBUTE_IN_BYTE));
  for (i = 0; i < INSTANTANEOUS_POWER_DEMAND_ATTRIBUTE_IN_BYTE; i++) {
    instantDemandPower += (uint32_t)(instantPowerMilliW[i] << (i * 8));
  }
  sl_zigbee_af_app_println("Instant Demand Power: %d.%d W",
                           (instantDemandPower * SIMPLE_METERING_MULTIPLIER)
                           / SIMPLE_METERING_DIVISOR,
                           (instantDemandPower * SIMPLE_METERING_MULTIPLIER)
                           % SIMPLE_METERING_DIVISOR);
}

/// Event triggering
// Initiating call back server

void sl_zigbee_af_power_consumption_model_init_cb(uint8_t init_level)
{
  switch (init_level) {
    case SL_ZIGBEE_INIT_LEVEL_EVENT:
    {
      sl_zigbee_af_event_init(&readEvent,
                              readEventHandler);
      break;
    }

    case SL_ZIGBEE_INIT_LEVEL_LOCAL_DATA:
    {
      lightbulbInit();
      break;
    }

    case SL_ZIGBEE_INIT_LEVEL_DONE:
    {
      simpleMeteringClusterSetup();
      sl_zigbee_af_event_set_active(&readEvent);
      break;
    }
  }
}

// After event triggered, this function is called
static void readEventHandler(sl_zigbee_af_event_t * event)
{
  sl_zigbee_af_event_set_delay_ms(&readEvent,
                                  SAMPLE_PERIOD_MILLI_SECOND);
  refreshingEnergyConsumptionData();
}

// This function sets up simple metering cluster.
// @params:
static void simpleMeteringClusterSetup(void)
{
  uint8_t cumulativeEnergyFormat = CUMULATIVE_ENERGY_FORMAT,
          instantaneousPowerFormat = INSTANTANEOUS_POWER_FORMAT,
          electricalMeasurementUnit = ELECTRICAL_UNIT_MEASUREMENT_KWH_AND_KW,
          electricalMeteringDeviceType = ELECTRICAL_METERING_DEVICE_TYPE,
          meteringDeviceStatus = METERING_DEVICE_GOOD_STATUS,
          historicalConsumptionFormatting = HISTORICAL_CONSUMPTION_FORMATTING;

  uint32_t attributeDivisor = SIMPLE_METERING_DIVISOR,
           attributeMultiplier = SIMPLE_METERING_MULTIPLIER;

  if (sl_zigbee_af_contains_server(lightbulbEndpoint,
                                   ZCL_SIMPLE_METERING_CLUSTER_ID)) {
    // Setting up formatting for cumulative energy (kWh) and instantaneous power (Wh)
    assert(SL_ZIGBEE_ZCL_STATUS_SUCCESS
           == sl_zigbee_af_write_server_attribute(lightbulbEndpoint,
                                                  ZCL_SIMPLE_METERING_CLUSTER_ID,
                                                  ZCL_SUMMATION_FORMATTING_ATTRIBUTE_ID,
                                                  (uint8_t *)(&cumulativeEnergyFormat),
                                                  ZCL_BITMAP8_ATTRIBUTE_TYPE));
    assert(SL_ZIGBEE_ZCL_STATUS_SUCCESS
           == sl_zigbee_af_write_server_attribute(lightbulbEndpoint,
                                                  ZCL_SIMPLE_METERING_CLUSTER_ID,
                                                  ZCL_DEMAND_FORMATTING_ATTRIBUTE_ID,
                                                  (uint8_t *)(&instantaneousPowerFormat),
                                                  ZCL_BITMAP8_ATTRIBUTE_TYPE));
    // Setting up device status (no errors)
    assert(SL_ZIGBEE_ZCL_STATUS_SUCCESS
           == sl_zigbee_af_write_server_attribute(lightbulbEndpoint,
                                                  ZCL_SIMPLE_METERING_CLUSTER_ID,
                                                  ZCL_STATUS_ATTRIBUTE_ID,
                                                  (uint8_t *)(&meteringDeviceStatus),
                                                  ZCL_BITMAP8_ATTRIBUTE_TYPE));

    // Setting up meter type (electrical metering)
    assert(SL_ZIGBEE_ZCL_STATUS_SUCCESS
           == sl_zigbee_af_write_server_attribute(lightbulbEndpoint,
                                                  ZCL_SIMPLE_METERING_CLUSTER_ID,
                                                  ZCL_METERING_DEVICE_TYPE_ATTRIBUTE_ID,
                                                  (uint8_t *)(&electricalMeteringDeviceType),
                                                  ZCL_BITMAP8_ATTRIBUTE_TYPE));

    // setting up unit of measurement (kWh for cumulative and W for instant)
    assert(SL_ZIGBEE_ZCL_STATUS_SUCCESS
           == sl_zigbee_af_write_server_attribute(lightbulbEndpoint,
                                                  ZCL_SIMPLE_METERING_CLUSTER_ID,
                                                  ZCL_UNIT_OF_MEASURE_ATTRIBUTE_ID,
                                                  (uint8_t *)(&electricalMeasurementUnit),
                                                  ZCL_ENUM8_ATTRIBUTE_TYPE));

    // setting up historical consumption formatting
    assert(SL_ZIGBEE_ZCL_STATUS_SUCCESS
           == sl_zigbee_af_write_server_attribute(lightbulbEndpoint,
                                                  ZCL_SIMPLE_METERING_CLUSTER_ID,
                                                  ZCL_HISTORICAL_CONSUMPTION_FORMATTING_ATTRIBUTE_ID,
                                                  (uint8_t *)(&historicalConsumptionFormatting),
                                                  ZCL_BITMAP8_ATTRIBUTE_TYPE));

    // setting up muliplier and divisor attribute
    assert(SL_ZIGBEE_ZCL_STATUS_SUCCESS
           == sl_zigbee_af_write_server_attribute(lightbulbEndpoint,
                                                  ZCL_SIMPLE_METERING_CLUSTER_ID,
                                                  ZCL_DIVISOR_ATTRIBUTE_ID,
                                                  (uint8_t *)(&attributeDivisor),
                                                  ZCL_INT24U_ATTRIBUTE_TYPE));
    assert(SL_ZIGBEE_ZCL_STATUS_SUCCESS
           == sl_zigbee_af_write_server_attribute(lightbulbEndpoint,
                                                  ZCL_SIMPLE_METERING_CLUSTER_ID,
                                                  ZCL_MULTIPLIER_ATTRIBUTE_ID,
                                                  (uint8_t *)(&attributeMultiplier),
                                                  ZCL_INT24U_ATTRIBUTE_TYPE));
  }
}
