#include <iostream>
#include <vector>
#include <fstream> 
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cuthill_mckee_ordering.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/bandwidth.hpp>

using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS,
	property<vertex_color_t, default_color_type,
	property<vertex_degree_t, int> > > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::vertices_size_type size_type;

int main(int argc, char *argv[]) {

	std::string strIn = "test.json";			//входной файл по умолчанию
	std::string strOut = "fOut.txt";		//выходной файл по умолчанию

	if (argc == 2)		//проверка на достаточное количество аргументов
	{
		std::cout << "Not enough arguments" << std::endl;
		std::cout << "Application terminated" << std::endl;
		return 1;
	}
	else if (argc >= 3)
	{
		//получение файловых путей из аргументов
		strIn = std::string(argv[1]);		
		strOut = std::string(argv[2]);

		if (strIn.find(".json") == std::string::npos)			//проверка входного файла
		{
			std::cout << "Input file not .json" << std::endl;
			std::cout << "Application terminated" << std::endl;
			return 1;
		}
		if (strOut.find(".txt") == std::string::npos)		//проверка выходного файла
		{
			std::cout << "Output file not .txt" << std::endl;
			std::cout << "Application terminated" << std::endl;
			return 1;
		}
	}

	property_tree::ptree pTree;
	std::vector<std::vector<int>> inVecData;
	Graph G;

	try {
		boost::property_tree::read_json(strIn, pTree);	//считываем .json файл в pTree
	}
	catch (...)										//проверяем на валидность
	{
		std::cout << "Invalid input file";			
		return 1;
	}

	int ki = 0, kj = 0;
	//запись данных в конетейнеры
	for (auto itKey = pTree.begin(); itKey != pTree.end(); ++itKey)
	{
		auto it = pTree.get_child(itKey->first.data());
		std::vector<int> tmpVec;

		for (auto itData = it.begin(); itData != it.end(); ++itData)
		{
			std::string str = itData->second.data();
			//вносим данные в виде списка смежности в граф G
			if(lexical_cast<int>(str) == 1)
				add_edge(ki, kj, G);
			++kj;
			//также вносим данные в двумерный массив inVecData
			tmpVec.push_back(lexical_cast<int>(str));
		}
		kj = 0;
		++ki;
		inVecData.push_back(tmpVec);
	}

	graph_traits<Graph>::vertex_iterator ui, ui_end;

	property_map<Graph, vertex_degree_t>::type deg = get(vertex_degree, G);
	for (boost::tie(ui, ui_end) = vertices(G); ui != ui_end; ++ui)
		deg[*ui] = degree(*ui, G);

	property_map<Graph, vertex_index_t>::type
		index_map = get(vertex_index, G);


	std::cout << "Original bandwidth: " << bandwidth(G) << std::endl;

	std::vector<Vertex> inv_perm(num_vertices(G));		//вектор для обратного порядка сохранения индексов
	std::vector<size_type> perm(num_vertices(G));		//вектор для прямого порядка сохранения индексов

	//алгоритм Катхилла — Макки
	{
		Vertex s = vertex(0, G);	//выбираем периферийную вершину 0
		//обратный порядок Катхилла-Макки
		cuthill_mckee_ordering(G, s, inv_perm.rbegin(), get(vertex_color, G),
			get(vertex_degree, G));
		//перестановка из старых индексов в новые
		for (size_type c = 0; c != inv_perm.size(); ++c)
			perm[index_map[inv_perm[c]]] = c;
		std::cout << "Bandwidth after the algorithm Cuthill-McKee: "
			<< bandwidth(G, make_iterator_property_map(&perm[0], index_map, perm[0]))
			<< std::endl;
	}

	//------------------------SAVE DATA--------------------------------------------------------------
	std::ofstream fOut(strOut, std::ios::binary); // создаём объект класса ofstream для записи и связываем его с файлом fOut
	if (fOut.is_open())
	{
		//вывод данных в виде матрицы смежности учитываю перестановку индексов мз массива perm
		for (int i = 0; i < (int)inVecData.size(); ++i)
		{
			for (auto el : inVecData[perm[i]])
			{
				fOut << el << " ";
			}
			fOut << std::endl;
		}
	}
	else
	{
		std::cout << "Invalid output file" << std::endl;
		return 1;
	}

	fOut.close(); // закрываем выходной файл

	std::cout << std::endl;
	std::cout << "Output file created" << std::endl;
	std::cout << "Application completed successfully" << std::endl;
	system("pause");
	return 0;
}