package com.jiaming.mycompsciproblems;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

public class ConstraintProblemActivity extends AppCompatActivity implements View.OnClickListener{

    private Button btnColorMap;
    private Button btnEightQueens;
    private Button btnWordsSearch;
    private Button btnSendMoreMoney;

    private TextView textConstraintResult;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_constraint_problem);

        btnColorMap = (Button)findViewById(R.id.map_coloring);
        btnColorMap.setOnClickListener(this);
        btnEightQueens = (Button)findViewById(R.id.eight_queens);
        btnEightQueens.setOnClickListener(this);
        btnWordsSearch = (Button)findViewById(R.id.words_search);
        btnWordsSearch.setOnClickListener(this);
        btnSendMoreMoney = (Button)findViewById(R.id.send_more_money);
        btnSendMoreMoney.setOnClickListener(this);

        textConstraintResult = (TextView)findViewById(R.id.test_constraint_result);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.map_coloring:
                // 列出每一个地区。
                List<String> variables = Arrays.asList("Western Australia", "Northern Territory",
                        "South Australia", "Queensland", "New South Wales", "Victoria", "Tasmania");
                Map<String, List<String>> domains = new HashMap<>();
                for (String variable : variables) {
                    // 每一个地区都可以采用红绿蓝三种颜色。
                    domains.put(variable, Arrays.asList("red", "green", "blue"));
                }
                CSP<String, String> csp = new CSP<>(variables, domains);
                // 添加一系列的约束。每一条约束表示传入的这两个区域的颜色不能是一样 。
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
                    textConstraintResult.setText("No solution found!");
                } else {
                    System.out.println(solution);
                    String strOutResult = new String();
                    for (Map.Entry<String, String> entry : solution.entrySet()) {
                        strOutResult += entry.getKey() + "=" + entry.getValue() + "\n";
                    }
                    textConstraintResult.setText(strOutResult);
                }
                break;
            case R.id.eight_queens:
                // 列出每一个皇后。
                List<Integer> columns = Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8);
                Map<Integer, List<Integer>> rows = new HashMap<>();
                for (int column : columns) {
                    // 每一个地区都可以位于1 -8的位置。
                    rows.put(column, Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8));
                }
                CSP<Integer, Integer> cspEightQueens = new CSP<>(columns, rows);
                // 添加一条约束。
                cspEightQueens.addConstraint(new QueensConstraint(columns));
                Map<Integer, Integer> solutionEightQueens  = cspEightQueens.backtrackingSearch();
                if (solutionEightQueens == null) {
                    System.out.println("No solution found!");
                    textConstraintResult.setText("No solution found!");
                } else {
                    System.out.println(solutionEightQueens);
                    String strOutResult = new String();
                    for (Map.Entry<Integer, Integer> entry : solutionEightQueens.entrySet()) {
                        strOutResult += entry.getKey() + "=" + entry.getValue() + ", ";
                    }
                    textConstraintResult.setText(strOutResult);
                }
                break;
            case R.id.words_search:
                // 生成一个放置网状格子的对象。
                WordGrid grid = new WordGrid(9, 9);
                // 列出每一个单词。
                List<String> words = Arrays.asList("MATTHEW", "JOE", "MARY", "SARAH", "SALLY");
                // generate domains for all words
                Map<String, List<List<WordGrid.GridLocation>>> domainsWords = new HashMap<>();
                for (String word : words) {
                    // 每一个单词都可以位于的位置。
                    domainsWords.put(word, grid.generateDomain(word));
                }
                CSP<String, List<WordGrid.GridLocation>> cspWords = new CSP<>(words, domainsWords);
                // 添加一条约束。
                cspWords.addConstraint(new WordSearchConstraint(words));
                Map<String, List<WordGrid.GridLocation>> solutionWords  = cspWords.backtrackingSearch();

                if (solutionWords == null) {
                    System.out.println("No solution found!");
                    textConstraintResult.setText("No solution found!");
                } else {
                    Random random = new Random();
                    for (Map.Entry<String, List<WordGrid.GridLocation>> item : solutionWords.entrySet()) {
                        String word = item.getKey();
                        List<WordGrid.GridLocation> locations = item.getValue();
                        // random reverse half the time
                        if (random.nextBoolean()) {
                            Collections.reverse(locations);
                        }
                        grid.mark(word, locations);
                    }
                    System.out.println(grid);
                    textConstraintResult.setText(grid.toString());
                }
                break;
            case R.id.send_more_money:
                List<Character> letters = Arrays.asList('S', 'E', 'N', 'D', 'M', 'O', 'R', 'Y');
                Map<Character, List<Integer>> possibleDigits = new HashMap<>();
                for (Character letter : letters) {
                    possibleDigits.put(letter, Arrays.asList(0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
                }
                // 我们预先给字母M赋了值，这是为了确保M的解中不包含0，因为约束中没有规定数字不能从0开始。
                // so we don't get answers starting with a 0
                possibleDigits.replace('M', Arrays.asList(1));
                CSP<Character, Integer> cspSendMoreMoney = new CSP<>(letters, possibleDigits);
                cspSendMoreMoney.addConstraint(new SendMoreMoneyConstraint(letters));
                Map<Character, Integer> solutionSendMoreMoney = cspSendMoreMoney.backtrackingSearch();

                if (solutionSendMoreMoney == null) {
                    System.out.println("No solution found!");
                } else {
                    System.out.println(solutionSendMoreMoney);
                    String strOutResult = new String();
                    for (Map.Entry<Character, Integer> entry : solutionSendMoreMoney.entrySet()) {
                        strOutResult += entry.getKey() + "=" + entry.getValue() + ", ";
                    }
                    textConstraintResult.setText(strOutResult);
                }
                break;
        }

    }
}

