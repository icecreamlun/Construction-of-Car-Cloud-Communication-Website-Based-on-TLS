package handler

import (
	"io"
	"net/http"

	"github.com/gin-gonic/gin"
)

func VideoHandler(c *gin.Context) {
	c.HTML(http.StatusOK, "video.html", gin.H{})
}

func VideoStreamHandler(c *gin.Context) {
	url := "http://192.168.1.102:8080/stream?topic=/camera/color/image_raw"

	resp, err := http.Get(url)
	if err != nil {
		c.String(http.StatusServiceUnavailable, "Error accessing video stream: %v", err)
		return
	}
	defer resp.Body.Close()

	c.Header("Content-Type", "multipart/x-mixed-replace;boundary=boundarydonotcross")

	io.Copy(c.Writer, resp.Body)
}
