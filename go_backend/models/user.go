package models

import "gorm.io/gorm"

type User struct {
	ID       uint   `gorm:"primary_key"`
	Username string `gorm:"uniqueIndex"`
	Password string
}

func (u *User) Create(db *gorm.DB) error {
	return db.Create(u).Error
}

func (u *User) GetByUsername(db *gorm.DB) error {
	return db.Where("username = ?", u.Username).First(u).Error
}

func (u *User) VerifyPassword(password string) bool {
	return u.Password == password
}
