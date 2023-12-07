package main

import "fmt"

// 使用无缓冲通道进行通信将导致发送和接收的goroutine同步化。因此，无缓冲通道也被称为同步通道。

func recv(c chan int){
	ret := <- c
	fmt.Println("recv success ",ret)
}

func main(){
	ch := make(chan int)
	go recv(ch)// 启用goroutine从通道接受
	ch <- 10
	fmt.Println("发送成功")
}
