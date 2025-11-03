# 这是我的数学软件期中项目作业——mandelbrot集的C语言实现
## 项目目的
用C语言实现mandelbrot集，将其通过静态图片、动态视频等进行可视化展现，使用多线程并行计算加速。
## 实现功能
1. mandelbrot集的总体静态图片展现
2. mandelbrot集的局部动态视频展现
3. 多线程并行计算加速
## 使用方法
1. make video或者make后输入./mandelbrot来编译运行程序，可生成mandelbrot集bmp静态图片和mp4动态视频
2. make doc生成项目报告pdf文档
3. make clean-all清除除了动态视频和说明文档外所有生成的文件
4. make clean-rest清除动态视频和说明文档
## 依赖关系
1. ffmpeg用于生成动态视频
2. math库
3. OpenMP库
## 加分因素
1. 使用bmp图片格式增加了展示效果
2. 达到了根据迭代次数改变色彩的展示效果
3. 生成了局部的动态视频
4. 使用并行计算提高了计算效率
