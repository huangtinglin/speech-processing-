// 何老师.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


//http://blog.csdn.net/sandeepin/article/details/60866222

#include<iostream>
#include<algorithm>
#include<vector>
#include<stack>
#include<queue>
#include<string>
#include<stdio.h>
#include<memory.h>
#include<string.h>
#include<time.h>
#include<armadillo>
using namespace std;
using namespace arma;
#define pi 3.1415926
typedef mat WIN;
int a = 5;

class Segment {
public:
	mat begin;
	mat end;
	mat duration;
	Segment(int n) {
		begin.set_size(1, n);
		end.set_size(1, n);
		duration.set_size(1, n);
	}
};

mat enframe(mat x, mat win, int inc) {
	int len;
	mat x1 = x;
	int nx = x1.n_elem;
	int nwin = win.n_elem;
	if (nwin == 1) {
		len = win(0);
	}
	else {
		len = nwin;
	}

	if (inc == -1) {
		inc = len;
	}

	int nf = floor((nx - len + inc) / inc);
	mat frameout(1, nf*len + 1);
	mat a(nf, 1);
	for (int i = 0; i < nf; i++) {
		a(i) = i;
	}
	mat indf = inc*a;
	mat inds(1, len);
	for (int i = 1; i <= len; i++) {
		inds(i - 1) = i;
	}

	int k = 0;
	inds = repmat(inds, nf, 1);
	indf = repmat(indf, 1, len);
	a = inds + indf;
	for (int i = 0; i < a.n_elem; i++) {
		frameout(k++) = x(a(i) - 1);
	}
	frameout.reshape(nf, len);

	if (nwin > 1) {
		a = win.t();
		frameout = frameout%repmat(a, nf, 1);
	}
	return frameout;
}

mat FrameTimeC(int frameNum, int framelen, int inc, int fs) {
	mat frameTime(1, frameNum);
	for (int i = 1; i <= frameNum; i++) {
		double k = i;
		frameTime(i-1) = ((k - 1)*inc + framelen / 2) / fs;
	}
	return frameTime;
}

/*
	matlab的Segment数组，属性为begin、end、duration
	我的是Segment变量，属性为begin矩阵、end矩阵、duration矩阵
	matlab要取soundSegment(1).begin
	这里是soundSegment.begin(1)
	因为mat一定要声明好大小，所以这里有个循环我做了两次，没写好，不过程序是对的，下标从0开始
*/
Segment findSegment(mat express)
{
	int i, k;
	mat voicedIndex(1, express.n_cols);
	if (express(0) == 0) {
		k = 0;
		for (i = 0; i < express.n_cols; i++) {
			if (express(i) != 0)
				voicedIndex(k++) = i + 1;
		}
	}
	else
		voicedIndex = express;
	
	k = 1;
	for (i = 0; i <= voicedIndex.n_elem - 2; i++) {
		if (voicedIndex(i + 1) - voicedIndex(i) > 1) k++;
	}

	Segment soundSegment(k);
	k = 0;
	soundSegment.begin(0) = voicedIndex(0);
	for (i = 0; i <= voicedIndex.n_elem - 2; i++) {
		if (voicedIndex(i + 1) - voicedIndex(i) > 1) {
			soundSegment.end(k) = voicedIndex(i);
			soundSegment.begin(k + 1) = voicedIndex(i + 1);
			k++;
		}
	}
	
	soundSegment.end(k) = voicedIndex(voicedIndex.n_elem - 1);
	for (i = 0; i <= k; i++) {
		soundSegment.duration(i) = soundSegment.end(i) - soundSegment.begin(i) + 1;
	}
	return soundSegment;
}

mat meta(int n) {
	mat x(2, n + 1);
	for (int i = 1; i <= n; i++) {
		x(1, i) = i;
	}
	return x;
}

double mean(mat a) {
	double s = 0;
	for (int i = 1; i <= a.n_rows; i++) {
		for (int j = 1; j < a.n_cols; j++) {
			s += a(i, j);
		}
	}
	return s / a.n_elem;
}

mat hanning(int wlen) {
	mat p = meta(wlen / 2);
	mat w(3, wlen / 2 + 1);

	for (int i = 1; i <= wlen / 2; i++) {
		double t = 0.5*(1 - cos(2 * pi*p(1, i) / (wlen + 1)));
		w(1, i) = t;
		w(2, wlen / 2 - i + 1) = t;
	}
	for (int j = 1; j <= 2; j++)
		for (int i = 1; i <= wlen / 2; i++)
			cout << w(j, i) << endl;

	return w;
}

mat linsomoothm(mat x, int n) 
{
	if (n == -1)
		n = 3;
	//mat win = hanning(n);
	mat win;
	win << 0.75 << endr << 0.75 << endr;
	mat s = sum(win);
	win = win / s(0);
	x.reshape(1, x.n_elem);
	int len = x.n_elem;
	mat y = zeros(len, 1);
	if (n % 2 == 0) 
	{
		int l = n / 2;
		mat x1(1, 1 + x.n_elem + l);
		for (int i = 0; i < 1 + x.n_elem + l; i++) {
			if (i == 0)
				x1(i) = x(0);
			else if (i >= x.n_elem + 1)
				x1(i) = x(len - 1);
			else
				x1(i) = x(i - 1);
		}
		x = x1.t();
	}
	else {
		int l = (n - 1) / 2;
		mat x1(1, 1 + x.n_elem + 2);
		for (int i = 0; i < 1 + x.n_elem + 2; i++) {
			if (i == 0)
				x1(i) = x(0);
			else if (i >= x.n_elem + 1)
				x1(i) = x(len-1);
			else
				x1(i) = x(i-1);
		}
		x = x1.t();
	}
	for (int i = 1; i <= len; i++) {
		//cout << x.rows(i - 1, i + n - 2) << endl;
		mat a = win % x.rows(i - 1, i + n - 2);
		a = sum(a);
		y(i - 1) = a(0);
	}
	return y;
}

int main()
{
	mat x = { 1,2,3,4,5,6,7,8,9 };
	cout << linsomoothm(x, 2) << endl;

	/*mat express(1, 68);
	for (int i = 0; i < 50; i++) {
		express(i) = i + 1;
	}
	for (int i = 50; i < 59; i++) {
		express(i) = i + 2;
	}
	for (int i = 59; i < 68; i++) {
		express(i) = i + 3;
	}
	Segment a = findSegment(express);

	cout << a.begin << endl;
	cout << a.end << endl;
	cout << a.duration << endl;*/

	//1、构造矩阵A、B
	/*mat A(2, 2), B(2, 2);
	for (int i = 0; i < 4; i++)
	{
		A(i) = i + 1;//i：以列为排列顺序，矩阵的第i个元素
		B(i) = i + 5;
	}
	A.print("矩阵A为：");
	B.print("矩阵B为：");
	//2、矩阵A与矩阵B相加
	mat AAddB = A + B;
	AAddB.print("A + B =");
	//3、矩阵A与矩阵B相减
	mat AMinusB = A - B;
	AMinusB.print("A - B =");
	//4、矩阵A与矩阵B相乘
	//mat AMulB = A * B;
	//AMulB.print("A * B =");
	//5、矩阵A与矩阵B点除
	mat ADiviB = A / B;
	ADiviB.print("A / B =");
	//6、矩阵A与矩阵B点乘
	mat ADotMulB = A % B;
	ADotMulB.print("A % B =");

	B.col(1) = A.col(1) * 2;
	cout << endl << B << endl;

	B.reshape(1, 2);
	cout << B << endl;


	//cout << repmat(B, 5, 1) << endl;

	mat c;
	c << 0.165300 << 0.454037 << 0.995795 << 0.124098 << 0.047084 << endr
		<< 0.688782 << 0.036549 << 0.552848 << 0.937664 << 0.866401 << endr
		<< 0.348740 << 0.479388 << 0.506228 << 0.145673 << 0.491547 << endr
		<< 0.148678 << 0.682258 << 0.571154 << 0.874724 << 0.444632 << endr
		<< 0.245726 << 0.595218 << 0.409327 << 0.367827 << 0.385736 << endr;
	cout << c << endl;

	mat x;
	x << 5 << endr
		<< 6 << endr
		<< 14 << endr
		<< 11 << endr
		<< 15 << endr
		<< 9 << endr
		<< 7 << endr
		<< 13 << endr
		<< 18 << endr
		<< 17 << endr
		<< 4 << endr
		<< 20 << endr
		<< 16 << endr
		<< 1 << endr
		<< 10 << endr
		<< 19 << endr
		<< 8 << endr
		<< 2 << endr
		<< 12 << endr
		<< 3 << endr;


	mat win;
	win << 10 << endr;
	int inc = 6;
	cout << enframe(x, win, inc);*/

	//cout << FrameTimeC(211, 256, 128, 8000) << endl;

	

	return 0;
}

