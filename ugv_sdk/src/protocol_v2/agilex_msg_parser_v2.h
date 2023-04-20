/* 
 * agilex_msg_parser.h
 * 
 * Created on: Dec 22, 2020 17:11
 * Description: public API to encode/decode protocol messages
 * 
 * Copyright (c) 2020 Ruixiang Du (rdu)
 */ 

#ifndef AGILEX_MSG_PARSER_H
#define AGILEX_MSG_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifdef __linux__
#include <linux/can.h>
#else
struct can_frame {
  uint32_t can_id;
  uint8_t can_dlc;
  uint8_t data[8] __attribute__((aligned(8)));
};
#endif

#include "ugv_sdk/details/interface/agilex_message.h"

bool DecodeCanFrameV2(const struct can_frame *rx_frame, AgxMessage *msg);
bool EncodeCanFrameV2(const AgxMessage *msg, struct can_frame *tx_frame);
uint8_t CalcCanFrameChecksumV2(uint16_t id, uint8_t *data, uint8_t dlc);

#ifdef __cplusplus
}
#endif

#endif /* AGILEX_MSG_PARSER_H */
