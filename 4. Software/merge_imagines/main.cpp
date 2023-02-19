#pragma warning(disable:4996)
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::endl;
using std::string;
using std::vector;


//定位到某一行
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
	string desImgPath = dirPath + "CXK.c";
	
	FILE* desImg = fopen(desImgPath.c_str(), "a+");
	if (desImg == NULL)
		printf("Can't create destination img.\r\n");

	for (int i = 0; i < 37; i++)
	{
		sprintf(tempImgPath, "C:\\Users\\28715\\Desktop\\CXK\\CXK%02d.c", i);
		printf("%s\r\n", tempImgPath);
		FILE* tempImg = fopen(tempImgPath, "r");
		if (tempImg == NULL)
			printf("Can't open temp img.\r\n");

		//跳转到某一行
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



