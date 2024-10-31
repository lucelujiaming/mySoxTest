package com.jiaming.mycompsciproblems;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;

public final class Util {
    public static double doProduct(double[] xs, double[] ys) {
        double sum = 0.0;
        for (int i = 0 ; i < xs.length; i++) {
            sum += xs[i] * ys[i];
        }
        return sum;
    }

    // Sigmoid函数： f(x) = 1.0 / (1.0 + e^(-z))
    public static double sigmoid(double x) {
        return 1.0 / (1.0 + Math.exp(-x));
    }

    // Sigmoid函数的导数
    public static double derivativeSigmod(double x) {
        double sigmoid = sigmoid(x);
        return sigmoid * (1 - sigmoid);
    }

    // 对于即将被处理的两个数据集，需要执行的清理工作就是归一化。
    // 归一化就是读取不同尺度（scale）记录的属性值，并将它们转化为相同的尺度。
    public static void normalizeByFeatureScaling (
            // dataset参数是对即将在原地被修改的List<double[]>类型列表的引用。
            // 也就是说，normalizeByFeatureScaling()收到的不是数据集的副本，而是对原始数据集的引用。
            // 这里是要对值进行修改，而不是接收转换过的副本。
            List<double[]> dataset) {
        // 一列一列的做归一化。
        for (int colNum = 0; colNum < dataset.get(0).length; colNum++) {
            List<Double> column = new ArrayList<>();
            // 把dataset的一列放到column中。
            for (double[] row : dataset) {
                column.add(row[colNum]);
            }
            // 计算出这一列的最大最小值。
            double maximum = Collections.max(column);
            double minimum = Collections.min(column);
            double difference = maximum - minimum ;
            for (double[] row : dataset) {
                // 将尺度从某一范围转换为0～1并不难。
                // 对于最大值为max、最小值为min的某个属性范围内的任意值V，
                // 转换公式就是newV=(oldV-min)/(max-min)。此操作被称为特征值缩放。
                row[colNum] = (row[colNum] -minimum) / difference;
            }
        }
    }

    // 从CSV文件加载数据的工具方法。
    // Load a CSV file into a List of String arrays
    public static List<String[]> loadCSV(String filename) {
        try (InputStream inputStream = Util.class.getResourceAsStream(filename)) {
            InputStreamReader inputStreamReader = new InputStreamReader(inputStream);
            BufferedReader bufferedReader = new BufferedReader(inputStreamReader);
            return bufferedReader.lines().map(line -> line.split(","))
                    .collect(Collectors.toList());
        }
        catch (IOException e) {
            e.printStackTrace();
            throw new RuntimeException(e.getMessage(), e);
        }
    }

    // 在数组中查找最大值的工具方法。
    // Find the maximum in an array of doubles
    public static double max(double[] numbers) {
        return Arrays.stream(numbers).max().orElse(Double.MAX_VALUE);
    }
}

