#include <iostream>
#include <stdio.h>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

class lado {
	public:
		int desde;
		int hasta;
		int costo;
		int benef;
		lado(int g, int v, int c, int w) : desde(g), hasta(v), costo(c), benef(w){
		};

	bool operator<(lado g){

		return ( (benef - costo) > (g.benef-g.costo) ) ;
	}

};
