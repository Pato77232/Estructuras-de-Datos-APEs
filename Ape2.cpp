#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// ============================================================
//  NodoNota  –  nodo de la lista enlazada de calificaciones
// ============================================================
class NodoNota {
public:
    double valor;
    NodoNota* siguiente;

    NodoNota(double v) {
        valor     = v;
        siguiente = nullptr;
    } //clase que definira como funcionara el nodo para las notas
};


// ============================================================
//  Estudiante  –  nodo de la lista enlazada del curso
// ============================================================
class Estudiante {
public://clase principal de estudiantes donde estableceremos que es y como manejaremos a los estudiantes
    string cedula;
    string nombres;
    string apellidos;
    string fechaNacimiento;

    NodoNota* notas;
    int cantNotas;
    static const int MAX_NOTAS = 7;

    Estudiante* siguiente;

    Estudiante(string ced, string nom, string ape, string fecha) {
        cedula          = ced;
        nombres         = nom;
        apellidos       = ape;
        fechaNacimiento = fecha;
        notas           = nullptr;
        cantNotas       = 0;
        siguiente       = nullptr;
    }//tipos de datos y variables que manejara la clase estudiante

    ~Estudiante() {
        NodoNota* actual = notas;
        while (actual != nullptr) {
            NodoNota* sig = actual->siguiente;
            delete actual;
            actual = sig;
        }
    }

    int calcularEdad() const {
        if (fechaNacimiento.size() < 4) return 0;
        int anio = stoi(fechaNacimiento.substr(fechaNacimiento.size() - 4));
        return 2025 - anio;
    }//metodo de calculo de edad a partir de una fecha de nacimiento 

    double calcularPromedio() const {
        if (cantNotas == 0) return 0.0;
        double suma = 0;
        NodoNota* actual = notas;
        while (actual != nullptr) {
            suma  += actual->valor;
            actual = actual->siguiente;
        }
        return suma / cantNotas;
    }//calculadora de promedio en base a las notas proporcionadas

    bool agregarNota(double valor) {
        if (cantNotas >= MAX_NOTAS) return false;
        NodoNota* nueva = new NodoNota(valor);
        if (notas == nullptr) {
            notas = nueva;
        } else {
            NodoNota* actual = notas;
            while (actual->siguiente != nullptr) actual = actual->siguiente;
            actual->siguiente = nueva;
        }
        cantNotas++;
        return true;
    }//verificador de notas si esta basia y si es posible añadir una mas al sistema

    bool modificarNota(int indice, double valor) {
        NodoNota* actual = notas;
        int pos = 1;
        while (actual != nullptr) {
            if (pos == indice) { actual->valor = valor; return true; }
            actual = actual->siguiente;
            pos++;
        }
        return false;
    }//recurso que nos permite modificar una nota ya registrada

    bool eliminarNota(int indice) {
        if (notas == nullptr || indice < 1 || indice > cantNotas) return false;
        if (indice == 1) {
            NodoNota* aBorrar = notas;
            notas = notas->siguiente;
            delete aBorrar;
        } else {
            NodoNota* anterior = notas;
            for (int i = 1; i < indice - 1; i++) anterior = anterior->siguiente;
            NodoNota* aBorrar   = anterior->siguiente;
            anterior->siguiente = aBorrar->siguiente;
            delete aBorrar;
        }
        cantNotas--;
        return true;
    }//buscador que permite eliminar notas ya ingresadas

    void mostrarDatos() const {
        cout << "  Nombre : " << nombres << " " << apellidos << "\n";
        cout << "  Cedula : " << cedula << "\n";
        cout << "  Edad   : " << calcularEdad() << " anos\n";
    }
//impresion de datos ingresados

void mostrarNotas() const {
        if (cantNotas == 0) { cout << "    (ninguna)\n"; return; }
        NodoNota* actual = notas;
        int i = 1;
        while (actual != nullptr) {
            cout << "    [" << i << "] " << fixed << setprecision(2) << actual->valor << "\n";
            actual = actual->siguiente;
            i++;
        }
    }
};


// ============================================================
//  Curso  –  lista enlazada de estudiantes
// ============================================================
class Curso {
public:
    string nombre;
    string id;

    Estudiante* cabeza;
    int cantidad;
    static const int MAX_ESTUDIANTES = 20;

    Curso(string nom = "Curso General", string identificador = "C001") {
        nombre   = nom;
        id       = identificador;
        cabeza   = nullptr;
        cantidad = 0;
    }

    ~Curso() {
        Estudiante* actual = cabeza;
        while (actual != nullptr) {
            Estudiante* sig = actual->siguiente;
            delete actual;
            actual = sig;
        }
    }

    int getCantidad() const { return cantidad; }

    Estudiante* buscar(const string& cedula) const {
        Estudiante* actual = cabeza;
        while (actual != nullptr) {
            if (actual->cedula == cedula) return actual;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    Estudiante* obtenerPorIndice(int indice) const {
        Estudiante* actual = cabeza;
        int pos = 1;
        while (actual != nullptr) {
            if (pos == indice) return actual;
            actual = actual->siguiente;
            pos++;
        }
        return nullptr;
    }

    bool agregarEstudiante(Estudiante* e) {
        if (cantidad >= MAX_ESTUDIANTES) return false;
        if (cabeza == nullptr) {
            cabeza = e;
        } else {
            Estudiante* actual = cabeza;
            while (actual->siguiente != nullptr) actual = actual->siguiente;
            actual->siguiente = e;
        }
        cantidad++;
        return true;
    }

    bool eliminarEstudiante(int indice) {
        if (cabeza == nullptr || indice < 1 || indice > cantidad) return false;
        if (indice == 1) {
            Estudiante* aBorrar = cabeza;
            cabeza = cabeza->siguiente;
            delete aBorrar;
        } else {
            Estudiante* anterior = obtenerPorIndice(indice - 1);
            Estudiante* aBorrar  = anterior->siguiente;
            anterior->siguiente  = aBorrar->siguiente;
            delete aBorrar;
        }
        cantidad--;
        return true;
    }

    void listarEstudiantes() const {
        if (cabeza == nullptr) {
            cout << "  (No hay estudiantes registrados)\n";
            return;
        }
        cout << left
             << setw(4)  << "N"
             << setw(12) << "Cedula"
             << setw(18) << "Nombres"
             << setw(18) << "Apellidos"
             << setw(12) << "F. Nac." << "\n";
        cout << string(64, '-') << "\n";
        Estudiante* actual = cabeza;
        int i = 1;
        while (actual != nullptr) {
            cout << setw(4)  << i
                 << setw(12) << actual->cedula
                 << setw(18) << actual->nombres
                 << setw(18) << actual->apellidos
                 << setw(12) << actual->fechaNacimiento << "\n";
            actual = actual->siguiente;
            i++;
        }
    }
ouble promedioCurso() const {
        double suma  = 0;
        int    total = 0;
        Estudiante* est = cabeza;
        while (est != nullptr) {
            NodoNota* n = est->notas;
            while (n != nullptr) { suma += n->valor; total++; n = n->siguiente; }
            est = est->siguiente;
        }
        return (total == 0) ? -1.0 : suma / total;
    }

    bool hayNotas() const {
        Estudiante* actual = cabeza;
        while (actual != nullptr) {
            if (actual->cantNotas > 0) return true;
            actual = actual->siguiente;
        }
        return false;
    }
};


// ============================================================
//  ConsoleManager  –  todos los menus por consola
// ============================================================
class ConsoleManager {
private:
    Curso curso;

    void limpiarBuffer() const { cin.ignore(1000, '\n'); }

    void pausar() const {
        cout << "\n  Presione ENTER para continuar...";
        limpiarBuffer();
        cin.get();
    }

    void encabezado(const string& titulo) const {
        cout << "\n========================================\n";
        cout << "  " << titulo << "\n";
        cout << "========================================\n";
    }

    string leerCadena(const string& etiqueta) const {
        string valor;
        cout << "  " << etiqueta << ": ";
        getline(cin, valor);
        return valor;
    }

    double leerNota(const string& etiqueta) const {
        double v;
        while (true) {
            cout << "  " << etiqueta << " (0-10): ";
            cin >> v;
            if (cin.fail() || v < 0 || v > 10) {
                cin.clear(); limpiarBuffer();
                cout << "  Valor invalido. Ingrese un numero entre 0 y 10.\n";
            } else { limpiarBuffer(); return v; }
        }
    }

    int leerEntero(const string& etiqueta, int min, int max) const {
        int v;
        while (true) {
            cout << "  " << etiqueta << " (" << min << "-" << max << "): ";
            cin >> v;
            if (cin.fail() || v < min || v > max) {
                cin.clear(); limpiarBuffer();
                cout << "  Opcion invalida.\n";
            } else { limpiarBuffer(); return v; }
        }
    }

    bool preguntar(const string& msg) const {
        char r;
        cout << "  " << msg << " (s/n): ";
        cin >> r;
        limpiarBuffer();
        return r == 's' || r == 'S';
    }

    // Opcion 1
    void menuEstudiantes() {
        bool salir = false;
        while (!salir) {
            encabezado("GESTION DE ESTUDIANTES");
            curso.listarEstudiantes();
            cout << "\n  1. Ingresar estudiante\n";
            cout << "  2. Modificar estudiante\n";
            cout << "  3. Eliminar estudiante\n";
            cout << "  4. Volver\n";

            int op = leerEntero("Opcion", 1, 4);

            if (op == 1) {
                bool seguir = true;
                while (seguir) {
                    if (curso.getCantidad() >= Curso::MAX_ESTUDIANTES) {
                        cout << "\n  El curso ya tiene el maximo de "
                             << Curso::MAX_ESTUDIANTES << " estudiantes.\n";
                        pausar(); break;
                    }
                    encabezado("NUEVO ESTUDIANTE");
                    string ced = leerCadena("Cedula");
                    if (curso.buscar(ced) != nullptr) {
                        cout << "  Ya existe un estudiante con esa cedula.\n";
                        pausar(); continue;
                    }
                    string nom   = leerCadena("Nombres");
                    string ape   = leerCadena("Apellidos");
                    string fecha = leerCadena("Fecha de nacimiento (DD/MM/AAAA)");
                    Estudiante* e = new Estudiante(ced, nom, ape, fecha);
                    curso.agregarEstudiante(e);
                    cout << "\n  Estudiante registrado correctamente.\n";
                    seguir = preguntar("Desea ingresar otro estudiante?");
                }

            } else if (op == 2) {
                if (curso.getCantidad() == 0) {
                    cout << "\n  No hay estudiantes registrados.\n";
                    pausar(); continue;
                }
                int idx = leerEntero("Numero del estudiante a modificar", 1, curso.getCantidad());
                Estudiante* e = curso.obtenerPorIndice(idx);
                if (!e) { cout << "\n  No encontrado.\n"; pausar(); continue; }
                cout << "\n  Datos actuales:\n";
                e->mostrarDatos();
                cout << "\n  Ingrese los nuevos datos (ENTER para mantener el actual):\n";
                cout << "  Nombres [" << e->nombres << "]: ";
                string tmp; getline(cin, tmp);
                if (!tmp.empty()) e->nombres = tmp;
                cout << "  Apellidos [" << e->apellidos << "]: ";
                getline(cin, tmp);
                if (!tmp.empty()) e->apellidos = tmp;
                cout << "  Fecha nacimiento [" << e->fechaNacimiento << "]: ";
                getline(cin, tmp);
                if (!tmp.empty()) e->fechaNacimiento = tmp;
                cout << "\n  Datos actualizados.\n";
                pausar();
} else if (op == 3) {
                if (curso.getCantidad() == 0) {
                    cout << "\n  No hay estudiantes para eliminar.\n";
                    pausar(); continue;
                }
                bool seguir = true;
                while (seguir && curso.getCantidad() > 0) {
                    curso.listarEstudiantes();
                    int idx = leerEntero("Numero del estudiante a eliminar", 1, curso.getCantidad());
                    Estudiante* e = curso.obtenerPorIndice(idx);
                    if (!e) { cout << "\n  No encontrado.\n"; break; }
                    if (preguntar("Confirma eliminar a " + e->nombres + " " + e->apellidos + "?")) {
                        curso.eliminarEstudiante(idx);
                        cout << "\n  Estudiante eliminado.\n";
                    }
                    if (curso.getCantidad() == 0) break;
                    seguir = preguntar("Desea eliminar otro estudiante?");
                }
                pausar();

            } else {
                salir = true;
            }
        }
    }

    // Opcion 2
    void menuCalificaciones() {
        bool salir = false;
        while (!salir) {
            encabezado("REGISTRO DE CALIFICACIONES");
            string ced = leerCadena("Cedula del estudiante");
            Estudiante* e = curso.buscar(ced);
            if (e == nullptr) {
                cout << "\n  No se encontro un estudiante con esa cedula.\n";
                if (!preguntar("Desea intentar con otra cedula?")) salir = true;
                continue;
            }
            cout << "\n";
            e->mostrarDatos();
            bool terminar = false;
            while (!terminar) {
                cout << "\n  Calificaciones registradas:\n";
                e->mostrarNotas();
                if (e->cantNotas >= Estudiante::MAX_NOTAS) {
                    cout << "\n  Se han ingresado todas las calificaciones posibles (7/7).\n";
                    pausar(); break;
                }
                cout << "\n  1. Agregar calificacion\n";
                cout << "  2. Modificar calificacion\n";
                cout << "  3. Eliminar calificacion\n";
                cout << "  4. Volver\n";
                int op = leerEntero("Opcion", 1, 4);
                if (op == 1) {
                    double nota = leerNota("Calificacion");
                    e->agregarNota(nota);
                    cout << "  Calificacion agregada.\n";
                    if (e->cantNotas >= Estudiante::MAX_NOTAS) {
                        cout << "  Maximo de calificaciones alcanzado (7/7).\n";
                        pausar(); terminar = true;
                    }
                } else if (op == 2) {
                    if (e->cantNotas == 0) { cout << "  No hay calificaciones.\n"; continue; }
                    int idx  = leerEntero("Numero de calificacion a modificar", 1, e->cantNotas);
                    double n = leerNota("Nueva calificacion");
                    e->modificarNota(idx, n);
                    cout << "  Calificacion actualizada.\n";
                } else if (op == 3) {
                    if (e->cantNotas == 0) { cout << "  No hay calificaciones.\n"; continue; }
                    int idx = leerEntero("Numero de calificacion a eliminar", 1, e->cantNotas);
                    e->eliminarNota(idx);
                    cout << "  Calificacion eliminada.\n";
                } else {
                    terminar = true;
                }
            }
            salir = true;
        }
    }

    // Opcion 3
    void mostrarPromedioEstudiante() {
        encabezado("PROMEDIO DE UN ESTUDIANTE");
        string ced = leerCadena("Cedula del estudiante");
        Estudiante* e = curso.buscar(ced);
        if (e == nullptr) {
            cout << "\n  No se encontro un estudiante con esa cedula.\n";
        } else {
            cout << "\n";
            e->mostrarDatos();
            if (e->cantNotas == 0)
                cout << "  Promedio  : Sin calificaciones registradas\n";
            else
                cout << "  Promedio  : " << fixed << setprecision(2) << e->calcularPromedio() << "\n";
        }
        pausar();
    }

    // Opcion 4
    void mostrarPromedioCurso() {
        encabezado("PROMEDIO GENERAL DEL CURSO");
        if (!curso.hayNotas())
            cout << "\n  No se han registrado calificaciones de estudiantes.\n";
        else
            cout << "\n  Promedio general: " << fixed << setprecision(2) << curso.promedioCurso() << "\n";
        pausar();
    }

public:
    ConsoleManager() : curso("Programacion I", "INF-101") {}

    void mostrarMenu() {
        bool activo = true;
        while (activo) {
            encabezado("GESTOR DE PERSONAS");
            cout << "  1. Estudiantes\n";
            cout << "  2. Registro de calificaciones\n";
            cout << "  3. Determinar el promedio de notas de un estudiante\n";
            cout << "  4. Determinar el promedio de notas del curso\n";
            cout << "  0. Salir\n";

            int op = leerEntero("Teclee su opcion", 0, 4);

            switch (op) {
                case 1: menuEstudiantes();           break;
                case 2: menuCalificaciones();        break;
                case 3: mostrarPromedioEstudiante(); break;
                case 4: mostrarPromedioCurso();      break;
                case 0: activo = false;              break;
            }
        }
        cout << "\n  Hasta luego.\n\n";
    }
};


// ============================================================
//  main  –  solo instancia y lanza ConsoleManager
// ============================================================
int main() {
    ConsoleManager app;
    app.mostrarMenu();
    return 0;
}
