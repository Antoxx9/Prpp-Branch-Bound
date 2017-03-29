#include <iostream>
#include <stdio.h>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

class lado {
	public:
		int index;
		int vertex;
		int costo;
		int win;
		int * veces;
		lado(int g, int v, int c, int w) : index(g), vertex(v), costo(c), win(w){
		};
	
	int get_costo(){
		if ( !(*veces) ){
			return -(win - costo);
		}	
		else
			return  costo;
	}

	bool operator<(lado g){

		return ( win - costo > (g.win-g.costo) ) ;
	}

};

class solucion
{
public:
	vector<int> recorrido;
	vector<lado> camino;
	int beneficio;
	solucion():beneficio(0){
		recorrido.push_back(1);
	};
	
	void agregar(lado e){
		recorrido.push_back(e.vertex);
		beneficio += e.win - e.costo;
		camino.push_back(e);
	}

	lado eliminarUL(){
		if(!camino.empty()){
			recorrido.pop_back();
			lado l = camino.back();
			beneficio -= l.win - l.costo;
    		camino.pop_back();
    		return l;
		} 
	}

	int verticeExterno(){
		if(!camino.empty()){
			return camino.back().vertex;
		} else {
			return 1;
		}
	}

	int esta_veces(lado e){
		int tmp = 0;
		for (std::vector<lado>::iterator i = camino.begin(); i != camino.end(); ++i) {
			if ( i->vertex == e.vertex && i->index == e.index ){
				tmp++;
			} else if (  i->vertex == e.index && i->index == e.vertex  ){
				tmp++;
			}
		}
		return tmp;
	}

	lado esta_lado(lado e){
		int tmp = 0;
		for (std::vector<lado>::iterator i = camino.begin(); i != camino.end(); ++i) {
			if ( i->vertex == e.vertex && i->index == e.index ){
				return (*i);
			}
		}
	}

	bool esta_sol_parcial(lado e){
		int vcs = esta_veces(e);
		if (vcs == 0){
			return false;
		} else if (vcs == 1) {
			//lado l = esta_lado(e);
			if(e.win == 0){
				return false;
			} else {
				return true;
			}
		} else {
			return true;
		}
	}

	bool cargar_solucion(const char * nombre){
		int temp;
		bool failed = false;
		string line;
		ifstream in(nombre);
		cin.rdbuf(in.rdbuf());
		cin >> beneficio;
		cin >> line;
		getline(cin, line);
		cout << endl;
		stringstream ss(line);
		while(ss>>temp && !ss.eof()){
			recorrido.push_back(temp);
		}
		recorrido.push_back(1);
	}

	bool print(){
		cout << "Imprimiendo solucion: " << endl;
		vector<lado>::iterator end = --camino.end();
		cout << "Beneficio: " << beneficio << endl;  
		for (std::vector<lado>::iterator i = camino.begin(); i != camino.end(); ++i){
			cout << (*i).index << " " << " * " << i->win - i->costo << endl;
			if (i == end){
				cout << (*i).vertex << endl;
			}
		}
	}
	
	void print_recorrido(){
		cout << "Beneficio: " << beneficio << endl;  
		for (std::vector<int>::iterator i = recorrido.begin(); i != recorrido.end(); ++i) {
			if ((*i) == 1){
				cout << "d ";
			} else {
				cout << *i << " ";
			}
		}
		cout << endl;
	}

	/*
		Chequeada.
	*/
	bool cumple_acotamiento(lado e, int b_disponible, int b_mejor_sol){
		int b = e.win - e.costo;
		int b_parcial = beneficio + b;
		int max_beneficio = b_disponible - max(0, b) + b_parcial;
		if (max_beneficio <= b_mejor_sol){
			return false;
		};
		return true;
	}

	lado * ciclo(lado e){
		int tmp = 0;
		for (std::vector<lado>::iterator i = camino.begin(); i != camino.end(); ++i){
			if( (*i).vertex == e.vertex ) {
				tmp++;
			} 
			if (tmp == 2){
				return &(*i);
			}
		}
		return NULL;
	}

	bool repite_ciclo(lado e){
		//return false;
		lado * r = ciclo(e);
		//cout <<"Se formac ciclo ccon" << r->index << " " << r->vertex << endl;
		if (r != NULL ){
			if ( (e.win - e.costo) < (r->win - r->costo) ){
				return true;
			} else { 
				return false; 
			}
		} 
		return false;
	}

	/*
				Chequeada.
	*/
	bool ciclo_negativo(lado e){
		// PUEDE HACERSE CON UN SOLO CICLO
		for (std::vector<lado>::iterator i = camino.begin(); i != camino.end(); ++i){
			if( (*i).index == e.vertex ) {
				vector<lado> ciclo(i,camino.end());
				ciclo.push_back(e);
				if (negativo(ciclo)){
						return true;
				}
			}
		}
		return false;
	}

	bool negativo(vector<lado> ls){
		int acum = 0;
		for (std::vector<lado>::iterator i = ls.begin(); i != ls.end(); ++i) {
			//cout << "i " << i->index <<  " " << i->vertex << endl;
			acum += i->win - i->costo;
			//cout << "acum " << acum << endl;
		}
		return acum < 0;
	}

};


class vertice{
	public:
		int index, distancia;
		bool conex;
		vertice * padre;
		vector<lado> adj;
		vertice() : conex(0){ padre = NULL;};
		vertice(int i) : conex(0), index(i){ padre = NULL;};

		void print_adj(){
			cout << index << " * " << endl;
			for (vector<lado>::iterator it = adj.begin(); it != adj.end(); ++it){
				cout << it->vertex << " " << it->costo << " * " ; 
			}
			cout << endl;
		}

		/*
				Chequeada.
		*/
		vector<lado> obtener_adj(){
			vector<lado> temp;
			for (std::vector<lado>::iterator i = adj.begin(); i != adj.end(); ++i) {
				temp.push_back(*i);
				lado g(i->index, i->vertex, i->costo, 0) ;
				temp.push_back(g);
			}

			sort(temp.begin(), temp.end());

			return temp;
		}

		int return_path(vector<int> * ac){
			if (padre == NULL){
				ac->push_back(index);
				//cout << index << " ";
				return index;
			} else {
				padre->return_path(ac);
				ac->push_back(index);
				//cout << index << " ";
				return index;
			}
		}

		void limpiar(){
			conex = 0;
			padre = NULL;
		}
};



class grafo {
	public:
		vector<vertice> v;
		vector<int> recorrido;
		int costo;
		int maxbeneficio;

		grafo() : costo(0), maxbeneficio(0){};

		void resize(int n){
			v.resize(n);
		}

		void agregar_recorrido(vector<int> * r){
			vector<int>::iterator g;
			g = r->begin();
			if(!recorrido.empty()){
				g++;
			}
			recorrido.insert( recorrido.end(), g, r->end() );
		}

		void print_recorrido(){
			cout << "Recorrido completo: " <<endl;
			for (vector<int>::iterator it = recorrido.begin(); it != recorrido.end(); ++it){
				cout << *it << " ";
			}
			cout << endl;
		}

		void limpiar(){
			for (vector<vertice>::iterator it = v.begin(); it != v.end(); ++it){
				it->limpiar();
			}
		}

		void enumerar(){
			int n = 0;
			for (vector<vertice>::iterator it = v.begin(); it != v.end(); ++it){
				it->index = n++;
			}
		}

		void agregar_lado(int nodo, lado r){
			v[nodo].adj.push_back(r);
		}
		
		lado * buscar_lado(int a, int b){
			vector<lado> * lista = &(v[a].adj);
			for (vector<lado>::iterator it = lista->begin(); it != lista->end(); ++it)
				{
					if(it->vertex == b) return &(*it);
				}
		}

		void recorrer(vector<int> * ac){
			int r = -1;
			for ( vector<int>::iterator it = ac->begin(); it != ac->end(); ++it ){
				cout << *it << " ";
				if (r != -1){
					(*buscar_lado(r,*it)->veces)++ ;
				}
				r = *it;
			}
			cout << endl;
			agregar_recorrido(ac);
		}
};
