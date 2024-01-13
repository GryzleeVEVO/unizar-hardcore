package eina.unizar.gestorrural.helper;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;


/**
 * Clase ayudante que ayuda a preparar la base de datos para su uso por la habitación
 *
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Sergio Martinez Lahoz (801621@unizar.es)
 * @see SQLiteOpenHelper
 */
public class DatabaseHelper extends SQLiteOpenHelper {

    /** Nombre de la base de datos */
    private static final String DATABASE_NAME = "data";

    /** Versión de la habitación */
    private static final int DATABASE_VERSION = 2;

    /** Tag para depuración */
    private static final String TAG = "DatabaseHelper";

    /** Sentencia de creación de la tabla Habitaciones de la base de datos*/
    private static final String DATABASE_HABITACIONES_CREATE =
        "create table habitaciones(" +
            "_id integer primary key autoincrement," +
            "nombre text not null," +
            "descripcion text not null," +
            "maxocupantes int not null," +
            "precio real not null," +
            "recargo real not null);";

    /** Sentencia de creación de la tabla Reservas de la base de datos*/
    private static final String DATABASE_RESERVAS_CREATE =
        "create table reservas(" +
            "_id integer primary key autoincrement," +
            "nomcliente text not null," +
            "tlfcliente int not null," +
            "fentrada real not null," +
            "fsalida real not null," +
            "precio real not null);";

    /** Sentencia de creación de la tabla HabitacionesReservadas de la base de datos*/
    private static final String DATABASE_HABITACIONES_RESERVADAS_CREATE =
        "create table habitacionesreservadas(" +
                "_id integer primary key autoincrement," +
                "id_habitacion int not null references habitacion(_id)," +
                "id_reserva int not null references reservas(_id)," +
                "numinquilinos int not null);";

    /**
     * Constructor de un DatabaseHelper.
     *
     * @param context Contexto de la actividad invocadora
     */
    public DatabaseHelper(Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }

    /**
     * Al crear la clase, crea las tablas necesarias para almacenar los datos de la aplicación.
     *
     * @param db Comunicación con la base de datos
     */
    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL(DATABASE_HABITACIONES_CREATE);
        db.execSQL(DATABASE_RESERVAS_CREATE);
        db.execSQL(DATABASE_HABITACIONES_RESERVADAS_CREATE);
    }

    /**
     * Al actualizar, elimina y vuelve a crear las tablas de la aplicación.
     *
     * @param db Comunicación con la base de datos
     * @param oldVersion Valor de versión de la que se está actualizando
     * @param newVersion Valor de versión a actualizar
     */
    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        Log.w(TAG, "Upgrading database from version " + oldVersion + " to "
                + newVersion + ", which will destroy all old data");
        db.execSQL("DROP TABLE IF EXISTS habitacionesreservadas");
        db.execSQL("DROP TABLE IF EXISTS habitaciones");
        db.execSQL("DROP TABLE IF EXISTS reservas");
        onCreate(db);
    }
}