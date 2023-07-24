#ifndef LABERINTO_H
#define LABERINTO_H
#include <vector>
#include <queue>
#include <iostream>
#include <ctime>
#include <cmath>
#include <tuple>
#define loop(a,b,c) for(int a=b;a<c;++a)
#define ui unsigned int
#define vi std::vector<int>
#define vvi std::vector<vi>
#define pdi std::pair<double, int>
#define vpdi std::vector<pdi>
#define vvpdi std::vector<vpdi>
#define tdii std::tuple<double,int,int>
#define vtdii std::vector<tdii>
#define pqtdii std::priority_queue<tdii, vtdii, std::greater<tdii>>

// B = bloque o muro
// WB = bloque blanco
enum { WB = 0, B = 1, taken = 2, no_taken = 3 };

class Laberinto
{
public:
	// datos matriz
	ui n, m;
	int start_x;
	int end_x;
	vvi matrix;
	// datos grafo
	vvi vertex;		// todos los vertices forma x, y
	vvpdi adjacent;	// lista adyacencia -> peso, vertice llegada
	pqtdii PQ;		// cola de prioridad para Prim
	ui n_vertex;
	vi vertexTaken;	// vertices tomados para Prim
	ui edges_taken;	// limite de aristas n-1

	Laberinto(ui _n, ui _m);
	void showMatrix();
	void showVertex();
	void showDistances();
	vvi getLaberinto() { return matrix; }
	void transp();				// transpuesta de la matriz
	void fixed();

private:
	void setBC();				// asigna Bloques candidatos
	void setDistances();		// distancias entre todos los vertices
	void process(int u);		// proceso para Prim
	void Prim();				// algoritmo Prim
	void setPath(int u, int v);	// genera camino aleatorio de u a v
};

Laberinto::Laberinto(ui _n, ui _m)
{
	n = _n;
	m = _m;
	matrix.assign(n, vi(m, B)); // B todos muros
	// entrada y salida aleatoria a la derecha e izquierda
	srand(time(nullptr));
	start_x = rand() % (n - 2) + 1;
	end_x = rand() % (n - 2) + 1;
	matrix[start_x][0] = matrix[end_x][m-1] = WB;
	// muestra matriz con entrada y salida
	showMatrix();
	// asigna zonas descubiertas aleatorias
	setBC();
	// muestra matriz con zonas descubiertas para conectar
	showMatrix();
	// generar vertices y sus indices
	loop(i, 0, n)
		loop(j, 0, m)
		{
			if (matrix[i][j] == WB)	// zonas blancas son vertices
			{
				vi vert = { i, j };
				vertex.push_back(vert);
			}
		}

	edges_taken = 0;
	n_vertex = vertex.size();
	vertexTaken.assign(n_vertex, no_taken);
	adjacent.assign(n_vertex,vpdi(0));
	setDistances();
	Prim();
}

// asigna Bloques candidatos
void Laberinto::setBC()
{
	int min = 0;
	int max = n*m;
	double prob = 0.3;
	srand(time(nullptr));
	loop(i, 1, n-1)
	{
		loop(j, 1, m-1)
		{
			// num = rand() % (max - min) + min;
			double num = (rand() % (max - min) + min) / double(max);
			// std::cout << num << '\n';
			if (num <= prob)
				matrix[i][j] = WB;	// añade una zona descubierta
		}
	}
}

// distancias entre todos los vertices
void Laberinto::setDistances()
{
	loop(i, 0, n_vertex)
	{
		loop(j, i+1, n_vertex)
		{
			double dist = std::sqrt( std::pow(vertex[i][0] - vertex[j][0], 2) + std::pow(vertex[i][1] - vertex[j][1], 2));
			adjacent[i].push_back({ dist, j });
			adjacent[j].push_back({ dist, i });
		}
	}
}

// proceso para Prim
void Laberinto::process(int u)
{
	vertexTaken[u] = taken;
	for (auto v : adjacent[u])
		if (vertexTaken[v.second] == no_taken)
			PQ.push(std::make_tuple( v.first, u, v.second ));	// peso, u, v
}

// algoritmo Prim
void Laberinto::Prim()
{
	process(0);
	while (!PQ.empty() && edges_taken != n_vertex - 1)
	{
		tdii a = PQ.top(); PQ.pop();
		double w = std::get<0>(a);
		int u = std::get<1>(a);
		int v = std::get<2>(a);
		if (vertexTaken[v] == taken) continue;
		setPath(u, v);
		process(v);
		edges_taken++;
	}
}

// genera camino aleatorio de u a v
void Laberinto::setPath(int u, int v)
{
	int u_f = vertex[u][0], u_c = vertex[u][1];
	int v_f = vertex[v][0], v_c = vertex[v][1];
	srand(time(nullptr));
	while (u_f != v_f || u_c != v_c)
	{
		int dist_f = u_f - v_f;
		int dist_c = u_c - v_c;
		// num = rand() % (max - min) + min;
		double op = (rand() % (100)) / double(100);
		//std::cout << "op " << op << "\n";
		if (op < 0.5)	// 1 izq o der
		{
			if (dist_c == 0) continue;
			else if (dist_c < 0)
				u_c++;
			else if (dist_c > 0)
				u_c--;
			matrix[u_f][u_c] = WB;	// añade una zona descubierta
		}
		else	// 0 arriba o abajo
		{
			if (dist_f == 0) continue;
			else if (dist_f < 0)
				u_f++;
			else if (dist_f > 0)
				u_f--;
			matrix[u_f][u_c] = WB;	// añade una zona descubierta
		}
	}
}

// transpuesta de la matriz
void Laberinto::transp()
{
	vvi aux(m,vi(n,0));
	loop(f, 0, n)
	{
		loop(c, 0, m)
		{
			aux[c][f] = matrix[f][c];
		}
	}
	matrix.clear();
	matrix = aux;
	std::swap(n, m);
}

void Laberinto::fixed()
{
	vvi aux = matrix;
	loop(f, 0, n)
	{
		loop(c, 0, m)
		{
			matrix[f][c] = aux[n-f-1][c];
		}
	}
}

void Laberinto::showMatrix()
{
	loop(i, 0, n)
	{
		loop(j, 0, m)
			std::cout << matrix[i][j] << " ";
		std::cout << '\n';
	}
	std::cout << '\n';
}

void Laberinto::showVertex()
{
	loop(i, 0, vertex.size())
	{
		std::cout << i << ": ";
		loop(j, 0, 2)
			std::cout << vertex[i][j] << " ";
		std::cout << '\n';
	}
	std::cout << '\n';
}

void Laberinto::showDistances()
{
	loop(i, 0, adjacent.size())
	{
		std::cout << i << ": ";
		loop(j, 0, adjacent[i].size())
			std::cout << "{" << adjacent[i][j].first << " " << adjacent[i][j].second << "} ";
		std::cout << '\n';
	}
}

#endif