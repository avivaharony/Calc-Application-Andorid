package com.example.calcapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.text.InputType;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.math.BigDecimal;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("calc-lib");
    }



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        EditText editText1= (EditText) findViewById(R.id.editTextEquation);
        editText1.setInputType(InputType.TYPE_NULL);
        
        createCalculatorJNI();

        Log.e("Calc. onCreate AViv: ", "onCreate1");


    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        destroyCalculatorJNI();
        Log.e("Calc. onDestroy AViv: ", "onDestroy1");
    }

    public native void createCalculatorJNI();
    public native void destroyCalculatorJNI();

    public native CalculatorObject calcEquationWithObjectJNI(String equation);


    public void CalculateEquation(View view) {

        EditText equationView = findViewById(R.id.editTextEquation);

        CalculatorObject resultAndStatus = calcEquationWithObjectJNI(String.valueOf(equationView.getText()));

        int scale = 2;
        String resultString = BigDecimal.valueOf(resultAndStatus.result).setScale(scale, BigDecimal.ROUND_HALF_UP).toPlainString();

        equationView.setText(resultString);
        TextView resultView = findViewById(R.id.textview1);

        if (resultAndStatus.returnStatus.equals("SUCCESS")){
            resultView.setText(resultString);
        } else {
            resultView.setText(resultAndStatus.returnStatus);

        }

    }




    //@RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR1)
    public void WriteEquationChar(View view) {

        EditText equationView = findViewById(R.id.editTextEquation);
        String currentPressedLetter = ((Button)view).getText().toString();
        String currentResult = equationView.getText().toString();

        equationView.setText(String.format("%s%s", currentResult, currentPressedLetter));

    }

    public void DeleteEquationChar(View view) {

        EditText equationView = (EditText)findViewById(R.id.editTextEquation);
        String currentEquation = equationView.getText().toString();

        Log.w("currentResult AViv: ", currentEquation);

        int equationLength = currentEquation.length();

        if (equationLength > 0) {
            String cuttedEquation = currentEquation.substring(0,equationLength - 1);
            equationView.setText(cuttedEquation);
        }

    }

    public void ClearEquation(View view) {
        EditText equationView = (EditText)findViewById(R.id.editTextEquation);
        equationView.setText("");
    }
}




class CalculatorObject {

    double result;
    String returnStatus;


    public CalculatorObject(double result, String returnStatus){
        this.result = result;
        this.returnStatus = returnStatus;
    }

}
