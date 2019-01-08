package be.kdg.tempuino.tempuino;

import android.arch.persistence.room.Dao;
import android.arch.persistence.room.Delete;
import android.arch.persistence.room.Insert;
import android.arch.persistence.room.Query;

import java.util.List;

@Dao
public interface DaoAccess {

    @Insert
    void insertSingleMeasurement(Measurement measurement);

    @Insert
    void insertMultipleMeasurements(List<Measurement> measurements);

    @Query("SELECT * FROM Measurement WHERE measurementId = :measurementId")
    Measurement findBymeasurementId(int measurementId);

    @Query("SELECT * FROM MEASUREMENT WHERE measurementType = 'HUMIDITY'")
    List<Measurement> getAllHum();

    @Query("SELECT * FROM MEASUREMENT WHERE measurementType = 'TEMPERATURE'")
    List<Measurement> getAllTemp();

    @Query("SELECT * FROM MEASUREMENT WHERE measurementType = 'HUMIDITY' ORDER BY measurementId DESC limit 1")
    Measurement getlastHum();

    @Query("SELECT * FROM MEASUREMENT WHERE measurementType = 'TEMPERATURE' ORDER BY measurementId DESC limit 1")
    Measurement getLastTemp();

    @Query("SELECT COUNT(*) FROM Measurement")
    int getAmountValuesPresent();

    @Query("SELECT COUNT(*) FROM Measurement WHERE measurementType = 'HUMIDITY'")
    int getAmountValuesPresentHum();

    @Query("SELECT COUNT(*) FROM Measurement WHERE measurementType = 'TEMPERATURE'")
    int getAmountValuesPresentTemp();

    @Delete
    void delete(Measurement measurement);
}
