#include <iostream.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>

// Definición del tipo str20 que faltaba en las estructuras originales
typedef char str20[21];

// Definiciones de las estructuras
struct sFec {
    short dia, mes, year;
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

// Funciones para validar datos
bool validarFecha(sFec fecha) {
    if (fecha.mes < 1 || fecha.mes > 12) return false;
    if (fecha.dia < 1) return false;

    switch(fecha.mes) {
        case 4: case 6: case 9: case 11:
            if (fecha.dia > 30) return false;
            break;
        case 2:
            if ((fecha.year % 4 == 0 && fecha.year % 100 != 0) || fecha.year % 400 == 0) {
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

// Funciones para ingresar datos
void ingresarFecha(sFec &fecha) {
    do {
        cout << "Dia: "; cin >> fecha.dia;
        cout << "Mes: "; cin >> fecha.mes;
        cout << "Año: "; cin >> fecha.year;
    } while (!validarFecha(fecha));
}

void ingresarCliente(sCli &cliente) {
    cout << "ID Cliente: ";
    cin >> cliente.idCliente;
    cin.ignore();

    cout << "Razón Social: ";
    cin.getline(cliente.razSoc, 21);

    cout << "Domicilio: ";
    cin.getline(cliente.domic, 21);

    cout << "Localidad: ";
    cin.getline(cliente.localidad, 21);

    cout << "Código Postal: ";
    cin >> cliente.codPos;

    cout << "Fecha de Alta:\n";
    ingresarFecha(cliente.fechaAlta);

    cout << "Saldo inicial: ";
    cin >> cliente.saldo;
}

void ingresarArticulo(sArt &articulo) {
    cout << "Código de Artículo: ";
    cin >> articulo.codArticulo;
    cin.ignore();

    cout << "Descripción: ";
    cin.getline(articulo.descripcion, 21);

    cout << "Stock: ";
    cin >> articulo.stock;

    cout << "Precio Unitario: ";
    cin >> articulo.precioUnitario;
}

void ingresarPedido(sPed &pedido) {
    cout << "ID Cliente: ";
    cin >> pedido.idCliente;

    cout << "Código de Artículo: ";
    cin >> pedido.codArticulo;

    cout << "Cantidad Pedida: ";
    cin >> pedido.cantPedida;
}

// Funciones para escribir en archivos
void escribirCliente(const char* archivo) {
    FILE *f = fopen(archivo, "ab");
    if (f == NULL) {
        cout << "Error al abrir el archivo" << endl;
        return;
    }

    sCli cliente;
    ingresarCliente(cliente);
    fwrite(&cliente, sizeof(sCli), 1, f);
    fclose(f);
}

void escribirArticulo(const char* archivo) {
    FILE *f = fopen(archivo, "ab");
    if (f == NULL) {
        cout << "Error al abrir el archivo" << endl;
        return;
    }

    sArt articulo;
    ingresarArticulo(articulo);
    fwrite(&articulo, sizeof(sArt), 1, f);
    fclose(f);
}

void escribirPedido(const char* archivo) {
    FILE *f = fopen(archivo, "ab");
    if (f == NULL) {
        cout << "Error al abrir el archivo" << endl;
        return;
    }

    sPed pedido;
    ingresarPedido(pedido);
    fwrite(&pedido, sizeof(sPed), 1, f);
    fclose(f);
}

// Funciones para mostrar datos
void mostrarFecha(sFec fecha) {
    cout << fecha.dia << "/" << fecha.mes << "/" << fecha.year;
}

void mostrarClientes(const char* archivo) {
    FILE *f = fopen(archivo, "rb");
    if (f == NULL) {
        cout << "Error al abrir el archivo" << endl;
        return;
    }

    sCli cliente;
    cout << "\nListado de Clientes:" << endl;
    cout << "===================" << endl;

    while(fread(&cliente, sizeof(sCli), 1, f)) {
        cout << "\nID: " << cliente.idCliente << endl;
        cout << "Razón Social: " << cliente.razSoc << endl;
        cout << "Domicilio: " << cliente.domic << endl;
        cout << "Localidad: " << cliente.localidad << endl;
        cout << "Código Postal: " << cliente.codPos << endl;
        cout << "Fecha Alta: ";
        mostrarFecha(cliente.fechaAlta);
        cout << "\nSaldo: $" << cliente.saldo << endl;
        cout << "-------------------" << endl;
    }
    fclose(f);
}

void mostrarArticulos(const char* archivo) {
    FILE *f = fopen(archivo, "rb");
    if (f == NULL) {
        cout << "Error al abrir el archivo" << endl;
        return;
    }

    sArt articulo;
    cout << "\nListado de Artículos:" << endl;
    cout << "====================" << endl;

    while(fread(&articulo, sizeof(sArt), 1, f)) {
        cout << "\nCódigo: " << articulo.codArticulo << endl;
        cout << "Descripción: " << articulo.descripcion << endl;
        cout << "Stock: " << articulo.stock << endl;
        cout << "Precio: $" << articulo.precioUnitario << endl;
        cout << "-------------------" << endl;
    }
    fclose(f);
}

void mostrarPedidos(const char* archivo) {
    FILE *f = fopen(archivo, "rb");
    if (f == NULL) {
        cout << "Error al abrir el archivo" << endl;
        return;
    }

    sPed pedido;
    cout << "\nListado de Pedidos:" << endl;
    cout << "==================" << endl;

    while(fread(&pedido, sizeof(sPed), 1, f)) {
        cout << "\nCliente: " << pedido.idCliente << endl;
        cout << "Artículo: " << pedido.codArticulo << endl;
        cout << "Cantidad: " << pedido.cantPedida << endl;
        cout << "-------------------" << endl;
    }
    fclose(f);
}

void menu() {
    int opcion;
    do {
        clrscr();  // Limpia la pantalla (específico de Borland)
        cout << "\nMenú Principal" << endl;
        cout << "1. Agregar Cliente" << endl;
        cout << "2. Agregar Artículo" << endl;
        cout << "3. Agregar Pedido" << endl;
        cout << "4. Mostrar Clientes" << endl;
        cout << "5. Mostrar Artículos" << endl;
        cout << "6. Mostrar Pedidos" << endl;
        cout << "0. Salir" << endl;
        cout << "Opción: ";
        cin >> opcion;

        switch(opcion) {
            case 1:
                escribirCliente("CLIENTES.DAT");
                break;
            case 2:
                escribirArticulo("ARTICULOS.DAT");
                break;
            case 3:
                escribirPedido("PEDIDOS.DAT");
                break;
            case 4:
                mostrarClientes("CLIENTES.DAT");
                break;
            case 5:
                mostrarArticulos("ARTICULOS.DAT");
                break;
            case 6:
                mostrarPedidos("PEDIDOS.DAT");
                break;
        }
        if (opcion != 0) {
            cout << "\nPresione cualquier tecla para continuar...";
            getch();
        }
    } while(opcion != 0);
}

void main() {
    menu();
}
