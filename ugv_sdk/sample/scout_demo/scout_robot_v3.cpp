#include <unistd.h>
#include <memory>
#include <iostream>
#include <mysql/mysql.h>    // MySQL库头文件

#include "ugv_sdk/mobile_robot/scout_robot.hpp"
#include "ugv_sdk/utilities/protocol_detector.hpp"

using namespace westonrobot;

int main(int argc, char **argv) {

  std::unique_ptr<ScoutRobot> scout;
  std::string device_name = "can0";    // 设备名称
  bool is_scout_mini = false;          // 是否为scout mini

  ProtocolDectctor detector;
  if (detector.Connect(device_name)) {
    auto proto = detector.DetectProtocolVersion(5);
    if (proto == ProtocolVersion::AGX_V1) {
      std::cout << "Detected protocol: AGX_V1" << std::endl;
      scout = std::unique_ptr<ScoutRobot>(
          new ScoutRobot(ProtocolVersion::AGX_V1, is_scout_mini));
    } else if (proto == ProtocolVersion::AGX_V2) {
      std::cout << "Detected protocol: AGX_V2" << std::endl;
      scout = std::unique_ptr<ScoutRobot>(
          new ScoutRobot(ProtocolVersion::AGX_V2, is_scout_mini));
    } else {
      std::cout << "Detected protocol: UNKONWN" << std::endl;
      return -1;
    }
  } else {
    return -1;
  }

  if (scout == nullptr)
    std::cout << "Failed to create robot object" << std::endl;

  scout->Connect(device_name);

  if (scout->GetParserProtocolVersion() == ProtocolVersion::AGX_V2) {
    scout->EnableCommandedMode();
  }

  // MySQL连接
  MYSQL mysql;
  mysql_init(&mysql);
  if (mysql_real_connect(&mysql, "localhost", "root", "123456", "dbname2", 0, nullptr, 0)) {    // 修改为实际的MySQL连接参数
    std::cout << "Connected to MySQL server" << std::endl;
  } else {
    std::cout << "Failed to connect to MySQL server: " << mysql_error(&mysql) << std::endl;
    return -1;
  }

  int count = 0;
  //创建mysql事件调度器，定期更新表格时间戳列
  if (mysql_query(&mysql, "CREATE EVENT update_robot_state \
      ON SCHEDULE EVERY 1 SECOND \
      DO \
        UPDATE robot_state SET timestamp = timestamp + INTERVAL 1 SECOND;")) {
    std::cout << "Failed to create MySQL event scheduler: " << mysql_error(&mysql) << std::endl;
    return -1;
  }

  // 启用MySQL事件调度程序线程
  if (mysql_query(&mysql, "SET GLOBAL event_scheduler = ON;")) {
    std::cout << "Failed to start MySQL event scheduler: " << mysql_error(&mysql) << std::endl;
    return -1;
  }

  while (true) {
    // motion control
    std::cout << "Motor: 1.0, 0" << std::endl;
    scout->SetMotionCommand(1.0, 0.0);

    // get robot state
    auto state = scout->GetRobotState();
    auto actuator = scout->GetActuatorState();
    std::cout << "-------------------------------" << std::endl;
    std::cout << "count: " << count << std::endl;

    // 将机器人状态信息插入到MySQL数据库中
    std::string query =
        "INSERT INTO robot_state2 (id, control_mode, vehicle_state, error_code, "
        "battery_voltage, linear_velocity, angular_velocity, core_state_age, "
        "motor_current, rpm, driver_temp, motor_temp, actuator_state_age) VALUES " 
        "(1, '" + std::to_string(state.system_state.control_mode) + "', '" +
        std::to_string(state.system_state.vehicle_state) + "', '" +
        std::to_string(static_cast<int>(state.system_state.error_code)) + "', '" +
        std::to_string(state.system_state.battery_voltage) + "', '" +
        std::to_string(state.motion_state.linear_velocity) + "', '" +
        std::to_string(state.motion_state.angular_velocity) + "', '" +
        std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
                           AgxMsgRefClock::now() - state.time_stamp)
                           .count()) +
        "', '" + std::to_string(actuator.actuator_state[0].current) + "', '" +
        std::to_string(actuator.actuator_state[0].rpm) + "', '" +
        std::to_string(actuator.actuator_state[0].driver_temp) + "', '" +
        std::to_string(actuator.actuator_state[0].motor_temp) + "', '" +
        std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
                           AgxMsgRefClock::now() - actuator.time_stamp)
                           .count()) +
        "')ON DUPLICATE KEY UPDATE control_mode = VALUES(control_mode), vehicle_state = VALUES(vehicle_state), "
        "error_code = VALUES(error_code), battery_voltage = VALUES(battery_voltage), "
        "linear_velocity = VALUES(linear_velocity), angular_velocity = VALUES(angular_velocity), "
        "core_state_age = VALUES(core_state_age), motor_current = VALUES(motor_current), "
        "rpm = VALUES(rpm), driver_temp = VALUES(driver_temp), motor_temp = VALUES(motor_temp), "
        "actuator_state_age = VALUES(actuator_state_age)";
    if (mysql_query(&mysql, query.c_str())) {
      std::cout << "Failed to execute MySQL query: " << mysql_error(&mysql) << std::endl;
    }

    
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
    } 
    std::cout << "-------------------------------" << std::endl;


++count;
usleep(1000000);
}

// 关闭MySQL连接
//mysql_close(&mysql);

return 0;
}