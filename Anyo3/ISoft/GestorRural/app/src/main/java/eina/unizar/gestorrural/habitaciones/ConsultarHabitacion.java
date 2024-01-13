package eina.unizar.gestorrural.habitaciones;

import android.database.Cursor;
import android.os.Bundle;
import android.view.MenuItem;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import java.util.Objects;

import eina.unizar.gestorrural.R;

/**
 * La clase implementa una actividad que muesta los datos de una consulta a una habitación.
 * Se invoca al pulsar sobre una habitación existente en la lista de habitaciones.
 *
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Sergio Martinez Lahoz (801621@unizar.es)
 */
public class ConsultarHabitacion extends AppCompatActivity {

    /** Texto con el nombre de la habitación */
    private TextView mNombre;
    /** Texto con la descripción de la habitación */
    private TextView mDescripcion;
    /** Texto con el número de ocupantes de la habitación */
    private TextView mOcupantes;
    /** Texto con el precio de la habitación */
    private TextView mPrecio;
    /** Texto con el recargo de la aplicación */
    private TextView mRecargo;

    /**
     * Adaptador para realizar operaciones en la base de datos sobre habitaciones
     * @see HabitacionDbAdapter
     * */
    private HabitacionDbAdapter mDbHelper;

    /** Clave primaria de la habitación a consultar */
    private Long mRowId;

    /**
     * Implementación del comportamiento de la actividad ConsultarHabitacion al ser invocada.
     *
     * @param savedInstanceState Contiene la clave primaria de la habitación
     * */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Abre la base de datos
        mDbHelper = new HabitacionDbAdapter(this);
        mDbHelper.open();

        // Inicializa elementos visuales
        setContentView(R.layout.activity_consultar_habitacion);
        setTitle(R.string.title_consultar_habitacion);
        Objects.requireNonNull(getSupportActionBar()).setDisplayHomeAsUpEnabled(true);

        // Obtiene textos
        mNombre = findViewById(R.id.text_nombre_habitacion);
        mDescripcion = findViewById(R.id.text_descripcion_habitacion);
        mOcupantes = findViewById(R.id.text_ocupantes_habitacion);
        mPrecio = findViewById(R.id.text_precio_habitacion);
        mRecargo = findViewById(R.id.text_recargo_habitacion);

        // Obtiene datos
        if (savedInstanceState != null) {
            mRowId = (Long) savedInstanceState.getSerializable(HabitacionDbAdapter.KEY_ROWID);
        } else {
            Bundle extras = getIntent().getExtras();
            mRowId = extras.getLong(HabitacionDbAdapter.KEY_ROWID);
        }
        populateFields();
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
        outState.putSerializable(HabitacionDbAdapter.KEY_ROWID, mRowId);
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
     * Rellena el contenido de la actividad con datos obtenidos de la base de datos.
     */
    private void populateFields() {
        Cursor c = mDbHelper.fetchHabitacion(mRowId);
        startManagingCursor(c);

        mNombre.setText(c.getString(c.getColumnIndexOrThrow(
                HabitacionDbAdapter.KEY_NOMBRE)));
        mDescripcion.setText(c.getString(c.getColumnIndexOrThrow(
                HabitacionDbAdapter.KEY_DESCRIPCION)));
        mOcupantes.setText(c.getString(c.getColumnIndexOrThrow(
                HabitacionDbAdapter.KEY_MAXOCUPANTES)));
        mPrecio.setText(c.getString(c.getColumnIndexOrThrow(
                HabitacionDbAdapter.KEY_PRECIO)));
        mRecargo.setText(c.getString(c.getColumnIndexOrThrow(
                HabitacionDbAdapter.KEY_RECARGO)));
    }
}
