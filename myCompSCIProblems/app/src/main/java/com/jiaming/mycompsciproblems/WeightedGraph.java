package com.jiaming.mycompsciproblems;

import java.util.List;

public class WeightedGraph<V> extends Graph<V, WeightedEdge> {
    public WeightedGraph(List<V> vertices) {
        super(vertices);
    }

    public void addEdge(WeightedEdge edge) {
        // 首先将一条边添加到顶点u的邻接表中，
        // 接着将该条边反转，然后添加到顶点v的邻接表中。
        edges.get(edge.u).add(edge);
        edges.get(edge.v).add(edge.reversed());

    }

    public void addEdge(int u, int v, float weight) {
        addEdge(new WeightedEdge(u, v, weight));
    }

    public void addEdge(V first, V second, float weight) {
        addEdge(new WeightedEdge(indexOf(first), indexOf(second), weight));
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < getVertexCount(); i++) {
            sb.append(vertexAt(i));
            sb.append(" -> ");
            // sb.append(Arrays.toString(edgesOf(i).stream()
            //         .map(we -> "(" + vertexAt(we.v) + ", " + we.weight + ")").toArray()));
            List<WeightedEdge> objWeightedEdge = edgesOf(i);
            for (WeightedEdge edge : objWeightedEdge){
                sb.append("(" + vertexAt(edge.v) + ", " + edge.weight + ")");
            }
            sb.append(System.lineSeparator());
        }
        return sb.toString();
    }

    public static double totalWeight(List<WeightedEdge> path) {
        int sumPath = 0;
        for (WeightedEdge item : path) {
            sumPath += item.weight;
        }
        return sumPath;
    }

   //  public List<WeightedEdge> mst(int start) {
   //
   //  }
}
