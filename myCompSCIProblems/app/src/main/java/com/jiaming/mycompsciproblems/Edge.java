package com.jiaming.mycompsciproblems;

// Edge定义了两个顶点之间的连线（边），每个顶点都由一个整数索引表示。
// 这里按照惯例用u表示第一个顶点，v表示第二个顶点。也可以将u视为“起点”而v视为“终点”。
public class Edge {
    public final int u;
    public final int v;

    public Edge(int u, int v) {
        this.u = u;
        this.v = v;
    }

    public Edge reversed() {
        return new Edge(v, u);
    }

    @Override
    public String toString() {
        return u + " -> " + v;
    }
}

