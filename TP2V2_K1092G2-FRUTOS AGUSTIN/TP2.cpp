#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

template <typename T>
struct sNodo
{
    T info;
    sNodo<T> *sgte;
};

const short CAN_CLI = 1000;
const short CAN_ART = 100;
typedef char str20[21];

struct sFec
{
    short agno, mes, dia;
};

struct sCli
{
    int idCli;
    str20 razSoc, domic, localidad;
    short codPos;
    sFec fechaAlta;
    float saldo;
};

struct sArt
{
    short codArt;
    str20 descripcion;
    short stock;
    float preUni;
};

struct sInfo
{
    int idCli, posCli;
};


typedef sNodo<sInfo> *tListaCli;
typedef sNodo<sArt> *vrPtrArt[CAN_ART+1];

bool fIdCliMayor(sInfo &elem1, sInfo &elem2)
{
    return elem1.idCli > elem2.idCli;
};

bool fCodArtMayor(sArt &elem1, sArt &elem2)
{
    return elem1.codArt > elem2.codArt;
};

void Abrir(ifstream &Clientes, ifstream &Articulos, ifstream &Pedidos, ofstream &Facturas, ofstream &MovPedidos)
{
    Clientes.open("Clientes.Dat", ios::binary);
    Articulos.open("Articulos.Dat", ios::binary);
    Pedidos.open("Pedidos.Dat", ios::binary);
    Facturas.open("Facturas.Dat", ios::binary || ios::app);
    MovPedidos.open("MovPedidos.Dat", ios::binary || ios::app);
};

void Cerrar(ifstream &Clientes, ifstream &Articulos, ifstream &Pedidos, ofstream &Facturas, ofstream &MovPedidos)
{
    Clientes.close();
    Articulos.close();
    Pedidos.close();
    Facturas.close();
    MovPedidos.close();
};

void InicializarListaArt(vrPtrArt &lista)
{
    for (int i = 1; i <= CAN_ART; i++)
    {
        lista[i] = NULL;
    }
}

template <typename T>
void InsertarInicio(sNodo<T> *&lista, T info)
{
    sNodo<T> *newNodo = new sNodo<T>;
    newNodo->info = info;
    newNodo->sgte = lista;
    lista = newNodo;
}

template <typename T>
void InsertarEnLugar(sNodo<T> *&pActual, T info)
{
    sNodo<T> *pNew = new sNodo<T>;
    pNew->info = info;

    pNew->sgte = pActual->sgte;
    pActual->sgte = pNew;
}

template <typename T>
void InsertarEnMedio(sNodo<T> *&lista, T info, bool (*criterio)(void *, void *))
{
    sNodo<T> *pActual;
    pActual = lista;
    while (pActual->sgte && criterio(&info, &pActual->sgte->info)) {
        pActual = pActual->sgte;
    cout << pActual->sgte << endl;

    }

    InsertarEnLugar(pActual, info);
}

template <typename T>
void InsertarNodo(sNodo<T> *&lista, T info, bool (*criterio)(void *, void *))
{

    if (!lista || criterio(&lista->info, &info)) {
        cout << "entre " << endl;
        InsertarInicio<T>(lista, info);
    }
    else {
        cout << "entre aca " << endl;
        InsertarEnMedio<T>(lista, info, (bool (*)(void *, void *))(criterio));

    }
}

void MostrarLista(tListaCli lista)
{
    cout << "Lista: ";
    while (lista != NULL)
    {
        cout << "idCli: " << lista->info.idCli << " pos: " << lista->info.posCli << endl;
        lista = lista->sgte;
    }
}
void MostrarLista(vrPtrArt lista)
{
    cout << fixed << setprecision(2);
      for (int i = 1; i <= CAN_ART; i++)
    {

    while (lista[i] != NULL)
    {
        cout << "Articulo: " << lista[i]->info.codArt << " " << lista[i]->info.descripcion<< " " << lista[i]->info.preUni<< " " << lista[i]->info.stock << endl;
        lista[i] = lista[i]->sgte;
    }
    }
}
void LiberarLista(tListaCli &lista)
{
    while (lista != NULL)
    {
        tListaCli temp = lista;
        lista = lista->sgte;
        delete temp;
    }
}
void LiberarLista(vrPtrArt &lista)
{
    for (int i = 1; i <= CAN_ART; i++)
    {
        while (lista[i] != NULL) {
        sNodo<sArt>* temp = lista[i];
        lista[i] = lista[i]->sgte;
        delete temp;
        }
    }
}
void ProcClientes(ifstream &Clientes, tListaCli &lstClientes)
{
    sCli rCli;
    sInfo rInfo;
    int pos = 0;
    while (Clientes.read((char *)&rCli, sizeof(rCli)))
    {
        rInfo.idCli = rCli.idCli;
        rInfo.posCli = pos;
        InsertarNodo<sInfo>(lstClientes, rInfo, (bool (*)(void *, void *))(fIdCliMayor));
        pos++;
    }
    MostrarLista(lstClientes);
}

void ProcArticulos(ifstream &Articulos , vrPtrArt &vPtrArticulos) {

    sArt rArt;

    while (Articulos.read((char*)&rArt, sizeof(rArt))) {
      InsertarNodo<sArt>(vPtrArticulos[rArt.codArt], rArt, (bool (*) (void*,void*))(fCodArtMayor));
    }

    MostrarLista(vPtrArticulos);
}

int main()
{

    ifstream Clientes, Articulos, Pedidos;
    ofstream Facturas, MovPedidos;
    tListaCli lstClientes = NULL;
    vrPtrArt vPtrArticulos;

    InicializarListaArt(vPtrArticulos);

    Abrir(Clientes, Articulos, Pedidos, Facturas, MovPedidos);
    ProcClientes(Clientes, lstClientes);
    ProcArticulos(Articulos,vPtrArticulos);
    //   ProcPedidos(Pedidos,Clientes,ListaClientes,vPtrArticulos,Facturas,MovPedidos);
    Cerrar(Clientes, Articulos, Pedidos, Facturas, MovPedidos);

    LiberarLista(lstClientes);
    LiberarLista(vPtrArticulos);
    return 0;
}