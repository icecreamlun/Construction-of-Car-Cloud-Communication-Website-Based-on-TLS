/*
 * scout_robot_demo.cpp
 *
 * Created on: Jul 08, 2021 11:12
 * Description:
 *
 * Copyright (c) 2021 Weston Robot Pte. Ltd.
 */

#include <unistd.h>

#include <memory>
#include <iostream>

#include "ugv_sdk/mobile_robot/scout_robot.hpp"
#include "ugv_sdk/utilities/protocol_detector.hpp"

using namespace westonrobot;

int main(int argc, char **argv) {
  std::string device_name;

  if (argc == 2) {
    device_name = {argv[1]};
    std::cout << "Selected interface " << device_name << std::endl;
  } else {
    std::cout << "Usage: demo_scout_mini_omni_robot <interface>" << std::endl
              << "Example 1: ./demo_scout_mini_omni_robot can0" << std::endl;
    return -1;
  }

  std::unique_ptr<ScoutMiniOmniRobot> scout;

  ProtocolDetector detector;
  detector.Connect(device_name);
  auto proto = detector.DetectProtocolVersion(5);
  if (proto == ProtocolVersion::AGX_V1) {
    std::cout << "Detected protocol: AGX_V1" << std::endl;
    scout = std::unique_ptr<ScoutMiniOmniRobot>(
        new ScoutMiniOmniRobot(ProtocolVersion::AGX_V1));

  } else if (proto == ProtocolVersion::AGX_V2) {
    std::cout << "Detected protocol: AGX_V2" << std::endl;
    scout = std::unique_ptr<ScoutMiniOmniRobot>(
        new ScoutMiniOmniRobot(ProtocolVersion::AGX_V2));
  } else {
    std::cout << "Detected protocol: UNKONWN" << std::endl;
    return -1;
  }

  if (scout == nullptr)
    std::cout << "Failed to create robot object" << std::endl;

  scout->Connect(device_name);

  if (scout->GetParserProtocolVersion() == ProtocolVersion::AGX_V2) {
    scout->EnableCommandedMode();
  }

  // light control
  std::cout << "Light: const off" << std::endl;
  scout->SetLightCommand(CONST_OFF, 0, CONST_OFF, 0);
  sleep(3);
  std::cout << "Light: const on" << std::endl;
  scout->SetLightCommand(CONST_ON, 0, CONST_ON, 0);
  sleep(3);
  std::cout << "Light: breath" << std::endl;
  scout->SetLightCommand(BREATH, 0, BREATH, 0);
  sleep(3);
  std::cout << "Light: custom 30-40" << std::endl;
  scout->SetLightCommand(CUSTOM, 30, CUSTOM, 40);
  sleep(3);
  scout->SetLightCommand(CONST_OFF, 0, CONST_OFF, 0);

  int count = 0;
  while (true) {
    // motion control
    std::cout << "Setting motion command: 0.0, 0,0, 0.8" << std::endl;
    scout->SetMotionCommand(0.0, 0.3, 0.0);

    // get robot state
    auto state = scout->GetRobotState();
    std::cout << "-------------------------------" << std::endl;
    std::cout << "count: " << count << std::endl;
    std::cout << "control mode: "
              << static_cast<int>(state.system_state.control_mode)
              << " , vehicle state: "
              << static_cast<int>(state.system_state.vehicle_state)
              << " , error code: " << std::hex << state.system_state.error_code
              << std::dec
              << ", battery voltage: " << state.system_state.battery_voltage
              << std::endl;
    std::cout << "velocity (linear, angular, lateral): "
              << state.motion_state.linear_velocity << ", "
              << state.motion_state.angular_velocity << ", "
              << state.motion_state.lateral_velocity << std::endl;
    std::cout << "core state age (ms): "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     AgxMsgRefClock::now() - state.time_stamp)
                     .count()
              << std::endl;

    auto actuator = scout->GetActuatorState();
    if (scout->GetParserProtocolVersion() == ProtocolVersion::AGX_V1) {
      for (int i = 0; i < 4; ++i) {
        printf("motor %d: current %f, rpm %d, driver temp %f, motor temp %f\n",
               actuator.actuator_state[i].motor_id,
               actuator.actuator_state[i].current,
               actuator.actuator_state[i].rpm,
               actuator.actuator_state[i].driver_temp,
               actuator.actuator_state[i].motor_temp);
      }
      std::cout << "actuator state age (ms): "
                << std::chrono::duration_cast<std::chrono::milliseconds>(
                       AgxMsgRefClock::now() - actuator.time_stamp)
                       .count()
                << std::endl;
    } else {
      for (int i = 0; i < 4; ++i) {
        printf("motor %d: current %f, rpm %d, driver temp %f, motor temp %f\n",
               actuator.actuator_hs_state[i].motor_id,
               actuator.actuator_hs_state[i].current,
               actuator.actuator_hs_state[i].rpm,
               actuator.actuator_ls_state[i].driver_temp,
               actuator.actuator_ls_state[i].motor_temp);
      }
      std::cout << "actuator state age (ms): "
                << std::chrono::duration_cast<std::chrono::milliseconds>(
                       AgxMsgRefClock::now() - actuator.time_stamp)
                       .count()
                << std::endl;
    }
    std::cout << "-------------------------------" << std::endl;

    usleep(20000);
    ++count;
  }

  return 0;
}