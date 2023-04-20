/*
 * ranger_interface.hpp
 *
 * Created on: Jul 08, 2021 09:40
 * Description:
 *
 * Copyright (c) 2021 Weston Robot Pte. Ltd.
 */

#ifndef RANGER_INTERFACE_HPP
#define RANGER_INTERFACE_HPP

#include <string>

#include "ugv_sdk/details/interface/agilex_message.h"
#include "ugv_sdk/details/interface/robot_common_interface.hpp"

namespace westonrobot {
struct RangerCoreState {
  // system state
  AgxMsgTimeStamp time_stamp;

  SystemStateMessage system_state;
  MotionStateMessage motion_state;
  LightStateMessage light_state;
  MotionModeStateMessage current_motion_mode;

  RcStateMessage rc_state;
  OdometryMessage odometry;
};

struct RangerActuatorState {
  AgxMsgTimeStamp time_stamp;

  ActuatorHSStateMessage actuator_hs_state[8];
  ActuatorLSStateMessage actuator_ls_state[8];
};

struct RangerMotorState {
  MotorAngleMessage motor_angle_state;
  MotorSpeedMessage motor_speed_state;
};

struct RangerBmsState {
  BmsBasicMessage bmsbasic;
};

/////////////////////////////////////////////////////////////////////////

struct RangerInterface {
  virtual ~RangerInterface() = default;
  
  virtual bool Connect(std::string dev_name) = 0;

  // robot control
  virtual void SetMotionMode(uint8_t mode) = 0;
  virtual void SetMotionCommand(double linear_vel, double steer_angle,
                                double lateral_vel, double angular_vel) = 0;
  virtual void SetLightCommand(AgxLightMode f_mode, uint8_t f_value,
                               AgxLightMode r_mode, uint8_t r_value) = 0;

  // get robot state
  virtual RangerCoreState GetRobotState() = 0;
  virtual RangerActuatorState GetActuatorState() = 0;
  virtual RangerMotorState GetMotorState() = 0;
  virtual RangerBmsState GetBmsState() = 0;
};
}  // namespace westonrobot

#endif /* RANGER_INTERFACE_HPP */
