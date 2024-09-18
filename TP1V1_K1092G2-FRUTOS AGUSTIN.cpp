/*
    # Trabajo Práctico #1:
    Metodología-DP-ECP-Módulos-ED: Registro, Arreglo y Archivo texto.
    Proceso de facturación y armado de pedidos de clientes.

    # Fecha entrega: 25/09/2024

    # Versión: 1.0.0

    # Comisión: K1092 Miercoles Turno noche,

    # Integrantes de Grupo 2:
     Agustin Frutos  222.320-0
     Alexis Pieronee 222.600-5
     Nicolas Mancuso 222.481-1
     Lautaro Velozo  221.258-4

    # Nombre del compilador: Borland C++ V.5.5
*/
#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <conio.h>
#include <iomanip>

using namespace std;

#define IVA 0.21
#define MAX_CLI 1000
#define MAX_ART 100
typedef unsigned short ushort;
typedef char str3[4];
typedef char str8[9];
typedef char str20[21];

struct sFec
{
  int year,
      mes,
      dia;
};

struct sCliIndice
{
  str8 idCliente;
  short posicion;
};

struct sCli
{
  str8 idCliente;
  str20 razSoc,
      domic, localidad;
  short codPos;
  sFec fechaAlta;
  float saldo;
};

struct sArt
{
  str3 codArticulo;
  str20 descripcion;
  ushort stock;
  float precioUnitario;
};

struct sPed
{
  str8 idCliente;
  str3 codArticulo;
  ushort cantPedida;
};
typedef sCliIndice tvrClientes[MAX_CLI-1];
typedef sArt tvrArticulos[MAX_ART-1];

string replicate(char car, unsigned n)
{
  string cad = "";
  for (unsigned i = 1; i <= n; i++)
    cad += car;
  return cad;
} // replicate

long GetDate(int &year, int &mes, int &dia)
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

void InsertarEnOrden(tvrClientes vrClientes, str8 idCliente, short &card)
{
  int i = card - 1;
  while (i >= 0 && strcmp(idCliente, vrClientes[i].idCliente) < 0)
  {
    vrClientes[i + 1] = vrClientes[i];
    i--;
  }
  strcpy(vrClientes[i + 1].idCliente, idCliente);
  vrClientes[i + 1].posicion = card;
  card++;
} // InsertarEnOrden

short BusLinVec(tvrArticulos vrArticulos, str3 codArticulo, short card)
{
  short i = 1;
  while (i <= card && strcmp(codArticulo, vrArticulos[i].codArticulo) != 0)
    i++;
  return i;
} // BusLinVec

short BusBinVec(tvrClientes vrClientes, str8 idCliente, short card)
{
  int ult = card - 1;
  int prim = 0;
  int med;
  while (prim <= ult)
  {
    med = (prim + ult) / 2;
    int comparacion = strcmp(vrClientes[med].idCliente, idCliente);
    if (comparacion == 0)
      return vrClientes[med].posicion;
    else if (comparacion < 0)
      prim = med + 1;
    else
      ult = med - 1;
  }
  return -1;
} // BusBinVec

void Abrir(ifstream &Clientes, ifstream &Articulos, ifstream &Pedidos, ofstream &Facturas, ofstream &MvtosPedidos)
{
  Clientes.open("Clientes.txt");
  Articulos.open("Articulos.txt");
  Pedidos.open("Pedidos.txt");
  MvtosPedidos.open("MvtosPedidos.txt");
  Facturas.open("Facturas.txt");
} // Abrir

void Cerrar(ifstream &Clientes, ifstream &Articulos, ifstream &Pedidos, ofstream &Facturas, ofstream &MvtosPedidos)
{
  Clientes.close();
  Articulos.close();
  Pedidos.close();
  MvtosPedidos.close();
  Facturas.close();
} // Cerrar

bool LeerCli(ifstream &Clientes, sCliIndice &cliente)
{
  if (Clientes.get(cliente.idCliente, 9))
  {
    Clientes.ignore(91, '\n');
    return true;
  };
  return false;
}

void ProcClientes(ifstream &Clientes, tvrClientes vrClientes, short &canCli)
{
  sCliIndice cliente;
  while (LeerCli(Clientes, cliente)) InsertarEnOrden(vrClientes, cliente.idCliente, canCli);
}

bool LeerArt(ifstream &Articulos, sArt &articulo)
{
  if (Articulos.eof())
    return false;
  Articulos.get(articulo.codArticulo, 4);
  Articulos.ignore(1);
  Articulos.get(articulo.descripcion, 21);
  Articulos.ignore(1);
  Articulos >> articulo.stock;
  Articulos >> articulo.precioUnitario;
  Articulos.ignore();

  return true;
}; // LeerArt

void AcumProductos(tvrArticulos vrArticulos, sArt articulo, short &canArt)
{
  short posicion;
  posicion = BusLinVec(vrArticulos, articulo.codArticulo, canArt);
  if (posicion > canArt)
  {
    canArt++;
    strcpy(vrArticulos[posicion].codArticulo, articulo.codArticulo);
    strcpy(vrArticulos[posicion].descripcion, articulo.descripcion);
    vrArticulos[posicion].precioUnitario = articulo.precioUnitario;
    vrArticulos[posicion].stock = articulo.stock;
  }
}

void ProcArticulos(ifstream &Articulos, tvrArticulos vrArticulos, short &canArt)
{
  sArt articulo;
  while (LeerArt(Articulos, articulo))
    AcumProductos(vrArticulos, articulo, canArt);
}

bool LeerPed(ifstream &Pedidos, sPed &pedido)
{
  if (Pedidos.eof()) return false;
  Pedidos.get(pedido.idCliente, 9);
  Pedidos.ignore(1);
  Pedidos.get(pedido.codArticulo, 4);
  Pedidos.ignore(1);
  Pedidos >> pedido.cantPedida;
  Pedidos.ignore();
  return true;
}

void LeerCliByPosicion(ifstream &Clientes, sCli &cliente, short posicion)
{
  Clientes.clear();
  Clientes.seekg(posicion * 100, ios::beg);
  Clientes.get(cliente.idCliente, 9);
  Clientes.ignore(1);
  Clientes.get(cliente.razSoc, 21);
  Clientes.ignore(1);
  Clientes.get(cliente.domic, 21);
  Clientes.ignore(1);
  Clientes.get(cliente.localidad, 21);
  Clientes.ignore(1);
  Clientes >> cliente.codPos;
  Clientes.ignore(1);
  Clientes >> cliente.fechaAlta.dia;
  Clientes.ignore(1);
  Clientes >> cliente.fechaAlta.mes;
  Clientes.ignore(1);
  Clientes >> cliente.fechaAlta.year;
  Clientes.ignore(1);
  Clientes >> cliente.saldo;
}

void EmiteCabFac(ofstream &Facturas, sCli cliente, sFec fecha, int numFactura)
{
  Facturas << '\n' << replicate('-', 92) << '\n'
           << setw(10) << "Id.Cliente" << left << "  " << setw(24) << "Razon Social" << setw(24) << "Domicilio" << setw(24) << "Localidad" << setw(12) << "Cod.Pos." << '\n'
           << right << setw(10) << cliente.idCliente << left << "  " << setw(24) << cliente.razSoc << setw(24) << cliente.domic << setw(24) << cliente.localidad << setw(12) << cliente.codPos << '\n'
           << '\n'
           << "Nro.Factura: " << setw(9) << numFactura << setw(4) << " "
           << "Fecha Emision: " << setw(2) << fecha.dia << "-" << setw(2) << right << setfill('0') << fecha.mes << setfill(' ') << "-" << fecha.year
           << setw(4) << " " << "Tipo Factura: " << "(A)" << '\n'
           << '\n'
           << "        Nro.Ord  Id.Articulo  Descripcion          Cantidad     Pre.Uni    Tot.Item" << '\n';
} // EmiteCabClis

short BuscarArticuloPorCodigo(tvrArticulos vrArticulos, str3 codArticulo, sArt &articulo, short cantArt)
{
  for (int i = 1; i <= cantArt; i++)
  {
    if (strcmp(vrArticulos[i].codArticulo, codArticulo) == 0)
    {
      articulo = vrArticulos[i];
      return 1;
    }
  }
  return 0;
} // BuscarArticuloPorCodigo

void EmiteDetFac(ofstream &Facturas, sArt art, short numOrden, short cantPedida, double totalImporte)
{
  Facturas << setw(15) << numOrden << setw(13) << art.codArticulo << setw(22) << art.descripcion << setw(9) << cantPedida << setw(12) << art.precioUnitario << setw(12) << totalImporte << '\n';
} // EmiteDetFac

void CalcDetFac(int cantidad, sArt &art, short &numOrden, double &totalItem, double &totalFactura)
{
  numOrden++;
  totalItem = cantidad * art.precioUnitario;
  art.stock -= cantidad;
  totalFactura += totalItem;
} // CalcDetFac

void EmitPieFac(ofstream &Facturas, double totalFactura)
{
  double ivaFactura = totalFactura * IVA;
  Facturas << '\n'
           << setw(80) << "Tot.Bruto : $ " << totalFactura << '\n'
           << setw(80) << "IVA 21%   : $ " << ivaFactura << '\n'
           << setw(80) << "Total Neto: $ " << (totalFactura + ivaFactura);
} // EmitPieFac

void EmitMvtosPedidos(ofstream &MvtosPedidos, str8 idCliente, int numFactura, sFec fecha, double totalFactura)
{
  MvtosPedidos << setw(10) << idCliente << setw(8) << numFactura << setw(8) << fecha.year << setw(2) << setfill('0') << fecha.mes << setfill(' ') << fecha.dia << setw(12) << totalFactura  << "\n";
} // EmitMvtosPedidos

void ProcPedidos(ifstream &Pedidos, ifstream &Clientes, ofstream &Facturas, ofstream &MvtosPedidos, tvrClientes vrClientes, short canCli, tvrArticulos vrArticulos, short canArt)
{
  short posicion;

  sPed pedido;
  sCli cliente;
  sArt articulo;
  sFec fecha;

  short numOrden = 1;
  int numFactura = 222320;
  GetDate(fecha.year, fecha.mes, fecha.dia);
  bool pedidoLeido = LeerPed(Pedidos, pedido);
  while (pedidoLeido)
  {
    posicion = BusBinVec(vrClientes, pedido.idCliente, canCli);
    if (posicion >= 0)
    {
      double totalFactura = 0;
      LeerCliByPosicion(Clientes, cliente, posicion);
      EmiteCabFac(Facturas, cliente, fecha, numFactura);
      numFactura++;
      while (pedidoLeido && strcmp(pedido.idCliente, cliente.idCliente) == 0)
      {
        double totalItem = 0;
        BuscarArticuloPorCodigo(vrArticulos, pedido.codArticulo, articulo, canArt);
        CalcDetFac(pedido.cantPedida, articulo, numOrden, totalItem, totalFactura);
        EmiteDetFac(Facturas, articulo, numOrden, pedido.cantPedida, totalItem);
        pedidoLeido = LeerPed(Pedidos, pedido);
      }
      EmitPieFac(Facturas, totalFactura);
      EmitMvtosPedidos(MvtosPedidos, cliente.idCliente, numFactura, fecha, totalFactura);
    }
    else pedidoLeido = LeerPed(Pedidos, pedido);
  }
}; // ProcPedidos

int main()
{
  ifstream Clientes, Articulos, Pedidos;
  ofstream Facturas, MvtosPedidos;

  tvrClientes vrClientes;
  tvrArticulos vrArticulos;

  short canCli = 0;
  short canArt = 0;

  Abrir(Clientes, Articulos, Pedidos, Facturas, MvtosPedidos);
  ProcClientes(Clientes, vrClientes, canCli);
  ProcArticulos(Articulos, vrArticulos, canArt);
  ProcPedidos(Pedidos, Clientes, Facturas, MvtosPedidos, vrClientes, canCli, vrArticulos, canArt);
  Cerrar(Clientes, Articulos, Pedidos, Facturas, MvtosPedidos);
  return 0;
}
