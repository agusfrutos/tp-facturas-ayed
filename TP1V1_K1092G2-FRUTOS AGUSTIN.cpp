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

struct sCli
{
  int idCliente;
  str20 razSoc,
      domic, localidad;
  short codPos;
  sFec fechaAlta;
  float saldo;
  short posicion;
};

struct sArt
{
  short codArticulo;
  str20 descripcion;
  ushort stock;
  float precioUnitario;
};

struct sPed
{
  int idCliente;
  short codArticulo;
  ushort cantPedida;
};
typedef sCli tvrClientes[MAX_CLI-1];
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

void InsertarEnOrden(tvrClientes vrClientes, int idCliente, short &card)
{
  int i = card - 1;
  while (i >= 0 && idCliente < vrClientes[i].idCliente)
  {
    vrClientes[i + 1] = vrClientes[i];
    i--;
  }
  vrClientes[i + 1].idCliente = idCliente;
  vrClientes[i + 1].posicion = card;
  card++;
} // InsertarEnOrden

short BusLinVec(tvrArticulos vrArticulos, short codArticulo, short card)
{
  short i = 1;
  while (i <= card && codArticulo != vrArticulos[i].codArticulo)
    i++;
  return i;
} // BusLinVec

short BusBinVec(tvrClientes vrClientes, int idCliente, short card)
{
  int ult = card - 1;
  int prim = 0;
  int med;
  while (prim <= ult)
  {
    med = (prim + ult) / 2;
    if (vrClientes[med].idCliente = idCliente)
      return vrClientes[med].posicion;
    else if (vrClientes[med].idCliente < idCliente)
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

bool LeerCli(ifstream &Clientes, sCli &rCli)
{
  Clientes >> rCli.idCliente;
  Clientes.ignore(1);
  Clientes.get(rCli.razSoc, 21);
  Clientes.ignore(1);
  Clientes.get(rCli.domic, 21);
  Clientes.ignore(1);
  Clientes.get(rCli.localidad, 21);
  Clientes >> rCli.codPos
  >> rCli.fechaAlta.dia
  >> rCli.fechaAlta.mes
  >> rCli.fechaAlta.year
  >> rCli.saldo;
  Clientes.ignore(1);
  cout << rCli.idCliente << " " <<  Clientes.good() << endl;
  return Clientes.good();
}

void ProcClientes(ifstream &Clientes, tvrClientes vrClientes, short &canCli)
{
  sCli rCliente;
  while (LeerCli(Clientes, rCliente)) {
     InsertarEnOrden(vrClientes, rCliente.idCliente, canCli);
  }
}

bool LeerArt(ifstream &Articulos, sArt &rArt)
{
  Articulos >> rArt.codArticulo;
  Articulos.ignore(1);
  Articulos.get(rArt.descripcion, 21);
  // Articulos.ignore(1);
  Articulos >> rArt.stock >> rArt.precioUnitario;
  Articulos.ignore();

  return Articulos.good();
}; // LeerArt

void AcumProductos(tvrArticulos vrArticulos, sArt rArt, short &canArt)
{
  short posicion;
  posicion = BusLinVec(vrArticulos, rArt.codArticulo, canArt);
  if (posicion > canArt)
  {
    canArt++;
    vrArticulos[posicion].codArticulo = rArt.codArticulo;
    strcpy(vrArticulos[posicion].descripcion, rArt.descripcion);
    vrArticulos[posicion].precioUnitario = rArt.precioUnitario;
    vrArticulos[posicion].stock = rArt.stock;
  }
}

void ProcArticulos(ifstream &Articulos, tvrArticulos vrArticulos, short &canArt)
{
  sArt rArt;
  while (LeerArt(Articulos, rArt))
    AcumProductos(vrArticulos, rArt, canArt);
}

bool LeerPed(ifstream &Pedidos, sPed &pedido)
{
  Pedidos >> pedido.idCliente >> pedido.codArticulo >> pedido.cantPedida;
  Pedidos.ignore();
  return Pedidos.good();
}

bool BusDDCli(ifstream &Clientes, sCli &cliente, short posicion)
{
  Clientes.clear();
  Clientes.seekg(posicion * 100, ios::beg);
  Clientes >> cliente.idCliente;
  Clientes.ignore(1);
  Clientes.get(cliente.razSoc, 21);
  Clientes.ignore(1);
  Clientes.get(cliente.domic, 21);
  Clientes.ignore(1);
  Clientes.get(cliente.localidad, 21);
  Clientes >> cliente.codPos
  >> cliente.fechaAlta.dia
  >> cliente.fechaAlta.mes
  >> cliente.fechaAlta.year
  >> cliente.saldo;
  Clientes.ignore();
  return Clientes.good();
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

short BusDDArt(tvrArticulos vrArticulos, short codArticulo, sArt &articulo, short cantArt)
{
  for (int i = 1; i <= cantArt; i++)
  {
    if (vrArticulos[i].codArticulo == codArticulo)
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

void EmitMvtosPedidos(ofstream &MvtosPedidos, int idCliente, int numFactura, sFec fecha, double totalFactura)
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
      BusDDCli(Clientes, cliente, posicion);
      EmiteCabFac(Facturas, cliente, fecha, numFactura);
      numFactura++;
      while (pedidoLeido && pedido.idCliente == cliente.idCliente)
      {
        double totalItem = 0;
        BusDDArt(vrArticulos, pedido.codArticulo, articulo, canArt);
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

  for (int i = 0; i < canCli; i++) {
    cout << vrClientes[i].idCliente << " " << vrClientes[i].posicion << endl;
  }
 // ProcArticulos(Articulos, vrArticulos, canArt);
  //ProcPedidos(Pedidos, Clientes, Facturas, MvtosPedidos, vrClientes, canCli, vrArticulos, canArt);
  Cerrar(Clientes, Articulos, Pedidos, Facturas, MvtosPedidos);
  return 0;
}
