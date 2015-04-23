package com.meetme.rccar;

import android.app.Activity;
import android.app.FragmentTransaction;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.telephony.SmsManager;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.Toast;

import butterknife.ButterKnife;
import butterknife.InjectView;
import butterknife.OnClick;


public class MainActivity extends Activity {

    private boolean settingsShowing = false;

    @InjectView(R.id.buttons_container)
    LinearLayout buttonsContainer;

    @InjectView(R.id.settings_container)
    LinearLayout settingsContainer;

    @InjectView(R.id.phone_number)
    EditText phoneNumberView;

    private SharedPreferences prefs;
    private String SHARED_PREFS_KEY = "RCCAR_PHONE_NUMBER";

    private String phoneNumber;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        prefs = this.getSharedPreferences(
                "com.meetme.rccar", Context.MODE_PRIVATE);

        phoneNumber = prefs.getString(SHARED_PREFS_KEY,"");

        setContentView(R.layout.main);
        ButterKnife.inject(this);
    }

    @OnClick(R.id.turn_on) void turnOn() {
        sendSMS(phoneNumber, "Start");
    }
    @OnClick(R.id.open) void open() {
        sendSMS(phoneNumber, "Open");
    }
    @OnClick(R.id.close) void close() {
        sendSMS(phoneNumber, "Close");
    }
    @OnClick(R.id.button_okay) void save() {
        phoneNumber = phoneNumberView.getText().toString();

        prefs.edit().putString(SHARED_PREFS_KEY,phoneNumber).apply();
        settingsShowing = false;
        settingsContainer.setVisibility(View.INVISIBLE);
        buttonsContainer.setVisibility(View.VISIBLE);
    }

    @Override
    public void onBackPressed() {
        if(settingsShowing) {
            settingsShowing = false;
            settingsContainer.setVisibility(View.INVISIBLE);
            buttonsContainer.setVisibility(View.VISIBLE);
        } else {
            super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings && !settingsShowing) {
            phoneNumberView.setText(phoneNumber);
            settingsShowing = true;
            settingsContainer.setVisibility(View.VISIBLE);
            buttonsContainer.setVisibility(View.INVISIBLE);
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    //---sends an SMS message to another device---
    private void sendSMS(String phoneNumber, String message)
    {
        String SENT = "SMS_SENT";
        String DELIVERED = "SMS_DELIVERED";

        PendingIntent sentPI = PendingIntent.getBroadcast(this, 0,
                new Intent(SENT), 0);

        PendingIntent deliveredPI = PendingIntent.getBroadcast(this, 0,
                new Intent(DELIVERED), 0);

        //---when the SMS has been sent---
        registerReceiver(new BroadcastReceiver(){
            @Override
            public void onReceive(Context arg0, Intent arg1) {
                switch (getResultCode()) {
                    case Activity.RESULT_OK:
                        Toast.makeText(getBaseContext(), "SMS sent",
                                Toast.LENGTH_SHORT).show();
                        break;
                    case SmsManager.RESULT_ERROR_GENERIC_FAILURE:
                        Toast.makeText(getBaseContext(), "Generic failure",
                                Toast.LENGTH_SHORT).show();
                        break;
                    case SmsManager.RESULT_ERROR_NO_SERVICE:
                        Toast.makeText(getBaseContext(), "No service",
                                Toast.LENGTH_SHORT).show();
                        break;
                    case SmsManager.RESULT_ERROR_NULL_PDU:
                        Toast.makeText(getBaseContext(), "Null PDU",
                                Toast.LENGTH_SHORT).show();
                        break;
                    case SmsManager.RESULT_ERROR_RADIO_OFF:
                        Toast.makeText(getBaseContext(), "Radio off",
                                Toast.LENGTH_SHORT).show();
                        break;
                }
            }
        }, new IntentFilter(SENT));

        //---when the SMS has been delivered---
        registerReceiver(new BroadcastReceiver(){
            @Override
            public void onReceive(Context arg0, Intent arg1) {
                switch (getResultCode())
                {
                    case Activity.RESULT_OK:
                        Toast.makeText(getBaseContext(), "SMS delivered",
                                Toast.LENGTH_SHORT).show();
                        break;
                    case Activity.RESULT_CANCELED:
                        Toast.makeText(getBaseContext(), "SMS not delivered",
                                Toast.LENGTH_SHORT).show();
                        break;
                }
            }
        }, new IntentFilter(DELIVERED));

        SmsManager sms = SmsManager.getDefault();
        sms.sendTextMessage(phoneNumber, null, message, sentPI, deliveredPI);
    }
}
