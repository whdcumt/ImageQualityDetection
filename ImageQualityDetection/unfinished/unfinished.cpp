// unfinished.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

/******************************************************************************** 
*����������  DefRto ���㲢����һ��ͼ���������    
*���������� frame  ��ɫ֡ͼ 
*��������ֵ��double   �����ȱ�ʾֵ����Ը���Ƶ����������С��10Ϊģ��������14Ϊ���                   
*********************************************************************************/  
double DefRto(Mat frame)  
{  
    Mat gray;  
    cvtColor(frame,gray,CV_BGR2GRAY);  
    IplImage *img = &(IplImage(gray));  
    double temp = 0;  
    double DR = 0;  
    int i,j;//ѭ������  
    int height=img->height;  
    int width=img->width;  
    int step=img->widthStep/sizeof(uchar);  
    uchar *data=(uchar*)img->imageData;  
    double num = width*height;  
  
    for(i=0;i<height;i++)  
    {  
        for(j=0;j<width;j++)  
        {  
            temp += sqrt((pow((double)(data[(i+1)*step+j]-data[i*step+j]),2) + pow((double)(data[i*step+j+1]-data[i*step+j]),2)));  
            temp += abs(data[(i+1)*step+j]-data[i*step+j])+abs(data[i*step+j+1]-data[i*step+j]);  
        }  
    }  
    DR = temp/num;  
    return DR;  
}  
/******************************************************************************************** 
*����������  calcCast    ���㲢����һ��ͼ���ɫƫ���Լ���ɫƫ����    
*����������  InputImg    ��Ҫ�����ͼƬ��BGR��Ÿ�ʽ����ɫ��3ͨ�������Ҷ�ͼ��Ч 
*           cast        �������ƫ��ֵ��С��1��ʾ�Ƚ�����������1��ʾ����ɫƫ 
*           da          ��/��ɫƫ����ֵ��da����0����ʾƫ�죻daС��0��ʾƫ�� 
*           db          ��/��ɫƫ����ֵ��db����0����ʾƫ�ƣ�dbС��0��ʾƫ�� 
*��������ֵ�� ����ֵͨ��cast��da��db����Ӧ�÷��أ�����ʽ����ֵ 
*********************************************************************************************/  
void colorException(Mat InputImg,float& cast,float& da,float& db)  
{  
    Mat LABimg;  
    cvtColor(InputImg,LABimg,CV_BGR2Lab);//�ο�http://blog.csdn.net/laviewpbt/article/details/9335767  
                                       //����OpenCV����ĸ�ʽ��uint8�����������LABimg�ӱ�׼��0��100��-127��127��-127��127����ӳ�䵽��0��255��0��255��0��255�ռ�  
    float a=0,b=0;  
    int HistA[256],HistB[256];  
    for(int i=0;i<256;i++)  
    {  
        HistA[i]=0;  
        HistB[i]=0;  
    }  
    for(int i=0;i<LABimg.rows;i++)  
    {  
        for(int j=0;j<LABimg.cols;j++)  
        {  
            a+=float(LABimg.at<cv::Vec3b>(i,j)[1]-128);//�ڼ�������У�Ҫ���ǽ�CIE L*a*b*�ռ仹ԭ ��ͬ  
            b+=float(LABimg.at<cv::Vec3b>(i,j)[2]-128);  
            int x=LABimg.at<cv::Vec3b>(i,j)[1];  
            int y=LABimg.at<cv::Vec3b>(i,j)[2];  
            HistA[x]++;  
            HistB[y]++;  
        }  
    }  
    da=a/float(LABimg.rows*LABimg.cols);  
    db=b/float(LABimg.rows*LABimg.cols);  
    float D =sqrt(da*da+db*db);  
    float Ma=0,Mb=0;  
    for(int i=0;i<256;i++)  
    {  
        Ma+=abs(i-128-da)*HistA[i];//���㷶Χ-128��127  
        Mb+=abs(i-128-db)*HistB[i];  
    }  
    Ma/=float((LABimg.rows*LABimg.cols));  
    Mb/=float((LABimg.rows*LABimg.cols));  
    float M=sqrt(Ma*Ma+Mb*Mb);  
    float K=D/M;  
    cast = K;  
    return;  
} 
int main(int argc, char* argv[])
{
	const char*imagename="E:\\ImageQualityDetection\\ImageQualityDetection\\t5.jpg";  //�˴���Ҫ����Ҫ��ʾͼƬ�ļ���ʵ���ļ�λ�ø���
	//���ļ��ж���ͼ��
	Mat img=imread(imagename);
	//�����ȡͼ��ʧ��
	if(img.empty())
	{
		fprintf(stderr,"Can not load image%s\n",imagename);
		return -1;
	}
	//��ʾͼ��
	imshow("image",img); 
	//*****************************************************************************
	 //ɫƫ���Գ���
	//float cast=0.0;
	//float da=0.0;
	//float db=0.0;
	//colorException(img,cast,da,db);
	//printf("ɫƫֵΪ:%f,����ƫ����ֵΪ��%f,����ƫ����ֵΪ��%f\n",cast,da,db);
	//�˺����ȴ����������������������
	//*****************************************************************************
	double def;
	def=DefRto(img);  
	printf("������Ϊ:%f\n",def);
	waitKey();
	while(1);
	return 0;
}

 