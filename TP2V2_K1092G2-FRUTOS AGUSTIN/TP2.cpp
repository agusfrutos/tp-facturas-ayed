#include <iostream>
#include <fstream>

using namespace std;

typedef int tInfo;


typedef struct sNodo* tListaCli;
struct sNodo {
tInfo valor;
tListaCli siguiente;
};

void Abrir(ifstream &Clientes,ifstream &Articulos,ifstream &Pedidos,ofstream &Facturas,ofstream &MovPedidos) {
    Clientes.open("Clientes.Dat", ios::binary);
    Articulos.open("Articulos.Dat", ios::binary);
    Pedidos.open("Pedidos.Dat", ios::binary);
    Facturas.open("Facturas.Dat", ios::binary || ios::app);
    MovPedidos.open("MovPedidos.Dat", ios::binary || ios::app);
};

void Cerrar(ifstream &Clientes,ifstream &Articulos,ifstream &Pedidos,ofstream &Facturas,ofstream &MovPedidos) {
    Clientes.close();
    Articulos.close();
    Pedidos.close();
    Facturas.close();
    MovPedidos.close();
};

void ProcClientes(ifstream &Clientes, tListaCli ListaClientes) {
   cout << "holi"; 
}

int main() {

    ifstream Clientes, Articulos, Pedidos;
    ofstream Facturas, MovPedidos;
    tListaCli ListaClientes;


    Abrir(Clientes,Articulos,Pedidos,Facturas,MovPedidos);
      ProcClientes(Clientes,ListaClientes);
  //  ProcArticulos(Articulos,vPtrArticulos);
  //  ProcPedidos(Pedidos,Clientes,ListaClientes,vPtrArticulos,Facturas,MovPedidos);
    Cerrar(Clientes,Articulos,Pedidos,Facturas,MovPedidos);
    return 0;
}