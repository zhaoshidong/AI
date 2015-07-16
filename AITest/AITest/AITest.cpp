// AITest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

//设计原理：个体是无意识的，群体在整体的无意识中却
//找到了方向（得到了最优解）
bool IsRight(int Num)//细胞的初始条件:30<N<50
{
	if (Num > 30 && Num < 50)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Cell_No(string pro_name)//返回细胞自身编号
{
	unsigned int loc1;
	loc1 = pro_name.find_last_of("_");
	string file_no;
	int No = 0;
	if (loc1 != string::npos)
	{
		file_no = pro_name.substr(loc1 + 1, 1);
		No = atoi(file_no.c_str());
	}
	return No;
}

int Next_CellLink(int nowNo, int totalCellNo)//返回细胞神经连接编号
{
	if (nowNo <= totalCellNo)
	{
		return nowNo;
	}
	else
	{
		return (nowNo) % totalCellNo;
	}
}

bool msg_check(string filename, bool create = true)//对自身神经连接检查,
{
	fstream file;
	file.open(filename.c_str(), ios::in);
	if (!file)
	{
		if (create)
		{
			file.open(filename.c_str(), ios::out);
		}
		else
		{
			return false;
		}
	}
	file.close();
	return true;
}

int random(int start, int end)//细胞根据应激，做出随机反应
{
	//srand(time(NULL));
	return start + rand() % (end - start);
}

int _tmain(int argc, _TCHAR* argv[])
{
	int iLen = 2 * wcslen(argv[1]);
	char* arg1 = new char[iLen + 1];
	//wcstombs(arg1, argv[1], iLen + 1);
	WideCharToMultiByte(CP_ACP, 0, argv[1], -1, arg1, iLen, NULL, NULL);
	string pro_name(arg1);

	int no_self = Cell_No(pro_name);
	const int total_cell_no = 3;
	char file1[10];
	char file2[10];
	char file3[10];
	itoa(Next_CellLink(no_self + 1, total_cell_no), file1, 10);
	itoa(Next_CellLink(no_self + 2, total_cell_no), file2, 10);
	itoa(no_self, file3, 10);
	string str_file1 = file1;
	string str_file2 = file2;
	string str_file3 = file3;

	string filename1 = "N" + str_file1 + ".txt";	//神经1
	string filename2 = "N" + str_file2 + ".txt";	//神经2
	string filename3 = "Y" + str_file3 + ".txt";	//本身细胞质
	//前一级神经连接，由上一级负责，本级负责检查，没有则退出
	string filename4 = "TN1.txt";

	if (!msg_check(filename1) || !msg_check(filename2))
	{
		cout << "The cell link destroyed." << endl;
	}
	if (!msg_check(filename3))
	{
		cout << "The cytoplasm destroyed." << endl;
	}
	if (!msg_check(filename4, true))
	{
		cout << "The before cell link destroyed." << endl;
		system("PAUSE");
		return 0;
	}
	//以上为初始化，细胞完整性自检
	int times = 20;
	fstream nfile1, nfile2, yfile, tnfile;
	nfile1.open(filename1.c_str(), ios::in);
	nfile2.open(filename2.c_str(), ios::in);
	yfile.open(filename3.c_str(), ios::in);
	tnfile.open(filename4.c_str(), ios::in);
	int flag, data, msg_id;
	int studyData = 0;
	while (times--)
	{
		Sleep(2000);
		tnfile.clear();
		tnfile.seekg(0);
		tnfile >> flag >> data >> msg_id;
		cout << "Received:" << flag << ", " << data
			<< ", " << msg_id << endl;
		if (!flag || msg_id == no_self)
		{
			continue;//自身消息和零神经冲动，跳过
		}
		if (IsRight(data))//细胞判断,符合
		{
			fstream tnfile2;
			bool b_studyWrite = true;
			tnfile2.open(filename4.c_str(), ios::out);
			if (!tnfile2)
			{
				cout << "wrong write" << endl;
			}
			tnfile2 << 0 << " " << data << " " << no_self;
			tnfile2.flush();
			tnfile2.close();
			int i = 0;
			yfile.clear();
			yfile.seekg(0);
			while (! yfile.eof())
			{
				yfile >> studyData;
				i++;
				if (studyData == data)
				{
					b_studyWrite = false;
					break;
				}
			}
			if (b_studyWrite)
			{
				//细胞的学习功能
				fstream yfile_w;
				yfile_w.open(filename3.c_str(), ios::out | ios::app);
				if (! yfile_w)
				{
					cout << "no" << endl;
				}
				yfile_w << data << endl;

			}
		}
	}

	return 0;
}


