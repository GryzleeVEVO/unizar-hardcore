#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <chrono>
#include <vector>
#include <numeric>
#include <stack>
#include <algorithm>

using namespace std;

const int MIN_N = 3, MAX_N = 900;

// Generador de números aleatorios
mt19937 generator(chrono::system_clock::now().time_since_epoch().count());

class Grafo
{
private:
    int _vertices;
    int _aristas;

    vector<int> _tiempo;
    vector<double> _probabilidad;

public:
    Grafo()
        : _vertices(0), _aristas(0) {}

    Grafo(int vertices, int aristas)
        : _vertices(vertices), _aristas(aristas)
    {
        _tiempo = vector<int>(_vertices * _vertices, 0);
        _probabilidad = vector<double>(_vertices * _vertices, 0.0);
    }

    int vertices() const
    {
        return _vertices;
    }

    int aristas() const
    {
        return _aristas;
    }

    int tiempo(int origen, int destino) const
    {
        return _tiempo[origen * _vertices + destino];
    }

    int &tiempo(int origen, int destino)
    {
        return _tiempo[origen * _vertices + destino];
    }

    int probabilidad(int origen, int destino) const
    {
        return _probabilidad[origen * _vertices + destino];
    }

    double &probabilidad(int origen, int destino)
    {
        return _probabilidad[origen * _vertices + destino];
    }
};

bool casa_es_alcanzable(Grafo g, int origen, int destino)
{
    vector<bool> visitado(g.vertices(), false);

    if (origen == destino)
        return true;

    stack<int> s;
    s.push(origen);

    while (!s.empty())
    {
        int current = s.top();
        s.pop();

        if (!visitado[current])
        {
            visitado[current] = true;

            for (int i = 0; i < g.vertices(); i++)
            {
                if (i != current && g.probabilidad(current, i) > 0.0 && !visitado[i])
                {
                    if (i == destino)
                        return true;

                    s.push(i);
                }
            }
        }
    }

    return false;
}

int simular(Grafo g, int origen, int destino)
{
    uniform_real_distribution<double> distribution(0.0, 1.0);

    int actual = origen;
    int tiempo = 0;

    while (actual != destino)
    {
        double rand = distribution(generator),
               acum = 0.0;

        // Recorre los vertices de salida posibles
        for (int j = 0; j < g.vertices(); j++)
        {
            if (j != actual && g.probabilidad(actual, j) > 0.0)
            {
                acum += g.probabilidad(actual, j);

                // Encuentra el vertice de salida elegido
                if (rand <= acum)
                {
                    tiempo += g.tiempo(actual, j);
                    actual = j;
                    break;
                }
            }
        }
    }

    return tiempo;
}

struct ConfidenceInterval
{
    double left;
    double right;
};

ConfidenceInterval bootstrap(vector<double> times, int iterations, double confidence)
{
    uniform_int_distribution<int> distribution(0, times.size() - 1);

    // Vector de medias
    vector<double> bootstrap_means(iterations, 0);

    for (int i = 0; i < iterations; i++)
    {
        double mean = 0.0;

        // Escoge n muestras con reemplazamiento de un vector de n elementos
        for (int j = 0; j < (int)times.size(); j++)
        {
            mean += times[distribution(generator)];
        }

        bootstrap_means[i] = mean / times.size();
    }

    // Ordena el vector de medias de menor a mayor
    sort(bootstrap_means.begin(), bootstrap_means.end());

    // Calcula los percentiles
    return {bootstrap_means[iterations * (1 - confidence) / 2],
            bootstrap_means[iterations * (1 + confidence) / 2]};
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "Sintaxis: " << argv[0] << " <fichero_entrada>" << endl;
        exit(EXIT_FAILURE);
    }

    string path = argv[1];

    ifstream in(path);
    Grafo g;

    int C, A, B;

    // Lee el fichero
    if (in.is_open())
    {
        // Lee la entrada
        int N, M;
        in >> N >> M >> C >> A >> B;

        // Comprueba restricciones
        if (N < MIN_N || N > MAX_N)
        {
            cerr << "[ERROR] El número de intersecciones debe encontrarse en el rango  [" << MIN_N << ", " << MAX_N << "], pero se ha introducido " << N << endl;
            exit(EXIT_FAILURE);
        }

        if (M < N / 2 || M > N * (N - 1) / 2)
        {
            cerr << "[ERROR] El número de carreteras debe encontrarse en el rango [" << N / 2 << ", " << N * (N - 1) / 2 << "], pero se ha introducido " << M << endl;
            exit(EXIT_FAILURE);
        }

        if (C < 0 || C >= N)
        {
            cerr << "[ERROR] La intersección destino debe encontrarse en el rango [0, " << N << "), pero se ha introducido " << C << endl;
            exit(EXIT_FAILURE);
        }

        if (A < 0 || A >= N)
        {
            cerr << "[ERROR] El almacen A debe encontrarse en el rango [0, " << N << "), pero se ha introducido " << A << endl;
            exit(EXIT_FAILURE);
        }

        if (B < 0 || B >= N)
        {
            cerr << "[ERROR] El almacen B debe encontrarse en el rango [0, " << N << "), pero se ha introducido " << B << endl;
            exit(EXIT_FAILURE);
        }

        g = Grafo(N, M);

        // Obtiene aristas
        int u, v, t_uv;
        double p_uv, p_vu;

        for (int i = 1; i <= M; i++)
        {
            if (in.eof())
            {
                cerr << "[ERROR] Se han introducido menos carreteras de las especificadas" << endl;
                exit(EXIT_FAILURE);
            }

            in >> u >> v >> t_uv >> p_uv >> p_vu;

            if (u < 0 || u >= N)
            {
                cerr << "[ERROR] Carretera " << i << ": La intersección 1 debe encontrarse en el rango [0, " << N << "), pero se ha introducido " << u << endl;
                exit(EXIT_FAILURE);
            }

            if (v < 0 || v >= N)
            {
                cerr << "[ERROR] Carretera " << i << ": La intersección 2 debe encontrarse en el rango [0, " << N << "), pero se ha introducido " << v << endl;
                exit(EXIT_FAILURE);
            }

            if (u == v)
            {
                cerr << "[ERROR] Carretera " << i << ": Las intersecciones de la carretera deben ser diferentes" << endl;
                exit(EXIT_FAILURE);
            }

            if (t_uv < 0)
            {
                cerr << "[ERROR] Carretera " << i << ": El tiempo de la carretera debe ser positivo, pero se ha introducido " << t_uv << endl;
                exit(EXIT_FAILURE);
            }

            if (u == C && p_uv != 0.0)
            {
                cerr << "[ERROR] Carretera " << i << ": La probabilidad de salir de la intersección de entrega debe ser 0, pero se ha introducido " << p_uv << endl;
                exit(EXIT_FAILURE);
            }
            else if (u != C && (p_uv <= 0 || p_uv > 1))
            {
                cerr << "[ERROR] Carretera " << i << ": La probabilidad de ir de " << u << " a " << v << " se debe encontrar en el rango (0, 1], pero se ha introducido " << p_uv << endl;
                exit(EXIT_FAILURE);
            }

            if (v == C && p_vu != 0.0)
            {
                cerr << "[ERROR] Carretera " << i << ": La probabilidad de salir de la intersección de entrega debe ser 0, pero se ha introducido " << p_vu << endl;
                exit(EXIT_FAILURE);
            }
            else if (v != C && (p_vu <= 0 || p_vu > 1))
            {
                cerr << "[ERROR] Carretera " << i << ": La probabilidad de ir de " << v << " a " << u << " se debe encontrar en el rango (0, 1], pero se ha introducido " << p_vu << endl;
                exit(EXIT_FAILURE);
            }

            g.tiempo(u, v) = t_uv;
            g.tiempo(v, u) = t_uv;
            g.probabilidad(u, v) = p_uv;
            g.probabilidad(v, u) = p_vu;
        }

        /*
        if (!in.eof())
        {
            cerr << "[ERROR] Se han introducido más carreteras de las especificadas" << endl;
            exit(EXIT_FAILURE);
        }
        */

        for (int i = 0; i < N; i++)
        {
            double sum = 0.0;

            if (i == C)
                continue;

            for (int j = 0; j < N; j++)
            {
                sum += g.probabilidad(i, j);
            }

            // Por precision de decimales a veces no da 1 exacto, con que este
            // en un rango de confianza se considera correcto
            if (abs(1.0 - sum) > 0.01)
            {
                cerr << "[ERROR] La suma de las probabilidades de salir de la intersección " << i << " debe ser 1, pero se ha obtenido " << fixed << setprecision(12) << sum << endl;

                exit(EXIT_FAILURE);
            }
        }
    }
    else
    {
        cerr << "[ERROR] No se ha podido abrir el fichero " << path << endl;
        exit(EXIT_FAILURE);
    }

    // Se realizan una serie de simulaciones

    int simulations = 20;

    bool a_alcanzable = casa_es_alcanzable(g, A, C),
         b_alcanzable = casa_es_alcanzable(g, B, C);

    vector<double> times_a(simulations, 0);
    vector<double> times_b(simulations, 0);

    vector<double> run_a(simulations, 0);
    vector<double> run_b(simulations, 0);

    if (!a_alcanzable)
    {
        cout << "[AVISO] Almacen A: \"Intentamos entregar tu paquete, pero no estabas\"" << endl;
    }
    else
    {
        for (int i = 0; i < simulations; i++)
        {
            auto start = chrono::high_resolution_clock::now();
            times_a[i] = simular(g, A, C);
            auto end = chrono::high_resolution_clock::now();
            run_a[i] = chrono::duration_cast<chrono::microseconds>(end - start).count();
        }
    }

    if (!b_alcanzable)
    {
        cout << "[AVISO] Almacen B: \"Intentamos entregar tu paquete, pero no estabas\"" << endl;
    }
    else
    {
        for (int i = 0; i < simulations; i++)
        {
            auto start = chrono::high_resolution_clock::now();
            times_b[i] = simular(g, B, C);
            auto end = chrono::high_resolution_clock::now();
            run_b[i] = chrono::duration_cast<chrono::microseconds>(end - start).count();
        }
    }

    // Aplicar bootstrap para obtener intervalos de confianza
    if (a_alcanzable)
    {
        ConfidenceInterval interval_a = bootstrap(times_a, 500, 0.90);
        cout << "Media de tiempos de A: " << accumulate(times_a.begin(), times_a.end(), 0.0) / times_a.size() << endl;
        cout << "Intervalo de confianza para A: [" << interval_a.left << ", " << interval_a.right << "]" << endl;
        cout << "Tiempo medio de ejecución de A: " << fixed << setprecision(6) << accumulate(run_a.begin(), run_a.end(), 0.0) / run_a.size() << endl;
    }
    if (b_alcanzable)
    {
        ConfidenceInterval interval_b = bootstrap(times_b, 500, 0.90);
        cout << "Media de tiempos de B: " << fixed << setprecision(6) << accumulate(times_b.begin(), times_b.end(), 0.0) / times_b.size() << endl;
        cout << "Intervalo de confianza para B: [" << interval_b.left << ", " << interval_b.right << "]" << endl;
        cout << "Tiempo medio de ejecución de B: " << fixed << setprecision(6) << accumulate(run_b.begin(), run_b.end(), 0.0) / run_b.size() << endl;
    }

    return 0;
}