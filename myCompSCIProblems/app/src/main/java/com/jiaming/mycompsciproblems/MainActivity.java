package com.jiaming.mycompsciproblems;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity  implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button btnSimpleCalc = (Button)findViewById(R.id.simple_problem);
        btnSimpleCalc.setOnClickListener(this);
        Button btnSearch = (Button)findViewById(R.id.search_problem);
        btnSearch.setOnClickListener(this);
        Button btnConstraint = (Button)findViewById(R.id.constraint_problem);
        btnConstraint.setOnClickListener(this);
        Button btnGraph = (Button)findViewById(R.id.graph_problem);
        btnGraph.setOnClickListener(this);
        Button btnGenetic = (Button)findViewById(R.id.genetic_problem);
        btnGenetic.setOnClickListener(this);
        Button btnKMeans = (Button)findViewById(R.id.kmeans_clustering);
        btnKMeans.setOnClickListener(this);

    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.simple_problem:
                // 创建需要启动的Activity对应的Intent
                Intent intentSimpleMath = new Intent(MainActivity.this,SimpleMathActivity.class);
                // 启动intent对应的Activity
                startActivity(intentSimpleMath);
                break;
            case R.id.search_problem:
                // 创建需要启动的Activity对应的Intent
                Intent intentSearchProblem = new Intent(MainActivity.this,SearchProblemActivity.class);
                // 启动intent对应的Activity
                startActivity(intentSearchProblem);
                break;
            case R.id.constraint_problem:
                // 创建需要启动的Activity对应的Intent
                Intent intentConstraintProblem = new Intent(MainActivity.this,ConstraintProblemActivity.class);
                // 启动intent对应的Activity
                startActivity(intentConstraintProblem);
                break;
            case R.id.graph_problem:
                // 创建需要启动的Activity对应的Intent
                Intent intentGraphProblem = new Intent(MainActivity.this,RailwayGraphActivity.class);
                // 启动intent对应的Activity
                startActivity(intentGraphProblem);
                break;
            case R.id.genetic_problem:
                // 创建需要启动的Activity对应的Intent
                Intent intentGeneticProblem = new Intent(MainActivity.this,GeneticAlgorithmActivity.class);
                // 启动intent对应的Activity
                startActivity(intentGeneticProblem);
                break;
            case R.id.kmeans_clustering:
                // 创建需要启动的Activity对应的Intent
                Intent intentKMeansProblem = new Intent(MainActivity.this,KMeansActivity.class);
                // 启动intent对应的Activity
                startActivity(intentKMeansProblem);
                break;
        }
    }
}

