package be.kdg.tempuino.tempuino;

import android.arch.persistence.room.Entity;
import android.arch.persistence.room.PrimaryKey;
import android.arch.persistence.room.TypeConverters;
import android.support.annotation.NonNull;

import java.time.LocalDateTime;
import java.util.Date;

@Entity
public class Measurement {
    @NonNull
    @PrimaryKey(autoGenerate = true)
    private int measurementId;
    private int value;
    @TypeConverters(DateConverter.class)
    private Date timestamp;
    private MeasurementType measurementType;

    public Measurement(int value, MeasurementType measurementType) {
        this.value = value;
        this.measurementType = measurementType;
    }

    @NonNull
    public int getMeasurementId() {
        return measurementId;
    }

    public void setMeasurementId(@NonNull int measurementId) {
        this.measurementId = measurementId;
    }

    public int getValue() {
        return value;
    }

    public void setValue(int value) {
        this.value = value;
    }

    public Date getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(Date timestamp) {
        this.timestamp = timestamp;
    }

    public MeasurementType getMeasurementType() {
        return measurementType;
    }

    public void setMeasurementType(MeasurementType measurementType) {
        this.measurementType = measurementType;
    }

    public enum MeasurementType {
        TEMPERATURE(0),
        HUMIDITY(1);
        private int code;

        MeasurementType(int code) {
            this.code = code;
        }

        public int getCode() {
            return code;
        }
    }
}
