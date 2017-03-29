#include <vector>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <utility>
#include <queue>
#include <map>
#include <cmath>
#include <iostream>
#include <time.h>
#include <signal.h>
#include "clases.cpp"

#define ss second
#define ff first
#define mp make_pair
#define Inf 1000000

using namespace std;

vector<int> sol_parcial;
vector<int> mejor_sol;
vector<lado> sol_parcial2;
vector< vector< pair<int,int> > > grafo;
int beneficio_disponible = 0;
double tiempo;
ofstream o_file;
ifstream file;
string auxiliar,nombre;
clock_t start, diff;

int calcular_beneficio(vector<int> ciclo,vector< vector< pair<int,int> > > grafo) {
	int beneficio = 0;
	for(int i = 0; i < ciclo.size()-1; i++) {
		beneficio += (grafo[ciclo[i]][ciclo[i+1]].ss - grafo[ciclo[i]][ciclo[i+1]].ff);
		grafo[ciclo[i]][ciclo[i+1]].ss = 0;
		grafo[ciclo[i+1]][ciclo[i]].ss = 0; 
	}
	return beneficio;
}

void sig_handler(int SIG){
    o_file.open(nombre.append("-salida.txt").c_str());
	o_file << calcular_beneficio(mejor_sol,grafo) << endl;
	for(int i = 0; i < mejor_sol.size(); i++){
		if(mejor_sol[i] == 1){
			o_file << "d" << " ";
		}
		else{
			o_file << mejor_sol[i] << " ";
		}
	}
	diff = clock() - start; 
    tiempo = (double)diff / CLOCKS_PER_SEC;
	o_file << endl;
	o_file << "Tiempo: " << tiempo << endl;
	o_file.close();
    exit(0);
}

vector<lado> obtener_lista_de_sucesores(int v){
	vector<lado> sucesores;
	for(int i = 0; i < grafo.size(); i++){
		if(grafo[v][i].ff != -1){
			lado aux = lado(v,i,grafo[v][i].ff,grafo[v][i].ss);
			sucesores.push_back(aux);
			lado aux2 = lado(v,i,grafo[v][i].ff,0);
			sucesores.push_back(aux2);
		}	
	}
	sort(sucesores.begin(),sucesores.end());
	return sucesores;	
}
	
int cantidad(lado e, vector<lado> sol_parcial){
	int contador = 0;
	for(int i = 0; i < sol_parcial.size(); i++){
		if(sol_parcial[i].desde == e.desde && sol_parcial[i].hasta == e.hasta){
			contador++;
		}
		else if(sol_parcial[i].desde == e.hasta && sol_parcial[i].hasta == e.desde){
			contador++;
		}
	}
	return contador;
}

bool esta_en_sol_parcial(lado e,vector<lado> sol_parcial){
	int contador = cantidad(e,sol_parcial);
	if (contador == 0){
		return false;
	} 
	else if (contador == 1) {
		if(e.benef == 0){
			return false;
		} 
		else {
			return true;
		}
	} 
	else {
		return true;
	}
}


bool cumple_acotamiento(lado e){
	int beneficio = e.benef - e.costo;
	int b_parcial = calcular_beneficio(sol_parcial,grafo) + beneficio;
	int max_beneficio = beneficio_disponible - max(0, beneficio) + b_parcial;
	if (max_beneficio <= calcular_beneficio(mejor_sol,grafo)){
		return false;
	};
	return true;
}

lado existe_ciclo(lado e,vector<lado> sol_parcial){
	int contador = 0;
	for(int i = 0; i < sol_parcial.size(); i++){
		if(e.hasta == sol_parcial[i].hasta){
			contador++;
		}
		if (contador == 2){
			return sol_parcial[i];
		}
	}
	return lado(-1,-1,-1,-1);
}

bool repite_ciclo(lado e,vector<lado> sol_parcial){
	lado prima = existe_ciclo(e,sol_parcial);
	if (prima.desde != -1 ){
		if ( (e.benef - e.costo) < (prima.benef - prima.costo) ){
			return true;
		} else { 
			return false; 
		}
	}
	return false;
}

int costo(vector<lado> sol_parcial){
	int total = 0;
	for(int i = 0; i < sol_parcial.size(); i++){
		total += sol_parcial[i].benef - sol_parcial[i].costo;
	}
	return total;
}

bool ciclo_negativo(lado e,vector<lado> sol_parcial){
	for(int i = 0; i < sol_parcial.size(); i++){
		if(sol_parcial[i].desde == e.hasta){
			vector<lado> ciclo;
			for(int j = i; j < sol_parcial.size(); j++){
				ciclo.push_back(sol_parcial[j]);
			}
			ciclo.push_back(e);
			if(costo(ciclo) < 0){
				return true;
			}
		}
	}
	return false;
}

void dfs(){
	int v;
	if(sol_parcial2.empty()){
		v = 1;
	}
	else{
		v = sol_parcial2.back().hasta;
	}

	if(v == 1){
		if(calcular_beneficio(sol_parcial,grafo) > calcular_beneficio(mejor_sol,grafo)){
			mejor_sol = sol_parcial;
			//printf("%d\n",calcular_beneficio(mejor_sol,grafo));
		}
	}
	vector<lado> sucesores = obtener_lista_de_sucesores(v);
	for(int i = 0; i < sucesores.size(); i++){
		if(!ciclo_negativo(sucesores[i],sol_parcial2) 
			&& !esta_en_sol_parcial(sucesores[i],sol_parcial2)
			&& !repite_ciclo(sucesores[i],sol_parcial2)
			&& cumple_acotamiento(sucesores[i])){
			sol_parcial.push_back(sucesores[i].hasta);
			sol_parcial2.push_back(sucesores[i]);
			beneficio_disponible -= max(0,sucesores[i].benef-sucesores[i].costo);
			dfs();
		}
	}
	sol_parcial.pop_back();
	lado e = sol_parcial2.back();
	beneficio_disponible += max(0,e.benef-e.costo);
	sol_parcial2.pop_back();
}

// Programa principal
int main(int argc, const char **argv){
	vector< pair< int,pair<int,int> > > lados; // Estructura para representar los lados de un grafo.
	vector<int> sol_inicial;
	int v1,v2,costo,beneficio,edges,edg1,edg2,ids,tam;
	pair<int,int> aux,aux1,aux2;
	signal(SIGTERM, sig_handler);
	file.open(argv[1]);
	nombre = argv[1];
	file >> auxiliar;
	file >> auxiliar;
	file >> auxiliar;
	file >> auxiliar;
	file >> edges;
	file >> auxiliar;
	file >> auxiliar;
	file >> auxiliar;
	file >> auxiliar;
	file >> edg1;
	// Inicializacion de las estructuras
	grafo.resize(edges+1);

	for(int i = 0; i <= edges; i++){
		grafo[i].resize(edges+1);
	}

	for(int i = 0; i <= edges; i++){
		for(int j = 0;j <= edges; j++){
			grafo[i][j] = mp(-1,-1);
		}
	}

	// Comienzo de la lectura
	while(edg1--){
		file >> v1 >> v2 >> costo >> beneficio;
		aux = mp(v1,v2);
		aux1 = mp(v2,v1);
		aux2 = mp(costo,beneficio);
		grafo[v1][v2] = aux2;
		grafo[v2][v1] = aux2;
		beneficio_disponible += beneficio-costo;
	}
	file >> auxiliar;
	file >> auxiliar;
	file >> auxiliar;
	file >> auxiliar;
	file >> auxiliar;
	file >> edg2;
	
	while(edg2--){
		file >> v1 >> v2 >> costo >> beneficio;
		aux = mp(v1,v2);
		aux1 = mp(v2,v1);
		aux2 = mp(costo,beneficio);
		grafo[v1][v2] = aux2;
		grafo[v2][v1] = aux2;
	}

	while(file >> tam){
		sol_inicial.push_back(tam);
	}
	file.close();
	sol_parcial.push_back(1);
	mejor_sol = sol_inicial;
	start = clock();
	dfs();
	diff = clock() - start; 
    tiempo = (double)diff / CLOCKS_PER_SEC;
	o_file.open(nombre.append("-salida.txt").c_str());
	o_file << calcular_beneficio(mejor_sol,grafo) << endl;
	for(int i = 0; i < mejor_sol.size(); i++){
		if(mejor_sol[i] == 1){
			o_file << "d" << " ";
		}
		else{
			o_file << mejor_sol[i] << " ";
		}
	}
	o_file << endl;
	o_file << "Tiempo: " << tiempo << endl;
	o_file.close();
}