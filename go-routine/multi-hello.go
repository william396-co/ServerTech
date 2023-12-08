package main

import "fmt"
import "sync"

var wg sync.WaitGroup

func hello(i int){
		defer wg.Done()// goroutine finished -1(like DecRef())
		fmt.Println("Hello Gorotine!", i)
}

func main(){
		for i :=0; i < 10;i++ {
				wg.Add(1); // active one goroutine +1(like IncRef())
				go hello(i)
		}
		wg.Wait() // wait all goroutine finished
}
