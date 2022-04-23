#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdlib.h>

using namespace cv;
using namespace std;
int juli(Point p1, Point p2)//计算两点距离
{
	int a, b,c;
	a = abs(p1.x - p2.x);
	b = abs(p1.y - p2.y);
	c = sqrt(a * a + b * b);
	return c;
}
float gaussrand_NORMAL() {//生成满足高斯分布的数据
	static float V1, V2, S;
	static int phase = 0;
	float X;

	
	if (phase == 0) {
		do {
			float U1 = (float)rand() / RAND_MAX;
			float U2 = (float)rand() / RAND_MAX;


			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while (S >= 1 || S == 0);


		X = V1 * sqrt(-2 * log(S) / S);
	}
	else
		X = V2 * sqrt(-2 * log(S) / S);


	phase = 1 - phase;


	return X;
}
float gaussrand(float mean, float stdc) {
	return mean + gaussrand_NORMAL() * stdc;
}


int main(int argc, char** argv) {
	Mat src = imread("zuobiao.jpg");
	if (src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
	Mat dst;
	pyrUp(src, dst, Size(src.cols * 2, src.rows * 2));
	Point p;
	int n = 0;//数据量标志149截止（150个数据）
	Point poi[150];//数据集
	float data = 0;
	//集合1(0.2,0.3)
	for (int i = 0; i < 50; ++i) {
		
		for (;;)
		{
			data = gaussrand(0, 1);
			if (data > 1 || data < 0)
			{
				continue;
			}
			else
			{
				data = (data * dst.cols);// * 0.2;
				p.x = data;
				break;
			}
		}
		for (;;)
		{
			data = gaussrand(0, 1);
			if (data > 1 || data < 0)
			{
				continue;
			}
			else
			{
				data = (data * dst.cols);//* 0.3;
				p.y = data;
				break;
			}
		}
		poi[n] = p;
		n++;
	}
	//集合2(0.5,0.7)
	for (int i = 0; i < 50; ++i) {

		for (;;)
		{
			data = gaussrand(0, 1);
			if (data > 1 || data < 0)
			{
				continue;
			}
			else
			{
				data = (data * dst.cols);// * 0.5;
				p.x = data;
				break;
			}
		}
		for (;;)
		{
			data = gaussrand(0, 1);
			if (data > 1 || data < 0)
			{
				continue;
			}
			else
			{
				data = (data * dst.cols);//* 0.7;
				p.y = data;
				break;
			}
		}
		poi[n] = p;
		n++;
	}

	//集合3(0.7,0.3)
	for (int i = 0; i < 50; ++i) {

		for (;;)
		{
			data = gaussrand(0, 1);
			if (data > 1 || data < 0)
			{
				continue;
			}
			else
			{
				data = (data * dst.cols);// * 0.7;
				p.x = data;
				break;
			}
		}
		for (;;)
		{
			data = gaussrand(0, 1);
			if (data > 1 || data < 0)
			{
				continue;
			}
			else
			{
				data = (data * dst.cols);//* 0.3;
				p.y = data;
				break;
			}
		}
		poi[n] = p;
		n++;
	}
	//随机生成数据
	srand((unsigned)time(NULL));
	for (int i = 0; i < n; i++)
	{
		p.x = rand() % (dst.cols);
		p.y = rand() % (dst.rows);
		//circle(dst, p, 2, Scalar(0, 0, 0), 2, 2);
		poi[i] = p;
	}
	//初始点设置,随机设置2初始点
	Point c1 = poi[1], c1_;
	Point c2 = poi[99], c2_;

	Point c1_p = c1, c2_p = c2;
	int s1, s2;
	Point c1_n[100], c2_n[100];
	Mat z;
	pyrUp(src, z, Size(src.cols * 2, src.rows * 2));
	z.copyTo(dst);
	int k;
	cout << "k=?:" << endl;
	cin >> k;
	for (int m=0;;m++)//kmeans
	{
		z.copyTo(dst);
		//绘制中心坐标
		circle(z, c1, 6, Scalar(255, 0, 0), 1, 2);
		circle(z, c2, 6, Scalar(0, 255, 0), 1, 2);
		int a = 0, b = 0;
		for (int i = 0; i < n; i++)//判断距离，分类
		{
			s1 = juli(poi[i], c1);
			s2 = juli(poi[i], c2);
			if (s1 <= s2)//点距离c1更近
			{
				circle(z, poi[i], 1, Scalar(255, 0, 0), 2, 2);
				c1_n[a] = poi[i];
				a++;
			}
			else//距离c2近
			{
				circle(z, poi[i], 1, Scalar(0, 255, 0), 2, 2);
				c2_n[b] = poi[i];
				b++;
			}
		}
		imshow("1", z);
		waitKey();
		//清空上一个聚类中心绘制的坐标
		circle(z, c1, 6, Scalar(255, 255, 255), 1, 2);
		circle(z, c2, 6, Scalar(255, 255, 255), 1, 2);
		//计算聚类中心1
		int sum_x = 0,sum_y=0;
		for (int i = 0; i < a; i++)
		{
			sum_x = c1_n[i].x + sum_x;
			sum_y = c1_n[i].y + sum_y;
		}
		c1_.x = sum_x / a;
		c1_.y = sum_y / a;
		//计算聚类中心2
		sum_x = sum_y = 0;
		for (int i = 0; i < b; i++)
		{
			sum_x = c2_n[i].x + sum_x;
			sum_y = c2_n[i].y + sum_y;
		}
		c2_.x = sum_x / b;
		c2_.y = sum_y / b;
		//结束判断
		if (juli(c1, c1_) <= 1 && juli(c2, c2_) <= 1)//中心点变化范围很小
		{
			dst.copyTo(z);
			break;
		}
		c1 = c1_; c2 = c2_;
		if (m >= 50)//达到最大循环次数
		{
			dst.copyTo(z);
			break;
		}
	}
	imshow("final", dst);
	waitKey();

	return 0;
}