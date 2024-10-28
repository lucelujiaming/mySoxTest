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
        }
    }
}
