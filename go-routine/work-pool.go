package main

import "fmt"
import "time"

func worker(id int, jobs <-chan int, results chan<- int){
	for j:= range jobs{
		fmt.Printf("woker:%d start job:%d\n",id,j)
		time.Sleep(time.Second)
		fmt.Printf("Woker:%d end job:%d\n",id,j)
		results <- j * 2
	}
}


func main(){
	jobs := make(chan int, 100)
	results := make(chan int, 100)

	// start 3 goroutine
	for w:=1;w <=3;w++{
		go worker(w,jobs,results)
	}

	// 5 tasks
	for j:= 1;j <= 5;j++{
		jobs <-j
	}
	close(jobs)

	// print results
	for a:=1; a<=5;a++{
		<-results
	}
}
