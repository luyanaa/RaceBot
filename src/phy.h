#pragma once
#include <ctime>
#include <string>
class Phy {
public:
	Phy();
	float _v;
	/** 下一次运算*/
	void nextFrerame();
	/** 虫豸*/
	void reset();
	/** 设置功率 [-1,1]*/
	void setW(float w);
	/** 设置转向 [-1,1]*/
	void setR(float r) ;
	/** 设置刹车 [0-1]*/
	void setB(float b);
	/** 获得角度变化*/
	float R();
	/** 获得速度*/
	float V();
	std::string debugmsg();

public:
	clock_t _c;
	float _r, _w, _b, old_step,_rr;
	float r_Scale;
	float b_Scale;
};