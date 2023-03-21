//
// Procesadores de lenguajes
// Práctica 2
//
// Fichero: test1.cl
//
// Autores:
//         Wozniak, Dorian Boleslaw  (817570@unizar.es)
//         Jaime Royo, Oscar    (818961@unizar.es)
//
//  Descripción: Programa de test que prueba el correcto funcionamiento de
//  las sentencias condicionales y de bucles para el compilador clike,
//  así como el reconocimiento de guardas.
//
//  También prueba que se reconozcan las funciones integradas.
//
//  El programa está hecho para probar el análisis sintáctico del compilador,
//  no para generar un programa util

int func()
{
    return 1;
}

void main()
{

    int a, b;
    bool c, d;
    char e;

    a = 1 + 2;
    b = 4 * 3;

    c = b % 2 == 0;
    d = c && false;

    e = int2char(a);

    if (a > b)
    {
        b = 6 * 5 + 3 % 2;
    }
    // Se prueban condiciones elaboradas
    else
    {
        if (e == 'B' && c)
        {
            while (!d || (a - 1) < b)
            {
                a = a + 1;
                e = int2char(a);
            }
        }
        else
        {
            if (c && func() == 1)
            {
                b = char2int(e);
            }
            else
            {
                d = false;
            }
        }
    }

    while (e == 'B' && c)
    {
        if (!d || (a - 1) < b)
        {
            a = a + 1;
            e = int2char(a);
        }
        else
        {
            if (c && func() == 1)
            {
                b = char2int(e);
            }
            else
            {
                if (a > b)
                {
                    b = 6 * 5 + 3 % 2;
                }
                else
                {
                    d = false;
                    a = 1;
                    b = 1;
                }
            }
        }
    }
}