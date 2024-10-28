package com.jiaming.mycompsciproblems;

import java.util.List;

public class UnweightedGraph<V> extends Graph<V , Edge> {
    public UnweightedGraph(List<V> vertices) {
        super(vertices);
    }
    public String  strGoal = new String("");

    public void addEdge(Edge edge) {
        // 首先将一条边添加到顶点u的邻接表中，
        // 接着将该条边反转，然后添加到顶点v的邻接表中。
        edges.get(edge.u).add(edge);
        edges.get(edge.v).add(edge.reversed());
        // 因为我们使用的是无向图，所以需要添加反转后的边。
        // 我们需要在两个方向上添加每条边，这代表u是v的相邻顶点，同样v也是u的相邻顶点。
        // 为了便于记忆，你可以把无向图看成双向的，这也说明可以在任意方向上遍历每条边。
    }

    public void addEdge(int u, int v) {
        addEdge(new Edge(u, v));
    }

    public void addEdge(V first, V second) {
        addEdge(new Edge(indexOf(first), indexOf(second)));
    }

    public void setStrGoal(String goal) {
        strGoal = goal;
    }
    public boolean goalTest(String strLocation) {
        if(strLocation.equals(strGoal))
            return true;
        else
            return false;
    }
}

