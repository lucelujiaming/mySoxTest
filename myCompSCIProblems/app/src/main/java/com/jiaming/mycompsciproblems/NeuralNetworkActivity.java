package com.jiaming.mycompsciproblems;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class NeuralNetworkActivity extends AppCompatActivity  implements View.OnClickListener{

    private Button btnClassifyIris;
    private Button btnClassifyWines;

    private TextView textNeuralNetworkResult;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_neural_network);

        btnClassifyIris = (Button)findViewById(R.id.test_classify_iris);
        btnClassifyIris.setOnClickListener(this);
        btnClassifyWines = (Button)findViewById(R.id.test_classify_wines);
        btnClassifyWines.setOnClickListener(this);

        textNeuralNetworkResult = (TextView)findViewById(R.id.test_neural_network_result);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.test_classify_iris:
                IrisTest irisTest = new IrisTest();
                Network<String>.Results results = irisTest.classify();
                System.out.println(results.correct + " correct of " + results.trials + " = " +
                        results.percentage * 100 + "%");

                String strKMeansOutResult = new String();
                strKMeansOutResult = results.correct + " correct of " + results.trials + " = " +
                        results.percentage * 100 + "%";
                textNeuralNetworkResult.setText(strKMeansOutResult);
                break;
        }
    }
}
