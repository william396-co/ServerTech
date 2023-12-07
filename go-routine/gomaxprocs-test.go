package main

import "fmt"
import "time"
import "runtime"

func a(){
	for i :=1; i < 10;i++{
		fmt.Println("A:",i)
	}
}

func b(){
	for i := 1;i < 10;i++{
		fmt.Println("B:",i)
	}
}

func main(){
	runtime.GOMAXPROCS(2)
	go a()
	go b()
	time.Sleep(time.Second)
}
