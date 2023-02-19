#pragma warning(disable:4996)
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::endl;
using std::string;
using std::vector;


//��λ��ĳһ��
int GetSeek(FILE* fp, int line)	//�����ǿɶ���ָ��,ֻ�п�д�Ĳ��� 
{
	char ch;
	int i = 1;

	fseek(fp, SEEK_SET, 0);
	if (fgetc(fp) == EOF) {		//�����ͷ��ȡ������������-1 
		return -1;
	}
	fseek(fp, SEEK_SET, 0);		//ȷ��ָ���ڿ�ͷ 

	while (!(line == i)) {

		ch = fgetc(fp);		// ��ȡ 
		if (ch == '\n') {
			i = i + 1;			//���� 
		}
		if (ch == EOF) {		//�����ȡ�ж�������������-1 
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

		//��ת��ĳһ��
		GetSeek(tempImg, 23);

		//��ʼ�����ļ�
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



