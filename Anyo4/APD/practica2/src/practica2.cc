/**
 * @file practica2.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Alvaro Seral Gracia (819425@unizar.es)
 * @brief Programa que recibe, a través de un fichero, un cuadrado latino de
 * tamaño arbitrario compuesto de filas y columnas de enteros. Si el cuadrado es
 * resoluble, trata de traducirlo a una fórmula en CNF usando el formato DIMACS
 * para ser resuelta por minisat, aplicando una reducción. La solución, si es
 * encontrada, se traduce a un cuadrado latino.
 */

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

const string BLANK_SQAUARE = "*";
const string MINISAT_INPUT_FILE = "square.dimacs";
const string MINISAT_OUTPUT_FILE = "square.sat";
const string MINISAT_EXEC = "minisat " + MINISAT_INPUT_FILE + " " + MINISAT_OUTPUT_FILE;

/******************************************************************************/
/* CUADRADOS LATINOS */

/**
 * @brief Representa un cuadrado latino en memoria
 */
class LatinSquare
{
private:
    int _size;     // Tamaño del cuadrado
    string *_data; // Datos del cuadrado

public:
    LatinSquare(int size)
    {
        _size = size;
        _data = new string[_size * _size];
    }

    LatinSquare(const string &path, int size) : LatinSquare(size)
    {
        read(path);
    }

    ~LatinSquare()
    {
        delete[] _data;
    }

    int size() const
    {
        return _size;
    }

    string &operator()(int row, int column)
    {
        return _data[row * _size + column];
    }

    string operator()(int row, int column) const
    {
        return _data[row * _size + column];
    }

    /**
     * @brief Lee un cuadrado latino a partir de un fichero
     *
     * @param path Ruta del fichero
     */
    void read(const string &path)
    {
        ifstream in(path);

        if (in.is_open())
        {
            string s;

            for (int i = 0; i < _size * _size; i++)
            {
                if (in >> s)
                {
                    _data[i] = s;
                }
                else
                {
                    cerr << "El fichero se ha acabado antes de tiempo" << endl;
                    exit(EXIT_FAILURE);
                }
            }

            in.close();
        }
        else
        {
            cerr << "No se ha podido abrir el fichero " << path << endl;
            exit(EXIT_FAILURE);
        }
    }

    /**
     * @brief Escribe el cuadrado latino en un fichero
     *
     * @param path Ruta del fichero
     */
    void write(const string &path) const
    {
        ofstream out(path);

        if (out.is_open())
        {
            for (int i = 0; i < _size * _size; i++)
            {
                out << _data[i] << ((i + 1) % _size == 0 ? "\n" : " ");
            }

            out.close();
        }
        else
        {
            cerr << "No se ha podido crear el fichero" << path << endl;
            exit(EXIT_FAILURE);
        }
    }

    /**
     * @brief  Comprueba que el cuadrado latino es correcto.
     *
     * @param no_blanks Contar las casillas vacías como incorrectas
     * @return El cuadrado es correcto
     */
    bool is_correct(bool no_blanks) const
    {
        // Comprueba que las filas no contengan etiquetas repetidas
        for (int row = 0; row < _size; row++)
        {
            vector<string> row_values = {};

            for (int column = 0; column < _size; column++)
            {
                if (_data[row * _size + column] != BLANK_SQAUARE)
                {
                    if (find(row_values.begin(), row_values.end(),
                             _data[row * _size + column]) != row_values.end())
                    {
                        cerr << "La fila " << row << " contiene etiquetas repetidas" << endl;
                        return false;
                    }
                    else
                    {
                        row_values.push_back(_data[row * _size + column]);
                    }
                }
                else if (no_blanks)
                {
                    cerr << "La fila " << row << " contiene casillas vacías" << endl;
                    return false;
                }
            }
        }

        // Comprueba que las columnas no contengan etiquetas repetidas
        for (int column = 0; column < _size; column++)
        {
            vector<string> column_values = {};

            for (int row = 0; row < _size; row++)
            {
                if (_data[row * _size + column] != BLANK_SQAUARE)
                {
                    if (find(column_values.begin(), column_values.end(),
                             _data[row * _size + column]) != column_values.end())
                    {
                        cerr << "La columna " << column << " contiene etiquetas repetidas" << endl;
                        return false;
                    }
                    else
                    {
                        column_values.push_back(_data[row * _size + column]);
                    }
                }
                else if (no_blanks)
                {
                    cerr << "La columna " << column << " contiene casillas vacías" << endl;
                    return false;
                }
            }
        }

        return true;
    }
};

/******************************************************************************/
/* CNF */

/**
 * @brief Representa una fórmula en CNF
 */
class CNF
{
private:
    int variables;          // Número de variables
    vector<string> clauses; // Lista de cláusulas

public:
    /**
     * @brief Genera una fórmula CNF nueva a partir de un cuadrado latino
     *
     * Adaptado de https://users.aalto.fi/~tjunttil/2020-DP-AUT/notes-sat/solving.html
     * @param sqr Cuadrado latino
     */
    CNF(const LatinSquare &sqr)
    {
        variables = sqr.size() * sqr.size() * sqr.size();
        int n = sqr.size();

        // Cada casilla del cuadrado contiene tiene algún valor
        // ⋀ (1≤r≤n, 1≤c≤n) (x(r,c,1) ∨ x(r,c,2) ∨…∨ x(r,c,n))
        for (int row = 1; row <= n; row++)
        {
            for (int column = 1; column <= n; column++)
            {
                string clause = "";

                for (int value = 1; value <= n; value++)
                {

                    clause += to_string(row * n * n + column * n + value) +
                              ((value + 1 <= n) ? " " : "");
                }

                clauses.push_back(clause);
            }
        }

        // Cada casilla del cuadrado contiene un solo valor
        // ⋀ (1≤r≤n, 1≤c≤n, 1≤v<v≤n) (¬x(r,c,v) ∨ ¬x(r,c,v′)
        for (int row = 1; row <= n; row++)
        {
            for (int column = 1; column <= n; column++)
            {
                for (int value = 1; value <= n; value++)
                {
                    for (int next_value = value + 1; next_value <= n; next_value++)
                    {
                        string clause = "-" + (to_string(row * n * n + column * n + value)) +
                                        " -" + (to_string(row * n * n + column * n + next_value));

                        clauses.push_back(clause);
                    }
                }
            }
        }

        // Cada fila contiene todos los valores posibles
        // ⋀ (1≤r≤n,1≤v≤n) (x(r,1,v) ∨ x(r,2,v) ∨…∨ x(r,n,v))
        for (int row = 1; row <= n; row++)
        {
            for (int value = 1; value <= n; value++)
            {
                string clause = "";

                for (int column = 1; column <= n; column++)
                {
                    clause += to_string(row * n * n + column * n + value) + ((column + 1 <= n) ? " " : "");
                }

                clauses.push_back(clause);
            }
        }

        // Cada columna contiene todos los valores posibles
        // ⋀(1≤c≤n,1≤v≤n) (x(1,c,v) ∨ x(2,c,v) ∨…∨ x(n,c,v))
        for (int column = 1; column <= n; column++)
        {
            for (int value = 1; value <= n; value++)
            {
                string clause = "";

                for (int row = 1; row <= n; row++)
                {
                    clause += to_string(row * n * n + column * n + value) +
                              ((row + 1 <= n) ? " " : "");
                }

                clauses.push_back(clause);
            }
        }

        // La solución contiene los valores del cuadrado ya conocidos
        // ⋀ ((r,c,v) ∈ H) (x(r,c,v))
        for (int row = 0; row < n; row++)
        {
            for (int column = 0; column < n; column++)
            {
                if (sqr(row, column) != "*")
                {
                    int value = stoi(sqr(row, column));
                    string clause = to_string((row + 1) * n * n + (column + 1) * n + value);

                    clauses.push_back(clause);
                }
            }
        }
    }

    /**
     * @brief Escribe la fórmula en formato DIMACS
     *
     * Formato: https://jix.github.io/varisat/manual/0.2.0/formats/dimacs.html
     * @param path Ruta del fichero
     */
    void write_dimacs(const string &path)
    {
        ofstream out(path);

        if (out.is_open())
        {
            out << "p cnf " << variables << " " << clauses.size() << endl;

            for (const string &clause : clauses)
            {
                out << clause << " 0" << endl;
            }
        }
        else
        {
            cerr << "No se ha podido generar el fichero DIMACS " << path << endl;
            exit(EXIT_FAILURE);
        }
    }

    /**
     * @brief Lee la solución de minisat y la traduce a un cuadrado latino
     *
     * @param path Ruta del fichero
     * @param n Tamaño del cuadrado
     * @return Cuadrado latino solución
     */
    LatinSquare read_sat_result(const string &path, int n)
    {
        LatinSquare res(n);
        ifstream in(path);

        if (in.is_open())
        {
            string s;

            while (in >> s)
            {
                if (s != "SAT")
                {
                    int id = stoi(s);

                    if (id > 0)
                    {
                        int row = (id - 1) / (n * n);
                        int column = ((id - 1) / n) % n;
                        int value = (id - 1) % n;

                        res(row - 1, column - 1) = to_string(value + 1);
                    }
                }
            }
        }
        else
        {
            cerr << "No se ha podido leer el fichero de salida de minisat" << endl;
            exit(EXIT_FAILURE);
        }

        return res;
    }
};

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "Faltan argumentos" << endl;
        cerr << "Uso: " << argv[0] << " <tamaño del cuadrado> <fichero_entrada> [fichero_salida]" << endl;
        return EXIT_FAILURE;
    }

    int n = stoi(argv[1]);

    if (n < 2)
    {
        cerr << "El tamaño del cuadrado debe ser 2 o mayor" << endl;
        return EXIT_FAILURE;
    }

    string input = argv[2], output = (argc > 3) ? argv[3] : "";

    // Lee el cuadrado y comprueba que es resoluble
    LatinSquare square(input, n);

    if (!square.is_correct(false))
    {
        cerr << "El cuadrado no es resoluble" << endl;
        return EXIT_FAILURE;
    }

    CNF cnf(square);
    cnf.write_dimacs(MINISAT_INPUT_FILE);

    // Trata de solucionarlo con minisat
    int status = system(MINISAT_EXEC.c_str());
    int exit_code = WEXITSTATUS(status);

    if (!(exit_code == 0 || exit_code == 10))
    {
        cerr << "AVISO: No se ha podido ejecutar minisat, código salida " << exit_code << endl;
    }

    LatinSquare res = cnf.read_sat_result(MINISAT_OUTPUT_FILE, n);

    if (!res.is_correct(true))
    {
        cerr << "La solución no es válida" << endl;
        return EXIT_FAILURE;
    }

    if (!output.empty())
        res.write(output);

    return EXIT_SUCCESS;
}