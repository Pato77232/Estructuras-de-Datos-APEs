#include <iostream>
#include <string>
#include <sstream>
using namespace std;

//  CLASE CANCION  —  representa cada nodo de la lista, contiene los atributos y metodos de cada Cancion

class Cancion {
public:
    string  titulo;
    string  artista;
    int     duracion;  // en segundos
    Cancion* sig;
    Cancion* ant;

    Cancion(string t, string a, int d)
        : titulo(t), artista(a), duracion(d), sig(NULL), ant(NULL) {}

    // cambia el formato de segundos a MM:SS
    string getDuracion() {
        int m = duracion / 60, s = duracion % 60;
        ostringstream ss;
        ss << (m < 10 ? "0" : "") << m << ":"
           << (s < 10 ? "0" : "") << s;
        return ss.str();
    }

    // devuelve una línea con los datos de la cancion
    string toString() {
        return titulo + " - " + artista + " [" + getDuracion() + "]";
    }
};

//  CLASE LISTA  —  Utiliza una lista doblemente enlazada de Canciones
class Lista {
    Cancion* primero;
    Cancion* ultimo;
    Cancion* actual;   // cancion en reproduccion
    int      total;

    // imprime una fila de la tabla, marcando la cancion actual con ">>>"
    void imprimirFila(Cancion* c, int i) {
        string info = c->toString();
        if ((int)info.size() > 36) info = info.substr(0, 33) + "...";
        int e = 36 - (int)info.size();
        string m = (c == actual) ? ">>>" : "   ";
        cout << "  | " << m << " " << i << " | " << info;
        for (int s = 0; s < e; s++) cout << " ";
        cout << " |\n";
    }

    void mostrar(string tit, bool inv) {
        if (!primero) { cout << "  Lista vacia.\n"; return; }
        cout << "\n  +--------------------------------------------+\n";
        cout << "  | " << tit;
        for (int i = (int)tit.size(); i < 43; i++) cout << " ";
        cout << "|\n";
        cout << "  +--------------------------------------------+\n";
        if (!inv) {
            Cancion* t = primero; int i = 1;
            while (t != NULL) { imprimirFila(t, i++); t = t->sig; }
        } else {
            Cancion* t = ultimo; int i = total;
            while (t != NULL) { imprimirFila(t, i--); t = t->ant; }
        }
        cout << "  +--------------------------------------------+\n";
        cout << "  Total: " << total
             << "  |  Actual: " << getActual() << "\n\n";
    }

public:
    Lista() : primero(NULL), ultimo(NULL), actual(NULL), total(0) {}

    ~Lista() {
        Cancion* aux;
        while (primero != NULL) {
            aux     = primero;
            primero = primero->sig;
            delete aux;
        }
        ultimo = actual = NULL;
    }

    // Insertar al inicio ------------------------------------------------
    void insertarInicio(string t, string a, int d) {
        Cancion* n = new Cancion(t, a, d);
        if (!primero) {
            primero = ultimo = actual = n;
        } else {
            n->sig       = primero;
            primero->ant = n;
            primero      = n;
        }
        total++;
        cout << "  [+] Insertada al inicio: \"" << t << "\"\n";
    }

    // Insertar al final -------------------------------------------------
    void insertarFinal(string t, string a, int d) {
        Cancion* n = new Cancion(t, a, d);
        if (!primero) {
            primero = ultimo = actual = n;
        } else {
            n->ant      = ultimo;
            ultimo->sig = n;
            ultimo      = n;
        }
        total++;
        cout << "  [+] Insertada al final: \"" << t << "\"\n";
    }

    // Buscar por titulo -------------------------------------------------
    void buscar(string titulo) {
        if (!primero) { cout << "  Lista vacia.\n"; return; }
        Cancion* t = primero; int pos = 1;
        while (t != NULL) {
            if (t->titulo == titulo) {
                actual = t;  // la cancion encontrada pasa a ser la actual
                cout << "  Encontrada en posicion " << pos
                     << ". Ahora es la actual.\n";
                return;
            }
            t = t->sig; pos++;
        }
        cout << "  No encontrada: \"" << titulo << "\"\n";
    }

    // Eliminar por titulo -----------------------------------------------
    void eliminar(string titulo) {
        if (!primero) { cout << "  Lista vacia.\n"; return; }
        Cancion* t = primero;
        while (t != NULL) {
            if (t->titulo == titulo) {
                if (total == 1) {
                    primero = ultimo = actual = NULL;
                } else {
                    if (t->ant != NULL) t->ant->sig = t->sig;
                    else                primero      = t->sig;

                    if (t->sig != NULL) t->sig->ant = t->ant;
                    else                ultimo       = t->ant;

                    // si se elimina el actual, mover al siguiente o anterior
                    if (t == actual)
                        actual = (t->sig != NULL) ? t->sig : t->ant;
                }
                delete t; total--;
                cout << "  [-] Eliminada: \"" << titulo << "\"\n";
                return;
            }
            t = t->sig;
        }
        cout << "  No encontrada: \"" << titulo << "\"\n";
    }

    // Navegar -----------------------------------------------------------
    void siguiente() {
        if (!actual) { cout << "  Lista vacia.\n"; return; }
        actual = (actual->sig != NULL) ? actual->sig : primero; // wrap
        cout << "  >> Ahora: " << actual->toString() << "\n";
    }

    void anterior() {
        if (!actual) { cout << "  Lista vacia.\n"; return; }
        actual = (actual->ant != NULL) ? actual->ant : ultimo;  // wrap
        cout << "  << Ahora: " << actual->toString() << "\n";
    }

    // Mostrar -----------------------------------------------------------
    void mostrarInicioFin() { mostrar("PRIMERO -> ULTIMO", false); }
    void mostrarFinInicio() { mostrar("ULTIMO -> PRIMERO", true);  }

    // Consultar extremos ------------------------------------------------
    void consultarExtremos() {
        if (!primero) { cout << "  Lista vacia.\n"; return; }
        cout << "  Primero: " << primero->toString() << "\n";
        cout << "  Ultimo:  " << ultimo->toString()  << "\n";
    }

    int    getTotal()  { return total; }
    string getActual() { return actual ? actual->titulo : "ninguna"; }
};

//  CLASE REPRODUCTOR  —  contiene la lista y el menu
class Reproductor {
    Lista  lista;
    string nombre;

    // pide los datos de una cancion al usuario para crear una instancia de cancion
    void leerCancion(string &titulo, string &artista, int &duracion) {
        cout << "  Titulo:         "; getline(cin, titulo);
        cout << "  Artista:        "; getline(cin, artista);
        cout << "  Duracion (seg): "; cin >> duracion; cin.ignore();
    }

public:
    Reproductor(string n) : nombre(n) {}

    void menu() {
        int    op;
        string titulo, artista;
        int    duracion;
        // Menu interactivo
        do {
            cout << "\n  ========================================\n";
            cout << "   REPRODUCTOR: " << nombre << "\n";
            cout << "   Canciones: " << lista.getTotal();
            cout << "  |  Actual: " << lista.getActual() << "\n";
            cout << "  ========================================\n";
            cout << "  1. Insertar cancion al inicio\n";
            cout << "  2. Insertar cancion al final\n";
            cout << "  3. Buscar cancion\n";
            cout << "  4. Eliminar cancion\n";
            cout << "  5. Siguiente\n";
            cout << "  6. Anterior\n";
            cout << "  7. Mostrar lista (primero -> ultimo)\n";
            cout << "  8. Mostrar lista (ultimo -> primero)\n";
            cout << "  9. Consultar primero y ultimo\n";
            cout << "  0. Salir\n";
            cout << "  Opcion: ";
            cin >> op; cin.ignore();

            cout << "\n";
            switch (op) {
                case 1:
                    leerCancion(titulo, artista, duracion);
                    lista.insertarInicio(titulo, artista, duracion);
                    break;
                case 2:
                    leerCancion(titulo, artista, duracion);
                    lista.insertarFinal(titulo, artista, duracion);
                    break;
                case 3:
                    cout << "  Titulo a buscar: "; getline(cin, titulo);
                    lista.buscar(titulo);
                    break;
                case 4:
                    cout << "  Titulo a eliminar: "; getline(cin, titulo);
                    lista.eliminar(titulo);
                    break;
                case 5: lista.siguiente();          break;
                case 6: lista.anterior();           break;
                case 7: lista.mostrarInicioFin();   break;
                case 8: lista.mostrarFinInicio();   break;
                case 9: lista.consultarExtremos();  break;
                case 0: cout << "  Hasta luego!\n"; break;
                default: cout << "  Opcion invalida.\n";
            }
        } while (op != 0);
    }
};

// -------------------------------------------------------
//  main
// -------------------------------------------------------
int main() {
    cout << "\n  ========================================\n";
    cout << "       REPRODUCTOR DE MUSICA v5.0\n";
    cout << "  ========================================\n";
    Reproductor r("Mi Playlist");
    r.menu();
    return 0;
}
