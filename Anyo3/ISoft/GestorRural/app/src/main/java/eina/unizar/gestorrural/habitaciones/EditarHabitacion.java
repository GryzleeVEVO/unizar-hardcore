package eina.unizar.gestorrural.habitaciones;

import android.database.Cursor;
import android.os.Bundle;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.EditText;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import java.util.Objects;

import eina.unizar.gestorrural.R;

/**
 * La clase implementa una actividad que permite añadir o modificar una habitación.
 * Consiste de un formulario donde se deben introducir el nombre de la habitación, su descripción,
 * el número máximo de ocupantes, el precio por noche para un solo ocupante y el recargo que se
 * añadirá por cada ocupante extra que se encuentre en la habitación.
 *
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Sergio Martinez Lahoz (801621@unizar.es)
 * */
public class EditarHabitacion extends AppCompatActivity {

    /** Campo del formulario para el nombre
     * @see EditText
     * */
    private EditText mNombreText;

    /** Campo del formulario para la descripción
     * @see EditText
     * */
    private EditText mDescripcionText;

    /** Campo del formulario para el número de ocupantes
     * @see EditText
     * */
    private EditText mOcupantesText;

    /** Campo del formulario para el el precio
     * @see EditText
     * */
    private EditText mPrecioText;

    /** Campo del formulario para el recargo
     * @see EditText
     * */
    private EditText mRecargoText;

    /**
     * Adaptador para realizar operaciones en la base de datos sobre habitaciones
     * @see HabitacionDbAdapter
     * */
    private HabitacionDbAdapter mDbHelper;

    /** Clave primaria de la habitación a añadir/modificar */
    private Long mRowId;

    /** Comprometer habitación en base de datos */
    private static boolean guardarHabitacion = false;

    /**
     * Implementación del comportamiento de la actividad EditarHabitacion al ser invocada.
     *
     * @param savedInstanceState Contiene la clave primaria de la habitación
     * */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Abre la base de datos
        mDbHelper = new HabitacionDbAdapter(this).open();

        // Comprueba si tiene que recuperar informacion
        if (savedInstanceState != null) {
            mRowId = (Long) savedInstanceState.getSerializable(HabitacionDbAdapter.KEY_ROWID);
        }
        else {
            mRowId = null;
            Bundle extras = getIntent().getExtras();
            // Comprueba si tiene que operar sobre una entrada existente
            if (extras != null) {
                mRowId = extras.getLong(HabitacionDbAdapter.KEY_ROWID);
                guardarHabitacion = true;
            }
        }

        // Inicializa elementos visuales
        setContentView(R.layout.activity_editar_habitacion);
        if (mRowId == null) {
            setTitle(R.string.title_habitacion_insert);
        } else {
            setTitle(R.string.title_habitacion_edit);
        }
        Objects.requireNonNull(getSupportActionBar()).setDisplayHomeAsUpEnabled(true); // Botón de marcha atrás

        // Obtiene campos de texto
        mNombreText = findViewById(R.id.form_nombre_habitacion);
        mDescripcionText = findViewById(R.id.form_descripcion_habitacion);
        mOcupantesText = findViewById(R.id.form_ocupantes_habitacion);
        mPrecioText = findViewById(R.id.form_precio_habitacion);
        mRecargoText = findViewById(R.id.form_recargo_habitacion);

        // Inicia escucha para pulsación del botón
        Button botonAceptar = findViewById(R.id.boton_aceptar_editar_habitaciones);
        botonAceptar.setOnClickListener(view -> {
            if (checkFields()) {
                setResult(RESULT_OK);
                guardarHabitacion = true;
                finish();
            }
        });
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
        } else {
            return super.onOptionsItemSelected(item);
        }
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
        // Comprometer habitación en BBDD
        if (guardarHabitacion) saveState();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (!guardarHabitacion && mRowId != null) { mDbHelper.deleteHabitacion(mRowId); }
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

    /**
     * Comprueba que todos los campos del formulario estén debidamente rellenados
     *
     * @return Verdad si todos los campos estan rellenados correctamente
     */
    private boolean checkFields() {
        boolean res;

        try { res = mDbHelper.checkHabitacion(mNombreText.getText().toString(),
                    mDescripcionText.getText().toString(),
                    Integer.parseInt(mOcupantesText.getText().toString().trim()),
                    Double.parseDouble(mPrecioText.getText().toString().trim()),
                    Double.parseDouble(mRecargoText.getText().toString().trim()));
        } catch (NumberFormatException e) { res = false; }


        return res;
    }

    /**
     * Rellena los campos del formulario con datos obtenidos de la base de datos si se está
     * modificando la habitación
     */
    private void populateFields() {
        if (mRowId != null) {
            Cursor c = mDbHelper.fetchHabitacion(mRowId);
            startManagingCursor(c);

            mNombreText.setText(c.getString(c.getColumnIndexOrThrow(
                    HabitacionDbAdapter.KEY_NOMBRE)));
            mDescripcionText.setText(c.getString(c.getColumnIndexOrThrow(
                    HabitacionDbAdapter.KEY_DESCRIPCION)));
            mOcupantesText.setText(c.getString(c.getColumnIndexOrThrow(
                    HabitacionDbAdapter.KEY_MAXOCUPANTES)));
            mPrecioText.setText(c.getString(c.getColumnIndexOrThrow(
                    HabitacionDbAdapter.KEY_PRECIO)));
            mRecargoText.setText(c.getString(c.getColumnIndexOrThrow(
                    HabitacionDbAdapter.KEY_RECARGO)));
        }
    }

    /**
     * Guarda los datos del formulario en la base de datos. Deben ser comprobados con checkFields()
     * anteriormente para asegurarse que los datos se pueden introducir.*
     *
     * @see EditarHabitacion#checkFields()
     */
    private void saveState() {

        int ocupantes;
        double precio, recargo;

        String nombre = mNombreText.getText().toString();
        String descripcion = mDescripcionText.getText().toString();

        try { ocupantes = Integer.parseInt(mOcupantesText.getText().toString()); }
        catch (NumberFormatException e) { ocupantes = 0; }

        try { precio = Double.parseDouble(mPrecioText.getText().toString()); }
        catch (NumberFormatException e) { precio = 0; }

        try { recargo = Double.parseDouble(mRecargoText.getText().toString()); }
        catch (NumberFormatException e) { recargo = 0; }

        if (mRowId == null) {
            Long id = mDbHelper.createHabitacion(nombre, descripcion, ocupantes, precio, recargo);
            if (id > 0) {
                mRowId = id;
            }
        } else {
            mDbHelper.updateHabitacion(mRowId, nombre, descripcion, ocupantes, precio, recargo);
        }
    }
}
