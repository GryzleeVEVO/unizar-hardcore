#pragma once

// Interfaz del TAD pila
// Implementación estática

// Límite tamaño de la agrupación
const int MAX = 40;

// Pre-declaraciones

template<typename T> struct static_stack;

// Operaciones sobre la pila
template<typename T> void init(static_stack<T>& s);
template<typename T> bool push(static_stack<T>& s, const T& p);
template<typename T> bool pop(static_stack<T>& s);
// Iterador
template<typename T> void startIterator(static_stack<T>& s);
template<typename T> bool hasNext(const static_stack<T>& s);
template<typename T> T next (static_stack<T>& s, bool& err);

// Declaración:
template<typename T>
struct static_stack
{
	// Representación interna del tipo
private:
	T	datos[MAX];
	int total;
	int iterador;

	// Permisos de acceso
	friend void init<T> (static_stack<T>& s);
	friend bool push<T> (static_stack<T>& s, const T& p);
	friend bool pop<T> (static_stack<T>& s);

	friend void startIterator<T> (static_stack<T>& s);
	friend bool hasNext<T> (const static_stack<T>& s);
	friend T next<T> (static_stack<T>& s, bool& err);
};

// Implementación de las operaciones

template<typename T>
void init(static_stack<T>& s)
{
	s.total = 0;
}

template<typename T>
bool push(static_stack<T>& s, const T& e)
{
	bool sePuede = s.total < MAX;

	if (sePuede)
	{
		s.datos[s.total] = e;
		s.total++;
	}

	return sePuede;
}

template<typename T>
bool pop(static_stack<T>& s)
{
	bool sePuede = s.total > 0;
	if (sePuede) s.total--;

	return sePuede;
}

template<typename T>
void startIterator(static_stack<T>& s)
{
	s.iterador = (s.total - 1);
}

template<typename T>
bool hasNext(const static_stack<T>& s)
{
	return s.iterador >= 0;
}

template<typename T>
T next(static_stack<T>& s, bool& err)
{
	err = (s.iterador < 0);
	return s.datos[s.iterador--];
}
