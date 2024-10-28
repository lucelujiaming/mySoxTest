package com.jiaming.mycompsciproblems;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ConstraintProblemActivity extends AppCompatActivity implements View.OnClickListener{

    private Button btnColorMap;

    private TextView textMapTestResult;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_constraint_problem);

        btnColorMap = (Button)findViewById(R.id.map_coloring);
        btnColorMap.setOnClickListener(this);

        textMapTestResult = (TextView)findViewById(R.id.test_map_color_result);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.map_coloring:
                List<String> variables = Arrays.asList("Western Australia", "Northern Territory",
                        "South Australia", "Queensland", "New South Wales", "Victoria", "Tasmania");
                Map<String, List<String>> domains = new HashMap<>();
                for (String variable : variables) {
                    domains.put(variable, Arrays.asList("red", "green", "blue"));
                }
                CSP<String, String> csp = new CSP<>(variables, domains);
                csp.addConstraint(new MapColoringConstraint("Western Australia", "Northern Territory"));
                csp.addConstraint(new MapColoringConstraint("Western Australia", "South Australia"));
                csp.addConstraint(new MapColoringConstraint("South Australia", "Northern Territory"));
                csp.addConstraint(new MapColoringConstraint("Queensland", "Northern Territory"));
                csp.addConstraint(new MapColoringConstraint("Queensland", "South Australia"));
                csp.addConstraint(new MapColoringConstraint("Queensland", "New South Wales"));
                csp.addConstraint(new MapColoringConstraint("New South Wales", "South Australia"));
                csp.addConstraint(new MapColoringConstraint("Victoria", "South Australia"));
                csp.addConstraint(new MapColoringConstraint("Victoria", "New South Wales"));
                csp.addConstraint(new MapColoringConstraint("Victoria", "Tasmania"));
                Map<String, String> solution = csp.backtrackingSearch();
                if (solution == null) {
                    System.out.println("No solution found!");
                } else {
                    System.out.println(solution);
                    String strOutResult = new String();
                    for (Map.Entry<String, String> entry : solution.entrySet()) {
                        strOutResult += entry.getKey() + "=" + entry.getValue() + "\n";
                    }
                    textMapTestResult.setText(strOutResult);
                }
                break;
        }

    }
}
