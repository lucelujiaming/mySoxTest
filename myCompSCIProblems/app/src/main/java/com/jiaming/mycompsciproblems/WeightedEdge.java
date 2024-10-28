package com.jiaming.mycompsciproblems;

public class WeightedEdge extends Edge implements Comparable<WeightedEdge> {
    // 增加了一个weight属性并通过compareTo()
    public final double weight;

    public WeightedEdge(int u, int v, double weight) {
        super(u, v);
        this.weight = weight;
    }

    @Override
    public WeightedEdge reversed() {
        return new WeightedEdge(v, u, weight);
    }

    // 实现了Comparable接口，使得两个WeightedEdge实例之间可以进行比较。
    @Override
    public int compareTo(WeightedEdge other) {
        Double mine = weight;
        Double theirs = other.weight;
        return mine.compareTo(theirs);
    }

    @Override
    public String toString() {
        return u + " -" + weight + "-> " + v;
    }
}

