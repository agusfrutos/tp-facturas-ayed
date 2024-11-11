#include <iostream>
#include <fstream>
#include <iomanip>
#include "LeerArchivoMvtos/LeerArchivosMvtos.cpp"

using namespace std;

#define IVA 0.21
#define CAN_CLI 1000
#define CAN_ART 100
template <typename T>
struct sNodo
{
    T info;
    sNodo<T> *sgte;
};

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

struct sPed
{
    int idCli;
    short codArt;
    short cantPedida;
};

struct sInfo
{
    int idCli, posCli;
};

struct sMvtosPedidos {
    int idCli,numFactura,fecha;
    float totalNeto;
};

typedef sNodo<sInfo> *tListaCli;
typedef sArt *vrPtrArt[CAN_ART + 1];

bool fIdCliMayor(sInfo &elem1, sInfo &elem2)
{
    return elem1.idCli > elem2.idCli;
};

bool fCodArtMayor(sArt &elem1, sArt &elem2)
{
    return elem1.codArt > elem2.codArt;
};

string replicate(char car, unsigned n)
{
    string cad = "";
    for (unsigned i = 1; i <= n; i++)
        cad += car;
    return cad;
} // replicate

int GetDate(short &year, short &mes, short &dia)
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    year = 1900 + timeinfo->tm_year;
    mes = 1 + timeinfo->tm_mon;
    dia = timeinfo->tm_mday;
    return (1900 + timeinfo->tm_year) * 10000 + (1 + timeinfo->tm_mon) * 100 +
           timeinfo->tm_mday;
} // GetDate

void Abrir(ifstream &sarasa, ifstream &Articulos, ifstream &Pedidos, ofstream &Facturas, ofstream &MovPedidos)
{
    sarasa.open("Clientes.Dat", ios::binary);
    Articulos.open("Articulos.Dat", ios::binary);
    Pedidos.open("Pedidos.Dat", ios::binary);
    Facturas.open("Facturas.txt");
    MovPedidos.open("MovPedidos.Dat", ios::binary);
};

void Cerrar(ifstream &Clientes, ifstream &Articulos, ifstream &Pedidos, ofstream &Facturas, ofstream &MovPedidos)
{
    Clientes.close();
    Articulos.close();
    Pedidos.close();
    Facturas.close();
    MovPedidos.close();
};

void EmiteCabFac(ofstream &Facturas, sCli rCli, sFec fecha, int numFactura)
{
    Facturas << fixed << setprecision(2) << '\n'
             << replicate('-', 92) << '\n'
             << setw(10) << "Id.Cliente" << left << "  " << setw(24) << "Razon Social" << setw(24) << "Domicilio" << setw(24) << "Localidad" << setw(12) << "Cod.Pos." << '\n'
             << right << setw(10) << rCli.idCli << left << "  " << setw(24) << rCli.razSoc << setw(24) << rCli.domic << setw(24) << rCli.localidad << setw(12) << rCli.codPos << '\n'
             << '\n'
             << "Nro.Factura: " << setw(9) << numFactura << setw(4) << " "
             << "Fecha Emision: " << setw(2) << right << setfill('0') << fecha.dia << "-" << setw(2) << right << setfill('0') << fecha.mes << setfill(' ') << "-" << fecha.agno
             << setw(4) << " " << "Tipo Factura: " << "(A)" << '\n'
             << '\n'
             << "        Nro.Ord  Id.Articulo  Descripcion          Cantidad     Pre.Uni    Tot.Item" << '\n';
} // EmiteCabClis

void EmiteDetFac(ofstream &Facturas, sArt* art, short numOrden, short cantPedida, double totalImporte)
{
    Facturas << setw(15) << numOrden << setw(13) << art->codArt << setw(22) << art->descripcion << setw(9) << cantPedida << setw(12) << art->preUni << setw(12) << totalImporte << '\n';
} // EmiteDetFac
void EmitPieFac(ofstream &Facturas, double totalFactura)
{
    double ivaFactura = totalFactura * IVA;
    Facturas << '\n'
             << setw(80) << "Tot.Bruto : $ " << totalFactura << '\n'
             << setw(80) << "IVA 21%   : $ " << ivaFactura << '\n'
             << setw(80) << "Total Neto: $ " << (totalFactura + ivaFactura) << '\n'
             << replicate('-', 92) << endl;
} // EmitPieFac

void CalcDetFac(int cantidad, sArt* &art, short &numOrden, double &totalItem, double &totalFactura)
{
    numOrden++;
    totalItem = cantidad * art->preUni;
    art->stock -= cantidad;
    totalFactura += totalItem;
} // CalcDetFac

void InicializarArt(vrPtrArt &lista)
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
    while (pActual->sgte && criterio(&info, &pActual->sgte->info))
    {
        pActual = pActual->sgte;
    }
    InsertarEnLugar(pActual, info);
}

template <typename T>
void InsertarNodo(sNodo<T> *&lista, T info, bool (*criterio)(void *, void *))
{

    if (!lista || criterio(&lista->info, &info))
        InsertarInicio<T>(lista, info);
    else
        InsertarEnMedio<T>(lista, info, (bool (*)(void *, void *))(criterio));
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
        if (lista[i] != NULL)
        {
            cout << "Articulo: " << lista[i]->codArt << " " << lista[i]->descripcion << " " << lista[i]->preUni << " " << lista[i]->stock << endl;
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
        sArt *temp = lista[i];
        delete temp;
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

void InsertarArticulo(sArt *&pActual, sArt nuevoArt)
{
    sArt *newNodo = new sArt;
    *newNodo = nuevoArt;
    pActual = newNodo;
}

void ProcArticulos(ifstream &Articulos, vrPtrArt &vPtrArticulos)
{
    sArt rArt;
    while (Articulos.read((char *)&rArt, sizeof(rArt)))
    {
        InsertarArticulo(vPtrArticulos[rArt.codArt], rArt);
    }
    MostrarLista(vPtrArticulos);
}

int BuscarPosCli(tListaCli lstClientes, int idCli)
{
    while (lstClientes != NULL)
    {
        if (lstClientes->info.idCli == idCli)
        {
            return lstClientes->info.posCli;
        }

        lstClientes = lstClientes->sgte;
    }
    return -1;
}

void ProcPedidos(ifstream &Pedidos, ifstream &Clientes, tListaCli &lstClientes, vrPtrArt &vPtrArticulos, ofstream &Facturas, ofstream &MovPedidos)
{
    int posCli;
    sCli rCli;
    sPed rPed;
    sArt rArt;
    sFec fecha;
    sMvtosPedidos mvtosPedidos;
    int numFactura = 222320;
    int aaaaMMdd = GetDate(fecha.agno, fecha.mes, fecha.dia);
    Pedidos.read((char *)&rPed, sizeof rPed);
    while (!Pedidos.eof())
    {
        double totalFactura = 0;
        posCli = BuscarPosCli(lstClientes, rPed.idCli);
        Clientes.clear();
        Clientes.seekg(posCli * sizeof(sCli), ios::beg);
        Clientes.read((char*)&rCli, sizeof(sCli));
        EmiteCabFac(Facturas, rCli,fecha,numFactura);
        short numOrden = 0;
        while (!Pedidos.eof() && rCli.idCli == rPed.idCli)
        {
            double totalItem = 0;
            
            CalcDetFac(rPed.cantPedida,vPtrArticulos[rPed.codArt],numOrden,totalItem, totalFactura);
            EmiteDetFac(Facturas,vPtrArticulos[rPed.codArt],numOrden,rPed.cantPedida,totalItem);
            Pedidos.read((char *)&rPed, sizeof rPed);
        }
            EmitPieFac(Facturas,totalFactura);
            mvtosPedidos.fecha = aaaaMMdd;
            mvtosPedidos.idCli = rCli.idCli;
            mvtosPedidos.numFactura = numFactura;
            mvtosPedidos.totalNeto = totalFactura;

            numOrden = 0;
            numFactura++;
            MovPedidos.write((char*)&mvtosPedidos, sizeof(mvtosPedidos));
    }
}

int main()
{
    ifstream Clientes, Articulos, Pedidos;
    ofstream Facturas, MovPedidos;
    tListaCli lstClientes = NULL;
    vrPtrArt vPtrArticulos;

    InicializarArt(vPtrArticulos);

    Abrir(Clientes, Articulos, Pedidos, Facturas, MovPedidos);
    ProcClientes(Clientes, lstClientes);
    ProcArticulos(Articulos, vPtrArticulos);
    ProcPedidos(Pedidos, Clientes, lstClientes, vPtrArticulos, Facturas, MovPedidos);
    Cerrar(Clientes, Articulos, Pedidos, Facturas, MovPedidos);

   // LiberarLista(lstClientes);
   // LiberarLista(vPtrArticulos);
    LeerArchivoMvtos();
    return 0;
}