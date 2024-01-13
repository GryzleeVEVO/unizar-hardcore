package eina.unizar.gestorrural.reservas;

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

import eina.unizar.gestorrural.R;

/**
 * La clase implementa una actividad para el menú con el listado de reservas existentes.
 * Se puede añadir una nueva reserva o reordenar la lista con el botón de ajuste en la esquina
 * superior derecha.
 * Se puede consultar los datos de una reserva al pulsar sobre una de las habitaciones
 * disponibles en la lista.
 * Se puede modificar o eliminar una reserva manteniendo pulsado el elemento de la lísta.
 *
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Sergio Martinez Lahoz (801621@unizar.es)
 * */
public class MenuReservas extends AppCompatActivity {

    /**
     * Adaptador para realizar operaciones en la base de datos sobre reservas
     * @see ReservaDbAdapter
     * */
    private ReservaDbAdapter mDbHelper;

    /** Elemento visual que contiene las reservas disponibles en forma de lista */
    private ListView mList;

    /** Referencia a la opción "Añadir reserva" en un menú */
    private static final int INSERT_ID = Menu.FIRST;
    /** Referencia a la opción "Eliminar reserva" en un menú */
    private static final int DELETE_ID = Menu.FIRST + 1;
    /** Referencia a la opción "Modificar reserva" en un menú */
    private static final int EDIT_ID = Menu.FIRST + 2;
    /** Referencia a la opción "Ordenar por nombre" en un menú */
    private static final int ORDER_BY_NAME_ID = Menu.FIRST + 3;
    /** Referencia a la opción "Ordenar por telefono" en un menú */
    private static final int ORDER_BY_PHONE_NUM = Menu.FIRST + 4;
    /** Referencia a la opción "Ordenar por fecha" en un menú */
    private static final int ORDER_BY_DATE = Menu.FIRST + 5;

    /** Referencia al código de la actividad "Crear Reserva" */
    private static final int ACTIVITY_CREATE = 0;
    /** Referencia al código de la actividad "Modificar Reserva" */
    private static final int ACTIVITY_EDIT = 1;
    /** Referencia al código de la actividad "Consultar Reserva"*/
    private static final int ACTIVITY_CONSULTAR = 2;

    /** Contiene el método de ordenación de resultas de la base de datos para la lista
     * @see eina.unizar.gestorrural.reservas.MenuReservas#mList
     * @see eina.unizar.gestorrural.reservas.MenuReservas#mDbHelper
     * */
    private static String ORDER = ReservaDbAdapter.KEY_NOMCLIENTE;

    /**
     * Implementación del comportamiento de la actividad MenuReservas al ser invocada.
     *
     * @param savedInstanceState Contiene el estado de la actividad anterior
     * */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Inicializa elementos visuales
        setContentView(R.layout.activity_menu_lista);
        setTitle(R.string.title_menu_reservas);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true); // Botón de marcha atrás
        mList = findViewById(R.id.list);
        TextView empty = findViewById(R.id.empty);
        empty.setText(R.string.menu_reservas_none);
        mList.setEmptyView(empty);

        // Abre la base de datos
        mDbHelper = new ReservaDbAdapter(this);
        mDbHelper.open();

        // Rellena la lista con las habitaciones disponibles
        mList = findViewById(R.id.list);
        fillData();

        // Activa menú contextual al mantener pulsado un elemento de la lista
        registerForContextMenu(mList);

        // Comportamiento al pulsar un elemento de la lista
        mList.setClickable(true);
        mList.setOnItemClickListener((adapterView, view, i, l) -> consultarReserva(l));
    }

    /**
     * Inicia un elemento menú en la parte superior derecha de la pantalla que contendrá las
     * opciones para añadir una nueva reserva, o para ordenar la lista de reservas por
     * nombre del cliente, teléfono del cliente o fecha de entrada.
     *
     * @param menu Elemento menú sobre el que actuar
     * @return Menú creado
     * */
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        boolean res = super.onCreateOptionsMenu(menu);

        // Añade los elementos del menú
        menu.add(Menu.NONE, INSERT_ID, Menu.NONE, R.string.title_reserva_insert);
        menu.add(Menu.NONE, ORDER_BY_NAME_ID, Menu.NONE, R.string.menu_reserva_order_name);
        menu.add(Menu.NONE, ORDER_BY_PHONE_NUM, Menu.NONE, R.string.menu_reserva_order_phone);
        menu.add(Menu.NONE, ORDER_BY_DATE, Menu.NONE, R.string.menu_reserva_order_date);

        return res;
    }


    /**
     * Implementa el comportamiento del menú de opciones dependiendo de la opción que se haya
     * seleccionado.
     *
     * @param item Elemento seleccionado del menú
     * @return Opción realizada correctamente
     * @see MenuReservas#onCreateOptionsMenu(Menu)
     * */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case INSERT_ID: // Añadir reserva
                createReserva();
                return true;
            case ORDER_BY_NAME_ID: // Ordenar por nombre del cliente
                ORDER = ReservaDbAdapter.KEY_NOMCLIENTE;
                fillData();
                return true;
            case ORDER_BY_PHONE_NUM: // Ordenar por teléfono del cliente
                ORDER = ReservaDbAdapter.KEY_TLFCLIENTE;
                fillData();
                return true;
            case ORDER_BY_DATE: // Ordenar por fecha de entrada
                ORDER = ReservaDbAdapter.KEY_FENTRADA;
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
     * y que contendrá las opciones para modificar o eliminar la reserva seleccionada.
     *
     * @param menu Elemento menú sobre el que actuar
     * @param v No usado salvo para invocar constructor de clase padre
     * @param menuInfo No usado salvo para invocar constructor de clase padre
     * */
    @Override
    public void onCreateContextMenu(ContextMenu menu, View v,
                                    ContextMenu.ContextMenuInfo menuInfo) {
        super.onCreateContextMenu(menu, v, menuInfo);
        menu.add(Menu.NONE, DELETE_ID, Menu.NONE, R.string.menu_reserva_delete);
        menu.add(Menu.NONE, EDIT_ID, Menu.NONE, R.string.title_reserva_edit);
    }

    /**
     * Implementa el comportamiento del menú contextual dependiendo de la opción que se haya
     * seleccionado.
     *
     * @param item Elemento seleciconado del menú
     * @return Opción seleccionada correctamente
     * @see eina.unizar.gestorrural.reservas.MenuReservas#onCreateContextMenu(ContextMenu, View, ContextMenu.ContextMenuInfo)
     * */
    @Override
    public boolean onContextItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case DELETE_ID: // Eliminar reserva
                AdapterView.AdapterContextMenuInfo info =
                        (AdapterView.AdapterContextMenuInfo) item.getMenuInfo();
                mDbHelper.deleteReserva(info.id);
                fillData();
                return true;
            case EDIT_ID: // Modificar reserva
                info = (AdapterView.AdapterContextMenuInfo) item.getMenuInfo();
                editReserva(info.id);
        }

        return super.onContextItemSelected(item);
    }

    /**
     * Al acabar una actividad invocada por esta clase, vuelve a llenar la lista de reservas
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
     * Inicia la actividad EditarReserva en modo "Añadir reserva" y espera a un resultado.
     * */
    protected void createReserva() {
        Intent i = new Intent(this, EditarReserva.class);
        startActivityForResult(i, ACTIVITY_CREATE);
    }

    /**
     * Inicia la actividad EditarReserva en modo "Modificar reserva" y espera a un resultado.
     *
     * @param id Clave principal de la reserva a modificar
     * */
    protected void editReserva(long id) {
        Intent i = new Intent(this, EditarReserva.class);
        i.putExtra(ReservaDbAdapter.KEY_ROWID, id);
        startActivityForResult(i, ACTIVITY_EDIT);
    }

    /**
     * Inicia la actividad Consultar Reserva y espera a un resultado.
     *
     * @param id Clave principal de la reserva a consultar
     * */
    protected void consultarReserva(long id) {
        Intent i = new Intent(this, ConsultarReserva.class);
        i.putExtra(ReservaDbAdapter.KEY_ROWID, id);
        startActivityForResult(i, ACTIVITY_CONSULTAR);
    }

    /**
     * Rellena la lista de reservas con datos obtenidos de la base de datos.
     *
     * @see ListView
     * @see ListView#setAdapter(ListAdapter)
     * @see ReservaDbAdapter#fetchAllReservas(String)
     * @see MenuReservas#ORDER
     * */
    private void fillData() {
        Cursor c = mDbHelper.fetchAllReservas(ORDER);

        String[] from = new String[] { ReservaDbAdapter.KEY_NOMCLIENTE };
        int[] to = new int[] { R.id.list_row_text };

        mList.setAdapter(new SimpleCursorAdapter(this, R.layout.list_row, c, from, to));
    }
}
