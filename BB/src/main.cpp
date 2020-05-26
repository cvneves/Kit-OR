#include <iostream>
using namespace std;

#include "data.h"
#include "hungarian.h"
#include "Tour.h"
#include "Node.h"

int main(int argc, char **argv)
{
	Data *data = new Data(argc, argv[1]);
	data->readData();

	double **cost = new double *[data->getDimension()];
	for (int i = 0; i < data->getDimension(); i++)
	{
		cost[i] = new double[data->getDimension()];
		for (int j = 0; j < data->getDimension(); j++)
		{
			cost[i][j] = data->getDistance(i, j);
		}
	}

	hungarian_problem_t p;
	int mode = HUNGARIAN_MODE_MINIMIZE_COST;

	hungarian_init(&p, cost, data->getDimension(), data->getDimension(), mode); // Carregando o problema

	// double obj_value = hungarian_solve(&p);
	// cout << "Obj. value: " << obj_value << endl;
	// cout << "Assignment" << endl;
	// hungarian_print_assignment(&p);

	// Inicialização da arvore

	Node root, best_node;

	best_node.obj_value = std::numeric_limits<double>::infinity();
	std::vector<int> current_subtour;

	std::list<Node> tree = {root};

	double upper_bound = 99999999;

	auto node = tree.begin();
	int cont = 0;

	while (tree.empty() == false)
	{
		double menor_lb = std::numeric_limits<double>::infinity();

		for (auto it = tree.begin(); it != tree.end(); it++)
		{
			if (it->obj_value - menor_lb < std::numeric_limits<double>::epsilon())
			{
				menor_lb = it->obj_value;
				node = it;
			}
		}

		//		node = tree.begin();
		// node = tree.end();
		// node--;

		hungarian_free(&p);
		hungarian_init(&p, cost, data->getDimension(), data->getDimension(), mode);


		current_subtour = calcularSolucao(&p, *node);

		
		

		// std::cout << node->obj_value << "\n";

		// std::cout << tree.size() << ", " << cont << "\n";

		// // LOG
		// if (cont == 1)
		// {
		// std::cout << "\n\nTree size: " << tree.size() << "\n";
		// std::cout << "Soluçao: ";
		// printSolucao(current_subtour);
		// std::cout << "LB: " << node->obj_value << "\n";
		// for (const auto &arco : node->arcos_proibidos)
		// {
		// 	std::cout << "(" << arco.first << "," << arco.second << "), ";
		// }
		// std::cout << "\n";
		// cont = 0;
		// }

		if (node->obj_value > upper_bound)
		{
			tree.erase(node);
			continue;
		}

		if (node->pruning == true)
		{
			if (node->obj_value < best_node.obj_value)
			{
				best_node = *node;
				upper_bound = node->obj_value;
			}
			tree.erase(node);
			continue;
		}

		for (int i = 0; i < current_subtour.size() - 1; i++)
		{
			Node n;
			n.arcos_proibidos = node->arcos_proibidos;
			std::pair<int, int> arco;
			arco.first = current_subtour[i];
			arco.second = current_subtour[i + 1];
			n.arcos_proibidos.push_back(arco);
			tree.insert(tree.end(), n);
			hungarian_free(&p);
			hungarian_init(&p, cost, data->getDimension(), data->getDimension(), mode);

			// current_subtour = calcularSolucao(&p, *node);
			// n.obj_value = node->obj_value;
		}

		tree.erase(node);
		cont++;
	}

	// for (auto node = tree.begin(); node != tree.end(); node++, cont++)
	// {
	// 	// std::cout << tree.size() * sizeof(Node) / 1000000.0 << "\n";

	// 	if (node != tree.begin())
	// 	{
	// 		hungarian_init(&p, cost, data->getDimension(), data->getDimension(), mode);
	// 		current_subtour = calcularSolucao(&p, *node);
	// 	}

	// 	//printSolucao(current_subtour);
	// 	//std::cout << node->obj_value << ", " << node->pruning << "\n";
	// 	std::cout << tree.size() << ", " << cont << "\n";

	// 	if(node->obj_value - upper_bound > std::numeric_limits<double>::epsilon())
	// 	{
	// 		continue;
	// 	}

	// 	if (node->pruning == true)
	// 	{
	// 		if (node->obj_value - best_node->obj_value < std::numeric_limits<double>::epsilon())
	// 		{
	// 			best_node = &(*node);
	// 		}
	// 		continue;
	// 	}
	// 	for (int i = 0; i < current_subtour.size() - 1; i++)
	// 	{
	// 		Node n;
	// 		n.arcos_proibidos = node->arcos_proibidos;
	// 		std::pair<int, int> arco;
	// 		arco.first = current_subtour[i];
	// 		arco.second = current_subtour[i + 1];
	// 		n.arcos_proibidos.push_back(arco);
	// 		tree.insert(tree.end(), n);
	// 	}
	// }

	std::cout << best_node.obj_value << "\n";

	// Memory management

	hungarian_free(&p);
	for (int i = 0; i < data->getDimension(); i++)
		delete[] cost[i];
	delete[] cost;
	delete data;

	return 0;
}
