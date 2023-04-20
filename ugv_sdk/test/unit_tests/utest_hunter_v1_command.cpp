/*
 * utest_hunter_v1_command.cpp
 *
 * Created on: Jul 12, 2021 17:28
 * Description:
 *
 * Copyright (c) 2021 Weston Robot Pte. Ltd.
 */

#include <stdio.h>
#include <vector>
#include <iostream>

#include "gtest/gtest.h"

#include "ugv_sdk/details/protocol_v1/protocol_v1_parser.hpp"
#include "protocol_v1/agilex_protocol_v1.h"

using namespace westonrobot;

struct HunterV1CommandTest : testing::Test {
  struct can_frame frame;
  HunterProtocolV1Parser parser;
};

TEST_F(HunterV1CommandTest, MotionLinearCommandTest) {
  AgxMessage msg;

  msg.type = AgxMsgMotionCommandV1;
  msg.body.v1_motion_command_msg.linear = 0.15;
  msg.body.v1_motion_command_msg.angular = 0;

  parser.EncodeMessage(&msg, &frame);

  uint8_t expected_data[8] = {0x01, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x44};

  ASSERT_EQ(frame.can_id, CAN_MSG_MOTION_COMMAND_ID);
  ASSERT_EQ(frame.can_dlc, 8);

  for (int i = 0; i < 6; ++i) {
    // printf("expecting: %x, getting: %x\n", expected_data[i], frame.data[i]);
    ASSERT_EQ(frame.data[i], expected_data[i]);
  }
}

TEST_F(HunterV1CommandTest, MotionAngularCommandTest) {
  AgxMessage msg;

  msg.type = AgxMsgMotionCommandV1;
  msg.body.v1_motion_command_msg.linear = 0.0;
  msg.body.v1_motion_command_msg.angular = 2.55;

  parser.EncodeMessage(&msg, &frame);

  uint8_t expected_data[8] = {0x01, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x01, 0x45};

  ASSERT_EQ(frame.can_id, CAN_MSG_MOTION_COMMAND_ID);
  ASSERT_EQ(frame.can_dlc, 8);

  for (int i = 0; i < 6; ++i) {
    // printf("expecting: %x, getting: %x\n", expected_data[i], frame.data[i]);
    ASSERT_EQ(frame.data[i], expected_data[i]);
  }
}

TEST_F(HunterV1CommandTest, ValueSetCommandTest) {
  AgxMessage msg;

  msg.type = AgxMsgValueSetCommandV1;
  msg.body.v1_value_set_command_msg.set_neutral = true;

  parser.EncodeMessage(&msg, &frame);

  uint8_t expected_data[6] = {0xaa, 0x00, 0x00, 0x00, 0x00, 0x00};

  ASSERT_EQ(frame.can_id, CAN_MSG_VALUE_SET_COMMAND_ID);
  ASSERT_EQ(frame.can_dlc, 8);

  for (int i = 0; i < 6; ++i) {
    // printf("expecting: %x, getting: %x\n", expected_data[i], frame.data[i]);
    ASSERT_EQ(frame.data[i], expected_data[i]);
  }
}

TEST_F(HunterV1CommandTest, LightCommandTest) {
  AgxMessage msg;

  msg.type = AgxMsgLightCommand;
  msg.body.light_command_msg.enable_cmd_ctrl = true;
  msg.body.light_command_msg.front_light.mode = CONST_ON;
  msg.body.light_command_msg.front_light.custom_value = 100;
  msg.body.light_command_msg.rear_light.mode = CUSTOM;
  msg.body.light_command_msg.rear_light.custom_value = 50;

  parser.EncodeMessage(&msg, &frame);

  uint8_t expected_data[8] = {0x01, 0x01, 0x64, 0x03, 0x32, 0x00, 0x00, 0xe4};

  ASSERT_EQ(frame.can_id, CAN_MSG_LIGHT_COMMAND_ID);
  ASSERT_EQ(frame.can_dlc, 8);

  for (int i = 0; i < 6; ++i) {
    // printf("expecting: %x, getting: %x\n", expected_data[i], frame.data[i]);
    ASSERT_EQ(frame.data[i], expected_data[i]);
  }
}