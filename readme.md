# HoloCubic--多功能透明显示屏桌面站
		
<div align=center><img width="300" src="https://github.com/Potatotatotato/HoloCubic/blob/master/Images/HoloCubic.jpg"/></div>
 
  
   
## 在原项目上作出的改进（改变）
  #### 1. PCB优化；
  #### 2. 用solidworks仿制了metal板外壳，使用半透明树脂光固化打印；
  #### 3. 天气app代码编写；
  #### 4. 用codebolcks配置了LVGL 8.3模拟器；
  #### 5. 总结了使用lvgl生成动画文件的流程。

## PCB
  1. 将tf卡座替换为tb上常见的型号（带弹簧的可以自锁的那种）；
  2. 替换fpc-8p封装，稚晖君的fpc排座最大的2个焊盘位置有点靠外，和我买的不一样；
  3. 将LDO替换为最大电流为500ma的rt9013；
  4. 将与EN引脚相连的0.1uF电容替换为4.7uF的电容（官方推荐1uF，但经本人测试，1uF的电容有时候也下载不进去程序）；
 5. 重新走线。为方便走线，减小回路长度，修改pcb为4层板；

  由于本人比较懒惰，所以我直接在pcb文件上修改封装、重新走线，所以pcb文件和sch文件之间的编译会出问题，但pcb本身是通过了DRC检测的。

## 外壳
### navie版本
  这个版本需要3d打印两个部分然后组装，上下部分可以用磁铁吸附。但这个版本需要粘贴棱镜，稚晖君推荐的是OCA干胶。然而我没想到这玩意这么难粘，我粘了十几次都失败了，始终有气泡，还弄坏了一块屏幕，我真的无语了，被迫放弃。
  
  <div align=center><img width="300" src="https://github.com/Potatotatotato/HoloCubic/blob/master/Images/metal_model.jpg"/></div>

### metal版本
  我使用solidworks绘制了可用光固化3d打印的metal板模型，并开源出来。本人强烈这个版本，因为这个版本不需要粘屏幕。取而代之的是，我采用了白乳胶将棱镜粘到外壳上，效果还不错！
  
  本模型适用于稚晖君naive版本pcb，适用于1.6mm板厚（1.2mm板厚应该也行，大家结合3D图查找有无冲突的部分），强烈推荐用半透明树脂3D打印，以获得最好的灯光效果。此外，我将solidworks源文件分享出来的原因是：由于最开始打错了板子，所以希望在网上找到metal版本的外壳的3D文件，进而修改成适合naive电路板的外壳，但找了很久都只有stl文件。无奈之下，只有自己画图。
  
  <div align=center><img width="300" src="https://github.com/Potatotatotato/HoloCubic/blob/master/Images/navie_model.jpg"/></div>
  
## 软件
  在稚晖君移植驱动的基础上，模仿透明小电视AIO版本[透明小电视AIO版本制作经验分享](https://www.bilibili.com/video/BV1d3411T77a/?spm_id_from=333.788.recommend_more_video.0&vd_source=e6cfc8577ccc9621465b12d49ef2c1c3)，用lvgl开发了天气页面。限于个人能力，没有实现app切换，等以后有空了再来完善。

## LVGL生成动画文件的流程
  ### 1. 制作或下载动画，导出图片序列至文件夹
  <div align=center><img width="600" src="https://github.com/Potatotatotato/HoloCubic/blob/master/Images/make_anim_1.jpg"/></div>
  
  ### 2. 使用LVGL在线的image converter，选择合适的color format，将图片序列导出为.c文件序列
  <div align=center><img width="600" src="https://github.com/Potatotatotato/HoloCubic/blob/master/Images/make_anim_2.jpg"/></div>
  
  ### 3. 将所有的.c文件合并，注意只保留数组和结构体。这里我编写了一段c++代码来进行自动合并
  
```cpp
#pragma warning(disable:4996)
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::endl;
using std::string;
using std::vector;

#define IMG_AMOUNT 37

//函数：定位到某一行
int GetSeek(FILE* fp, int line)	//必须是可读的指针,只有可写的不行 
{
	char ch;
	int i = 1;

	fseek(fp, SEEK_SET, 0);
	if (fgetc(fp) == EOF) {		//如果开头读取到结束符返回-1 
		return -1;
	}
	fseek(fp, SEEK_SET, 0);		//确保指针在开头 

	while (!(line == i)) {

		ch = fgetc(fp);		// 读取 
		if (ch == '\n') {
			i = i + 1;			//计数 
		}
		if (ch == EOF) {		//如果读取中读到结束符返回-1 
			return -1;
		}
	}
	return 0;
}


int main()
{
	char tempChar;
	char tempImgPath[100];
	
	string dirPath = "C:\\Users\\28715\\Desktop\\CXK\\";
	string rawImgPath = "C:\\Users\\28715\\Desktop\\CXK\\CXK%02d.c";
	string desImgPath = dirPath + "CXK.c";
	
	FILE* desImg = fopen(desImgPath.c_str(), "a+");
	if (desImg == NULL)
		printf("Can't create destination img.\r\n");
	
	//开始复制.c图片文件
	for (int i = 0; i < IMG_AMOUNT; i++)
	{
		//打开需要转化的图片
		sprintf(tempImgPath, rawImgPath.c_str(), i);
		printf("%s\r\n", tempImgPath);
		FILE* tempImg = fopen(tempImgPath, "r");
		if (tempImg == NULL)
			printf("Can't open temp img.\r\n");

		//前面的23行略过
		GetSeek(tempImg, 23);

		//开始复制文件
		tempChar = fgetc(tempImg);
		while (tempChar != EOF)
		{
			fputc(tempChar, desImg);
			tempChar = fgetc(tempImg);
		}
		fclose(tempImg);
	}
	fputc(0, desImg);

	fclose(desImg);


	return 0;
}
```
  
  
  ### 4. 用notepad++ 批量修改.c图片文件的宏定义

  <div align=center><img width="1000" src="https://github.com/Potatotatotato/HoloCubic/blob/master/Images/make_anim_4.jpg"/></div>
  
  ...
`\d` 表示匹配一个数字字符。
