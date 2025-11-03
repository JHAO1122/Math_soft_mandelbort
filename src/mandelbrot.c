# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <complex.h>
# include "mandelbrot.h"
# define M_PI 3.14159265358979323846
/**
 * @brief 文件头结构体
 * @param bftype 位图的类型
 * @param bfsize 位图的大小
 * @param bfreserved1 保留位1，必须为0
 * @param bfreserved2 保留位2，必须为0
 * @param bfoffbits 位图的数据偏移量
 */
#pragma pack(push, 1)
struct BMPfileheader
{
    unsigned short bftype;
    unsigned int bfsize;
    unsigned short bfreserved1;
    unsigned short bfreserved2;
    unsigned int bfoffbits;
};
/**
 * @brief 信息头结构体
 * @param bisize 信息头的大小
 * @param biwidth 位图的宽度
 * @param biheight 位图的高度
 * @param bipplanes 颜色平面数
 * @param bitcount 位深度，每个像素占用的位数 
 * @param bicompression 压缩类型
 * @param bisizeimage 位图数据大小
 * @param bixpelspermeter 水平分辨率
 * @param biypelspermeter 垂直分辨率
 * @param bicolor_used 实际使用的颜色数
 * @param bicolor_important 重要颜色数
 */
struct BMPinfoheader
{
    unsigned int bisize;
    int biwidth;
    int biheight;
    unsigned short bipplanes;
    unsigned short bitcount;
    unsigned int bicompression;
    unsigned int bisizeimage;
    int bixpelspermeter;
    int biypelspermeter;
    unsigned int bicolor_used;
    unsigned int bicolor_important;
};
#pragma pack(pop)

void mandelbrot_init(mandelbrot *m, double x_min, double x_max, double y_min, double y_max, int max_iter, int width, int height)
{
    m->x_min = x_min;
    m->x_max = x_max;
    m->y_min = y_min;
    m->y_max = y_max;
    m->max_iter = max_iter;
    m->width = width;
    m->height = height;
    m->iter_count = (int **)malloc(sizeof(int*) * m->width); //分配行内存
    for(int i = 0; i < m->width; i++)
    {
        m->iter_count[i] = (int*)malloc(sizeof(int) * m->height); //分配列内存
    }
    double dx = (m->x_max - m->x_min) / m->width; //迭代的水平变化量，即列宽度
    double dy = (m->y_max - m->y_min) / m->height; //迭代的垂直变化量，即行高度
    #pragma omp parallel for schedule(dynamic) //OpenMP多线程并行计算
    for(int i = 0; i < m->width; i++)
    {
        for(int j = 0; j < m->height; j++)
        {
            complex z = 0 + 0 * I; //初始化Z为0
            complex c = m->x_min + i * dx + (m->y_min + j * dy) * I; //计算当前点对应的复数值
            m->iter_count[i][j] = m->max_iter; //初始化迭代次数
            for(int iter_num = 0; iter_num < m->max_iter; iter_num++)
            {
                z = z * z + c; //迭代过程
                if(cabs(z) > 2.0) 
                {
                m->iter_count[i][j] = iter_num; //如果发散，更新迭代次数
                break; //退出迭代
                }
            }
        }
    }
}

void mandelbrot_save_bmp(const mandelbrot *m,const char *filename)
{
    FILE *fp = fopen(filename,"wb");
    if(fp == NULL) return;  
    int width_size = m->width * 3; //每个像素点需要3个字节rgb
    int filler = (4 - (width_size % 4)) % 4; //填充字节数，需要是4的倍数
    width_size += filler; 
    #pragma pack(push, 1)
    struct BMPfileheader fileheader;
    struct BMPinfoheader infoheader;
    fileheader.bftype = 0x4d42; //BMP文件的文件标识
    fileheader.bfsize = 54 + width_size * m->height; //14 + 40 +  像素点大小和
    fileheader.bfreserved1 = 0; //必须设为0
    fileheader.bfreserved2 = 0; //必须设为0
    fileheader.bfoffbits = 54; //偏移量，14 + 40
    infoheader.bisize = 40; //信息头的大小
    infoheader.biwidth = m->width; 
    infoheader.biheight = m->height; 
    infoheader.bipplanes = 1; //固定为1
    infoheader.bitcount = 24; //常见值24
    infoheader.bicompression = 0; //无压缩
    infoheader.bisizeimage = width_size * m->height; //像素点大小和
    infoheader.bixpelspermeter = 0;
    infoheader.biypelspermeter = 0;
    infoheader.bicolor_used = 0; //默认
    infoheader.bicolor_important = 0; //颜色同等重要
    #pragma pack(pop)
    //写入文件头和信息头
    fwrite(&fileheader, sizeof(fileheader), 1, fp);
    fwrite(&infoheader, sizeof(infoheader), 1, fp);
    for(int j = m->height - 1; j >= 0; j--) {
        for(int i = 0; i < m->width; i++) {
            int iter = m->iter_count[i][j];
            //通过迭代次数计算颜色分配（此处参考AI提供的算法）
            double ratio = (iter == m->max_iter) ? 0.0 : (double)iter / m->max_iter;
            ratio = pow(ratio, 0.3); 
            unsigned char red = 255 * 0.5 * (sin(2.0 * M_PI * ratio)) + 128;
            unsigned char green = 255 * 0.5 * (sin(2.0 * M_PI * ratio + 2.0 * M_PI / 3)) + 128;
            unsigned char blue = 255 * 0.5 * (sin(2.0 * M_PI * ratio + 4.0 * M_PI / 3)) + 128;
            //写入像素点的颜色数据，BRG格式
            fwrite(&blue, 1, 1, fp);
            fwrite(&green, 1, 1, fp);
            fwrite(&red, 1, 1, fp);
        } 
        for(int k = 0; k < filler; k++) fputc(0, fp);     
    }
    fclose(fp);
}

void mandelbrot_free(mandelbrot *m)
{
    for(int i = 0; i <m->width; i++) {free(m->iter_count[i]);}
    free(m->iter_count);
}

