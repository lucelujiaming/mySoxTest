package com.jiaming.mycompsciproblems;

import java.util.ArrayList;
import java.util.List;

// 所有聚类算法都是对数据点进行处理的，k均值算法也不例外。
public class DataPoint {
    // 维度值（numDimensions）。
    public final int numDimensions;
    // 构造函数接收初始数据的列表。这些维度稍后会被k均值替换成z-score，
    // 所以我们在originals中保留了初始数据的副本以便稍后进行打印。
    private List<Double> originals;
    // 列表dimensions使用double类型存储每个维度的实际值。
    // 当前的这个点进行过归一化的z-score值。也就是我们把全部的点都映射到一个
    // 以原点为中心所有坐标轴都位于(-1, 1)之间的空间中，所得到的坐标。
    public List<Double> dimensions;


    public DataPoint( List<Double> initials) {
        originals = initials;
        dimensions = new ArrayList<>(initials);
        numDimensions = dimensions.size();
    }
    // 计算任意两个同类型数据点之间的距离。
    // 计算距离的方式有很多种，但k均值算法最常用的就是欧氏距离。
    // 这是几何课程中我们最为熟悉的距离公式，由勾股定理推导而来。average
    public double distance(DataPoint other) {
        double differences = 0.0;
        // DataPoint所使用的版本要复杂一些，因为会涉及任意数量的维度。每个差值的平方会被累加，
        for (int i = 0; i < numDimensions; i++){
            double difference = dimensions.get(i) - other.dimensions.get(i);
            differences += Math.pow(difference, 2);
        }
        // distance()最终会返回这个累加和的平方根。
        return Math.sqrt(differences);
    }

    @Override
    public String toString() {
        return originals.toString();
    }
}

