package main

import "fmt"
import "sync"

var x int64
var wg sync.WaitGroup

func add(){
	for i:= 0;i < 5000;i++{
		x = x+1
	}
	wg.Done()
}

func main(){
	wg.Add(10)
	for i:= 0; i<10;i++{
		go add()
	}
	wg.Wait()
	fmt.Println(x)
}
