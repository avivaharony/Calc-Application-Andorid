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



/*
        public native double calcEquationJNI2(String equation);
        public native Calculater2 createCalc();
        public native DestroyCalc();

    }
*/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        EditText editText1= (EditText) findViewById(R.id.editTextEquation);
        editText1.setInputType(InputType.TYPE_NULL);
        
        createCalculatorJNI();

        Log.e("Calc. onCreate AViv: ", "onCreate1");
        
     /*
        Calculater myCalc = new Calculater();
      */

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        destroyCalculatorJNI();
        Log.e("Calc. onDestroy AViv: ", "onDestroy1");
    }

    public native String stringFromJNI();

    public native void createCalculatorJNI();
    public native void destroyCalculatorJNI();

    public native double calcEquationJNI(String equation);

    public native CalculatorObject calcEquationWithObjectJNI(String equation);
/*
    public void CalculateEquation(View view) {

        EditText equationView = findViewById(R.id.editTextEquation);

        Double result = calcEquationJNI(String.valueOf(equationView.getText()));

        TextView resultView = findViewById(R.id.textview1);
        resultView.setText(result.toString());
    }

*/
//Backup:
/*
    public void CalculateEquation(View view) {

        EditText equationView = findViewById(R.id.editTextEquation);

        double result = calcEquationJNI(String.valueOf(equationView.getText()));

        int scale = 2;
        String resultString = BigDecimal.valueOf(result).setScale(scale, BigDecimal.ROUND_HALF_UP).toPlainString();

        //   TextView resultView = findViewById(R.id.textview1);
        equationView.setText(resultString);
        TextView resultView = findViewById(R.id.textview1);
        resultView.setText(resultString);
    }
*/

    public void CalculateEquation(View view) {

        EditText equationView = findViewById(R.id.editTextEquation);

        CalculatorObject resultAndStatus = calcEquationWithObjectJNI(String.valueOf(equationView.getText()));

        int scale = 2;
        String resultString = BigDecimal.valueOf(resultAndStatus.result).setScale(scale, BigDecimal.ROUND_HALF_UP).toPlainString();

        //   TextView resultView = findViewById(R.id.textview1);
        equationView.setText(resultString);
        TextView resultView = findViewById(R.id.textview1);
     //   resultView.setText(resultString);

        if (resultAndStatus.returnStatus.equals("SUCCESS")){
            resultView.setText(resultString);
        } else {
            resultView.setText(resultAndStatus.returnStatus);

        }
/*
        if (resultAndStatus.returnStatus.equals("SUCCESS")){
            resultView.setText(resultString);
        } else {
            resultView.setText(Integer.valueOf(resultAndStatus.returnStatus).toString());

        }
*/
    }




    //@RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR1)
    public void WriteEquationChar(View view) {
 //       TextView resultView = findViewById(R.id.textview1);

        EditText equationView = findViewById(R.id.editTextEquation);
   //     resultView.setText(result.toString());

  //      EditText equationView = findViewById(R.id.editTextEquation);
     //   Log.w("WriteEquationChar - Aviv ", view.getDisplay().getName().toString());
        String currentPressedLetter = ((Button)view).getText().toString();
        String currentResult = equationView.getText().toString();

        equationView.setText(currentResult + currentPressedLetter);

   //     equationView.setText(equationView.getText() + "(" , );
 //       equationView.se

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

      //  Log.w("newResult Aviv : ", cuttedEquation);
    }

    public void ClearEquation(View view) {
        EditText equationView = (EditText)findViewById(R.id.editTextEquation);
        equationView.setText("");
    }
}


/*
calc_t *CalcCreate(void);

enum return_status CalcCalc(calc_t *calc, double *result, const char *equation);

    void CalcDestroy(calc_t *calc);
 */



class CalculatorObject {

    double result;
   // int returnStatus;
    String returnStatus;


    public CalculatorObject(double result, String returnStatus){
        this.result = result;
        this.returnStatus = returnStatus;
    }
    /*
    public CalculatorObject(double result, int returnStatus){
        this.result = result;
        this.returnStatus = returnStatus;
    }

     */
}
