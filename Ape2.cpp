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
    }//imoresion de datos ingresados
