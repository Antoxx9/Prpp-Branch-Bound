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

#define ss second
#define ff first
#define mp make_pair
#define Inf 1000000

using namespace std;

vector<int> sol_parcial;
vector<int> mejor_sol;
vector< pair< pair<int,int>, pair<int,int> > > sol2;
int beneficio_disponible;


bool ordenar(pair< pair<int,int>, pair<int,int> > a, pair< pair<int,int>, pair<int,int> > b){
	return (a.ss.ss > b.ss.ss);
}

int calcular_beneficio(vector<int> ciclo,vector< vector< pair<int,int> > > grafo) {
	int beneficio = 0;
	for(int i = 0; i < ciclo.size()-1; i++) {
		beneficio += (grafo[ciclo[i]][ciclo[i+1]].ss - grafo[ciclo[i]][ciclo[i+1]].ff);
		grafo[ciclo[i]][ciclo[i+1]].ss = 0;
		grafo[ciclo[i+1]][ciclo[i]].ss = 0; 
	}
	return beneficio;
}

int obtener_max_beneficio(vector< vector< pair<int,int> > > grafo){
	int beneficio = 0;
	for(int i = 0; i < grafo.size(); i++){
		for(int j = i+1; j < grafo.size(); j++){
			if(grafo[i][j].ff != -1){
				beneficio += grafo[i][j].ss;
			}
		}
	}
	return beneficio;
}

vector< pair< pair<int,int>, pair<int,int> > > obtener_lista_de_sucesores(int v, vector< vector< pair<int,int> > > &grafo){
	vector< pair< pair<int,int>, pair<int,int> > > sucesores;
	pair< pair<int,int>, pair<int,int> > aux;
	for(int i = 0; i < grafo.size(); i++){
		if(grafo[i][v].ff != -1){
			aux = mp(mp(i,v),mp(grafo[i][v].ff,grafo[i][v].ss));
			sucesores.push_back(aux);
			aux = mp(mp(i,v),mp(grafo[i][v].ff,0));
			sucesores.push_back(aux);
		}	
	}
	sort(sucesores.begin(),sucesores.end(),ordenar);
	return sucesores;	
}

bool existe_ciclo_negativo(vector<int> ciclo, vector< vector< pair<int,int> > > grafo){
	pair<int,int> lado,lado1,lado2;
	vector<int> ciclo_aux;
	int beneficio;
	for(int i = 1; i < ciclo.size()-1; i++){
		ciclo_aux.clear();
		lado = mp(ciclo[i],i);
		ciclo_aux.push_back(lado.ff);
		for(int j = i+1; j < ciclo.size(); j++){
			lado2 = mp(ciclo[j],j);
			ciclo_aux.push_back(lado2.ff);
			if(lado.ff == lado2.ff){
				beneficio = calcular_beneficio(ciclo_aux,grafo);
				if(beneficio < 0){
					return true;
				}
			}
		}
	}
	return false;
}

bool ciclo_negativo(pair< pair<int,int>, pair<int,int> > arista, vector<int> sol_parcial, 
					vector< vector< pair<int,int> > > grafo){
	sol_parcial.push_back(arista.ff.ff);
	//sol_parcial.push_back(arista.ff.ss);
	if(existe_ciclo_negativo(sol_parcial,grafo)){
		//printf("Forma ciclo negativo con la arista: %d %d\n",arista.ff.ff,arista.ff.ss);
		return true;
	}
	//printf("La porqueria no forma ciclo negativo\n");
	return false;
}

bool esta_en_lado_sol_parcial(pair< pair<int,int>, pair<int,int> > arista, vector<int> sol_parcial){
	int l1,l2;
	int contador = 0;
	l1 = arista.ff.ff;
	l2 = arista.ff.ss;
	for(int i = 0; i < sol_parcial.size()-1; i++){
		if(sol_parcial[i] == l1 && sol_parcial[i+1] == l2){
			contador++;
		}
		else if(sol_parcial[i] = l2 && sol_parcial[i+1] == l1){
			contador++;
		}
	}
	//printf("El contador en esta en lado es: %d\n",contador);
	if(contador == 0){
		return false;
	}
	else if(contador == 1){
		if(arista.ss.ss == 0){
			return false;
		}
		else{
			return true;
		}
	}
	else{
		return true;
	}
}

bool forma_ciclo(vector<int> sol_parcial){
	if(sol_parcial[0] == sol_parcial[sol_parcial.size()-1]){
		return true;
	}
	return false;
}

bool buscar_arista(vector< pair< pair<int,int>, pair<int,int> > > lista_suc, pair< pair<int,int>, pair<int,int> > &arista, 
				   pair<int,int> vertices, vector< vector< pair<int,int> > > grafo){

	int l1 = vertices.ff;
	int l2 = vertices.ss;
	/*
	for(int i = 0; i < sol_parcial.size()-1; i++){
		if(sol_parcial[i] == l1 && sol_parcial[i+1] == l2){
			arista = mp(vertices,mp(grafo[l1][l2].ff,grafo[l1][l2].ss));
			return true;
		}
		else if(sol_parcial[i] = l2 && sol_parcial[i+1] == l1){
			arista = mp(vertices,mp(grafo[l1][l2].ff,grafo[l1][l2].ss));	
			return true;
		}
	}
	*/
	for(int i = 0; i < lista_suc.size(); i++){
		if(lista_suc[i].ff == vertices){
			arista = lista_suc[i];
			return true;
		}
	}
	return false;
}


bool repite_ciclo(vector< pair< pair<int,int>, pair<int,int> > > lista_suc, pair< pair<int,int>, pair<int,int> > arista,
                  vector<int> sol_parcial, vector< vector< pair<int,int> > > grafo){
	pair< pair<int,int>, pair<int,int> > aux;
	pair<int,int> aux2;
	sol_parcial.push_back(arista.ff.ff);
	//sol_parcial.push_back(arista.ff.ss);
	if(forma_ciclo(sol_parcial)){
		aux2 = mp(sol_parcial[1],sol_parcial[0]);
		if( buscar_arista(lista_suc,aux,aux2,grafo) ){
			if((arista.ss.ss - arista.ss.ff) < (aux.ss.ss - aux.ss.ff)){
				return false;
			}
		}
		else{
			return true;
		}
	}
	return false;
}

bool cumple_acotamiento(pair< pair<int,int>, pair<int,int> > arista,
                  vector<int> sol_parcial,vector< vector< pair<int,int> > > grafo){
	int beneficio = arista.ss.ss - arista.ss.ff;
	int beneficio_parcial = calcular_beneficio(sol_parcial,grafo) + beneficio;
	int max_beneficio = beneficio_disponible - max(0, beneficio) + beneficio_parcial;
	if(max_beneficio <= calcular_beneficio(mejor_sol,grafo)){
		return false;
	}
	return true;

}

void dfs(vector< vector< pair<int,int> > > grafo){
	int v = sol_parcial[sol_parcial.size()-1];
	//printf("Estoy metido con v = %d\n", v);
	vector< pair< pair<int,int>, pair<int,int> > > sucesores;
	pair<int,int> last;
	if(v == 1){
		if(calcular_beneficio(sol_parcial,grafo) > calcular_beneficio(mejor_sol,grafo)){
			mejor_sol = sol_parcial;
		}
	}

	printf("La mejorn solucion hasta ahora es:  \n");
	for(int i = 0; i < mejor_sol.size(); i++){
		printf("%d ", mejor_sol[i]);
	}
	printf("\n");
	printf("Su costo es: %d\n",calcular_beneficio(mejor_sol,grafo));


	//printf("La solucion parcial hasta ahora es:  \n");
	//for(int i = 0; i < sol_parcial.size(); i++){	
	//	printf("%d ", sol_parcial[i]);
	//}
	//printf("\n");
	//printf("Su costo es: %d\n",calcular_beneficio(sol_parcial,grafo));	


	sucesores = obtener_lista_de_sucesores(v,grafo);
	//printf("Sus sucesores son: \n");
	//for(int i = 0; i < sucesores.size(); i++){
	//	printf("%d %d %d %d\n",sucesores[i].ff.ff,sucesores[i].ff.ss,sucesores[i].ss.ff,sucesores[i].ss.ss);
	//}

	for(int i = 0; i < sucesores.size(); i++){
		if(!ciclo_negativo(sucesores[i],sol_parcial,grafo) && !esta_en_lado_sol_parcial(sucesores[i],sol_parcial)
			&& cumple_acotamiento(sucesores[i],sol_parcial,grafo)){
			sol_parcial.push_back(sucesores[i].ff.ff);
			sol2.push_back(sucesores[i]);
			//sol_parcial.push_back(sucesores[i].ff.ss);
			beneficio_disponible = beneficio_disponible - max(0,(sucesores[i].ss.ss - sucesores[i].ss.ff));
			dfs(grafo);
		}
	}
	last = mp(sol_parcial[sol_parcial.size()-1],sol_parcial[sol_parcial.size()-2]);
	sol_parcial.pop_back();
	//sol_parcial.pop_back();
	beneficio_disponible = beneficio_disponible + max(0, sol2[sol2.size()-1].ss.ss - sol2[sol2.size()-1].ss.ff);
	sol2.pop_back();
	printf("El beneficio disponible es: %d\n",beneficio_disponible);
}


// Programa principal
int main(int argc, const char **argv){
	clock_t tStart = clock();
	vector< pair< int,pair<int,int> > > lados; // Estructura para representar los lados de un grafo.
	vector< vector< pair<int,int> > > grafo;   // Grafo original, construido durante la lectura.
	vector<int> sol_inicial;
	int v1,v2,costo,beneficio,edges,edg1,edg2,ids,tam;
	pair<int,int> aux,aux1,aux2;
	ifstream file;

	string auxiliar,nombre;
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
	beneficio_disponible = obtener_max_beneficio(grafo);
	dfs(grafo);
}
		
	

