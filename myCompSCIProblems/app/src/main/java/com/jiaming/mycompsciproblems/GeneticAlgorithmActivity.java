package com.jiaming.mycompsciproblems;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.util.ArrayList;

public class GeneticAlgorithmActivity extends AppCompatActivity implements View.OnClickListener{

    private Button btnGeneticSimpleEquation;
    private Button btnSendMoreMoneyGenetic;
    private Button btnListCompression;

    private TextView textGeneticResult;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_genetic_algorithm);

        btnGeneticSimpleEquation = (Button)findViewById(R.id.genetic_simple_equation);
        btnGeneticSimpleEquation.setOnClickListener(this);
        btnSendMoreMoneyGenetic = (Button)findViewById(R.id.send_more_money_equation);
        btnSendMoreMoneyGenetic.setOnClickListener(this);
        btnListCompression = (Button)findViewById(R.id.test_list_compression);
        btnListCompression.setOnClickListener(this);

        textGeneticResult = (TextView)findViewById(R.id.test_genetic_result);
    }

    @Override
    public void onClick(View v) {
        final int POPULATION_SIZE = 20;
        final int GENERATIONS = 100;
        final double THRESHOLD = 13.0;
        switch (v.getId()) {
            case R.id.genetic_simple_equation:
                ArrayList<SimpleEquation> initialSimpleEquationPopulation = new ArrayList<>();
                for (int i = 0; i < POPULATION_SIZE; i++) {
                    initialSimpleEquationPopulation.add(SimpleEquation.randomInstance());
                }
                GeneticAlgorithm<SimpleEquation> gaSimpleEquation = new GeneticAlgorithm<>(
                        initialSimpleEquationPopulation,
                        0.1, 0.7, GeneticAlgorithm.SelectionType.TOURNAMENT);
                SimpleEquation result = gaSimpleEquation.run(GENERATIONS, THRESHOLD);
                System.out.println(result);
                textGeneticResult.setText(result.toString());
                break;
            case R.id.send_more_money_equation:
                ArrayList<SendMoreMoneyGenetic> initialSendMoreMoneyPopulation = new ArrayList<>();
                for (int i = 0; i < POPULATION_SIZE; i++) {
                    initialSendMoreMoneyPopulation.add(SendMoreMoneyGenetic.randomInstance());
                }
                GeneticAlgorithm<SendMoreMoneyGenetic> gaSendMoreMoneyGenetic = new GeneticAlgorithm<>(
                        initialSendMoreMoneyPopulation,
                        0.2, 0.7, GeneticAlgorithm.SelectionType.ROULETTE);
                SendMoreMoneyGenetic resultSendMoreMoney = gaSendMoreMoneyGenetic.run(GENERATIONS, THRESHOLD);
                System.out.println(resultSendMoreMoney);
                textGeneticResult.setText(resultSendMoreMoney.toString());
                break;
            case R.id.test_list_compression:
                ListCompression originalOrder = new ListCompression(ListCompression.ORIGINAL_LIST);
                System.out.println(originalOrder);
                ArrayList<ListCompression> initialListCompressionPopulation = new ArrayList<>();
                for (int i = 0; i < POPULATION_SIZE; i++) {
                    initialListCompressionPopulation.add(ListCompression.randomInstance());
                }
                GeneticAlgorithm<ListCompression> ga = new GeneticAlgorithm<>(
                        initialListCompressionPopulation,
                        0.2, 0.7, GeneticAlgorithm.SelectionType.TOURNAMENT);
                ListCompression resultListCompression = ga.run(GENERATIONS, THRESHOLD);
                System.out.println(resultListCompression);
                textGeneticResult.setText(resultListCompression.toString());
                break;
        }
    }
}

