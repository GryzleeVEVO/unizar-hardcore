package eina.unizar.gestorrural.reservas;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;

import eina.unizar.gestorrural.habitaciones.HabitacionDbAdapter;
import eina.unizar.gestorrural.helper.DatabaseHelper;

public class ReservaDbAdapter {

    public static final String DATABASE_TABLE_RESERVAS = "reservas";
    public static final String DATABASE_TABLE_HABITACIONES_RESERVADAS = "habitacionesreservadas";
    public static final String KEY_ROWID = "_id";

    public static final String KEY_NOMCLIENTE = "nomcliente";
    public static final String KEY_TLFCLIENTE = "tlfcliente";
    public static final String KEY_FENTRADA = "fentrada";
    public static final String KEY_FSALIDA = "fsalida";
    public static final String KEY_PRECIO = "precio";

    public static final String[] TABLE_COLUMNS_RESERVAS =
            { KEY_ROWID, KEY_NOMCLIENTE, KEY_TLFCLIENTE, KEY_FENTRADA, KEY_FSALIDA, KEY_PRECIO };

    public static final String KEY_ID_HABITACION = "id_habitacion";
    public static final String KEY_ID_RESERVA = "id_reserva";
    public static final String KEY_NUMINQUILINOS = "numinquilinos";

    public static final String[] TABLE_COLUMNS_HABITACIONES_RESERVADAS =
            { KEY_ROWID, KEY_ID_HABITACION, KEY_ID_RESERVA, KEY_NUMINQUILINOS };

    private DatabaseHelper mDbHelper;
    private SQLiteDatabase mDb;
    private final Context mCtx;

    public ReservaDbAdapter(Context ctx) { this.mCtx = ctx; }

    public ReservaDbAdapter open() throws SQLException {
        mDbHelper = new DatabaseHelper(mCtx);
        mDb = mDbHelper.getWritableDatabase();
        return this;
    }

    public void close() { mDbHelper.close(); }

    public Long createReserva(String nomCliente, long tlfCliente, long fEntrada, long fSalida) {
        if (!checkReserva(nomCliente, tlfCliente, fEntrada, fSalida)) { return null; }

        ContentValues initialValues = new ContentValues();

        initialValues.put(KEY_NOMCLIENTE, nomCliente);
        initialValues.put(KEY_TLFCLIENTE, tlfCliente);
        initialValues.put(KEY_FENTRADA, fEntrada);
        initialValues.put(KEY_FSALIDA, fSalida);
        initialValues.put(KEY_PRECIO, 0);

        return mDb.insert(DATABASE_TABLE_RESERVAS, null, initialValues);
    }

    public boolean updateReserva(long rowId, String nomCliente, long tlfCliente, long fEntrada,
                                 long fSalida) {
        if (!checkReserva(nomCliente, tlfCliente, fEntrada, fSalida)) { return false; }

        ContentValues args = new ContentValues();

        args.put(KEY_NOMCLIENTE, nomCliente);
        args.put(KEY_TLFCLIENTE, tlfCliente);
        args.put(KEY_FENTRADA, fEntrada);
        args.put(KEY_FSALIDA, fSalida);

        return mDb.update(DATABASE_TABLE_RESERVAS, args, KEY_ROWID + "=" + rowId,
                null) > 0;
    }

    public boolean deleteReserva(long rowId) {
        mDb.delete(DATABASE_TABLE_HABITACIONES_RESERVADAS, KEY_ID_RESERVA + "=" + rowId,
                null);
        return mDb.delete(DATABASE_TABLE_RESERVAS, KEY_ROWID + "=" + rowId,
                null) > 0;
    }

    public Cursor fetchAllReservas(String orderBy) {
        return mDb.query(DATABASE_TABLE_RESERVAS, TABLE_COLUMNS_RESERVAS, null,
                null, null, null, orderBy, null);
    }

    public Cursor fetchReserva(long rowId) {
        Cursor mCursor = mDb.query(DATABASE_TABLE_RESERVAS, TABLE_COLUMNS_RESERVAS,
                KEY_ROWID + "=" + rowId, null, null, null,
                null, null);

        if (mCursor != null) mCursor.moveToFirst();
        return mCursor;
    }

    public Long createHabitacionReservada(long habitacionId, long reservaId, long fEntrada,
                                          long fSalida, long numInquilinos) {
        Log.d("DDD", "dolore");

        if (!checkHabitacionReservada(habitacionId, numInquilinos, fEntrada, fSalida)) return null;

        Log.d("AAA", "dolore");

        ContentValues initialValues = new ContentValues();

        initialValues.put(KEY_ID_HABITACION, habitacionId);
        initialValues.put(KEY_ID_RESERVA, reservaId);
        initialValues.put(KEY_NUMINQUILINOS, numInquilinos);

        return mDb.insert(DATABASE_TABLE_HABITACIONES_RESERVADAS, null, initialValues);
    }

    public boolean updateHabitacionReservada(long habitacionId, long reservaId, long fEntrada,
                                             long fSalida, long numInquilinos) {
        if (!checkHabitacionReservada(habitacionId, numInquilinos, fEntrada, fSalida)) return false;

        ContentValues args = new ContentValues();

        args.put(KEY_ID_HABITACION, habitacionId);
        args.put(KEY_ID_RESERVA, reservaId);
        args.put(KEY_NUMINQUILINOS, numInquilinos);

        return mDb.update(DATABASE_TABLE_HABITACIONES_RESERVADAS, args, KEY_ID_HABITACION + "=" +
                        habitacionId + " AND " + KEY_ID_RESERVA + "=" + reservaId,
                null) > 0;
    }

    public boolean deleteHabitacionReservada(long rowId) {
        return mDb.delete(DATABASE_TABLE_HABITACIONES_RESERVADAS, KEY_ROWID + "=" + rowId,
                null) > 0;
    }

    public Cursor fetchHabitacionReservada(long habitacionId, long reservaId) {
        Cursor mCursor = mDb.query(DATABASE_TABLE_HABITACIONES_RESERVADAS,
                TABLE_COLUMNS_HABITACIONES_RESERVADAS,
                KEY_ID_RESERVA + "=" + reservaId + " AND "+ KEY_ID_HABITACION + "="
                        + habitacionId, null,
                null, null, null, null);

        if (mCursor != null) { mCursor.moveToFirst(); }
        return mCursor;
    }

    public Cursor fetchAllHabitacionesReservadas(long reservaId) {
        /*
         * SELECT * FROM habitacionesreservadas
         * INNER JOIN habitaciones ON habitacionesreservadas.id_habitacion = habitaciones._id
         * WHERE
         *  id_reserva = reservaId
         * */
        return mDb.rawQuery("SELECT * FROM " + DATABASE_TABLE_HABITACIONES_RESERVADAS + " , "
                        + HabitacionDbAdapter.DATABASE_TABLE +
                " WHERE " + DATABASE_TABLE_HABITACIONES_RESERVADAS + "." + KEY_ID_HABITACION + "=" +
                HabitacionDbAdapter.DATABASE_TABLE + "." + KEY_ROWID +
                " AND " + DATABASE_TABLE_HABITACIONES_RESERVADAS + "." + KEY_ID_RESERVA + "=" +
                reservaId + ";", null);
    }

    public boolean checkReserva(String nomCliente, long tlfCliente, long fEntrada, long fSalida) {
        return !(nomCliente.isEmpty() || tlfCliente <= 0 || fEntrada >= fSalida);
    }

    public boolean checkHabitacionReservada(Long habitacionId, long numInquilinos,
                                            long fEntrada, long fSalida) {

        // Habitacion no excede cantidad de ocupantes
        Cursor c = mDb.query(HabitacionDbAdapter.DATABASE_TABLE, HabitacionDbAdapter.TABLE_COLUMNS,
                KEY_ROWID + "=" + habitacionId, null, null,
                null, null, null);

        if (c == null) return false;

        c.moveToFirst();
        long maxOcupantes = c.getLong(c.getColumnIndexOrThrow(HabitacionDbAdapter.KEY_MAXOCUPANTES));

        // No hay reservas de la habitación en el mismo periodo de tiempo
        /*
         * SELECT * FROM habitacionesreservadas
         * INNER JOIN reservas ON habitacionesreservadas.id_reserva = reservas._id
         * WHERE
         *  habitacionesreservadas.id_habitacion = habitacionId
         *  AND reservas.fentrada >= fEntrada AND reservas.fsalida <= fSalida
         * */

        /*
        Cursor c2 = mDb.rawQuery("SELECT * FROM " + DATABASE_TABLE_HABITACIONES_RESERVADAS +
                " INNER JOIN " + DATABASE_TABLE_RESERVAS + " ON " +
                        DATABASE_TABLE_HABITACIONES_RESERVADAS + "." + KEY_ID_RESERVA + "=" +
                        DATABASE_TABLE_RESERVAS + "." + KEY_ROWID +
                " WHERE " + DATABASE_TABLE_HABITACIONES_RESERVADAS + "." + KEY_ID_RESERVA + "=" +
                        habitacionId + " AND " + DATABASE_TABLE_RESERVAS + "." + KEY_FENTRADA + ">="
                        + fEntrada + " AND "+ DATABASE_TABLE_RESERVAS + "." + KEY_FSALIDA + "<="
                        + fSalida + ";", null);

         */

        return numInquilinos <= maxOcupantes ;//&& c2 == null;
    }
}
