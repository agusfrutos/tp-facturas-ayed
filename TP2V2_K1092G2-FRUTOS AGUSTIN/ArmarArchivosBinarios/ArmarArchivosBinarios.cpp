#include <iostream.h>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <conio.h>

typedef char str20[21];

struct sFec {
    short dia, mes, agno;
};

struct sCli {
    int idCliente;
    str20 razSoc, domic, localidad;
    short codPos;
    sFec fechaAlta;
    float saldo;
};

struct sArt {
    short codArticulo;
    str20 descripcion;
    short stock;
    float precioUnitario;
};

struct sPed {
    int idCliente;
    short codArticulo;
    short cantPedida;
};

bool validarFecha(sFec fecha) {
    if (fecha.mes < 1 || fecha.mes > 12) return false;
    if (fecha.dia < 1) return false;

    switch(fecha.mes) {
        case 4: case 6: case 9: case 11:
            if (fecha.dia > 30) return false;
            break;
        case 2:
            if ((fecha.agno % 4 == 0 && fecha.agno % 100 != 0) || fecha.agno % 400 == 0) {
                if (fecha.dia > 29) return false;
            } else {
                if (fecha.dia > 28) return false;
            }
            break;
        default:
            if (fecha.dia > 31) return false;
    }
    return true;
}

void ingresarFecha(sFec &fecha) {
    do {
        cout << "Dia: "; cin >> fecha.dia;
        cout << "Mes: "; cin >> fecha.mes;
        cout << "A�o: "; cin >> fecha.agno;
    } while (!validarFecha(fecha));
}

void ingresarCliente(sCli &cliente) {
    cout << "ID Cliente: ";
    cin >> cliente.idCliente;
    cin.ignore();

    cout << "Razon Social: ";
    cin.getline(cliente.razSoc, 21);

    cout << "Domicilio: ";
    cin.getline(cliente.domic, 21);

    cout << "Localidad: ";
    cin.getline(cliente.localidad, 21);

    cout << "Codigo Postal: ";
    cin >> cliente.codPos;

    cout << "Fecha de Alta:\n";
    ingresarFecha(cliente.fechaAlta);

    cout << "Saldo inicial: ";
    cin >> cliente.saldo;
}

void ingresarArticulo(sArt &articulo) {
    cout << "Codigo de Art�culo: ";
    cin >> articulo.codArticulo;
    cin.ignore();

    cout << "Descripcion: ";
    cin.getline(articulo.descripcion, 21);

    cout << "Stock: ";
    cin >> articulo.stock;

    cout << "Precio Unitario: ";
    cin >> articulo.precioUnitario;
}

void ingresarPedido(sPed &pedido) {
    cout << "ID Cliente: ";
    cin >> pedido.idCliente;

    cout << "Codigo de Articulo: ";
    cin >> pedido.codArticulo;

    cout << "Cantidad Pedida: ";
    cin >> pedido.cantPedida;
}


void escribirCliente(const char* archivo) {
    ofstream clienteFile(archivo,ios::binary | ios::app);
    if (clienteFile == NULL) {
        cout << "Error al abrir el archivo de clientes" << endl;
        return;
    }

    sCli cliente;
    ingresarCliente(cliente);
    clienteFile.write((char*) &cliente, sizeof(cliente));
    clienteFile.close();
}

void escribirArticulo(const char* archivo) {
    ofstream articulosFile(archivo,ios::binary | ios::app);

    if (articulosFile == NULL) {
        cout << "Error al abrir el archivo de articulos" << endl;
        return;
    }
    sArt articulo;
    ingresarArticulo(articulo);
    articulosFile.write((char*) &articulo, sizeof(articulo));
    articulosFile.close();
}

void escribirPedido(const char* archivo) {
    ofstream pedidosFile(archivo,ios::binary | ios::app);
    if (pedidosFile == NULL) {
        cout << "Error al abrir el archivo de pedidos" << endl;
        return;
    }
    sPed pedido;
    ingresarPedido(pedido);
    pedidosFile.write((char*) &pedido, sizeof(pedido));
    pedidosFile.close();
}

// Funciones para mostrar datos
void mostrarFecha(sFec fecha) {
    cout << fecha.dia << "/" << fecha.mes << "/" << fecha.agno;
}

void mostrarClientes(const char* archivo) {
    ifstream clientesFile(archivo, ios::binary);
    if (clientesFile == NULL) {
        cout << "Error al abrir el archivo" << endl;
        return;
    }

    sCli cliente;
    cout << "\nListado de Clientes:" << endl;
    cout << "===================" << endl;

    while(clientesFile.read((char*)&cliente, sizeof(sCli))) {
        cout << "\nID: " << cliente.idCliente << endl;
       cout << "Razon Social: " << cliente.razSoc << endl;
        cout << "Domicilio: " << cliente.domic << endl;
        cout << "Localidad: " << cliente.localidad << endl;
        cout << "Codigo Postal: " << cliente.codPos << endl;
        cout << "Fecha Alta: ";
        mostrarFecha(cliente.fechaAlta);
        cout << "\nSaldo: $" << cliente.saldo << endl;
        cout << "-------------------" << endl;
    }
    clientesFile.close();
}

void mostrarArticulos(const char* archivo) {
    ifstream articulosFile(archivo, ios::binary);
    if (articulosFile == NULL) {
        cout << "Error al abrir el archivo" << endl;
        return;
    }

    sArt articulo;
    cout << "\nListado de Articulos:" << endl;
    cout << "====================" << endl;

    while(articulosFile.read((char*) &articulo, sizeof articulo)) {
        cout << "\nCodigo: " << articulo.codArticulo << endl;
        cout << "Descripcion: " << articulo.descripcion << endl;
        cout << "Stock: " << articulo.stock << endl;
        cout << "Precio: $" << articulo.precioUnitario << endl;
        cout << "-------------------" << endl;
    }
    articulosFile.close();
}

void mostrarPedidos(const char* archivo) {
    ifstream pedidosFile(archivo, ios::binary);
    if (pedidosFile == NULL) {
        cout << "Error al abrir el archivo" << endl;
        return;
    }
    sPed pedido;
    cout << "\nListado de Pedidos:" << endl;
    cout << "==================" << endl;

    while(pedidosFile.read((char*)&pedido, sizeof(pedido))) {
        cout << "\nCliente: " << pedido.idCliente << endl;
        cout << "Articulo: " << pedido.codArticulo << endl;
        cout << "Cantidad: " << pedido.cantPedida << endl;
        cout << "-------------------" << endl;
    }
    pedidosFile.close();
}

void menu() {
    int opcion;
    do {
        clrscr();
        cout << "\nMen� Principal" << endl;
        cout << "1. Agregar Cliente" << endl;
        cout << "2. Agregar Articulo" << endl;
        cout << "3. Agregar Pedido" << endl;
        cout << "\n";
        cout << "4. Mostrar Clientes" << endl;
        cout << "5. Mostrar Articulos" << endl;
        cout << "6. Mostrar Pedidos" << endl;
        cout << "0. Salir" << endl;
        cout << "Opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1:
                escribirCliente("Clientes.Dat");
                break;
            case 2:
                escribirArticulo("Articulos.Dat");
                break;
            case 3:
                escribirPedido("Pedidos.Dat");
                break;
            case 4:
                mostrarClientes("Clientes.Dat");
                break;
            case 5:
                mostrarArticulos("Articulos.Dat");
                break;
            case 6:
                mostrarPedidos("Pedidos.Dat");
                break;
        }
        if (opcion != 0) {
            cout << "\nPresione cualquier tecla para continuar...";
            getch();
        }
    } while(opcion != 0);
}

int main() {
    menu();
    return 0;
}
