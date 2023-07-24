#include "laberinto.h"
#include <iostream>

int main()
{
	int n = 5, m = 10;
	Laberinto laberinto1(n, m);
	laberinto1.showMatrix();
	laberinto1.transp();
	laberinto1.showMatrix();
	//laberinto1.fixed();
	//laberinto1.showMatrix();
	// laberinto1.showVertex();
	// laberinto1.showDistances();
}
