package main

import "fmt"
import "time"


func hello(){
		fmt.Println("Hello Goroutine!")
}

func main(){
		go hello();
		fmt.Println("main goroutine done!")
		time.Sleep(time.Second)
}
