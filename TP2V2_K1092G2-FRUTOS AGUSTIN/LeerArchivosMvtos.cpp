#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

struct sMvtosPedidos {
    int idCli,numFactura,fecha;
    float totalNeto;
}; 

int LeerMvtosPedidos() {

    ifstream movPedidosFile;
    movPedidosFile.open("MovPedidos.Dat", ios::binary || ios::app);

    sMvtosPedidos rMov;

    cout << fixed << setprecision(2);
 while(movPedidosFile.read((char*)&rMov, sizeof(rMov))) {
        cout << rMov.fecha << " " << rMov.idCli << " " << rMov.numFactura << " " << rMov.totalNeto << endl;
    }

    movPedidosFile.close();

    return 0;
}
