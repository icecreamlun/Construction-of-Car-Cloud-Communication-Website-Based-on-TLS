package handler

import (
	"database/sql"
	"fmt"
	"sync"

	_ "github.com/go-sql-driver/mysql"
)

var (
	latestData []map[string]interface{}
	mutex      sync.RWMutex
)

func GetSimple() []map[string]interface{} {
	//连接数据库
	db, err := sql.Open("mysql", "lizhuolun:010415@tcp(localhost:3306)/dbname2")
	if err != nil {
		fmt.Printf("无法连接到数据库:%v\n", err)
		fmt.Println(err)
	}

	//创建需要返回值的·结构体
	type Scoutmini struct {
		ID                 int     `db: "id"`
		Control_mode       int     `db: "control_mode"`
		Vehicle_state      int     `db: "vehicle_state"`
		Error_code         int     `db: "error_code"`
		Battery_voltage    float64 `db: "battery_voltage"`
		Linear_velocity    float64 `db: "linear_velocity"`
		Angular_velocity   float64 `db: "angular_velocity"`
		Core_state_age     int64   `db: "core_state_age"`
		Motor_current      float64 `db: "motor_current"`
		Rpm                int     `db: "rpm"`
		Driver_temp        float64 `db: "driver_temp"`
		Motor_temp         float64 `db: "motor_temp"`
		Actuator_state_age int64   `db: "actuator_state_age"`
	}
	rows, err := db.Query("select * from robot_state2")
	if err != nil {
		fmt.Printf("无法获取返回值:%v\n", err)
		fmt.Println(err)
	}

	var split []map[string]interface{}
	for rows.Next() {
		var s Scoutmini
		err = rows.Scan(&s.ID, &s.Control_mode, &s.Vehicle_state, &s.Error_code, &s.Battery_voltage, &s.Linear_velocity, &s.Angular_velocity, &s.Core_state_age, &s.Motor_current, &s.Rpm, &s.Driver_temp, &s.Motor_temp, &s.Actuator_state_age)
		if err != nil {
			fmt.Println("can't scan rows")
			return nil
		}
		//var a map[string]interface{}
		a := make(map[string]interface{})
		a["scout_id"] = s.ID
		a["control_mode"] = s.Control_mode
		a["vehicle_state"] = s.Vehicle_state
		a["error_code"] = s.Error_code
		a["battery_voltage"] = s.Battery_voltage
		a["linear_velocity"] = s.Linear_velocity
		a["angular_velocity"] = s.Angular_velocity
		a["core_state_age"] = s.Core_state_age
		a["motor_current"] = s.Motor_current
		a["rpm"] = s.Rpm
		a["driver_temp"] = s.Driver_temp
		a["motor_temp"] = s.Motor_temp
		a["actuator_state_age"] = s.Actuator_state_age
		split = append(split, a)
	}
	mutex.Lock()
	latestData = split
	mutex.Unlock()
	fmt.Printf("执行了")
	return split
}
