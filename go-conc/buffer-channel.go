package main

import "fmt"

func main(){
	ch := make(chan int, 1)// 创建一个容量为1的有缓冲区通道
	ch <- 10
	fmt.Println("发送成功")
}
