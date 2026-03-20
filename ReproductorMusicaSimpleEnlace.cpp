#include <iostream>
#include <string>
#include <sstream>
using namespace std;


//  Clase Cancion  —  nodo de la lista simple
class Cancion {
public:
    string   titulo;
    string   artista;
    int      duracion;
    Cancion* sig;      // solo apunta al siguiente, no hay ant

    Cancion(string t, string a, int d)
        : titulo(t), artista(a), duracion(d), sig(NULL) {}

    string getDuracion() {
        int m = duracion / 60, s = duracion % 60;
        ostringstream ss;
        ss << (m < 10 ? "0" : "") << m << ":"
           << (s < 10 ? "0" : "") << s;
        return ss.str();
    }

    string toString() {
        return titulo + " - " + artista + " [" + getDuracion() + "]";
    }
};


//  Clase Lista  —  lista simplemente enlazada de canciones
class Lista {
    Cancion* primero;
    Cancion* ultimo;
    int      total;

    void imprimirFila(Cancion* c, int i) {
        string info = c->toString();
        if ((int)info.size() > 36) info = info.substr(0, 33) + "...";
        int e = 36 - (int)info.size();
        cout << "  |     " << i << " | " << info;
        for (int s = 0; s < e; s++) cout << " ";
        cout << " |\n";
    }

public:
    Lista() : primero(NULL), ultimo(NULL), total(0) {}

    ~Lista() {
        Cancion* aux;
        while (primero != NULL) {
            aux     = primero;
            primero = primero->sig;
            delete aux;
        }
        ultimo = NULL;
    }

    // Insertar al inicio
    void insertarInicio(string t, string a, int d) {
        Cancion* n = new Cancion(t, a, d);
        if (!primero) {
            primero = ultimo = n;
        } else {
            n->sig  = primero;
            primero = n;
        }
        total++;
        cout << "  [+] Insertada al inicio: \"" << t << "\"\n";
    }

    // Insertar al final
    void insertarFinal(string t, string a, int d) {
        Cancion* n = new Cancion(t, a, d);
        if (!primero) {
            primero = ultimo = n;
        } else {
            ultimo->sig = n;
            ultimo      = n;
        }
        total++;
        cout << "  [+] Insertada al final: \"" << t << "\"\n";
    }

    // Buscar por titulo
    void buscar(string titulo) {
        if (!primero) { cout << "  Lista vacia.\n"; return; }
        Cancion* t = primero;
        int pos = 1;
        while (t != NULL) {
            if (t->titulo == titulo) {
                cout << "  Encontrada en posicion " << pos << ".\n";
                cout << "  " << t->toString() << "\n";
                return;
            }
            t = t->sig;
            pos++;
        }
        cout << "  No encontrada: \"" << titulo << "\"\n";
    }

    // Eliminar por titulo 
    void eliminar(string titulo) {
        if (!primero) { cout << "  Lista vacia.\n"; return; }

        // es la primera cancion
        if (primero->titulo == titulo) {
            Cancion* aux = primero;
            primero = primero->sig;
            if (!primero) ultimo = NULL;
            delete aux;
            total--;
            cout << "  [-] Eliminada: \"" << titulo << "\"\n";
            return;
        }

        // buscar en el resto — necesitamos ant para reconectar
        Cancion* ant = primero;
        Cancion* aux = primero->sig;
        while (aux != NULL) {
            if (aux->titulo == titulo) {
                ant->sig = aux->sig;
                if (aux == ultimo) ultimo = ant;
                delete aux;
                total--;
                cout << "  [-] Eliminada: \"" << titulo << "\"\n";
                return;
            }
            ant = aux;
            aux = aux->sig;
        }
        cout << "  No encontrada: \"" << titulo << "\"\n";
    }

    //  Mostrar Primero a Ultimo
    void mostrar() {
        if (!primero) { cout << "  Lista vacia.\n"; return; }
        string tit = "PRIMERO -> ULTIMO";
        cout << "\n  +--------------------------------------------+\n";
        cout << "  | " << tit;
        for (int i = (int)tit.size(); i < 43; i++) cout << " ";
        cout << "|\n";
        cout << "  +--------------------------------------------+\n";
        Cancion* t = primero;
        int i = 1;
        while (t != NULL) { imprimirFila(t, i++); t = t->sig; }
        cout << "  +--------------------------------------------+\n";
        cout << "  Total: "   << total
             << "  |  Primero: \"" << primero->titulo
             << "\"  |  Ultimo: \"" << ultimo->titulo << "\"\n\n";
    }

    // Consultar extremos
    void consultarExtremos() {
        if (!primero) { cout << "  Lista vacia.\n"; return; }
        cout << "  Primero: " << primero->toString() << "\n";
        cout << "  Ultimo:  " << ultimo->toString()  << "\n";
    }

    int    getTotal()   { return total; }
    string getPrimero() { return primero ? primero->titulo : "ninguna"; }
};


//  Clase Reproductor  —  contiene la lista de canciones y el menu principal 

class Reproductor {
    Lista  lista;
    string nombre;

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

        do {
            cout << "\n  ========================================\n";
            cout << "   REPRODUCTOR (Lista Simple): " << nombre << "\n";
            cout << "   Canciones: " << lista.getTotal();
            cout << "  |  Primera: " << lista.getPrimero() << "\n";
            cout << "  ========================================\n";
            cout << "  1. Insertar cancion al inicio\n";
            cout << "  2. Insertar cancion al final\n";
            cout << "  3. Buscar cancion\n";
            cout << "  4. Eliminar cancion\n";
            cout << "  5. Mostrar lista\n";
            cout << "  6. Consultar primera y ultima\n";
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
                case 5: lista.mostrar();           break;
                case 6: lista.consultarExtremos(); break;
                case 0: cout << "  Hasta luego!\n"; break;
                default: cout << "  Opcion invalida.\n";
            }
        } while (op != 0);
    }
};


//  Main, llama al metodo menu() para poder interactuar con el reproductor de canciones
int main() {
    cout << "\n  ========================================\n";
    cout << "    REPRODUCTOR DE MUSICA - LISTA SIMPLE\n";
    cout << "  ========================================\n";
    Reproductor r("Mi Playlist");
    r.menu();
    return 0;
}
