package handler

import (
	"io"
	"net/http"

	"github.com/gin-gonic/gin"
)

func DeepVideoHandler(c *gin.Context) {
	c.HTML(http.StatusOK, "deep_video.html", gin.H{})
}

func DeepVideoStreamHandler(c *gin.Context) {
	url := "http://192.168.1.102:8080/stream_viewer?topic=/camera/depth/image_rect_raw&type=ros_compressed&quality=100&width=640&height=480&format=gray"

	resp, err := http.Get(url)
	if err != nil {
		c.String(http.StatusServiceUnavailable, "Error accessing deep video stream: %v", err)
		return
	}
	defer resp.Body.Close()

	c.Header("Content-Type", "multipart/x-mixed-replace;boundary=boundarydonotcross")

	io.Copy(c.Writer, resp.Body)
}
