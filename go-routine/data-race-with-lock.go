package main

import "fmt"
import "sync"

var x int64
var wg sync.WaitGroup
var lock sync.Mutex

func add(){
	for i:= 0;i < 5000;i++{
		lock.Lock()
		x = x+1
		lock.Unlock()
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
