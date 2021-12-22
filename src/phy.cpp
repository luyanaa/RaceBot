#include "phy.h"
#include <cmath>
#include <ctime>
#include <string>
Phy::Phy()
{
	_v = 0;
	reset();
	_c = clock();
	r_Scale = 0.2;
	b_Scale = 2;
}

/** 下一次运算*/
void Phy::nextFrerame()
{
	clock_t new_c = clock();
	double step = (new_c - _c) * 1.0 / CLOCKS_PER_SEC;
	old_step = step;
	_c = new_c;
	//损耗
	if (_b < 0.05)
		_b = 0.05;
	// 风阻
	_b += 0.2 * _v * _v;
	_v += _w * step;
	_v += _v > 0 ? -_b * step * b_Scale : +_b * step * b_Scale;
	if (_v < _b * step * b_Scale && _v > -_b * step * b_Scale)
		_v = 0;
	float d=10*step,dr=_rr-_r;
	if(dr>d)
	dr=d;
	if(dr<-d)
	dr=-d;
	_r+=dr;
	

}
/** 虫豸*/
void Phy::reset()
{
	setW(0);
	setB(0);
	setR(0);
}
/** 设置功率 [-1,1]*/
void Phy::setW(float w) { _w = w; }
/** 设置转向 [-1,1]*/
void Phy::setR(float r)
{
	_rr = r;
}
/** 设置刹车 [0-1]*/
void Phy::setB(float b) { _b = b; }
/** 获得角度变化*/
float Phy::R() { return (_r > 0 ? 1 : -1) * std::min(std::abs(_r), std::abs(1.5f / _v)) * r_Scale; }
/** 获得速度*/
float Phy::V() { return _v; }
std::string Phy::debugmsg()
{
	return "v:" + std::to_string(_v) + " w:" + std::to_string(_w) + " R:" + std::to_string(R()) +
		   " b:" + std::to_string(_b) + " step:" + std::to_string(old_step);
}
