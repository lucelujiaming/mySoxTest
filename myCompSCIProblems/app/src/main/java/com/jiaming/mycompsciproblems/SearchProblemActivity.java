package com.jiaming.mycompsciproblems;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.TextUtils;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;

public class SearchProblemActivity extends AppCompatActivity  implements View.OnClickListener{
    // fib components
    private EditText editGeneInfo;
    private EditText editGeneKey;
    private Button btnSearchGene;
    private TextView textTestResult;

    private Button btnMazeGene;
    private Button btnDfsSearch;
    private Button btnBfsSearch;
    private Button btnEuclideaneAStarGene;
    private Button btnManhattanAStarGene;
    private Button btnClearPath;
    private TextView textPathSteps;
    private MyTilesView viewMazeResult;

    private Button btnMissionaries;
    private TextView textMissionariesResult;

    private Maze objMaze;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_search_problem);

        editGeneInfo = (EditText)findViewById(R.id.gene_infomation);
        editGeneKey = (EditText)findViewById(R.id.gene_key);
        btnSearchGene = (Button)findViewById(R.id.search_gene);
        btnSearchGene.setOnClickListener(this);
        textTestResult = (TextView)findViewById(R.id.test_result);

        btnMazeGene = (Button)findViewById(R.id.create_maze);
        btnMazeGene.setOnClickListener(this);
        btnDfsSearch  = (Button)findViewById(R.id.dfs_search_maze);
        btnDfsSearch.setOnClickListener(this);
        btnBfsSearch  = (Button)findViewById(R.id.bfs_search_maze);
        btnBfsSearch.setOnClickListener(this);
        btnEuclideaneAStarGene  = (Button)findViewById(R.id.euclidean_astar_search_maze);
        btnEuclideaneAStarGene.setOnClickListener(this);
        btnManhattanAStarGene  = (Button)findViewById(R.id.manhattan_astar_search_maze);
        btnManhattanAStarGene.setOnClickListener(this);
        btnClearPath  = (Button)findViewById(R.id.clear_maze_path);
        btnClearPath.setOnClickListener(this);
        textPathSteps = (TextView)findViewById(R.id.path_steps);
        viewMazeResult = (MyTilesView)findViewById(R.id.maze_view);

        btnMissionaries  = (Button)findViewById(R.id.missionaries_problem);
        btnMissionaries.setOnClickListener(this);
        textMissionariesResult = (TextView)findViewById(R.id.missionaries_result);

        editGeneInfo.setText("ACGTGGCTCTCTAACGTACGTACGTACGGGGTTTATATATACCCTAGGACTCCCTTT");
        editGeneKey.setText("ACG");

        objMaze = null;
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.search_gene:
                if (TextUtils.isEmpty(editGeneInfo.getText().toString())){
                    Toast.makeText(SearchProblemActivity.this, "请输入需要被查找的内容!", Toast.LENGTH_SHORT).show();
                    return;
                }
                if (TextUtils.isEmpty(editGeneKey.getText().toString())){
                    Toast.makeText(SearchProblemActivity.this, "请输入需要查找的内容!", Toast.LENGTH_SHORT).show();
                    return;
                }
                Gene myGene = new Gene(editGeneInfo.getText().toString());
                Gene.Codon geneKey = new Gene.Codon(editGeneKey.getText().toString());
                String strResult = "";
                if(myGene.linearContains(geneKey))
                {
                    strResult = "linearContains OK ";
                }
                else
                {
                    strResult = "linearContains NG ";
                }
                if(myGene.binaryContains(geneKey))
                {
                    strResult += " and binaryContains OK ";
                }
                else
                {
                    strResult += " and binaryContains NG ";
                }
                textTestResult.setText(strResult);
                break;
            case R.id.create_maze:
                objMaze = new Maze(25, 25, new Maze.MazeLocation(0, 0), new Maze.MazeLocation(24, 24), 0.3);
                System.out.println(objMaze);
                viewMazeResult.setTilesGrid(objMaze.getGrid(), objMaze.getRows(), objMaze.getColumns());
                viewMazeResult.invalidate();
                break;
            case R.id.dfs_search_maze:
                if (objMaze == null)
                {
                    break;
                }
                GenericSearch.Node<Maze.MazeLocation> solution1 = GenericSearch.dfs(objMaze.getStart(), objMaze::goalTest, objMaze::successors);
                if (solution1 == null) {
                    System.out.println("No solution found using depth-first search!");
                    textPathSteps.setText("No solution found using depth-first search!");
                } else {
                    List<Maze.MazeLocation> path1 = GenericSearch.nodeToPath(solution1);
                    objMaze.mark(path1);
                    System.out.println("Found a solution found using depth-first search!");
                    System.out.println(objMaze);
                    viewMazeResult.setTilesGrid(objMaze.getGrid(), objMaze.getRows(), objMaze.getColumns());
                    viewMazeResult.invalidate();
                    textPathSteps.setText(String.valueOf("depth-first search needs" + path1.size()));
                    objMaze.clear(path1);
                }
                break;
            case R.id.bfs_search_maze:
                if (objMaze == null)
                {
                    break;
                }
                GenericSearch.Node<Maze.MazeLocation> solution2 = GenericSearch.bfs(objMaze.getStart(), objMaze::goalTest, objMaze::successors);
                if (solution2 == null) {
                    System.out.println("No solution found using breadth-first search!");
                    textPathSteps.setText("No solution found using breadth-first search!");
                } else {
                    List<Maze.MazeLocation> path2 = GenericSearch.nodeToPath(solution2);
                    objMaze.mark(path2);
                    System.out.println("Found a solution found using breadth-first search!");
                    System.out.println(objMaze);
                    viewMazeResult.setTilesGrid(objMaze.getGrid(), objMaze.getRows(), objMaze.getColumns());
                    viewMazeResult.invalidate();
                    textPathSteps.setText(String.valueOf("breadth-first search needs" + path2.size()));
                    objMaze.clear(path2);
                }
                break;
            case R.id.euclidean_astar_search_maze:
                if (objMaze == null)
                {
                    break;
                }
                GenericSearch.Node<Maze.MazeLocation> solution3 = GenericSearch.astar(objMaze.getStart(), objMaze::goalTest, objMaze::successors, objMaze::euclideanDistance);
                if (solution3 == null) {
                    System.out.println("No solution found using A*!");
                    textPathSteps.setText("No solution found using euclideanDistance A*!");
                } else {
                    List<Maze.MazeLocation> path3 = GenericSearch.nodeToPath(solution3);
                    objMaze.mark(path3);
                    System.out.println("Found a solution found using A*!");
                    System.out.println(objMaze);
                    viewMazeResult.setTilesGrid(objMaze.getGrid(), objMaze.getRows(), objMaze.getColumns());
                    viewMazeResult.invalidate();
                    textPathSteps.setText("euclideanDistance A* search needs" + String.valueOf(path3.size()));
                    objMaze.clear(path3);
                }
                break;
            case R.id.manhattan_astar_search_maze:
                if (objMaze == null)
                {
                    break;
                }
                GenericSearch.Node<Maze.MazeLocation> solution4 = GenericSearch.astar(objMaze.getStart(), objMaze::goalTest, objMaze::successors, objMaze::manhattanDistance);
                if (solution4 == null) {
                    System.out.println("No solution found using A*!");
                    textPathSteps.setText("No solution found using manhattanDistance A*!");
                } else {
                    List<Maze.MazeLocation> path4 = GenericSearch.nodeToPath(solution4);
                    objMaze.mark(path4);
                    System.out.println("Found a solution found using A*!");
                    System.out.println(objMaze);
                    viewMazeResult.setTilesGrid(objMaze.getGrid(), objMaze.getRows(), objMaze.getColumns());
                    viewMazeResult.invalidate();
                    textPathSteps.setText("manhattanDistance A* search needs" + String.valueOf(path4.size()));
                    objMaze.clear(path4);
                }
                break;
            case R.id.clear_maze_path:
                textPathSteps.setText("Path cleared");
                viewMazeResult.setTilesGrid(objMaze.getGrid(), objMaze.getRows(), objMaze.getColumns());
                viewMazeResult.invalidate();
                break;
            case R.id.missionaries_problem:
                MCState start = new MCState(MCState.MAX_NUM, MCState.MAX_NUM, true);
                GenericSearch.Node<MCState> solution = GenericSearch.bfs(start, MCState::goalTest, MCState::successors);
                if (solution == null) {
                    System.out.println("No solution found!");
                } else {
                    List<MCState> path = GenericSearch.nodeToPath(solution);
                    start.displaySolution(path);
                }
                break;
        }
    }
}

