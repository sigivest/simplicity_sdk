/***************************************************************************//**
 * @file
 * @brief  *
 * The Demand Response Load Control Event Table is responsible
 * for keeping track of all load control events scheduled
 * by the Energy Service Provider. This module provides
 * interfaces used to schedule and inform load shedding
 * devices of scheduled events.
 *
 * Any code that uses this event table is responsible for
 * providing four things:
 *   1. frequent calls to eventTableTick(), one per millisecond
 *      will do. These calls are used to drive the
 *      table's timing mechanism.
 *   2. A way to get the real time by implementing
 *      getCurrentTime(uint32_t *currentTime);
 *   3. An implementation of eventAction which
 *      will be called whenever event status changes
 *
 * The load control event table expects that currentTime, startTime
 * and randomization are provided in seconds. And that duration is
 * provided in minutes.
 *
 * The implementing code is responsible for over the
 * air communication based on event status changes
 * reported through the eventAction interface
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

#ifndef SILABS_LOAD_CONTROL_EVENT_TABLE_H
#define SILABS_LOAD_CONTROL_EVENT_TABLE_H

// include global header for public LoadControlEvent struct
#include "../../include/af.h"

#define RANDOMIZE_START_TIME_FLAG     1
#define RANDOMIZE_DURATION_TIME_FLAG  2

#define CANCEL_WITH_RANDOMIZATION        1

// Table entry status
enum {
  ENTRY_VOID,
  ENTRY_SCHEDULED,
  ENTRY_STARTED,
  ENTRY_IS_SUPERSEDED_EVENT,
  ENTRY_IS_CANCELLED_EVENT
};

enum {
  EVENT_OPT_FLAG_OPT_IN                           = 0x01,
  EVENT_OPT_FLAG_PARTIAL                          = 0x02
};

// EVENT TABLE API

void afLoadControlEventTableInit(uint8_t endpoint);

/**
 * @brief Schedules events in the
 * load control event table.
 * The interface expects that
 * the user will populate a LoadControlEvent with all the
 * necessary fields and will pass a pointer to this event.
 * The passed event will be copied into the event table so it does
 * not need to survive the call to scheduleEvent.
 *
 * A call to this function always generates an event response over the air.
 *
 */
void sli_zigbee_af_schedule_load_control_event(uint8_t endpoint,
                                               sl_zigbee_af_load_control_event_t *event);

/**
 * @brief Tells the event table when a tick has taken place.
 * This function should be called by the cluster that uses the
 * event table.
 **/
void sli_zigbee_af_load_control_event_table_tick(uint8_t endpoint);

/**
 * @brief Cancels all events in the event table.
 *
 * @return A bool value indicating that a response was
 * generated for this action.
 **/
bool sli_zigbee_af_cancel_all_load_control_events(uint8_t endpoint,
                                                  uint8_t cancelControl);

/**
 * @brief Cancels an event in the event table.
 *
 * A call to this function always generates an event response over the air.
 **/
void sli_zigbee_af_cancel_load_control_event(uint8_t endpoint,
                                             uint32_t eventId,
                                             uint8_t cancelControl,
                                             uint32_t effectiveTime);

/**
 * @brief Schedules a call to cancel an event.
 * Takes a LoadControlEvent structure, which is used to encapsulate
 * the necessary information for the scheduled cancel.
 *
 * The scheduleCancelEvent function only uses three fields
 * from the LoadControlEvent struct, eventId, startTime,
 * and eventControl, as follows:
 *
 * eventId: The eventId of the event to cancel.
 *
 * startTime: The starttime of the event should
 * be the same as the effective time of the
 * cancel and will be used to schedule the
 * call to cancel.
 *
 * eventControl: The eventControl of the event should
 * be the same as the cancelControl passed when sending the
 * initial cancel event call.
 *
 * NOTE: A call to cancelAllEvents() will wipe out
 * this scheduled cancel as well as all other events which
 * should be fine, since the event scheduled to cancel would
 * be affected as well by the cancel all call anyway.
 **/
void afScheduleCancelEvent(sl_zigbee_af_load_control_event_t *e);

/**
 * An interface for opting in and out of an event.
 */
void sli_zigbee_af_load_control_event_opt_in_or_out(uint8_t endpoint,
                                                    uint32_t eventId,
                                                    bool optIn);

// The module using this table is responsible for providing the following
// functions.
/**
 * @brief Called by the event table when an events status has
 * changed. This handle should be used to inform the
 * ESP(s) and or react to scheduled events. The event
 * table will take care of clearing itself if the event
 * is completed.
 **/
void sl_zigbee_af_event_action(sl_zigbee_af_load_control_event_t *event,
                               uint8_t eventStatus,
                               uint8_t sequenceNumber,
                               uint8_t esiIndex);

/** @brief Prints a message indicating
 *  an attempt to append a signature to the event status message
 *  has failed.
 **/
void sli_zigbee_af_note_signature_failure(void);

/** @brief Prints the load control event table.
**/
void sli_zigbee_af_load_control_event_table_print(uint8_t endpoint);

/**
 * @brief Initializes the load control event table.
 */
void sli_zigbee_af_load_control_event_table_init(uint8_t endpoint);

/*
 * @brief Clears the load control event table.
 */
void sli_zigbee_af_load_control_event_table_clear(uint8_t endpoint);

#endif //__LOAD_CONTROL_EVENT_TABLE_H__
