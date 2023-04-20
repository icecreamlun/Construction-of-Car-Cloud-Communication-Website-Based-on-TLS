package handler

import (
	"fmt"
	"net/http"

	"go_backend/models"

	"github.com/gin-gonic/gin"
	"gorm.io/gorm"
)

func RegisterHandler(c *gin.Context) {
	formType := "register"
	buttonLabel := "注册"
	c.HTML(http.StatusOK, "index.tmpl", gin.H{
		"FormType":    formType,
		"ButtonLabel": buttonLabel,
	})
}

//func LoginHandler(c *gin.Context) {
//	formType := "login"
//	buttonLabel := "登录"
//	c.HTML(http.StatusOK, "index.tmpl", gin.H{
//		"FormType":    formType,
//		"ButtonLabel": buttonLabel,
//	})
//}

func AuthHandler(db *gorm.DB) gin.HandlerFunc {
	return func(c *gin.Context) {
		formType := c.PostForm("formType")
		username := c.PostForm("username")
		password := c.PostForm("password")
		user := &models.User{
			Username: username,
			Password: password,
		}
		if formType == "register" {
			if err := user.Create(db); err != nil {
				fmt.Println("Failed to create user:", err)
				c.Redirect(http.StatusFound, "/")
				return
			}
			fmt.Println("User created:", user.Username)
		} else if formType == "login" {
			if err := user.GetByUsername(db); err != nil {
				fmt.Println("Failed to find user:", err)
				c.Redirect(http.StatusFound, "/")
				return
			}
			if !user.VerifyPassword(password) {
				fmt.Println("Invalid password")
				c.Redirect(http.StatusFound, "/")
				return
			} else {
				c.SetCookie("username", user.Username, 3600, "/", "", false, true)
			}
			fmt.Println("User logged in:", user.Username)
			c.Redirect(http.StatusFound, "/mainmenu")
			return
		}
		c.Redirect(http.StatusFound, "/")
	}
}
