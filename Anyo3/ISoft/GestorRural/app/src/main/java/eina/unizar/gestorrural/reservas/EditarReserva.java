package eina.unizar.gestorrural.reservas;

import android.annotation.SuppressLint;
import android.app.DatePickerDialog;
import android.database.Cursor;
import android.os.Bundle;
import android.util.Log;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;
import android.widget.Spinner;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Objects;

import eina.unizar.gestorrural.R;
import eina.unizar.gestorrural.habitaciones.HabitacionDbAdapter;

public class EditarReserva extends AppCompatActivity {

    /** Campo del formulario para el nombre del cliente */
    private EditText mNomCliente;

    /** Campo del formulario para el teléfono del cliente */
    private EditText mTlfCliente;

    /** Campo del formulario para la fecha de entrada */
    private EditText mFEntrada;

    /** Campo del formulario para la fecha de salida */
    private EditText mFSalida;

    /** Texto con el precio total de la reserva */
    private TextView mPrecio;

    /** Desplegable con la lista de habitaciones disponibles */
    private Spinner mListaHabitaciones;

    /** Campo del formulario para el número de inquilinos */
    private EditText mNumInquilinos;

    /** Elemento visual que contiene las habitaciones reservadas en forma de lista */
    private ListView mHabitacionesReservadas;

    /** */
    private ReservaDbAdapter mDbHelper;

    private Long mRowId;

    /** Comprometer reserva en base de datos */
    private static boolean guardarReserva = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Abre la base de datos
        mDbHelper = new ReservaDbAdapter(this).open();

        // Comprueba si tiene que recuperar informacion
        if (savedInstanceState != null) {
            mRowId = (Long) savedInstanceState.getSerializable(ReservaDbAdapter.KEY_ROWID);
        }
        else {
            mRowId = null;
            Bundle extras = getIntent().getExtras();
            // Comprueba si tiene que operar sobre una entrada existente
            if (extras != null) {
                mRowId = extras.getLong(ReservaDbAdapter.KEY_ROWID);
                guardarReserva = true;
            }
        }

        // Inicializa elementos visuales
        setContentView(R.layout.activity_editar_reserva);
        if (mRowId == null) setTitle(R.string.title_reserva_insert);
        else setTitle(R.string.title_reserva_edit);
        Objects.requireNonNull(getSupportActionBar()).setDisplayHomeAsUpEnabled(true);

        mNomCliente = findViewById(R.id.edittext_nombre_cliente_reserva);
        mTlfCliente = findViewById(R.id.edittext_tlf_cliente_reserva);
        mFEntrada   = findViewById(R.id.edittext_fecha_entrada_reserva);
        mFSalida    = findViewById(R.id.edittext_fecha_salida_reserva);
        mPrecio     = findViewById(R.id.textview_precio_reserva);
        mListaHabitaciones = findViewById(R.id.spinner_lista_habitaciones);
        mNumInquilinos = findViewById(R.id.edittext_numero_ocupantes_habitacion);
        Button botonAnadir  = findViewById(R.id.button_anadir_habitacion_reserva);
        mHabitacionesReservadas = findViewById(R.id.listview_habitaciones_reservadas);
        Button botonAceptar = findViewById(R.id.button_aceptar_reserva);

        registerForContextMenu(mHabitacionesReservadas);

        HabitacionDbAdapter mDbHelper2 = new HabitacionDbAdapter(this).open();
        Cursor c = mDbHelper2.fecthAllHabitaciones(HabitacionDbAdapter.KEY_NOMBRE);

        String[] from = new String[] { HabitacionDbAdapter.KEY_NOMBRE };
        int[] to = new int[] { android.R.id.text1 };

        SimpleCursorAdapter a = new SimpleCursorAdapter(this, android.R.layout.simple_spinner_item, c, from, to);
        a.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mListaHabitaciones.setAdapter(a);

        botonAnadir.setOnClickListener(view -> {

            long habitacionId, numInquilinos, fEntrada, fSalida;
            try {
                habitacionId = mListaHabitaciones.getSelectedItemId();
                numInquilinos = Long.parseLong(mNumInquilinos.getText().toString());
                fEntrada = Objects.requireNonNull(new SimpleDateFormat("dd/MM/yyyy").
                            parse(mFEntrada.getText().toString())).getTime();
                fSalida = Objects.requireNonNull(new SimpleDateFormat("dd/MM/yyyy").
                            parse(mFEntrada.getText().toString())).getTime();
            } catch (Exception e) { return; }

            if (checkFields()) {
                saveState();
                if (mDbHelper.fetchHabitacionReservada(habitacionId, mRowId) == null) {
                    mDbHelper.createHabitacionReservada(habitacionId, mRowId,
                            fEntrada, fSalida, numInquilinos);
                } else {
                    mDbHelper.updateHabitacionReservada(habitacionId, mRowId,
                            fEntrada, fSalida, numInquilinos);
                }
            }
            populateFields();
        });

        botonAceptar.setOnClickListener(view -> {
            if (checkFields()) {
                setResult(RESULT_OK);
                guardarReserva = true;
                finish();
            }
        });

        // Inicia escuchas para los campos de las fechas
        mFEntrada.setOnClickListener(view -> obtenerFecha(mFEntrada));
        mFSalida.setOnClickListener(view -> obtenerFecha(mFSalida));
    }

    /**
     * Implementa el comportamiento cuando se pulsa el botón de marcha atrás
     *
     * @param item El elemento de menú
     * @return Opción realizada completamente
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            finish();
            return true;
        } else return super.onOptionsItemSelected(item);
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
        saveState();
        outState.putSerializable(HabitacionDbAdapter.KEY_ROWID, mRowId);
    }

    /**
     * Comportamiento de la actividad al pasar a segundo plano, guardando la información obtenida
     * en la base de datos en caso de ser requerido.
     */
    @Override
    protected void onPause() {
        super.onPause();
        if (guardarReserva) saveState();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (!guardarReserva && mRowId != null) { mDbHelper.deleteReserva(mRowId); }
    }

    /**
     * Comportamiento de la actividad al volver al primer plano, recuperando información de la base
     * de datos en caso de ser necesario.
     */
    @Override
    protected void onResume() {
        super.onResume();
        populateFields();
    }

    private boolean checkFields() {
        boolean res;

        try { res = mDbHelper.checkReserva(
                    mNomCliente.getText().toString(),
                    Integer.parseInt(mTlfCliente.getText().toString().trim()),
                    Objects.requireNonNull(new SimpleDateFormat("dd/MM/yyyy").parse(mFEntrada.getText().
                            toString())).getTime(),
                    Objects.requireNonNull(new SimpleDateFormat("dd/MM/yyyy").parse(
                            mFSalida.getText().toString())).getTime());
        } catch (Exception e) { res = false; }

        return res;
    }

    @SuppressLint("SimpleDateFormat")
    private void populateFields() {
        if (mRowId != null) {
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
                    { HabitacionDbAdapter.KEY_NOMBRE, ReservaDbAdapter.KEY_NUMINQUILINOS };
            int [] to = new int[]
                    { R.id.custom_list_row_text, R.id.custom_list_row_subtext };
            mHabitacionesReservadas.setAdapter(new SimpleCursorAdapter(this,
                    R.layout.custom_list_row, c2, from, to));
        }
    }

    private void saveState() {
        long tlf, fentrada, fsalida;

        String nombre = mNomCliente.getText().toString();

        try { tlf = Integer.parseInt(mTlfCliente.getText().toString()); }
        catch (NumberFormatException e) { tlf = 0; }

        try { fentrada = Objects.requireNonNull(new SimpleDateFormat("dd/MM/yyyy").
                parse(mFEntrada.getText().toString())).getTime(); }
        catch (ParseException e) { fentrada = 0; }

        try {fsalida = Objects.requireNonNull(new SimpleDateFormat("dd/MM/yyyy").
                parse(mFSalida.getText().toString())).getTime(); }
        catch (ParseException e) { fsalida = 0; }

        if (mRowId == null) {
            Long id = mDbHelper.createReserva(nombre, tlf, fentrada, fsalida);
            if (id == null) {
                mRowId = id;
            }
        } else {
            mDbHelper.updateReserva(mRowId, nombre, tlf, fentrada, fsalida);
        }
    }


    /**
     * Abre un selector de fechas al pulsar un campo de texto. Al finalizar, rellena el campo
     * de texto con la fecha seleccionada
     *
     * @param field Campo de texto a rellenar
     */
    private void obtenerFecha(EditText field) {
        final Calendar c = Calendar.getInstance();

        @SuppressLint({"SetTextI18n", "SimpleDateFormat"})
        DatePickerDialog date = new DatePickerDialog(this, (datePicker, y, m, d) -> {
            c.set(y,m,d);
            field.setText(new SimpleDateFormat("dd/MM/yyyy").format(c.getTime()));
        }, c.get(Calendar.YEAR), c.get(Calendar.MONTH), c.get(Calendar.DAY_OF_MONTH));

        date.show();
    }
}
