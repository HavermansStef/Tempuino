package be.kdg.tempuino.tempuino;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.design.widget.BottomNavigationView;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.io.UnsupportedEncodingException;
import java.text.DateFormat;
import java.util.Date;

import butterknife.ButterKnife;
import github.vatsal.easyweather.Helper.TempUnitConverter;
import github.vatsal.easyweather.Helper.WeatherCallback;
import github.vatsal.easyweather.WeatherMap;
import github.vatsal.easyweather.retrofit.models.Weather;
import github.vatsal.easyweather.retrofit.models.WeatherResponseModel;

public class MainActivity extends AppCompatActivity {

    private TextView mTextMessage;
    private TextView tvCity;
    private TextView tvTemperatureValue;
    private TextView tvHumidValue;
    private TextView tvTempInsideValue;
    private TextView tvHumInsideValue;
    private TextView tvModified;
    private Button btnUpdate;

    private final String serverURL = "tcp://m15.cloudmqtt.com:16661";
    private final String userNameMQTT = "eblgndhd";
    private final String passwordMQTT = "n_jbOQxQAaTt";
    private final String topicTemp = "dht";
    private final String topicHum = "bmp";

    public final String APP_ID = "063057c739334060ed68e5e14f6a97d8";
    String city = "Antwerpen";

    private MqttAndroidClient client;

    private BottomNavigationView.OnNavigationItemSelectedListener mOnNavigationItemSelectedListener
            = new BottomNavigationView.OnNavigationItemSelectedListener() {

        @Override
        public boolean onNavigationItemSelected(@NonNull MenuItem item) {
            switch (item.getItemId()) {
                case R.id.navigation_home:
                    mTextMessage.setText(R.string.title_home);
                    return true;
                case R.id.navigation_dashboard:
                    mTextMessage.setText(getString(R.string.stats));
                    return true;
                case R.id.navigation_notifications:
                    mTextMessage.setText(getString(R.string.settings));
                    return true;
            }
            return false;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mTextMessage = findViewById(R.id.message);
        BottomNavigationView navigation = findViewById(R.id.navigation);
        navigation.setOnNavigationItemSelectedListener(mOnNavigationItemSelectedListener);
        tvModified = findViewById(R.id.tvDate);
        tvTemperatureValue = findViewById(R.id.tvTempValue);
        tvHumidValue = findViewById(R.id.tvHumValue);
        tvCity = findViewById(R.id.tvCity);
        btnUpdate = findViewById(R.id.btRefresh);
        ButterKnife.bind(this);
        loadWeather(city);
        connectMQTT();
        createEventListeners();
    }

    private void createEventListeners() {
        btnUpdate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                loadWeather(city);
                publish("refresh");
                updateTimeStamp();
            }
        });
    }

    private void loadWeather(String city) {
        WeatherMap weatherMap = new WeatherMap(this, APP_ID);
        weatherMap.getCityWeather(city, new WeatherCallback() {
            @Override
            public void success(WeatherResponseModel response) {
                tvHumidValue.setText("Bloep");
                populateWeather(response);
            }

            @Override
            public void failure(String message) {

            }
        });
    }

    private void populateWeather(WeatherResponseModel response) {
        Weather weather[] = response.getWeather();
        tvHumidValue.setText(response.getMain().getHumidity() + "%");
        tvHumidValue.setText("Hello");
        tvTemperatureValue.setText(TempUnitConverter.convertToCelsius(response.getMain().getTemp()).intValue() + " °C");
        tvCity.setText(response.getName());
    }


    private void connectMQTT() {
        String clientId = MqttClient.generateClientId();
        client = new MqttAndroidClient(
                this.getApplicationContext(), serverURL,
                clientId);
        MqttConnectOptions options = new MqttConnectOptions();
        options.setMqttVersion(MqttConnectOptions.MQTT_VERSION_3_1);
        options.setCleanSession(false);
        options.setUserName(userNameMQTT);
        options.setPassword(passwordMQTT.toCharArray());

        try {
            IMqttToken token = client.connect(options);
            token.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    //We zijn verbonden

                    Log.d("Hello", "Succesfully connected");
                    subscribe(topicHum);
                    subscribe(topicTemp);
                    client.setCallback(new MqttCallback() {
                        TextView tt = findViewById(R.id.tvTempInsideValue);
                        TextView th = findViewById(R.id.tvHumInsideValue);

                        @Override
                        public void connectionLost(Throwable cause) {

                        }

                        @Override
                        public void messageArrived(String topic, MqttMessage message) throws Exception {
                            Log.d("file", message.toString());

                            switch (topic) {
                                case "dht":
                                    tt.setText(message.toString());
                                    break;
                                case "bmp":
                                    th.setText(message.toString());
                                    break;
                            }
                            updateTimeStamp();
                        }

                        @Override
                        public void deliveryComplete(IMqttDeliveryToken token) {

                        }
                    });
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {

                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    private void subscribe(String topic) {
        int qos = 1;
        try {
            IMqttToken subToken = client.subscribe(topic, qos);
            subToken.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    Log.d("Sub", "Succesfully subscribed");
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {

                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    private void publish(String payload) {
        String topic = "settings";
        byte[] encoded = new byte[0];
        try {
            encoded = payload.getBytes("UTF-8");
            MqttMessage message = new MqttMessage(encoded);
            client.publish(topic, message);
        } catch (UnsupportedEncodingException | MqttException e) {
            e.printStackTrace();
        }
    }

    private void updateTimeStamp() {
        String currentDateTimeString = DateFormat.getDateTimeInstance().format(new Date());
        tvModified.setText(currentDateTimeString);
    }

}
