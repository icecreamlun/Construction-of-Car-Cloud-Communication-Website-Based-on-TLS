package config

import (
	"gorm.io/driver/mysql"
	"gorm.io/gorm"
)

func LoadDatabaseConfig() string {
	// load the file
	
	return "zhuolun:010415@tcp(localhost:3306)/rigester2"
}

func ConnectToDatabase(dsn string) *gorm.DB {
	db, err := gorm.Open(mysql.Open(dsn), &gorm.Config{})
	if err != nil {
	panic(err)
	}
	return db
}
