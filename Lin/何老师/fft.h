#ifndef _BA_FFT_H_
#define _BA_FFT_H_
#include "Complex.h"
#include <cmath>
// 最大FFT样本数
#define MAX_FFT_SIZE 4096

#define PI 3.14159265358979323846

// 指数计算
const int POW[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536 };

// 旋转因子
static Complex W[MAX_FFT_SIZE / 2];

// 对数计算（以2为底）
int LOG(int n)
{
	switch (n)
	{
	case 1:return 0; break;
	case 2:return 1; break;
	case 4:return 2; break;
	case 8:return 3; break;
	case 16:return 4; break;
	case 32:return 5; break;
	case 64:return 6; break;
	case 128:return 7; break;
	case 256:return 8; break;
	case 512:return 9; break;
	case 1024:return 10; break;
	case 2048:return 11; break;
	case 4096:return 12; break;
	case 8192:return 13; break;
	case 16384:return 14; break;
	case 32768:return 15; break;
	case 65536:return 16; break;
	default:return -1;
	}
}

// 旋转因子的生成
void GenerateRotationFactor(int size)
{
	double ReP = 0.0;
	double ImP = 0.0;
	// W[i] = exp(-2*pi*j*(i/N))
	// 只需要用到0~（Size-1）的旋转因子
	for (int i = 0; i < size / 2; i++)
	{
		// 欧拉公式展开
		ReP = cos(2.0 * PI * ((double)i / (double)size));
		ImP = -sin(2.0 * PI * ((double)i / (double)size));
		Complex temp(ReP, ImP);
		W[i] = temp;
	}
}

// 复数数组复制
void ArrayCopy(Complex dest[], Complex src[], int size)
{
	for (int i = 0; i < size; i++)
	{
		dest[i] = src[i];
	}
}

// 码位倒置
void BitReverse(int inIndex[], int indexSize)
{
	int temp = 0;
	int bitSize = LOG(indexSize);
	for (int i = 0; i < indexSize; i++)
	{
		temp = inIndex[i];
		inIndex[i] = 0;
		for (int c = 0; c < bitSize; c++)
		{
			//temp >>= c;
			if ((temp >> c) & 1)
			{
				inIndex[i] += POW[bitSize - 1 - c];
			}
		}
	}
}

// 快速傅立叶变换
void FFT(Complex IN[], Complex OUT[], int Size)
{
	Complex buf[2][MAX_FFT_SIZE];
	// 两个数组，用来交替存储各级蝶形运算的结果
	// Complex bufA[MAX_FFT_SIZE];
	// Complex bufB[MAX_FFT_SIZE];

	//==== 循环控制变量 =========================================

	// 1.序列长度
	// int Size; （形参）

	// 2.序列长度的对数
	int M = LOG(Size);

	// 3.运算级
	int Level = 0;

	// 4.子序列的组号
	int Group = 0;

	// 5.子序列内的元素下标
	// int i = 0; (循环内部有效)
	//==== 循环控制变量如上 =====================================

	// 码位倒置后的输入序列下标
	int indexIn[MAX_FFT_SIZE];

	//==== 对输入序列的下标采取码位倒置操作 =====================
	// 先按顺序初始化下标数组
	for (int i = 0; i < MAX_FFT_SIZE; i++)
	{
		indexIn[i] = i;
	}
	// 执行码位倒置
	BitReverse(indexIn, Size);
	// 此时的indexIn[]数组就是码位倒置后的下标
	//==== 码位倒置操作完成 =====================================

	//==== 计算旋转因子备用 =====================================
	GenerateRotationFactor(Size);
	//==== 旋转因子计算完成 =====================================

	// FFT核心算法开始

	// 算法说明
	// 一、输入序列长度为【Size】，其对数为【M】；
	// 二、共有M个运算级【Level】；
	// 三、每级运算有 POW[M-Level-1] 组【Group】子序列参与蝶形运算，子序列长度为POW[Level+1]；
	// 四、每组子序列只有前半部分作为蝶形运算的“首元”，子序列前半部分的长度为 POW[Level]；
	// 五、每组子序列有 POW[Level] 个蝶形运算结，即子序列前半部分（首元）个数；
	// 六、每个蝶形运算结需要使用位于子序列前半部分的一个元素（首元）和对应的后半部分元素（次元），次元下标=首元下标+POW[Level]；
	// 七、每级运算都是以bufA[]作输入、bufB[]作结果；
	// 八、每完成一级运算，均需要将bufB[]拷贝到bufA[]以供下级运算使用。

	// 开始主循环
	// 首先对每一级运算进行循环。共需要循环M级。
	for (Level = 0; Level < M; Level++)
	{
		// 对该级运算中的每组子序列进行蝶形运算的遍历。共需要遍历POW[M-Level-1]个子序列。
		for (Group = 0; Group < POW[M - Level - 1]; Group++)
		{
			// 对子序列的前半部分进行元素遍历
			// 每次遍历完成一个基本的蝶形运算结
			for (int i = 0; i < POW[Level]; i++)
			{
				// 下标临时变量
				// 该变量记录蝶形运算的绝对位置
				//   因为i仅仅是子序列前半部分内的元素下标，范围是0~POW[Level]
				//   由于每个子序列的长度是POW[Level+1]，所以
				//     绝对下标 = 组内相对下标i + 组偏移量Group*POW[Level+1]。
				int indexBuf = i + Group * POW[Level + 1];

				// 旋转因子上标的计算
				// 旋转因子上标放缩倍数 = 当前运算级的子序列组数
				int ScalingFactor = POW[M - Level - 1];


				// 蝶形运算结。FFT算法核心中的核心。

				// 若为第零级运算
				// 第零级运算使用的是码位倒置后的输入序列
				// 运算结果按照原下标（码位倒置前的下标）存入bufB[]
				//   因为后续的运算顺序控制都是基于自然数序下标进行的
				if (Level == 0)
				{
					buf[0][indexBuf] = IN[indexIn[indexBuf]] + W[i*ScalingFactor] * IN[indexIn[indexBuf + POW[Level]]];
					buf[0][indexBuf + POW[Level]] = IN[indexIn[indexBuf]] - W[i*ScalingFactor] * IN[indexIn[indexBuf + POW[Level]]];
				}
				// 若为其余级运算
				else
				{
					buf[Level % 2][indexBuf] = buf[(Level + 1) % 2][indexBuf] + W[i*ScalingFactor] * buf[(Level + 1) % 2][indexBuf + POW[Level]];
					buf[Level % 2][indexBuf + POW[Level]] = buf[(Level + 1) % 2][indexBuf] - W[i*ScalingFactor] * buf[(Level + 1) % 2][indexBuf + POW[Level]];
				}
			} // 对子序列的蝶形运算结束。共计算POW[Level]个蝶形运算结。

		} // 子序列遍历结束。共循环POW[M-Level-1]次。

		  // 将bufB[]中的运算结果拷贝至bufA[]
		  //ArrayCopy(bufA, bufB, Size);


	} // 运算级循环结束。共循环M次。

	  // 将最后一级运算结果从bufB拷贝到OUT
	ArrayCopy(OUT, buf[(Level + 1) % 2], Size);

} // FFT结束。


mat FFT(mat m) {
	

	int num = m.n_cols;

	Complex* OUT = new Complex[num];

	FFT(mat2com(m), OUT, num);

	return com2mat(OUT, num);

}
#endif
#pragma once
