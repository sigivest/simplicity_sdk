/***************************************************************************//**
 * @file
 * @brief Implemented routines for prepayment server.
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
#include "app/framework/util/common.h"
#include "app/framework/util/util.h"
#include "prepayment-debt-log.h"
#include "prepayment-debt-schedule.h"
#include "prepayment-tick.h"

#include "prepayment-server-config.h"

static void debtScheduleSetNextCollectionTimeSec(uint8_t debtType);

// 3 Debt collections may be scheduled - #1, #2, and #3.
#define DEBT_SCHEDULE_TABLE_SIZE    3
#define INVALID_DEBT_COLLECTION_TIME  0xFFFFFFFF
#define INVALID_ISSUER_EVENT_ID   0xFFFFFFFF
#define DEBT_TYPE_INVALID  0xFE

enum {
  COLLECTION_FREQUENCY_PER_HOUR    = 0,
  COLLECTION_FREQUENCY_PER_DAY     = 1,
  COLLECTION_FREQUENCY_PER_WEEK    = 2,
  COLLECTION_FREQUENCY_PER_MONTH   = 3,
  COLLECTION_FREQUENCY_PER_QUARTER = 4,

  COLLECTION_FREQUENCY_INVALID     = 0xFF
};

sli_zigbee_af_debt_schedule_entry DebtSchedule[DEBT_SCHEDULE_TABLE_SIZE];

void sl_zigbee_af_prepayment_server_init_debt_schedule(void)
{
  uint8_t i;
  for ( i = 0; i < DEBT_SCHEDULE_TABLE_SIZE; i++ ) {
    DebtSchedule[i].issuerEventId = INVALID_ISSUER_EVENT_ID;
    DebtSchedule[i].nextCollectionTimeUtc = INVALID_DEBT_COLLECTION_TIME;
    DebtSchedule[i].firstCollectionTimeSec = INVALID_DEBT_COLLECTION_TIME;
    DebtSchedule[i].collectionFrequency = COLLECTION_FREQUENCY_INVALID;
    DebtSchedule[i].debtType = DEBT_TYPE_INVALID;
    DebtSchedule[i].endpoint = 0;
  }
}

#define SECONDS_IN_10_MINS (60 * 10)
#define SECONDS_PER_HOUR  (60 * 60)
#define SECONDS_PER_DAY   (SECONDS_PER_HOUR * 24)
#define SECONDS_PER_WEEK  (SECONDS_PER_DAY * 7)
//#define SECONDS_PER_MONTH (SECONDS_PER_WEEK * 4)

void sl_zigbee_af_prepayment_server_schedule_debt_repayment(uint8_t endpoint,
                                                            uint32_t issuerEventId,
                                                            uint8_t debtType,
                                                            uint16_t collectionTime,
                                                            uint32_t startTime,
                                                            uint8_t collectionFrequency)
{
  uint8_t i;
  uint32_t startTimeTodaySec;
  uint32_t collectTimeSec;
  uint32_t todayRemainderSec;

  if ( debtType < DEBT_SCHEDULE_TABLE_SIZE ) {
    i = debtType;
    //DebtSchedule[i].collectionTime = collectionTime;
    //DebtSchedule[i].startTimeUtc = startTime;
    //DebtSchedule[i].collectionRate = collectionRate;
    DebtSchedule[i].endpoint = endpoint;
    DebtSchedule[i].issuerEventId = issuerEventId;
    DebtSchedule[i].collectionFrequency = collectionFrequency;

    // Calculate the first collection time - when debt will first be collected.  This time must be:
    //  1. Greater or equal to the start time, AND
    //  2. At a "collectionTime" moment, where the collectionTime specifies an offset, in minutes, from midnight.
    //
    // For example, if collectionTime==120 (2 hours), then the first collection time would start at
    // 2am after the startTime.

    // Set firstCollectionTime to startTime, then add in offset to reach next collectionTime moment.
    DebtSchedule[i].firstCollectionTimeSec = startTime;

    // Convert collection time in minute units, to collection time in second units
    collectTimeSec = collectionTime * 60;   // Number of seconds after start of day when collection should take place.

    // Examine "startTime" and figure out how many seconds it occurs after midnight.
    startTimeTodaySec = startTime % SECONDS_PER_DAY;    // Number of seconds after midnight where "startTime" falls.

    if ( startTimeTodaySec <= collectTimeSec ) {
      // Relative to midnight, collection time occurs after start time.
      DebtSchedule[i].firstCollectionTimeSec += (collectTimeSec - startTimeTodaySec);
    } else {
      // Relative to midnight, start time occurs after collection time, so the next start time will be tomorrow
      // at the collection time.
      // Calculate the remaining seconds in today (after start time), then add the collection time to get the time tomorrow.
      todayRemainderSec = SECONDS_PER_DAY - startTimeTodaySec;
      DebtSchedule[i].firstCollectionTimeSec += (todayRemainderSec + collectTimeSec);
    }
    // NOW - firstCollectionTimeSec holds the UTC time when the first debt collection should occur.
    // This time could be in the past.

    // NEXT - set the nextCollectionTimsSec to the next time when the debt repayment will occur.
    // We assume the first collection time is the next time, then verify that below.
    DebtSchedule[i].nextCollectionTimeUtc = DebtSchedule[i].firstCollectionTimeSec;

    // It is possible that the first collection time occurred a small time in the past, which is okay.
    // If debt collection occurred prior to the past 10 minutes, calculate the next time when
    // the debt collection should occur.
    if ( DebtSchedule[i].nextCollectionTimeUtc <= (sl_zigbee_af_get_current_time() - SECONDS_IN_10_MINS) ) {
      // Debt collection already started at some previous time.
      // Set next time to a multiple of collectionFrequencySec until next collection time >= Current Time.
      debtScheduleSetNextCollectionTimeSec(i);
    }

    sl_zigbee_af_prepayment_cluster_schedule_tick_cb(endpoint, PREPAYMENT_TICK_CHANGE_DEBT_EVENT);
  }
}

void sl_zigbee_af_prepayment_server_set_debt_mode(uint8_t endpoint, uint32_t timeNowUtc)
{
  uint8_t i;
  // Find which debt(s) is/are ready to be processed.
  for ( i = 0; i < DEBT_SCHEDULE_TABLE_SIZE; i++ ) {
    if ( DebtSchedule[i].nextCollectionTimeUtc <= timeNowUtc ) {
      // Update the debt attribute values, and schedule the next debt event.
      sl_zigbee_af_update_debt_on_collection_event(endpoint, i);
      debtScheduleSetNextCollectionTimeSec(i);
    }
  }
}

// This function calcuates the next time in the future when the particular debt should be collected.
// It does this by adding the debt collection rate (per hour, per day, per month, etc) to the
// last time debt was collected, until it finds a time in the future when debt should be collected.
static void debtScheduleSetNextCollectionTimeSec(uint8_t debtType)
{
  sl_zigbee_af_time_struct_t afTime;
  uint8_t collectionFrequency;
  uint32_t timeNowUtc;
  uint32_t nextCollectTimeUtc;
  uint8_t i;

  if ( debtType < DEBT_SCHEDULE_TABLE_SIZE ) {
    i = debtType;
    timeNowUtc = sl_zigbee_af_get_current_time();
    nextCollectTimeUtc = DebtSchedule[i].nextCollectionTimeUtc;
    collectionFrequency = DebtSchedule[i].collectionFrequency;

    while ( nextCollectTimeUtc <= timeNowUtc ) {
      // Schedule a new "next collection time" based on the collection frequency.
      switch ( collectionFrequency ) {
        case COLLECTION_FREQUENCY_PER_HOUR:
          nextCollectTimeUtc += SECONDS_PER_HOUR;
          break;
        case COLLECTION_FREQUENCY_PER_DAY:
          nextCollectTimeUtc += SECONDS_PER_DAY;
          break;
        case COLLECTION_FREQUENCY_PER_WEEK:
          nextCollectTimeUtc += SECONDS_PER_WEEK;
          break;
        case COLLECTION_FREQUENCY_PER_MONTH:
          sl_zigbee_af_fill_time_struct_from_utc(nextCollectTimeUtc, &afTime);
          afTime.month++;
          if ( afTime.month > 12 ) {
            afTime.year++;
            afTime.month = 1;
            nextCollectTimeUtc = sl_zigbee_af_get_utc_from_time_struct(&afTime);
          }
          break;
        case COLLECTION_FREQUENCY_PER_QUARTER:
          sl_zigbee_af_fill_time_struct_from_utc(nextCollectTimeUtc, &afTime);
          afTime.month += 3;
          if ( afTime.month > 12 ) {
            afTime.year++;
            afTime.month -= 12;
            nextCollectTimeUtc = sl_zigbee_af_get_utc_from_time_struct(&afTime);
          }
          break;
      }
    }
    DebtSchedule[i].nextCollectionTimeUtc = nextCollectTimeUtc;
  }
}

uint32_t sl_zigbee_af_prepayment_server_seconds_until_debt_collection_event(uint32_t timeNowUtc)
{
  uint8_t i;
  uint32_t eventDelaySec;
  uint32_t minEventDelaySec = 0xFFFFFFFF;

  for ( i = 0; i < DEBT_SCHEDULE_TABLE_SIZE; i++ ) {
    if ( DebtSchedule[i].nextCollectionTimeUtc != INVALID_DEBT_COLLECTION_TIME ) {
      if ( DebtSchedule[i].nextCollectionTimeUtc > timeNowUtc ) {
        eventDelaySec = DebtSchedule[i].nextCollectionTimeUtc - timeNowUtc;
        if ( eventDelaySec < minEventDelaySec ) {
          minEventDelaySec = eventDelaySec;
        }
      } else {
        // next collection time occurs now, or in the past.
        minEventDelaySec = 0;
        break;
      }
    }
  }
  sl_zigbee_af_prepayment_cluster_println("== Schedule Debt Collection Tick for %d seconds", minEventDelaySec);
  return minEventDelaySec;
}
