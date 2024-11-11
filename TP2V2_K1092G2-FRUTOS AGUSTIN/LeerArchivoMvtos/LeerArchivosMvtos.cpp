#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

struct sMvtos
{
    int idCli, numFactura, fecha;
    float totalNeto;
};

int LeerArchivoMvtos()
{
    ifstream movPedidosFile;
    movPedidosFile.open("MovPedidos.Dat", ios::binary);
    sMvtos rMov;

    cout << fixed << setprecision(2);
    while (movPedidosFile.read((char *)&rMov, sizeof(rMov)))
    {
        cout << rMov.idCli << " " << rMov.fecha << " " << rMov.numFactura << " " << rMov.totalNeto << endl;
    }

    movPedidosFile.close();

    return 0;
}
