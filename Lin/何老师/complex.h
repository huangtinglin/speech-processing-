#pragma once
#ifndef _BA_COMPLEX_H_
#define _BA_COMPLEX_H_

#include <cmath>
#include <armadillo>
using namespace arma;

class Complex
{

private:

	// 实部和虚部
	double RealPart;
	double ImaginaryPart;

public:

	// 默认构造函数
	Complex();

	// 构造函数
	Complex(double rep, double imp);

	// 直接置数
	void Update(double ReP, double ImP);

	// 取实部
	double Re(void);

	//取虚部
	double Im(void);

	//取模的平方
	double AbsSqr(void);

	//取模
	double Abs(void);

	// 复数加法
	Complex operator+(Complex c);

	// 复数减法
	Complex operator-(Complex c);

	// 复数乘法
	Complex operator*(Complex c);

	// 重载赋值运算符
	void operator=(Complex rv);
};

Complex* mat2com(mat a);

mat com2mat(Complex* c, int num);


#endif
#pragma once
