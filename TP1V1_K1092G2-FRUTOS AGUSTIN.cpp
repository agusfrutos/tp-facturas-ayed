/*
    # Trabajo Pr�ctico #1:
    Metodolog�a-DP-ECP-M�dulos-ED: Registro, Arreglo y Archivo texto.
    Proceso de facturaci�n y armado de pedidos de clientes.

    # Fecha entrega: 25/09/2024

    # Versi�n: 1.0.0

    # Comisi�n: K1092 Miercoles Turno noche,

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
  short year,
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
  short stock;
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

long GetDate(short &year, short &mes, short &dia)
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

short BusBinVec(tvrClientes vrClientes, int idCliente, short card)
{
  int ult = card - 1;
  int prim = 0;
  int med;
  while (prim <= ult)
  {
    med = (prim + ult) / 2;
    if (vrClientes[med].idCliente == idCliente)
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
  Clientes.clear();
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
  Clientes.ignore();
  return Clientes.good();
}

void ProcClientes(ifstream &Clientes, tvrClientes vrClientes, short &canCli)
{
  sCli rCliente;
  do {
    LeerCli(Clientes, rCliente);
    InsertarEnOrden(vrClientes, rCliente.idCliente, canCli);
  } while (Clientes.good());

} // ProcClientes

bool LeerArt(ifstream &Articulos, sArt &rArt)
{
  Articulos >> rArt.codArticulo;
  Articulos.ignore(1);
  Articulos.get(rArt.descripcion, 21);
  Articulos >> rArt.stock >> rArt.precioUnitario;
  Articulos.ignore();
  return Articulos.good();
}; // LeerArt

void AcumProductos(tvrArticulos vrArticulos, sArt rArt, short &canArt)
{
    vrArticulos[rArt.codArticulo].codArticulo = rArt.codArticulo;
    strcpy(vrArticulos[rArt.codArticulo].descripcion, rArt.descripcion);
    vrArticulos[rArt.codArticulo].precioUnitario = rArt.precioUnitario;
    vrArticulos[rArt.codArticulo].stock = rArt.stock;
    canArt++;
} // AcumProductos

void ProcArticulos(ifstream &Articulos, tvrArticulos vrArticulos, short &canArt)
{
  sArt rArt;
  do {
    LeerArt(Articulos, rArt);
    AcumProductos(vrArticulos, rArt, canArt);
  } while (Articulos.good());
} // ProcArticulos

bool LeerPed(ifstream &Pedidos, sPed &rPed)
{
  Pedidos >> rPed.idCliente >> rPed.codArticulo >> rPed.cantPedida;
  Pedidos.ignore();
  return Pedidos.good();
} // LeerPed

bool BusDDCli(ifstream &Clientes, sCli &rCli, short posicion)
{
 Clientes.clear();
 Clientes.seekg(posicion * 100, ios::beg);
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
  Clientes.ignore();
  return Clientes.good();
} // BusDDCli

void EmiteCabFac(ofstream &Facturas, sCli rCli, sFec fecha, int numFactura)
{
  Facturas << fixed << setprecision(2) <<'\n' << replicate('-', 92) << '\n'
           << setw(10) << "Id.Cliente" << left << "  " << setw(24) << "Razon Social" << setw(24) << "Domicilio" << setw(24) << "Localidad" << setw(12) << "Cod.Pos." << '\n'
           << right << setw(10) << rCli.idCliente << left << "  " << setw(24) << rCli.razSoc << setw(24) << rCli.domic << setw(24) << rCli.localidad << setw(12) << rCli.codPos << '\n'
           << '\n'
           << "Nro.Factura: " << setw(9) << numFactura << setw(4) << " "
           << "Fecha Emision: " << setw(2) << fecha.dia << "-" << setw(2) << right << setfill('0') << fecha.mes << setfill(' ') << "-" << fecha.year
           << setw(4) << " " << "Tipo Factura: " << "(A)" << '\n'
            << '\n'
           << "        Nro.Ord  Id.Articulo  Descripcion          Cantidad     Pre.Uni    Tot.Item" << '\n';
} // EmiteCabClis

short BusDDArt(tvrArticulos vrArticulos, short codArt, sArt &rArt, short cantArt)
{
      rArt.codArticulo = vrArticulos[codArt].codArticulo;
      strcpy(rArt.descripcion,vrArticulos[codArt].descripcion);
      rArt.precioUnitario = vrArticulos[codArt].precioUnitario;
      rArt.stock = vrArticulos[codArt].stock;
} // BusDDArt

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
  Facturas <<'\n'
           << setw(80) << "Tot.Bruto : $ " << totalFactura << '\n'
           << setw(80) << "IVA 21%   : $ " << ivaFactura << '\n'
           << setw(80) << "Total Neto: $ " << (totalFactura + ivaFactura) << '\n' << replicate('-', 92) << endl;
} // EmitPieFac

void EmitMvtosPedidos(ofstream &MvtosPedidos, int idCliente, int numFactura, sFec fecha, double totalFactura)
{
  double ivaFactura = totalFactura * IVA;
  MvtosPedidos << fixed << setprecision(2) << setw(8) << idCliente << " " << setw(6) << numFactura << " " << setw(4) << fecha.year << setw(2) << setfill('0') << fecha.mes << setfill(' ') << fecha.dia << setw(12) << (totalFactura + ivaFactura)  << "\n";
} // EmitMvtosPedidos

void ProcPedidos(ifstream &Pedidos, ifstream &Clientes, ofstream &Facturas, ofstream &MvtosPedidos, tvrClientes vrClientes, short canCli, tvrArticulos vrArticulos, short canArt)
{
  short posicion;
  sPed rPed;
  sCli rCli;
  sArt rArt;
  sFec fecha;
  int numFactura = 222320;
  GetDate(fecha.year, fecha.mes, fecha.dia);
    int idClienteAnterior = 0;
    double totalFactura = 0;
    short numItem = 0;
      do {
            LeerPed(Pedidos, rPed);
            posicion = BusBinVec(vrClientes, rPed.idCliente, canCli);
            if (posicion >= 0)

            {
              BusDDCli(Clientes, rCli, posicion);
              if (idClienteAnterior != rPed.idCliente) {
                  if (idClienteAnterior) {
                    EmitPieFac(Facturas, totalFactura);
                    EmitMvtosPedidos(MvtosPedidos, idClienteAnterior, numFactura, fecha, totalFactura);
                    numFactura++;
              }
              EmiteCabFac(Facturas, rCli, fecha, numFactura);
              idClienteAnterior = rPed.idCliente;
              totalFactura = 0;
              }
                double totalItem = 0;
                BusDDArt(vrArticulos, rPed.codArticulo, rArt, canArt);
                CalcDetFac(rPed.cantPedida, rArt, numItem, totalItem, totalFactura);
                EmiteDetFac(Facturas, rArt, numItem, rPed.cantPedida, totalItem);
              }
      }  while (Pedidos.good());
        EmitPieFac(Facturas, totalFactura);
      EmitMvtosPedidos(MvtosPedidos, idClienteAnterior, numFactura, fecha, totalFactura);
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
