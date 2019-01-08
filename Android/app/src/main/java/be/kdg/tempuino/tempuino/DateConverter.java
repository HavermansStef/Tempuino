package be.kdg.tempuino.tempuino;

import android.arch.persistence.room.TypeConverter;

import java.util.Date;

public class DateConverter {
    @TypeConverter
    public static Date fromTimestamp(Long value) {
        return value == null ? null : new Date(value);
    }

    @TypeConverter
    public static Long dateToTimestamp(Date date) {
        return date == null ? null : date.getTime();
    }

    @TypeConverter
    public static Measurement.MeasurementType toStatus(int status) {
        if (status == Measurement.MeasurementType.TEMPERATURE.getCode()) {
            return Measurement.MeasurementType.TEMPERATURE;
        } else if (status ==  Measurement.MeasurementType.HUMIDITY.getCode()) {
            return Measurement.MeasurementType.HUMIDITY;
        } else {
            throw new IllegalArgumentException("Could not recognize status");
        }
    }

    @TypeConverter
    public static Integer toInteger(Measurement.MeasurementType type) {
        return type.getCode();
    }
}
