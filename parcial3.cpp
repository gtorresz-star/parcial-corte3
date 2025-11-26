#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

struct Usuario {
    int id;
    string nombre;
    string correo;
    string contrasena;
    string direccion;
    string metodoPago;
};

struct Producto {
    int id;
    string nombre;
    string descripcion;
    double precio;
    int stock;
};

struct ItemCarrito {
    Producto producto;
    int cantidad;
};



int convertirEntero(const string &texto) {
    int valor = 0;
    stringstream ss(texto);
    ss >> valor;
    return valor;
}

double convertirDouble(const string &texto) {
    double valor = 0;
    stringstream ss(texto);
    ss >> valor;
    return valor;
}

vector<Usuario> cargarUsuarios(const string &nombreArchivo) {
    vector<Usuario> usuarios;
    ifstream archivo(nombreArchivo.c_str());
    string linea;
    getline(archivo, linea);

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        Usuario u;
        string idStr;
        getline(ss, idStr, ',');
        getline(ss, u.nombre, ',');
        getline(ss, u.correo, ',');
        getline(ss, u.contrasena, ',');
        getline(ss, u.direccion, ',');
        getline(ss, u.metodoPago, ',');
        u.id = convertirEntero(idStr);
        usuarios.push_back(u);
    }
    return usuarios;
}

vector<Producto> cargarProductos(const string &nombreArchivo) {
    vector<Producto> productos;
    ifstream archivo(nombreArchivo.c_str());
    string linea;
    getline(archivo, linea);

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        Producto p;
        string idStr, precioStr, stockStr;
        getline(ss, idStr, ',');
        getline(ss, p.nombre, ',');
        getline(ss, p.descripcion, ',');
        getline(ss, precioStr, ',');
        getline(ss, stockStr, ',');
        p.id = convertirEntero(idStr);
        p.precio = convertirDouble(precioStr);
        p.stock = convertirEntero(stockStr);
        productos.push_back(p);
    }
    return productos;
}

Usuario *iniciarSesion(vector<Usuario> &usuarios, const string &correo, const string &contrasena) {
    for (size_t i = 0; i < usuarios.size(); ++i)
        if (usuarios[i].correo == correo && usuarios[i].contrasena == contrasena)
            return &usuarios[i];
    return NULL;
}

vector<Producto> listarProductosBajoStock(const vector<Producto> &productos) {
    vector<Producto> bajos;
    for (size_t i = 0; i < productos.size(); ++i)
        if (productos[i].stock < 15)
            bajos.push_back(productos[i]);
    return bajos;
}

void guardarCarritoArchivo(const Usuario &usuario, const vector<ItemCarrito> &carrito) {
    ofstream archivo("Carrito.txt", ios::app);
    for (size_t i = 0; i < carrito.size(); ++i) {
        double subtotal = carrito[i].cantidad * carrito[i].producto.precio;
        archivo << usuario.id << ","
                << usuario.nombre << ","
                << carrito[i].producto.id << ","
                << carrito[i].producto.nombre << ","
                << carrito[i].cantidad << ","
                << carrito[i].producto.precio << ","
                << subtotal << "\n";
    }
    archivo.close();
}

void mostrarCarritoArchivo() {
    ifstream archivo("Carrito.txt");
    if (!archivo) {
        cout << "No hay carrito registrado.\n";
        return;
    }

    string linea;
    cout << "\n=== CONTENIDO DEL CARRITO GUARDADO ===\n";
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string idUsuario, nombreUsuario, idProd, nombreProd, cantidad, precio, subtotal;
        getline(ss, idUsuario, ',');
        getline(ss, nombreUsuario, ',');
        getline(ss, idProd, ',');
        getline(ss, nombreProd, ',');
        getline(ss, cantidad, ',');
        getline(ss, precio, ',');
        getline(ss, subtotal, ',');

        cout << nombreUsuario << " | " << nombreProd
             << " | Cantidad: " << cantidad
             << " | Precio: $" << precio
             << " | Subtotal: $" << subtotal << "\n";
    }
    archivo.close();
}

void mostrarCarrito(const vector<ItemCarrito> &carrito) {
    cout << "\n=== CARRITO DE COMPRAS ===\n";
    double total = 0;
    for (size_t i = 0; i < carrito.size(); ++i) {
        double subtotal = carrito[i].cantidad * carrito[i].producto.precio;
        cout << carrito[i].producto.nombre
             << " (Precio: $" << carrito[i].producto.precio
             << ", Cantidad: " << carrito[i].cantidad
             << ") = $" << subtotal << "\n";
        total += subtotal;
    }
    cout << "TOTAL: $" << total << "\n";
}




void reporteProductosMenorStock(const vector<Producto> &productos) {
    vector<Producto> orden = productos;

    sort(orden.begin(), orden.end(), [](Producto a, Producto b){
        return a.stock < b.stock;
    });

    cout << "\n=== TOP 5 PRODUCTOS CON MENOR STOCK ===\n";
    for (int i = 0; i < 5 && i < orden.size(); i++) {
        cout << orden[i].id << " | "
             << orden[i].nombre << " | Stock: "
             << orden[i].stock << "\n";
    }
}


void reporteComentariosPorFecha() {
    ifstream archivo("Comentarios.txt");
    if (!archivo) {
        cout << "No existe Comentarios.txt\n";
        return;
    }

    string fechaBuscada;
    cout << "Ingrese la fecha (YYYY-MM-DD): ";
    cin >> fechaBuscada;

    int contador = 0;
    string linea;

    getline(archivo, linea); 

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string id, producto, usuario, comentario, fecha;

        getline(ss, id, ',');
        getline(ss, producto, ',');
        getline(ss, usuario, ',');
        getline(ss, comentario, ',');
        getline(ss, fecha, ',');

        if (fecha == fechaBuscada)
            contador++;
    }

    cout << "\n=== REPORTE DE COMENTARIOS ===\n";
    cout << "Fecha: " << fechaBuscada << "\n";
    cout << "Número de comentarios: " << contador << "\n";
}


void reportePrecioMaxMin(const vector<Producto> &productos) {
    if (productos.empty()) return;

    double maximo = productos[0].precio;
    double minimo = productos[0].precio;

    for (size_t i = 1; i < productos.size(); i++) {
        if (productos[i].precio > maximo) maximo = productos[i].precio;
        if (productos[i].precio < minimo) minimo = productos[i].precio;
    }

    cout << "\n=== REPORTE PRECIO MAXIMO Y MINIMO ===\n";
    cout << "Precio maximo: $" << maximo << "\n";
    cout << "Precio minimo: $" << minimo << "\n";
}



int main() {
    vector<Usuario> usuarios = cargarUsuarios("Usuarios.txt");
    vector<Producto> productos = cargarProductos("Productos.txt");
    vector<ItemCarrito> carrito;

    string correo, contrasena;
    cout << "Correo electronico: ";
    getline(cin, correo);
    cout << "Contrasena: ";
    getline(cin, contrasena);

    Usuario *usuarioActual = iniciarSesion(usuarios, correo, contrasena);
    if (!usuarioActual) {
        cout << "Usuario o contraseña inválidos.\n";
        return 0;
    }

    int opcion;
    do {
        cout << "\n=== MENU PRINCIPAL ===\n";
        cout << "1. Listar productos con stock menor a 15\n";
        cout << "2. Listar todos los usuarios\n";
        cout << "3. Agregar producto al carrito\n";
        cout << "4. Ver carrito actual\n";
        cout << "5. Guardar carrito en archivo\n";
        cout << "6. Ver carrito guardado (archivo)\n";
        cout << "7. Reportes\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
        case 1: {
            vector<Producto> bajos = listarProductosBajoStock(productos);
            cout << "\n=== PRODUCTOS CON STOCK < 15 ===\n";
            for (size_t i = 0; i < bajos.size(); ++i)
                cout << bajos[i].id << " | " << bajos[i].nombre
                     << " | " << bajos[i].descripcion
                     << " | Precio: $" << bajos[i].precio
                     << " | Stock: " << bajos[i].stock << "\n";
            break;
        }
        case 2: {
            cout << "\n=== LISTA DE USUARIOS ===\n";
            for (size_t i = 0; i < usuarios.size(); ++i)
                cout << usuarios[i].id << " | " << usuarios[i].nombre
                     << " | " << usuarios[i].correo
                     << " | Dirección: " << usuarios[i].direccion
                     << " | Pago: " << usuarios[i].metodoPago << "\n";
            break;
        }
        case 3: {
            int idProd, cant;
            cout << "Ingrese el ID del producto: ";
            cin >> idProd;
            cout << "Cantidad: ";
            cin >> cant;
            cin.ignore();

            bool encontrado = false;
            for (size_t i = 0; i < productos.size(); ++i) {
                if (productos[i].id == idProd) {
                    ItemCarrito item;
                    item.producto = productos[i];
                    item.cantidad = cant;
                    carrito.push_back(item);
                    cout << "Producto agregado al carrito.\n";
                    encontrado = true;
                    break;
                }
            }
            if (!encontrado)
                cout << "Producto no encontrado.\n";
            break;
        }
        case 4:
            mostrarCarrito(carrito);
            break;

        case 5:
            guardarCarritoArchivo(*usuarioActual, carrito);
            cout << "Carrito guardado correctamente.\n";
            break;

        case 6:
            mostrarCarritoArchivo();
            break;

        case 7: {
            int opR;
            do {
                cout << "\n=== MENU DE REPORTES ===\n";
                cout << "1. Productos con menor stock\n";
                cout << "2. Comentarios por fecha\n";
                cout << "3. Precio maximo y minimo\n";
                cout << "0. Volver\n";
                cout << "Seleccione: ";
                cin >> opR;

                switch (opR) {
                    case 1: reporteProductosMenorStock(productos); break;
                    case 2: reporteComentariosPorFecha(); break;
                    case 3: reportePrecioMaxMin(productos); break;
                    case 0: break;
                    default: cout << "Opcion invalida\n";
                }
            } while (opR != 0);
            break;
        }

        case 0:
            cout << "Saliendo...\n";
            break;

        default:
            cout << "Opción no válida.\n";
        }
    } while (opcion != 0);

    return 0;
}

