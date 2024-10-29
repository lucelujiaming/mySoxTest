package com.jiaming.mycompsciproblems;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.util.Arrays;
import java.util.List;
import java.util.Map;

public class RailwayGraphActivity extends AppCompatActivity implements View.OnClickListener {

    private Button btnPrintRailwayGraph;
    private Button btnBfsRailwayGraph;

    private Button btnPrintWeightRailwayGraph;
    private Button btnJarnikWeightRailwayGraph;
    private Button btnDijkstraWeightRailwayGraph;

    private TextView textRailwayGraphResult;

    UnweightedGraph<String> cityGraph = null;
    WeightedGraph<String> cityWeightGraph = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_railway_graph);

        btnPrintRailwayGraph = (Button)findViewById(R.id.print_railway_graph);
        btnPrintRailwayGraph.setOnClickListener(this);
        btnBfsRailwayGraph = (Button)findViewById(R.id.bfs_railway_graph);
        btnBfsRailwayGraph.setOnClickListener(this);

        btnPrintWeightRailwayGraph = (Button)findViewById(R.id.print_weight_railway_graph);
        btnPrintWeightRailwayGraph.setOnClickListener(this);
        btnJarnikWeightRailwayGraph = (Button)findViewById(R.id.jarnik_weight_railway_graph);
        btnJarnikWeightRailwayGraph.setOnClickListener(this);
        btnDijkstraWeightRailwayGraph = (Button)findViewById(R.id.dijkstra_weight_railway_graph);
        btnDijkstraWeightRailwayGraph.setOnClickListener(this);

        textRailwayGraphResult = (TextView)findViewById(R.id.test_railway_graph_result);
    }

    @Override
    public void onClick(View v) {
        String strOutResult = new String();
        switch (v.getId()) {
            case R.id.print_railway_graph:
                // Represents the 15 largest MSAs in the United States
                cityGraph = new UnweightedGraph<>(
                        Arrays.asList("Seattle", "San Francisco", "Los Angeles", "Riverside", "Phoenix", "Chicago", "Boston",
                                "New York", "Atlanta", "Miami", "Dallas", "Houston", "Detroit", "Philadelphia", "Washington"));

                cityGraph.addEdge("Seattle", "Chicago");
                cityGraph.addEdge("Seattle", "San Francisco");
                cityGraph.addEdge("San Francisco", "Riverside");
                cityGraph.addEdge("San Francisco", "Los Angeles");
                cityGraph.addEdge("Los Angeles", "Riverside");
                cityGraph.addEdge("Los Angeles", "Phoenix");
                cityGraph.addEdge("Riverside", "Phoenix");
                cityGraph.addEdge("Riverside", "Chicago");
                cityGraph.addEdge("Phoenix", "Dallas");
                cityGraph.addEdge("Phoenix", "Houston");
                cityGraph.addEdge("Dallas", "Chicago");
                cityGraph.addEdge("Dallas", "Atlanta");
                cityGraph.addEdge("Dallas", "Houston");
                cityGraph.addEdge("Houston", "Atlanta");
                cityGraph.addEdge("Houston", "Miami");
                cityGraph.addEdge("Atlanta", "Chicago");
                cityGraph.addEdge("Atlanta", "Washington");
                cityGraph.addEdge("Atlanta", "Miami");
                cityGraph.addEdge("Miami", "Washington");
                cityGraph.addEdge("Chicago", "Detroit");
                cityGraph.addEdge("Detroit", "Boston");
                cityGraph.addEdge("Detroit", "Washington");
                cityGraph.addEdge("Detroit", "New York");
                cityGraph.addEdge("Boston", "New York");
                cityGraph.addEdge("New York", "Philadelphia");
                cityGraph.addEdge("Philadelphia", "Washington");
                System.out.println(cityGraph.toString());
                strOutResult =  cityGraph.toString();
                textRailwayGraphResult.setText(strOutResult);
                break;
            case R.id.bfs_railway_graph:
                if(cityGraph == null)
                {
                    break;
                }
                // GenericSearch.Node<String> bfsResult = GenericSearch.bfs("Boston",
                //         v -> v.equals("Miami"),
                //         cityGraph::neighborsOf);
                cityGraph.setStrGoal("Miami");
                GenericSearch.Node<String> bfsResult = GenericSearch.bfs("Boston",
                        cityGraph::goalTest,
                        cityGraph::neighborsOf);
                if (bfsResult == null) {
                    System.out.println("No solution found using breadth-first search!");
                    strOutResult = "No solution found using breadth-first search!";
                } else {
                    List<String> path = GenericSearch.nodeToPath(bfsResult);
                    System.out.println("Path from Boston to Miami:");
                    System.out.println(path);
                    strOutResult = "Path from Boston to Miami:\n";
                    for (String entry : path) {
                        strOutResult += " -> " + entry + "\n";
                    }
                }
                textRailwayGraphResult.setText(strOutResult);
                break;
            case R.id.print_weight_railway_graph:
                // Represents the 15 largest MSAs in the United States
                cityWeightGraph = new WeightedGraph<>(
                        Arrays.asList("Seattle", "San Francisco", "Los Angeles", "Riverside", "Phoenix", "Chicago", "Boston",
                                "New York", "Atlanta", "Miami", "Dallas", "Houston", "Detroit", "Philadelphia", "Washington"));

                cityWeightGraph.addEdge("Seattle", "Chicago", 1737);
                cityWeightGraph.addEdge("Seattle", "San Francisco", 678);
                cityWeightGraph.addEdge("San Francisco", "Riverside", 386);
                cityWeightGraph.addEdge("San Francisco", "Los Angeles", 348);
                cityWeightGraph.addEdge("Los Angeles", "Riverside", 50);
                cityWeightGraph.addEdge("Los Angeles", "Phoenix", 357);
                cityWeightGraph.addEdge("Riverside", "Phoenix", 307);
                cityWeightGraph.addEdge("Riverside", "Chicago", 1704);
                cityWeightGraph.addEdge("Phoenix", "Dallas", 887);
                cityWeightGraph.addEdge("Phoenix", "Houston", 1015);
                cityWeightGraph.addEdge("Dallas", "Chicago", 805);
                cityWeightGraph.addEdge("Dallas", "Atlanta", 721);
                cityWeightGraph.addEdge("Dallas", "Houston", 225);
                cityWeightGraph.addEdge("Houston", "Atlanta", 702);
                cityWeightGraph.addEdge("Houston", "Miami", 968);
                cityWeightGraph.addEdge("Atlanta", "Chicago", 588);
                cityWeightGraph.addEdge("Atlanta", "Washington", 543);
                cityWeightGraph.addEdge("Atlanta", "Miami", 604);
                cityWeightGraph.addEdge("Miami", "Washington", 923);
                cityWeightGraph.addEdge("Chicago", "Detroit", 238);
                cityWeightGraph.addEdge("Detroit", "Boston", 613);
                cityWeightGraph.addEdge("Detroit", "Washington", 396);
                cityWeightGraph.addEdge("Detroit", "New York", 482);
                cityWeightGraph.addEdge("Boston", "New York", 190);
                cityWeightGraph.addEdge("New York", "Philadelphia", 81);
                cityWeightGraph.addEdge("Philadelphia", "Washington", 123);

                System.out.println(cityWeightGraph);
                strOutResult = cityWeightGraph.toString();
                textRailwayGraphResult.setText(strOutResult);
                break;
            case R.id.jarnik_weight_railway_graph:
                if(cityWeightGraph == null)
                {
                    break;
                }
                List<WeightedEdge> mst = cityWeightGraph.mst(0);
                strOutResult = "Run mst : ";
                strOutResult += cityWeightGraph.printWeightedPath(mst);

                textRailwayGraphResult.setText(strOutResult);
                break;
            case R.id.dijkstra_weight_railway_graph:
                if(cityWeightGraph == null)
                {
                    break;
                }
                WeightedGraph.DijkstraResult dijkstraResult = cityWeightGraph.dijkstra("Los Angeles");
                Map<String, Double> nameDistance = cityWeightGraph.distanceArrayToDistanceMap(dijkstraResult.distances);
                System.out.println("Distances from Los Angeles:");
                strOutResult = "Distances from Los Angeles: \n";
                nameDistance.forEach((name, distance) -> System.out.println(name + " : " + distance));
                for (Map.Entry<String, Double> entry : nameDistance.entrySet()) {
                    strOutResult += entry.getKey() + " : " + entry.getValue() + "\n";
                }

                System.out.println(); // spacing

                System.out.println("Shortest path from Los Angeles to Boston: ");
                strOutResult += "\nShortest path from Los Angeles to Boston: \n";
                List<WeightedEdge> path = cityWeightGraph.pathMapToPath(cityWeightGraph.indexOf("Los Angeles"), cityWeightGraph.indexOf("Boston"),
                        dijkstraResult.pathMap);
                strOutResult += cityWeightGraph.printWeightedPath(path);
                textRailwayGraphResult.setText(strOutResult);
                break;
        }
    }
}
