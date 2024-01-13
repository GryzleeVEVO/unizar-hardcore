package eina.unizar.gestorrural.habitaciones;

import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.view.ContextMenu;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import java.util.Objects;

import eina.unizar.gestorrural.R;

/**
 * La clase implementa una actividad para el menú con el listado de habitaciones existentes.
 * Se puede añadir una nueva habitación o reordenar la lista con el botón de ajuste en la esquina
 * superior derecha.
 * Se puede consultar los datos de una habitación al pulsar sobre una de las habitaciones
 * disponibles en la lista.
 * Se puede modificar o eliminar una habitación manteniendo pulsado el elemento de la lísta.
 *
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Sergio Martinez Lahoz (801621@unizar.es)
 * */
public class MenuHabitaciones extends AppCompatActivity {

    /**
     * Adaptador para realizar operaciones en la base de datos sobre habitaciones
     * @see HabitacionDbAdapter
     * */
    private HabitacionDbAdapter mDbHelper;

    /**
     * Elemento visual que contiene las habitaciones disponibles en forma de lista
     * @see ListView
     * */
    private ListView mList;

    /** Referencia a la opción "Añadir habitación" en un menú */
    private static final int INSERT_ID = Menu.FIRST;
    /** Referencia a la opción "Eliminar habitación" en un menú */
    private static final int DELETE_ID = Menu.FIRST + 1;
    /** Referencia a la opción "Modificar habitación" en un menú */
    private static final int EDIT_ID = Menu.FIRST + 2;
    /** Referencia a la opción "Ordenar por nombre" en un menú */
    private static final int ORDER_BY_NAME_ID = Menu.FIRST + 3;
    /** Referencia a la opción "Ordenar por ocupantes" en un menú */
    private static final int ORDER_BY_OCCUPANCY = Menu.FIRST + 4;
    /** Referencia a la opción "Ordenar por precio" en un menú */
    private static final int ORDER_BY_PRICE_ID = Menu.FIRST + 5;

    /** Referencia al código de la actividad "Crear Habitación" */
    private static final int ACTIVITY_CREATE = 0;
    /** Referencia al código de la actividad "Modificar Habitación" */
    private static final int ACTIVITY_EDIT = 1;
    /** Referencia al código de la actividad "Consultar Habitación "*/
    private static final int ACTIVITY_CONSULTAR = 2;

    /** Contiene el método de ordenación de resultas de la base de datos para la lista
     * @see MenuHabitaciones#mList
     * @see MenuHabitaciones#mDbHelper
     * */
    private static String ORDER = HabitacionDbAdapter.KEY_NOMBRE;

    /**
     * Implementación del comportamiento de la actividad MenuHabitaciones al ser invocada.
     *
     * @param savedInstanceState Contiene el estado de la actividad anterior
     * */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Inicializa elementos visuales
        setContentView(R.layout.activity_menu_lista);
        setTitle(R.string.title_menu_habitaciones);
        Objects.requireNonNull(getSupportActionBar()).setDisplayHomeAsUpEnabled(true); // Botón de marcha atrás
        mList = findViewById(R.id.list);
        TextView empty = findViewById(R.id.empty);
        empty.setText(R.string.menu_habitaciones_none);
        mList.setEmptyView(empty);

        // Abre la base de datos
        mDbHelper = new HabitacionDbAdapter(this);
        mDbHelper.open();

        // Rellena la lista con las habitaciones disponibles
        fillData();

        // Activa menú contextual al mantener pulsado un elemento de la lista
        registerForContextMenu(mList);

        // Comportamiento al pulsar un elemento de la lista
        mList.setClickable(true);
        mList.setOnItemClickListener((adapterView, view, i, l) -> consultarHabitacion(l));

    }

    /**
     * Inicia un elemento menú en la parte superior derecha de la pantalla que contendrá las
     * opciones para añadir una nueva habitación, o para ordenar la lista de habitaciones por
     * nombre, ocupantes o precio.
     *
     * @param menu Elemento menú sobre el que actuar
     * @return Menú creado
     * */
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        boolean res = super.onCreateOptionsMenu(menu);

        // Añade los elementos del menú
        menu.add(Menu.NONE, INSERT_ID, Menu.NONE, R.string.title_habitacion_insert);
        menu.add(Menu.NONE, ORDER_BY_NAME_ID, Menu.NONE, R.string.menu_habitacion_order_name);
        menu.add(Menu.NONE, ORDER_BY_OCCUPANCY, Menu.NONE, R.string.menu_habitacion_order_occupancy);
        menu.add(Menu.NONE, ORDER_BY_PRICE_ID, Menu.NONE, R.string.menu_habitacion_order_price);

        return res;
    }


    /**
     * Implementa el comportamiento del menú de opciones dependiendo de la opción que se haya
     * seleccioado.
     *
     * @param item Elemento seleccionado del menú
     * @return Opción realizada correctamente
     * @see MenuHabitaciones#onCreateOptionsMenu(Menu)
     * */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case INSERT_ID: // Añadir habitación
                createHabitacion();
                return true;
            case ORDER_BY_NAME_ID: // Ordenar por nombre
                ORDER = HabitacionDbAdapter.KEY_NOMBRE;
                fillData();
                return true;
            case ORDER_BY_OCCUPANCY: // Ordenar por número de ocupantes
                ORDER = HabitacionDbAdapter.KEY_MAXOCUPANTES;
                fillData();
                return true;
            case ORDER_BY_PRICE_ID: // Ordenar por precio
                ORDER = HabitacionDbAdapter.KEY_PRECIO;
                fillData();
                return true;
            case android.R.id.home: // Botón marcha atrás
                finish();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    /**
     * Inicia un elemento menú contextual que aparecerá al mantener pulsado un elemento de la lista
     * y que contendrá las opciones para modificar o eliminar la habitación seleccionada.
     *
     * @param menu Elemento menú sobre el que actuar
     * @param v No usado salvo para invocar constructor de clase padre
     * @param menuInfo No usado salvo para invocar constructor de clase padre
     * */
    @Override
    public void onCreateContextMenu(ContextMenu menu, View v,
                                    ContextMenu.ContextMenuInfo menuInfo) {
        super.onCreateContextMenu(menu, v, menuInfo);
        menu.add(Menu.NONE, DELETE_ID, Menu.NONE, R.string.menu_habitacion_delete);
        menu.add(Menu.NONE, EDIT_ID, Menu.NONE, R.string.title_habitacion_edit);
    }

    /**
     * Implementa el comportamiento del menú contextual dependiendo de la opción que se haya
     * seleccionado.
     *
     * @param item Elemento seleciconado del menú
     * @return Opción seleccionada correctamente
     * @see MenuHabitaciones#onCreateContextMenu(ContextMenu, View, ContextMenu.ContextMenuInfo)
     * */
    @Override
    public boolean onContextItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case DELETE_ID: // Eliminar habitación
                AdapterView.AdapterContextMenuInfo info =
                        (AdapterView.AdapterContextMenuInfo) item.getMenuInfo();
                mDbHelper.deleteHabitacion(info.id);
                fillData();
                return true;
            case EDIT_ID: // Modificar habitación
                info = (AdapterView.AdapterContextMenuInfo) item.getMenuInfo();
                editHabitacion(info.id);
        }

        return super.onContextItemSelected(item);
    }

    /**
     * Al acabar una actividad invocada por esta clase, vuelve a llenar la lista de habitaciones
     * aplicando cualquier cambio en las habitaciones contenidas.
     *
     * @param requestCode Código de actividad invocada
     * @param resultCode Respuesta de actividad invocada
     * @param intent Contiene la actividad que ha sido realizada
     * @see Intent
     * */
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent intent) {
        super.onActivityResult(requestCode, resultCode, intent);
        fillData();
    }

    /**
     * Inicia la actividad EditarHabitación en modo "Añadir habitación" y espera a un resultado.
     *
     * */
    protected void createHabitacion() {
        Intent i = new Intent(this, EditarHabitacion.class);
        startActivityForResult(i, ACTIVITY_CREATE);
    }

    /**
     * Inicia la actividad EditarHabitación en modo "Modificar habitación" y espera a un resultado.
     *
     * @param id Clave principal de la habitación a modificar
     * */
    protected void editHabitacion(long id) {
        Intent i = new Intent(this, EditarHabitacion.class);
        i.putExtra(HabitacionDbAdapter.KEY_ROWID, id);
        startActivityForResult(i, ACTIVITY_EDIT);
    }

    /**
     * Inicia la actividad Consultar Habitación y espera a un resultado.
     *
     * @param id Clave principal de la habitación a consultar
     * */
    protected void consultarHabitacion(long id) {
        Intent i = new Intent(this, ConsultarHabitacion.class);
        i.putExtra(HabitacionDbAdapter.KEY_ROWID, id);
        startActivityForResult(i, ACTIVITY_CONSULTAR);
    }

    /**
     * Rellena la lista de habitaciones con datos obtenidos de la base de datos.
     *
     * @see ListView
     * @see ListView#setAdapter(ListAdapter)
     * @see HabitacionDbAdapter#fecthAllHabitaciones(String)
     * @see MenuHabitaciones#ORDER
     * */
    private void fillData() {
        Cursor c = mDbHelper.fecthAllHabitaciones(ORDER);

        String[] from = new String[] { HabitacionDbAdapter.KEY_NOMBRE };
        int[] to = new int[] { R.id.list_row_text };

        mList.setAdapter(new SimpleCursorAdapter(this, R.layout.list_row, c, from, to));
    }
}