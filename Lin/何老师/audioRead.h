#ifndef _BA_audioRead_H_
#define _BA_audioRead_H_

#include <iostream>
#include <fstream>
#include <string.h>
#include<stdlib.h>
#include <bitset>
#include <iomanip>
#include<armadillo>
#include<vector>
using namespace std;
using namespace arma;

//要在int main()的前面加上函数的声明，因为你的函数写在main函数的后面
int hex_char_value(char ss);
int hex_to_decimal(const char* s);
//string hex_to_binary(char* szHex);

extern double fs;
extern mat x;

struct wav_struct
{
	unsigned long file_size;        //文件大小
	unsigned short channel;            //通道数
	unsigned long frequency;        //采样频率
	unsigned long Bps;                //Byte率
	unsigned short sample_num_bit;    //一个样本的位数
	unsigned long data_size;        //数据大小
	unsigned char *data;            //音频数据 ,这里要定义什么就看样本位数了，我这里只是单纯的复制数据
};

void audioread(string path);

#endif
#pragma once
