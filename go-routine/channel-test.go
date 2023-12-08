package main

import "fmt"

func main(){

	ch1 := make(chan int)
	ch2 := make(chan int)

	// send 0-100 to ch1
	go func(){
		for i:=0; i < 100;i++{
			ch1 <- i
		}
		close(ch1)
	}()
	// recv from ch1, and square it to ch2
	go func(){
		for{
			i,ok := <- ch1
			if !ok{
				break
			}
			ch2 <- i*i
		}
		close(ch2)
	}()

	// main goroutine print from ch2
	for i:= range ch2{
		fmt.Println(i)
	}
}


