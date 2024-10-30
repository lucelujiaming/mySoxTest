package com.jiaming.mycompsciproblems;

import java.util.DoubleSummaryStatistics;
import java.util.LinkedList;
import java.util.List;

public final class Statistics {
    private List<Double> list;
    private DoubleSummaryStatistics dss;

    public Statistics(List<Double> list) {
        this.list = list;
        // dss = list.stream().collect(Collectors.summarizingDouble(d -> d));
        dss = new DoubleSummaryStatistics();
        for (Double item : list) {
            dss.accept(item);
        }
    }

    public double sum() {
        return dss.getSum();
    }

    public double mean() {
        return dss.getAverage();
    }
    // variance()返回总体方差。
    public double variance() {
        double mean = mean();
        // return list.stream().mapToDouble(x -> Math.pow(x - mean, 2)).average().getAsDouble();
        double doublePowSum = 0.0;
        for (Double item : list) {
            doublePowSum += (item - mean) * (item - mean);
        }
        return doublePowSum / list.size();
    }

    public double std() {
        return Math.sqrt(variance());
    }
    // 使不同类型的数据（本例中是不同的维度）具有可比性的过程被称为归一化过程。
    // 归一化常用的一种方法是基于其相较于同一类型的其他数据的z-score（也称为标准分数）来计算每个值的。
    // z-score的计算方法是：读取一个数据值，从中减去所有数据的均值，再除以所有数据的标准差。
    // zscored()会把列表中的每一项转换为对应的z-score，即原始值与均值的差除以标准差。
    public List<Double> zscored() {
        double mean = mean();
        double std = std();
        // return list.stream()
        //         .map(x -> std != 0 ? ((x - mean) / std) : 0.0)
        //         .collect(Collectors.toList());

        List<Double> listRet = new LinkedList<>();
        if (std != 0.0) {
            for (Double item : list) {
                listRet.add((item - mean)/std);
            }
        }
        else {
            for (Double item : list) {
                listRet.add(0.0);
            }
        }
        return listRet;
    }

    public double max() {
        return dss.getMax();
    }

    public double min() {
        return dss.getMin();
    }
}

