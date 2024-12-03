#pragma once

#include <cstdio>
#include <sys/time.h>

time_t getMSec(){
	struct timeval tv;
	return 0;
}


// 定时器数据结构的定义
template<typename _User_Data>
class Timer{
using TimeOutCbFunc = void(*)(_User_Data*);
public:
	Timer(){}
	Timer(int msec){
		_expire = getMSec()+msec;
	} 

	~Timer(){}
	void setTimeout(time_t timeout){
		_expire = getMSec()+timeout;
	}

	time_t getExpire()const{
		return _expire;
	}
	void setUserData(_User_Data*userData){
		_user_data= userData;
	}
	
	void handleTimeOut(){
		if(_cb_func_){
			_cb_func_(_user_data);
		}
	}

private:
	time_t _expire{};
	_User_Data* _user_data{};
	TimeOutCbFunc _cb_func{};
};

template<typename _UData>
class ITimerContainer
{
public:
	ITimerContainer(){}
	virtual ~ITimerContainer(){}

public:
	virtual void tick()=0;
	virtual Timer<_UData>* addTimer(time_t timeout)=0;
	virtual void delTimer(Timer<_UData>* timer)=0;
	virtual void resetTimer(Timer<_UData>*timer,time_t timeout)=0;
	virtual int getMinExpire()const=0;
};
