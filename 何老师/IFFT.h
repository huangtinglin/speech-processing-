#ifndef _IFFT_
#define _IFFT_
#include<armadillo>
#include"Complex.h"
using namespace arma;

int Pow(char BaseNumber, int IndexNumber)
{
	int m = 1;//指数函数值
	char i;
	for (i = 0; i<IndexNumber; i++)
	{
		m = BaseNumber*m;
	}
	return m;
}

int Log(char BaseNumber, int AntiNumber)
{
	int m = 0;//对数函数值
	while (1)
	{
		AntiNumber = AntiNumber / BaseNumber;
		if (AntiNumber)m++;
		else break;
	}
	return m;
}

void Invert_order(int N, Complex *x)
{
	int LH, j, N1, i, k, n;
	Complex T;
	LH = N / 2;
	j = LH;
	N1 = N - 2;
	for (i = 1; i <= N1; i++)
	{
		if (i<j)
		{
			T = x[i];
			x[i] = x[j];
			x[j] = T;
		}
		k = LH;
		while (j >= k)
		{
			j = j - k;
			k = k / 2;
		}
		j = j + k;
	}
	for (n = 0; n<N; n++)
	{
		if (x[n].Re()<-10000000)
		{
			x[n].Update(0,x[n].Im());
		}
		if (x[n].Im()<-10000000)
		{
			x[n].Update(x[n].Re(),0);
		}
	}
}


void Ifft1(int N, Complex *x)//1、求旋转因子的-kn，再乘1/N
{
	int i;
	Complex temp;//临时复数变量
	int L, M = Log(2, N);//M级蝶形图
	int k, j;
	int StepLength;//k值步长
	int Bank;//两点间的距离
	double ps;
	int r;//旋转因子的幂
	Complex W;//旋转因子
	Invert_order(N, x);//位倒序输入
	for (L = 1; L <= M; L++)
	{
		StepLength = Pow(2, L);
		Bank = StepLength / 2;
		for (j = 0; j <= Bank - 1; j++)
		{
			r = Pow(2, M - L)*j;
			ps = 2 * PI / N*r;
			W.Update(cos(ps), sin(ps));//改变旋转因子
			for (k = j; k <= N - 1; k = k + StepLength)
			{
				Complex x_temp;
				x_temp = W * x[k + Bank];
				temp = x[k] + x_temp;
				x[k + Bank] = x[k] - x_temp;
				x[k] = temp;
			}
		}
	}
	for (k = 0; k<N; k++)
	{
		x[k].Update(x[k].Re() / N, x[k].Im() / N);
	}
}

void Dit2Fft(int N, Complex *x)
{
	int i;
	Complex temp;//临时复数变量
	int L, M = Log(2, N);//M级蝶形图
	int k, j;
	int StepLength;//k值步长
	int Bank;//两点间的距离
	double ps;
	int r;//旋转因子的幂
	Complex W;//旋转因子
	for (L = 1; L <= M; L++)//从第一级开始，逐级进行，共进行M级运算
	{
		StepLength = Pow(2, L);
		Bank = StepLength / 2;
		for (j = 0; j <= Bank - 1; j++)//求出每一级的旋转因子
		{
			r = Pow(2, M - L)*j;
			ps = 2 * PI / N*r;
			W.Update(cos(ps), -sin(ps));
			for (k = j; k <= N - 1; k = k + StepLength)//求出每个旋转因子对应的所有蝶形
			{
				Complex x_temp;
				x_temp = W * x[k + Bank];
				temp = x[k] + x_temp;
				x[k + Bank] = x[k] - x_temp;
				x[k] = temp;
			}
		}
	}
}

void Ifft2(int N, Complex *x)//2、x(n)=IFFT[x(k)]=1/N*{FFT[x*(k)]}*
{
	int k;
	Invert_order(N, x);//位倒序输入
	for (k = 0; k<N; k++)
	{
		x[k].Update(x[k].Re(), -x[k].Im());
	}
	Dit2Fft(N, x);//直接调用FFT
	for (k = 0; k<N; k++)
	{
		x[k].Update(x[k].Re(), -x[k].Im());
	}
	for (k = 0; k<N; k++)
	{
		x[k].Update(x[k].Re()/N, x[k].Im()/N);
	}
}

mat IFFT(mat m) {

	Complex* x = mat2com(m);

	Ifft1(m.n_cols, x);

	return com2mat(x, m.n_cols);

}



#endif