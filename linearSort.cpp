#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

//функция слияния двух отсортированных файлов, запись идет в первый файл
void mergeFile(std::ifstream& pFile1, std::ifstream& pFile2)
{

	std::ofstream fTmp("mergeResult.bin", ios::binary);
	if (pFile1.is_open() && pFile2.is_open())
	{
		uint32_t a;
		uint32_t b;
		int size = sizeof(a);
		while (pFile1.read((char *)&a, sizeof(a)) && pFile2.read((char *)&b, sizeof(b)))
		{
			if (a > b)
			{
				fTmp.write((char *)&b, sizeof(b));
				pFile1.seekg(-size, ios_base::cur);
			}
			else
			{
				fTmp.write((char *)&a, sizeof(a));
				pFile2.seekg(-size, ios_base::cur);
			}
		}
		if (pFile2.eof())
			pFile1.seekg(-size, ios_base::cur);
		while (pFile1.read((char *)&a, sizeof(a)))
		{
			fTmp.write((char *)&a, sizeof(a));

		}
		while (pFile2.read((char *)&b, sizeof(b)))
			fTmp.write((char *)&b, sizeof(b));
	}
	fTmp.close();

	pFile1.close();
	std::ifstream fMerge("mergeResult.bin", ios::binary);
	std::ofstream fTmp1("output.bin", ios::binary);
	if (fMerge.is_open() && fTmp1.is_open())
	{
		uint32_t value;
		while (!fMerge.eof() && fMerge.read((char *)&value, sizeof(value)))
		{
			fTmp1.write((char *)&value, sizeof(value));
		}
	}
	fMerge.close();

	remove("mergeResult.bin");
}

int main()
{
	//uint32_t start_time = clock();

	uint32_t a;
	int arrSize = 15000000;
	vector< uint32_t> Arr;
	ifstream fIn("input.bin", ios::binary);
	uint32_t i = 0;
	if (fIn.is_open())
	{
		while (!fIn.eof())
		{
			while ((i < arrSize) && !fIn.eof() && fIn.read((char *)&a, sizeof(a)))
			{
				//cout << a << " ";
				Arr.push_back(a);
				++i;
			}
			i = 0;

			std::ifstream fTmp1("output.bin", ios::binary);		//открываем первый файл на чтение
			std::ifstream fTmp2("tmp.bin", ios::binary);		//отркываем второй файл на чтение
			if (!fTmp1.is_open())			//если первый файл существует, считываем в массив, сортируем, и записываем в первый файл
			{
				sort(Arr.begin(), Arr.end());
				fTmp1.close();
				fTmp2.close();
				std::ofstream fT1("output.bin", ios::binary);
				for ( uint32_t j = 0; j < ( uint32_t)Arr.size(); ++j)
					fT1.write((char *)&Arr[j], sizeof(Arr[j]));
				fT1.close();
			}
			else if(!fTmp2.is_open())		//если второй файл существует, считываем в массив, сортируем, и записываем в во второй файл
			{
				sort(Arr.begin(), Arr.end());
				fTmp1.close();
				fTmp2.close();
				std::ofstream fT2("tmp.bin", ios::binary);
				for ( uint32_t j = 0; j < ( uint32_t)Arr.size(); ++j)
					fT2.write((char *)&Arr[j], sizeof(Arr[j]));
				fT2.close();
				//открываем два отсортированных файла и сливаем
				std::ifstream f1("output.bin", ios::binary);	
				std::ifstream f2("tmp.bin", ios::binary);
				mergeFile(f1, f2);
				f1.close();
				f2.close();

				remove("tmp.bin");
			}
			Arr.clear();
		}
	}
	else
	{
		std::cout << "Invalid intput file" << std::endl;
		fIn.close();
		return 1;
	}

	fIn.close();

	remove("tmp.bin");
	//uint32_t end_time = clock();
	//cout << "RunTime: " << (end_time - start_time) / 1000.0 << endl;
	system("pause");
	return 0;
}