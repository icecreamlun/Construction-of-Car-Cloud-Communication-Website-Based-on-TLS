package handler

import (
	"net/http"

	"github.com/gin-gonic/gin"
	"gorm.io/gorm"
)

func MessageHandler(db *gorm.DB) gin.HandlerFunc {
	return func(c *gin.Context) {
		mutex.RLock()
		defer mutex.RUnlock()
		c.HTML(http.StatusOK, "message.html", gin.H{
			//装载需要返回的jason
			"data":    latestData,
			"status":  200,
			"message": "您已经成功获取到数据",
		})
	}
}
