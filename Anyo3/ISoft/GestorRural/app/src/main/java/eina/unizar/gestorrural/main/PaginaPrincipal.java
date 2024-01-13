package eina.unizar.gestorrural.main;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

import eina.unizar.gestorrural.R;
import eina.unizar.gestorrural.habitaciones.MenuHabitaciones;
import eina.unizar.gestorrural.reservas.MenuReservas;

/**
 * La clase implementa una actividad para la página principal del gestor de hoteles rurales.
 * Consiste en una simple pantalla de bienvenida que permite acceder a las dos secciones de la
 * aplicación: al gestor de habitaciones y al gestor de reservas.
 *
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Sergio Martinez Lahoz (801621@unizar.es)
 * */
public class PaginaPrincipal extends AppCompatActivity {

    /**
     * Implementación del comportamiento de la actividad PaginaPrincipal al ser invocada.
     *
     * @param savedInstanceState No utilizado salvo para invocar constructor de clase padre
     * */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Inicializa elementos visuales
        setContentView(R.layout.activity_pagina_principal);
        setTitle(R.string.title_pagina_principal);
    }

    /**
     * Comportamiento del botón "Gestionar habitación" al ser pulsado.
     * Al ser pulsado el botón, inicia la actividad "Gestionar habitaciones".
     *
     * @param view No utilizado
     * @see View
     * */
    public void actividadMenuHabitaciones(View view) {
        startActivity(new Intent(this, MenuHabitaciones.class));
    }

    /**
     * Comportamiento del botón "Gestionar reservas" al ser pulsado.
     * Al ser pulsado el botón, inicia la actividad "Gestionar reservas".
     *
     * @param view No utilizado
     * @see View
     * */
    public void actividadMenuReservas(View view) {
        startActivity(new Intent(this, MenuReservas.class));
    }
}
