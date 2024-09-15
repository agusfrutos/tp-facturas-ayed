#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <conio.h>
#include <iomanip.h>

using namespace std;

typedef unsigned char byte;
typedef unsigned short ushort;
typedef char str3[4];
typedef char str8[9];
typedef char str20[21];

struct sFec
{
  int year,
      mes,
      dia,
      dsem;
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

struct Articulo
{
  str3 codArticulo;
  str20 descripcion;
  ushort stock;
  float precioUnitario;
};

struct Pedido
{
  str8 idCliente;
  str3 codArticulo;
  short cantPedida; // max 4 dig
};
typedef sCliIndice vClientes[1001];
typedef Articulo vArticulos[101];

string replicate(char car, unsigned n)
{
  string cad = "";
  for (unsigned i = 1; i <= n; i++)
    cad += car;
  return cad;
} // replicate

long GetDate(int &year, int &mes, int &dia, int &ds)
{

  time_t rawtime;
  struct tm *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  year = 1900 + timeinfo->tm_year;
  mes = 1 + timeinfo->tm_mon;
  dia = timeinfo->tm_mday;
  ds = 1 + timeinfo->tm_wday;
  return (1900 + timeinfo->tm_year) * 10000 + (1 + timeinfo->tm_mon) * 100 +
         timeinfo->tm_mday;
} // GetDate

void InsertarEnOrden(vClientes lstClientes, str8 idCliente, short &card)
{
  int i = card - 1;
  while (i >= 0 && strcmp(idCliente, lstClientes[i].idCliente) < 0)
  {
    lstClientes[i + 1] = lstClientes[i];
    i--;
  }
  strcpy(lstClientes[i + 1].idCliente, idCliente);
  lstClientes[i + 1].posicion = card;
  card++;
}

short BusLinVec(vArticulos lstArticulos, str3 codArticulo, short card)
{
  short i = 1;
  while (i <= card && strcmp(codArticulo, lstArticulos[i].codArticulo) != 0)
    i++;
  return i;
}

short BusBinVec(vClientes lstClientes, str8 idCliente, int card)
{
  int ult = card - 1;
  int prim = 0;
  int med;
  while (prim <= ult)
  {
    med = (prim + ult) / 2;
    int comparacion = strcmp(lstClientes[med].idCliente, idCliente);

    if (comparacion == 0)
      return lstClientes[med].posicion;
    else if (comparacion < 0)
      prim = med + 1;
    else
      ult = med - 1;
  }
  return -1;
} // BusBinVec

void Abrir(ifstream &clientesFile, ifstream &articulosFile, ifstream &pedidosFile, ofstream &facturasFile, ofstream &mvtosPedidosFile)
{
  clientesFile.open("Clientes.txt");
  articulosFile.open("Articulos.txt");
  pedidosFile.open("Pedidos.txt");
  mvtosPedidosFile.open("MvtosPedidos.txt"); // iofstream
  facturasFile.open("Facturas.txt", ios::trunc);
}

void Cerrar(ifstream &clientesFile, ifstream &articulosFile, ifstream &pedidosFile, ofstream &facturasFile, ofstream &mvtosPedidosFile)
{
  clientesFile.close();
  articulosFile.close();
  pedidosFile.close();
  mvtosPedidosFile.close();
  facturasFile.close();
}

bool LeerCli(ifstream &clientesFile, sCliIndice &cliente)
{
  if (clientesFile.get(cliente.idCliente, 9))
  {
    clientesFile.ignore(91, '\n');
    return true;
  };
  return false;
}

void ProcClientes(ifstream &clientesFile, vClientes lstClientes, short &canCli)
{
  sCliIndice cliente;
  while (LeerCli(clientesFile, cliente))
  {
    InsertarEnOrden(lstClientes, cliente.idCliente, canCli);
  }
}

bool LeerArt(ifstream &articulosFile, Articulo &articulo)
{
  articulosFile.get(articulo.codArticulo, 4);
  articulosFile.ignore(1);
  articulosFile.get(articulo.descripcion, 21);
  articulosFile >> articulo.stock;
  articulosFile >> articulo.precioUnitario;
  articulosFile.ignore();

  return articulosFile.good();
};

void AcumProductos(vArticulos lstArticulos, Articulo articulo, short &canArt)
{
  short posicion;

  posicion = BusLinVec(lstArticulos, articulo.codArticulo, canArt);
  if (posicion > canArt)
  {
    canArt++;
    strcpy(lstArticulos[posicion].codArticulo, articulo.codArticulo);
    strcpy(lstArticulos[posicion].descripcion, articulo.descripcion);
    lstArticulos[posicion].precioUnitario = articulo.precioUnitario;
    lstArticulos[posicion].stock = articulo.stock;
  }
}

void ProcArticulos(ifstream &articulosFile, vArticulos lstArticulos, short &canArt)
{
  Articulo articulo;
  while (LeerArt(articulosFile, articulo))
    AcumProductos(lstArticulos, articulo, canArt);
  // for (int i = 0; i < card; i++)
  // {
  //   cout << lstArticulos[i].codArticulo << " "
  //   << lstArticulos[i].descripcion << " "
  //   << fixed << setprecision(2) << lstArticulos[i].precioUnitario << " "
  //   << lstArticulos[i].stock << endl;
  // }
}

bool LeerPed(ifstream &Pedidos, Pedido &pedido)
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
    // dudas: la fechaAlta no la uso, el saldo deberia calcular si llega con los productos comprados?

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

void EmiteCabFac(ofstream &facturas, sCli cliente, sFec fecha, int numFactura)
{

  facturas << replicate('-', 92) << '\n'
           << setw(10) << "Id.Cliente" << left << "  " << setw(24) << "Razon Social" << setw(24) << "Domicilio" << setw(24) << "Localidad" << setw(12) << "Cod.Pos." << '\n'
           << right << setw(10) << cliente.idCliente << left << "  " << setw(24) << cliente.razSoc << setw(24) << cliente.domic << setw(24) << cliente.localidad << setw(12) << cliente.codPos << '\n'
           << '\n'
           << "Nro.Factura: " << setw(9) << numFactura << setw(4) << " "
           << "Fecha Emision: " << setw(2) << fecha.dia << "-" << setw(2) << right << setfill('0') << fecha.mes << setfill(' ') << "-" << fecha.year
           << setw(4) << " " << "Tipo Factura: " << "(A)" << '\n' << '\n'
           << "        Nro.Ord  Id.Articulo  Descripcion          Cantidad     Pre.Uni    Tot.Item" << '\n';
} // EmiteCabClis

short BuscarArticuloPorCodigo(vArticulos lstArticulos, str3 codArticulo, Articulo &articulo, short cantidadArticulos)
{
  // Crear un articulo vacio como valor por defecto - ??

  for (int i = 1; i <= cantidadArticulos; i++)
  {
        //cout << lstArticulos[i].codArticulo << " y " << codArticulo << endl;
    if (strcmp(lstArticulos[i].codArticulo, codArticulo) == 0)
    {
      articulo = lstArticulos[i];
      return 1;
    }
  }
  return 0;
}

void EmiteDetFac(ofstream &Facturas, Articulo art,short numOrden, short cantPedida, float totalImporte)
{
  Facturas << setw(15) << numOrden << setw(13) << art.codArticulo << setw(22) << art.descripcion << setw(9) << cantPedida << setw(12) << art.precioUnitario << setw(12) << totalImporte << '\n';

}

void CalcDetFac(int &cantidad, Articulo &art, short &numOrden, float &totalItem, float &totalFactura)
{
    numOrden++;
    totalItem = cantidad * art.precioUnitario;
    art.stock -= cantidad;
    totalFactura += totalItem;
} // CalcDetFac

void ProcPedidos(ifstream &Pedidos, ifstream &Clientes, ofstream &Facturas, ofstream &MvtosPedidos, vClientes lstClientes, short canCli, vArticulos lstArticulos, short canArt)
{
  short posicion;
  Pedido pedido;
  sCli cliente;
  Articulo articulo;
  sFec fecha;

  float totalFactura = 0;
  short numOrden = 1;
  int numFactura = 2223200;

    bool pedidoLeido = LeerPed(Pedidos, pedido);

  while (pedidoLeido)
  {
    posicion = BusBinVec(lstClientes, pedido.idCliente, canCli);
    cout << (posicion >= 0 ? "Cliente Encontrado" : " Cliente No encontrado") << endl;
    if (posicion >= 0)
    {
      cout << "Leyendo pedido numero " << numOrden << endl;
      LeerCliByPosicion(Clientes, cliente, posicion);
      cout << "pedido de cliente: " << cliente.razSoc << endl;
      GetDate(fecha.year, fecha.mes, fecha.dia, fecha.dsem);
      EmiteCabFac(Facturas, cliente, fecha, numFactura);

    //cout << (artEncontrado ? "Art Encontrado" : " Art No encontrado") << endl;
      cout << "Articulo pedido: " << articulo.codArticulo << " - " << pedido.codArticulo << endl;
      while (pedidoLeido && strcmp(pedido.idCliente, cliente.idCliente) == 0)
      {
        float totalItem = 0;
        BuscarArticuloPorCodigo(lstArticulos, pedido.codArticulo, articulo, canArt);

        CalcDetFac(pedido.cantPedida, articulo,numOrden,totalItem,totalFactura);

        EmiteDetFac(Facturas, articulo,numOrden, pedido.cantPedida, totalItem);
        pedidoLeido = LeerPed(Pedidos, pedido);
      }
    }
    pedidoLeido = LeerPed(Pedidos, pedido);
   cout << "Pedidonuevo: " << pedido.idCliente << " - " << pedido.codArticulo << endl;
  }
};

int main()
{
  cout << "Procesando..." << endl;
  ifstream clientesFile, articulosFile, pedidosFile;
  ofstream facturasFile, mvtosPedidosFile;

  vClientes lstClientes;
  vArticulos lstArticulos;

  short canCli = 0;
  short canArt = 0;

  Abrir(clientesFile, articulosFile, pedidosFile, facturasFile, mvtosPedidosFile);
  ProcClientes(clientesFile, lstClientes, canCli);
  ProcArticulos(articulosFile, lstArticulos, canArt);
  ProcPedidos(pedidosFile, clientesFile, facturasFile, mvtosPedidosFile, lstClientes, canCli, lstArticulos, canArt);
  Cerrar(clientesFile, articulosFile, pedidosFile, facturasFile, mvtosPedidosFile);
  cout << "Proceso finalizado con exito." << endl;
  return 0;
}
