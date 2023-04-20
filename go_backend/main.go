package main

import (
	"crypto/tls"
	"fmt"
	"go_backend/config"
	"go_backend/handler"
	"go_backend/middleware"
	"go_backend/models"
	"net/http"
	"time"

	"github.com/gin-gonic/gin"
	"gorm.io/gorm"
)

var db *gorm.DB

func main() {
	// 加载配置文件 /load config
	databaseConfig := config.LoadDatabaseConfig()

	// 连接数据库 / connect database
	db = config.ConnectToDatabase(databaseConfig)

	// 自动迁移数据表 / create automigrate
	db.AutoMigrate(&models.User{})

	// 创建Gin路由 / create gin route
	router := gin.Default()

	// 加载HTML模板 / load HTML template
	router.LoadHTMLGlob("templates/*")

	//静态文件 / load static
	router.Static("/static", "./static")

	// 注册和登录页面
	//router.GET("/", handler.IndexHandler)

	authorized := router.Group("/")
	authorized.Use(middleware.AuthRequired(db))
	{
		// 消息路由 / message route
		authorized.GET("/message", handler.MessageHandler(db))

		go func() {
			for range time.Tick(time.Second) {
				fmt.Println("重新获取数据")
				handler.GetSimple()
			}
		}()

		//注册和登录页面 / login_register
		router.GET("/", handler.RegisterHandler)

		// 添加主菜单路由 / add mainmenu
		authorized.GET("/mainmenu", func(c *gin.Context) {
			c.HTML(http.StatusOK, "mainmenu.tmpl", gin.H{})
		})

		// 添加深度视频路由 / add deep_video route
		router.GET("/deep_video2", handler.DeepVideoHandler)
		router.GET("/deep_video", handler.DeepVideoStreamHandler)

		// 添加视频路由 / add video route
		router.GET("/video2", handler.VideoHandler)
		router.GET("/video", handler.VideoStreamHandler)

		// 注册和登录处理 / handle register and login
		router.POST("/", handler.AuthHandler(db))
	}

	// 启动HTTPS服务器 / start https
	certFile := "cert/server.crt"
	keyFile := "cert/server.key"
	cert, err := tls.LoadX509KeyPair(certFile, keyFile)
	if err != nil {
		panic(err)
	}
	tlsConfig := &tls.Config{Certificates: []tls.Certificate{cert}}
	server := &http.Server{
		Addr:      ":8081",
		Handler:   router,
		TLSConfig: tlsConfig,
	}
	err = server.ListenAndServeTLS("", "")
	if err != nil {
		panic("Failed to start server: " + err.Error())
	}
}
