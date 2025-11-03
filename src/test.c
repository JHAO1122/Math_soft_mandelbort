#include <stdio.h>
#include "mandelbrot.h"

int main() {
    mandelbrot m;
    int max_iter = 1000;     // 最大迭代次数
    int width = 800;      // 图像宽度  
    int height = 600;       // 图像高度
    int frames = 90;      // 帧数
    double center_x = -0.743643887;    //缩放中心点横坐标
    double center_y = 0.131825911;     //缩放中心点纵坐标
    double zoom = 0.95;    //缩放倍数
    double x0_min = -1.5;
    double x0_max = 1.5;
    double y0_min = -1.0;
    double y0_max = 1.0;
        // 初始化mandelcrot集
        mandelbrot_init(&m, -1.5, 1.5, -1.0, 1.0, max_iter, width, height);
        // 保存mandelbrot图像为BMP文件
        const char *filename = "mandelbrot.bmp";
        mandelbrot_save_bmp(&m, filename);
        printf("已将mandelbrot集图像保存为 %s\n", filename);
        // 释放先前分配的行列内存
        mandelbrot_free(&m);
        // 生成mandelbrot集的动画
        for(int i = 0; i < frames; i++)
        {
            int current_max_iter = max_iter + i * 100; 
            double decay = pow(zoom, i);  // 缩放
            double x_min = center_x - (center_x - x0_min) * decay;
            double x_max = center_x + (x0_max - center_x) * decay;
            double y_min = center_y - (center_y - y0_min) * decay;
            double y_max = center_y + (y0_max - center_y) * decay;    
            mandelbrot_init(&m, x_min, x_max, y_min, y_max, current_max_iter, width, height);
            char filename[100];
            sprintf(filename, "mandelbrot%03d.bmp", i);
            mandelbrot_save_bmp(&m, filename);
            mandelbrot_free(&m);
        }
    return 0;
}