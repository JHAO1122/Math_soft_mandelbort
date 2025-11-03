# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <complex.h>
# include <ctype.h>
# include <unistd.h>
/**
 * @brief 结构体mandelbrot
 * @param x_min 定义区域的x轴最小值，复平面左边界
 * @param x_max 定义区域的x轴最大值，复平面右边界
 * @param y_min 定义区域的y轴最小值，复平面下边界
 * @param y_max 定义区域的y轴最大值，复平面上边界
 * @param max_iter 最大迭代次数
 * @param width 输出图像的宽度或列数
 * @param height 输出图像的高度或行数
 */
typedef struct
{
    double x_min;
    double x_max;
    double y_min;
    double y_max;
    int max_iter;
    int width;
    int height;
    int **iter_count;
}mandelbrot;
/**
 * @brief 初始化mandelbrot结构体的函数
 */
void mandelbrot_init(mandelbrot *m, double x_min, double x_max, double y_min, double y_max, int max_iter, int width, int height);
/**
 * @brief 生成并保存mandelbrot图像为bmp文件的函数
 * @param m mandelbrot结构体指针
 * @param filename 输出图像的文件名
 */
void mandelbrot_save_bmp(const mandelbrot *m,const char *filename);

/**
 * @brief 释放mandelbrot结构体的内存的函数
 */
void mandelbrot_free(mandelbrot *m);




