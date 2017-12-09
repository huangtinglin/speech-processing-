#ifndef  _BA_hamming_H_
#define  _BA_hamming_H_

#include<iostream>
#include<cmath>
#include<armadillo>
#include"meta.h"
#define pi 3.1415926
using namespace std;
using namespace arma;

mat hamming(int n)
{
	int i, j;
	int m = (n + 1) / 2;

	mat result(n, 1);
	for (i = 0; i < m; i++)
	{
		result(i) = 0.54 - 0.46*cos(2 * pi*i / (n - 1));
	}
	
	for (i = m; i < n; i++)
	{
		result(i) = result(n - i - 1); 
	}

	return result;
}



#endif

#pragma once
