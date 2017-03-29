#include <iostream>
#include <stdio.h>
#include <vector>
#include <bits/stdc++.h>
#include "includes.cpp"
#include <time.h>

using namespace std;


solucion * sol_parcial;
solucion mejor_solucion;
int beneficio_disp = 0; 
grafo g;
int i = 0;

int obtener_numero(){
	int numero;
    while(1){
    	cin >> numero;
    	if (cin.fail()){
    		cin.clear();
        	cin.ignore(256,' ');
    	} else {
    		return numero;
    	}
    }
	return numero;
}

grafo leer_grafo(){
	
	int nodos, ladosR, ladosE;
	int n1,n2,n3,n4;
	grafo g;
	lado * temp1, * temp2;
	int * rtmp;
	
	nodos = obtener_numero();
	//cout << "Numero de nodos: " << nodos << endl;
	ladosR = obtener_numero();
	//cout << "Lados R: " << ladosR << endl;

	g.resize(nodos+1);
	g.enumerar();
	for(int i = 0; i < ladosR; i++){
		cin >> n1 >> n2 >> n3 >> n4;
		temp1 = new lado(n1,n2,n3,n4);
		temp2 = new lado(n2,n1,n3,n4);
		g.agregar_lado(n1,*temp1);
		g.agregar_lado(n2,*temp2);
		beneficio_disp += n4-n3;
	}

	ladosE = obtener_numero();
	for(int i = 0; i < ladosE; i++){
		cin >> n1 >> n2 >> n3 >> n4;
		temp1 = new lado(n1,n2,n3,n4);
		temp2 = new lado(n2,n1,n3,n4);
		g.agregar_lado(n1,*temp1);
		g.agregar_lado(n2,*temp2);
	}

	return g;
}


void busqueda_en_profundidad(){
	i++;
	if (i % 500000 == 0){
		sol_parcial->print_recorrido();
	}
	vector<lado> v_adj;
	int v = sol_parcial->verticeExterno();
	if (v == 1) {
		//cout << "Comparando " << sol_parcial->beneficio << " y " << mejor_solucion->beneficio <<endl;
		if (sol_parcial->beneficio > mejor_solucion.beneficio){
			cout << "Nueva solucion parcial!" << endl;
			sol_parcial->print();
			mejor_solucion = (*sol_parcial);
		}
	}

	v_adj = g.v[v].obtener_adj();
	for (vector<lado>::iterator i = v_adj.begin(); i != v_adj.end(); ++i){
		if (!sol_parcial->ciclo_negativo(*i) && 
			!sol_parcial->repite_ciclo(*i) &&
			!sol_parcial->esta_sol_parcial(*i) &&
			sol_parcial->cumple_acotamiento(*i, beneficio_disp, mejor_solucion.beneficio) ){
			
			sol_parcial->agregar(*i);
			beneficio_disp -= max(0, i->win - i->costo);
			busqueda_en_profundidad();
		}
	}
	while(!v_adj.empty()){
		v_adj.erase(v_adj.begin());
	}
	lado e = sol_parcial->eliminarUL();
	beneficio_disp += max(0, e.win - e.costo);
	
}


int main(int argc, char const *argv[])
{

	clock_t tStart = clock();
    if (! (argc > 1 )) return 1;
    string instance(argv[1]);
    string initial(instance + string("-salida.txt"));
    cout << "Evaluando " << argv[1] << endl;
    cout << "Solucion inicial: " << initial << endl; 
	ifstream in(argv[1]);

	cin.rdbuf(in.rdbuf());
	g = leer_grafo();
	//beneficio_disp = g.maxbeneficio;
	//cout << "Beneficio " << beneficio_disp << endl; 
	sol_parcial = new solucion();
	//mejor_solucion = new solucion();
	mejor_solucion.cargar_solucion(initial.c_str());

	//sol_parcial->agregar(lado(1,3,1,10));
	//sol_parcial->agregar(lado(3,5,1,10));
	//sol_parcial->agregar(lado(5,10,1,10));
	//cout << "Ciclo neg " << sol_parcial->repite_ciclo(lado(10,3,50,1)) << endl;

	busqueda_en_profundidad();
	cout << "Mejor solucion encontrada!" << endl;
	mejor_solucion.print_recorrido();
	/* Do your stuff here */
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	return 0;
}