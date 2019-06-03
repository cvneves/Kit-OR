#include "Kruskal.h"

Kruskal::Kruskal(vvi dist)
{
	for (int i = 1; i < dist.size(); ++i)
	{
		for (int j = 1; j < dist[i].size(); ++j)
		{
			graph.push(make_pair(-dist[i][j], make_pair(i, j)));
		}
	}
	this->dist = dist;
}

void Kruskal::initDisjoint(int n)
{
	pset.resize(n);
	for (int i = 0; i < n; ++i)
	{
		pset[i] = i;
	}
}

int Kruskal::findSet(int i)
{
	return (pset[i] == i) ? i : (pset[i] = findSet(pset[i]));
}

void Kruskal::unionSet(int i, int j)
{
	pset[findSet(i)] = findSet(j);
}

bool Kruskal::isSameSet(int i, int j)
{
	return (findSet(i) == findSet(j)) ? true : false;
}

vii Kruskal::getEdges()
{
	return edges;
}

double Kruskal::MST(int nodes)
{
	initDisjoint(nodes);

	double cost = 0;

	while (!graph.empty())
	{
		pair<double, ii> p = graph.top();
		graph.pop();

		if (!isSameSet(p.second.first, p.second.second))
		{
			edges.push_back(make_pair(p.second.first, p.second.second));
			cost += (-p.first);
			unionSet(p.second.first, p.second.second);
		}
	}

	double melhor1, melhor2;
	std::pair<int, int> e1, e2;

	if (dist[0][1] < dist[0][2])
	{
		melhor1 = dist[0][1];
		melhor2 = dist[0][2];
		e1.first = 0;
		e1.second = 1;
		e2.first = 0;
		e2.second = 2;
	}
	else
	{
		melhor2 = dist[0][1];
		melhor1 = dist[0][2];
		e2.first = 0;
		e2.second = 1;
		e1.first = 0;
		e1.second = 2;
	}

	for (int i = 3; i < nodes; i++)
	{
		if (dist[0][i] < melhor1)
		{
			melhor2 = melhor1;
			e2.second = e1.second;

			melhor1 = dist[0][i];
			e1.second = i;
		}

		else if ((dist[0][i] < melhor2))
		{
			melhor2 = dist[0][i];
			e2.second = i;
		}
	}

	//std::cout << melhor1 << melhor2;

	edges.push_back(e1);
	edges.push_back(e2);

	cost += melhor1 + melhor2;

	return cost;
}