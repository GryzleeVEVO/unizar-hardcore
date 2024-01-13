package eina.unizar.gestorrural.reservas;

import android.annotation.SuppressLint;
import android.database.Cursor;
import android.os.Bundle;
import android.view.MenuItem;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import java.text.SimpleDateFormat;
import java.util.Objects;

import eina.unizar.gestorrural.R;

/**
 * La clase implementa una actividad que muesta los datos de una consulta a una reserva.
 * Se invoca al pulsar sobre una reserva existente en la lista de reservas.
 *
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Sergio Martinez Lahoz (801621@unizar.es)
 */
public class ConsultarReserva extends AppCompatActivity {

    /** Texto con el nombre del cliente */
    private TextView mNomCliente;

    /** Texto con el teléfono del cliente */
    private TextView mTlfCliente;

    /** Texto con la fecha de entrada */
    private TextView mFEntrada;

    /** Texto con la fecha de salida */
    private TextView mFSalida;

    /** Texto con el precio total de la reserva */
    private TextView mPrecio;

    /** Elemento visual que contiene las habitaciones reservadas en forma de lista */
    private ListView mHabitacionesReservadas;

    /**
     * Adaptador para realizar operaciones en la base de datos sobre reservas
     * @see ReservaDbAdapter
     * */
    private ReservaDbAdapter mDbHelper;

    /** Clave primaria de la reserva a consultar */
    private Long mRowId;

    /**
     * Implementación del comportamiento de la actividad ConsultarReserva al ser invocada.
     *
     * @param savedInstanceState Contiene la clave primaria de la reserva
     * */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Abre la base de datos
        mDbHelper = new ReservaDbAdapter(this).open();

        // Inicializa elementos visuales
        setContentView(R.layout.activity_consultar_reserva);
        setTitle(R.string.title_consultar_reserva);
        Objects.requireNonNull(getSupportActionBar()).setDisplayHomeAsUpEnabled(true);

        mNomCliente = findViewById(R.id.textview_nombre_cliente_reserva);
        mTlfCliente = findViewById(R.id.textview_tlf_cliente_reserva);
        mFEntrada   = findViewById(R.id.textview_fecha_entrada_reserva);
        mFSalida    = findViewById(R.id.textview_fecha_salida_reserva);
        mPrecio     = findViewById(R.id.textview_precio_reserva);

        mHabitacionesReservadas = findViewById(R.id.listview_habitaciones_reservadas);
        TextView empty = findViewById(R.id.empty);
        empty.setText(R.string.habitaciones_reservadas_none);

        // Carga datos
        if (savedInstanceState != null) {
            mRowId = (Long) savedInstanceState.getSerializable(ReservaDbAdapter.KEY_ROWID);
        } else {
            mRowId = getIntent().getExtras().getLong(ReservaDbAdapter.KEY_ROWID);
        }

        fillData();

        // TODO: Botón para enviar mensaje
    }

    /**
     * Implementa el comportamiento cuando se pulsa el botón de marcha atrás
     *
     * @param item El elemento de menú
     * @return Opción realizada completamente
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) { finish(); return true; }
        else return super.onOptionsItemSelected(item);
    }

    /**
     * Serializa y la clave principal al pasar la actividad a segundo plano.
     *
     * @param outState Estado de la actividad al salir
     * @see Bundle
     */
    @Override
    protected void onSaveInstanceState(@NonNull Bundle outState) {
        super.onSaveInstanceState(outState);
        outState.putSerializable(ReservaDbAdapter.KEY_ROWID, mRowId);
    }

    /**
     * Comportamiento de la actividad al volver al primer plano, recuperando información de la base
     * de datos en caso de ser necesario.
     */
    @Override
    protected void onResume() { super.onResume(); fillData(); }


    /**
     * Rellena el contenido de la actividad con datos obtenidos de la base de datos.
     */
    @SuppressLint("SimpleDateFormat")
    private void fillData() {
        // Obtiene datos de la reserva
        Cursor c = mDbHelper.fetchReserva(mRowId);
        startManagingCursor(c);

        mNomCliente.setText(c.getString(c.getColumnIndexOrThrow(
                ReservaDbAdapter.KEY_NOMCLIENTE)));
        mTlfCliente.setText(c.getString(c.getColumnIndexOrThrow(
                ReservaDbAdapter.KEY_TLFCLIENTE)));
        mFEntrada.setText(new SimpleDateFormat("dd/MM/yyyy").format(
                c.getLong(c.getColumnIndexOrThrow(ReservaDbAdapter.KEY_FENTRADA))));
        mFSalida.setText(new SimpleDateFormat("dd/MM/yyyy").format(
                c.getLong(c.getColumnIndexOrThrow(ReservaDbAdapter.KEY_FSALIDA))));
        mPrecio.setText(c.getString(c.getColumnIndexOrThrow(
                ReservaDbAdapter.KEY_PRECIO)));

        // Obtiene lista de habitaciones reservadas
        Cursor c2 = mDbHelper.fetchAllHabitacionesReservadas(mRowId);
        String[] from = new String[]
                { ReservaDbAdapter.KEY_ID_HABITACION, ReservaDbAdapter.KEY_ID_RESERVA };
        int [] to = new int[]
                { R.id.custom_list_row_text, R.id.custom_list_row_subtext };
        mHabitacionesReservadas.setAdapter(new SimpleCursorAdapter(this,
                R.layout.custom_list_row, c2, from, to));
    }
}

