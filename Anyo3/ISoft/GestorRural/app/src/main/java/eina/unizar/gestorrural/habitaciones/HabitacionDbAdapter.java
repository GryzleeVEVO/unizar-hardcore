package eina.unizar.gestorrural.habitaciones;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;

import eina.unizar.gestorrural.helper.DatabaseHelper;

/***
 * La clase encapsula la comunicación y las acciones disponibles para añadir, eliminar, modificar y
 * obtener elementos de la tabla Habitaciones.<br/>
 *
 * La tabla habitaciones contiene las siguientes columnas:
 * <table>
 *    <tr>
 *       <th>ROW_ID</th> <td>Clave primaria única de la habitación, independiente de los datos</td>
 *    <tr/>
 *    <tr>
 *       <th>Nombre</th> <td>Nombre de la habitación</td>
 *    </tr>
 *    <tr>
 *        <th>Descripcion</th> <td>Descripcion la habitación</td>
 *    </tr>
 *    <tr>
 *        <th>MaxOcupantes</th> <td>Número máximo de ocupantes permitidos para una reserva</td>
 *    </tr>
 *    <tr>
 *        <th>Precio</th> <td>Precio para un solo ocupante para una sola noche</td>
 *    </tr>
 *    <tr>
 *        <th>Recargo</th> <td>Procentaje que se añade al precio por cada ocupante extra en una reserva</td>
 *    </tr>
 * </table>
 *
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Sergio Martinez Lahoz (801621@unizar.es)
 * @see SQLiteDatabase
 * @see DatabaseHelper
 * */
public class HabitacionDbAdapter {

    /** Nombre de la tabla Habitaciones */
    public static final String DATABASE_TABLE = "habitaciones";
    /** Referencia a la columna ROW_ID */
    public static final String KEY_ROWID = "_id";
    /** Referencia a la columna Nombre */
    public static final String KEY_NOMBRE = "nombre";
    /** Referencia a la columna Descripcion */
    public static final String KEY_DESCRIPCION = "descripcion";
    /** Referencia a la columna MaxOcupantes */
    public static final String KEY_MAXOCUPANTES = "maxocupantes";
    /** Referencia a la columna Precio */
    public static final String KEY_PRECIO = "precio";
    /** Referencia a la columna Recargo */
    public static final String KEY_RECARGO = "recargo";
    /** Lista con todas las columnas de la tabla Habitaciones  */
    public static final String[] TABLE_COLUMNS =
            { KEY_ROWID, KEY_NOMBRE, KEY_DESCRIPCION, KEY_MAXOCUPANTES, KEY_PRECIO, KEY_RECARGO };

    /**
     * Ayudante para la conexión con la base de datos
     * @see DatabaseHelper
     * */
    private DatabaseHelper mDbHelper;

    /**
     * Conexión a la base de datos SQLite
     * @see SQLiteDatabase
     * */
    private SQLiteDatabase mDb;

    /**
     * Contexto de la actividad invocadora
     * @see Context
     * */
    private final Context mCtx;

    /**
     * Constructor de clase HabitacionDbAdapter.
     *
     * @param ctx Contexto de la actividad invocadora
     * */
    public HabitacionDbAdapter(Context ctx) {
        this.mCtx = ctx;
    }

    /**
     * Abre una conexión con la base de datos SQLite.
     *
     * @return Referencia a si mismo
     * @throws SQLException Excepción en caso de no poder abrir la base de datos
     */
    public HabitacionDbAdapter open() throws SQLException {
        mDbHelper = new DatabaseHelper(mCtx);
        mDb = mDbHelper.getWritableDatabase();
        return this;
    }

    /**
     * Cierra la conexión con la base de datos SQLite.
     * */
    public void close() {
        mDbHelper.close();
    }

    /**
     * Añade una nueva habitación a la base de datos.
     *
     * @param nombre Nombre de la habitación
     * @param descripcion Descripción de la habitación
     * @param maxOcupantes Número de ocupantes máximo de la habitación
     * @param precio Precio para un solo ocupante para una sola noche
     * @param recargo Procentaje de recargo por cada ocupante extra a partir del primero
     * @return Valor de la clave principal donde se ha realizado la insercción
     * @see SQLiteDatabase#insert(String, String, ContentValues)
     * */
    public Long createHabitacion(String nombre, String descripcion, int maxOcupantes,
                                 double precio, double recargo) {
        if (!checkHabitacion(nombre, descripcion, maxOcupantes, precio, recargo)) { return null; }

        ContentValues initialValues = new ContentValues();

        initialValues.put(KEY_NOMBRE, nombre);
        initialValues.put(KEY_DESCRIPCION, descripcion);
        initialValues.put(KEY_MAXOCUPANTES, maxOcupantes);
        initialValues.put(KEY_PRECIO, precio);
        initialValues.put(KEY_RECARGO, recargo);

        return mDb.insert(DATABASE_TABLE, null, initialValues);
    }

    /**
     * Actualiza una habitación de la base de datos con los datos dados.
     *
     * @param rowId Clave primaria de la habitación
     * @param nombre Nombre de la habitación
     * @param descripcion Descripción de la habitación
     * @param maxOcupantes Número de ocupantes máximo de la habitación
     * @param precio Precio para un solo ocupante para una sola noche
     * @param recargo Procentaje de recargo por cada ocupante extra a partir del primero
     * @return Éxito de la operación
     * @see SQLiteDatabase#update(String, ContentValues, String, String[])
     * */
    public boolean updateHabitacion(long rowId, String nombre, String descripcion, int maxOcupantes,
                                    double precio, double recargo) {

        if (!checkHabitacion(nombre, descripcion, maxOcupantes, precio, recargo)) { return false; }

        ContentValues args = new ContentValues();

        args.put(KEY_NOMBRE, nombre);
        args.put(KEY_DESCRIPCION, descripcion);
        args.put(KEY_MAXOCUPANTES, maxOcupantes);
        args.put(KEY_PRECIO, precio);
        args.put(KEY_RECARGO, recargo);

        return mDb.update(DATABASE_TABLE, args, KEY_ROWID + "=" + rowId, null)
                > 0;
    }

    /**
     * Elimina una habitación de la base de datos.
     *
     * @param rowId Clave primaria de la habitación
     * @return Éxito de la operación
     */
    public boolean deleteHabitacion(long rowId) {
        return mDb.delete(DATABASE_TABLE, KEY_ROWID + "=" + rowId, null) > 0;
    }

    /**
     * Devuelve un cursor a una lista con todas las habitaciones siguiendo el orden dado.
     *
     * @param orderBy Clave de la columna a ordenar
     * @return Cursor al resultado de la operación
     * @see Cursor
     */
    public Cursor fecthAllHabitaciones(String orderBy) {
        return mDb.query(DATABASE_TABLE, TABLE_COLUMNS, null, null, null,
                null, orderBy, null);
    }

    /**
     * Devuelve, si está, una habitación dada su clave de la tabla.
     *
     * @param rowId Clave primaria de la habitación a obtener
     * @return Cursor al elemento de la selección de la tabla, nulo si no hay coincidencias
     * @throws SQLException Error al intentar ejecutar la acción
     * @see Cursor
     */
    public Cursor fetchHabitacion(long rowId) throws SQLException {
        Cursor mCursor = mDb.query(DATABASE_TABLE, TABLE_COLUMNS, KEY_ROWID + "=" + rowId,
                null, null, null, null, null);

        if (mCursor != null) {
            mCursor.moveToFirst();
        }

        return mCursor;
    }

    /***
     * Devuelve si los datos a introducir de la hoabitación son correctos
     *
     * @param nombre Cadena no vacía
     * @param descripcion Cadena no vacía
     * @param maxOcupantes Entero, mayor o igual que 0
     * @param precio Decimal, mayor o igual que 0
     * @param recargo Decimal, mayor o igual que 0
     * @return Parámetros con formato correcto
     */
     public boolean checkHabitacion(String nombre, String descripcion, int maxOcupantes,
                                    double precio, double recargo) {
        return !(nombre.isEmpty() || descripcion.isEmpty() || maxOcupantes < 0 || precio < 0
                            || recargo < 0);
    }
}
