/**
 * @file static-stack.h
 * @author Dorian Boleslaw Wozniak	817570
 * @brief 	Practica 1 Tecnología de Programación
 * 
 * 			Implementación de una clase static_stack, que representa una pila
 * 			dinámica que implementa operaciones push, pop y un iterador
 * 
 * 			No se han modificado los comentarios
 * @version 1
 * @date 2022-02-25
 */

#pragma once

// Interfaz del TAD pila

template<typename T>
class static_stack
{
private:
	//Límite tamaño de la agrupación, en esta implementación.
	static const int MAX = 40;

	// Mantenemos aqui los mismos atributos privados, ignorando aquellos que
	// tengan que ver con el iterador
	T	datos[MAX];
	int total;

public:
	// Ahora la funcion iniciar de la estructura es el constructor.
	// TODO: Invoca a los constructores de los miembros privados (o deja
	// que se construyan por defecto si lo consideras asi).
	// Separados por comas (si no sabes cómo hacerlo, mira cómo está hecho
	// en la clase 'iterator')
	static_stack() : total(0)
	{
		// Si invocas a los constructores de los miembros privados no necesitas
		// rellenar el código del constructor.
	}

	// TODO: La funcion push ahora es el metodo push. Rellénalo.
	// Para acceder a atributos
	// y métodos de la propia clase, puedes hacerlo a través del puntero this->
	bool push(const T& e)
	{
		bool notFull = total < MAX;
		if(notFull) { datos[total] = e; total++; }
		return notFull;
	}

	// TODO: La funcion pop se transforma tambien en un metodo. Rellénalo.
	bool pop()
	{
		bool notEmpty = total > 0;
		if(notEmpty) { total--; }
		return notEmpty;
	}

	// Declarando la clase iterator como friend y
	// como clase local, conseguimos que pueda haber varios iteradores,
	// cada uno de ellos llevando su propio indice, indicando en qué posición
	// de la estructura estamos.
	friend class iterator;

	class iterator
	{
	private:
		// Al separar el iterador en una clase aparte, trasladamos los miembros
		// privados que estaban en el struct a esta clase.
		// Además, mantenemos una referencia a la pila.
		const static_stack<T>& stk;
		int i;
	public:
		// Este constructor sirve como inicializador del iterador,
		// tanto al principio como al final.
		iterator(const static_stack& stk_, int i_) : stk(stk_), i(i_)
		{ }

		// En la definición por defecto de los iteradores, se separa el avance
		// del iterador de la obtención del elemento apuntado por el iterador
		// (que en la otra implementación ocurren en la misma función).

		// Este método redefine el operador de pre-incremento (++x).
		// Representa el avance del iterador.
		iterator& operator++()
		{
			// TODO: Rellena este hueco para que el iterador sobre la agrupación
			// avance. Recuerda que en nuestra definición de agrupación la
			// estructura se recorre desde el último elemento introducido hasta
			// el primero (como si se tratara de una pila).
			i = i - 1; return (*this);
		}

		// Este método redefine el operador de "apuntado" (*x)
		// Representa la obtención de lo apuntado por el iterador.
		const T& operator*() const
		{
			// TODO: Rellena este método para que devuelva el elemento T al que
			// está apuntando el iterador.
			return stk.datos[i];
		}

		// En la definición por defecto de los iteradores, no existe la
		// comprobación de si existe siguiente elemento.
		// Por defecto las estructuras de datos devuelven iteradores al
		// principio y al final,
		// Para recorrer la estructura se compara el iterador que avanza con
		// el iterador que apunta al final de la estructura.
		bool operator!=(const iterator& that) const
		{
			// TODO: Rellenar este método para devuelva true si este iterador y
			// el iterador "that" apuntan a sitios diferentes,
			// y false en caso contrario.

			return i != that.i;
		}

	};

	// Observa como nuestra pila permite generar dos iteradores:
	// uno al principio y otro al final.
	// El iterador que apunta al final de la agrupación se mantiene en este caso
	// únicamente para hacer la comprobación de que "existe siguiente".
	// Date cuenta que los valores que le pasamos como índice del iterador son
	// para que se recorra la estructura
	// desde el último elemento (this->total - 1) hasta el primero (0).
	iterator begin() const
	{
		return iterator(*this,this->total-1);
	}
	iterator end() const
	{
		return iterator(*this,-1);
	}
};
