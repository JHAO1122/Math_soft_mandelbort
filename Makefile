CC = gcc # 使用gcc编译器
CFLAGS = -Wall -Wextra -O3 -march=native -fopenmp  # 编译警告和调试，使用O3优化和openmp多线程并行加速
LDLIBS = -lm -fopenmp# 链接所需要的math库
XELATEX = xelatex # latex编译器

TARGET = mandelbrot # 生成的可执行文件名
SRCS = src/test.c src/mandelbrot.c # 源文件
OBJS = $(SRCS:.c=.o) # 将.c文件编译成.o文件
TEX = doc/report.tex # 生成latex文档文件
VIDEO = mandelbrot.mp4 # 生成视频文件


.PHONY: all  doc  clean  clean-image  clean-doc  clean-all  # 定义伪目标
all: $(TARGET) # 生成可执行文件

# 链接目标文件生成可执行文件
$(TARGET):$(OBJS) 
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

#生成latex文档
doc:$(TEX)
	cd doc && $(XELATEX) report.tex

frames:$(TARGET)
	./$(TARGET)
video:frames
	ffmpeg -r 30 -i mandelbrot%03d.bmp -vcodec libx264 -pix_fmt yuv420p $(VIDEO)


# 清理目标文件和中间文件
clean:
	rm -f $(OBJS) $(TARGET)
clean-image:
	rm -f *.bmp 
clean-doc:
	rm -f   doc/report.aux doc/report.log doc/report.out doc/report.toc doc/report.bbl doc/report.blg doc/report.dvi doc/report.synctex.gz
clean-all: clean clean-image clean-doc rm -f $(VIDEO)