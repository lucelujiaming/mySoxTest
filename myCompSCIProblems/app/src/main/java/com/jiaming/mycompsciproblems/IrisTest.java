package com.jiaming.mycompsciproblems;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class IrisTest {
    public static final String IRIS_SETOSA = "Iris-setosa";
    public static final String IRIS_VERSICOLOR = "Iris-versicolor";
    public static final String IRIS_VIRGINICA = "Iris-virginica";

    // irisParameters代表每个样本的4种属性集，这些样本将用于对鸢尾花进行分类。
    private List<double[]> irisParameters = new ArrayList<>();
    // irisClassifications是每个样本的实际类别。
    // 此处的神经网络包含三个输出神经元，每个神经元代表一种可能的品种，
    // 例如最终输出{0.9, 0.3, 0.1}将代表山鸢尾（irissetosa），因为第一个神经元代表该品种，这里它的数值最大。
    private List<double[]> irisClassifications = new ArrayList<>();
    // IrisSpecies直接对应每条花朵数据应该归属的英文类别名称。
    // 山鸢尾在数据集中将被标记为“Iris-setosa”。
    private List<String> irisSpecies = new ArrayList<>();

    // 这个函数就是从csv文件中填充irisParameters， irisClassifications和irisSpecies这三个成员变量。
    public IrisTest() {
        List<String[]> irisDataset = Util.loadCSV("/data/android/files/iris.csv");
        Collections.shuffle(irisDataset);
        for (String[] iris : irisDataset) {
            double[] parameters = Arrays.stream(iris).limit(4)
                    .mapToDouble(Double::parseDouble).toArray();
            irisParameters.add(parameters);

            String species = iris[4];
            switch (species) {
                case IRIS_SETOSA:
                    // 为了训练，我们需要知道正确类别，因此每条鸢尾花数据都带有预先标记的类别。
                    // 对于应为山鸢尾的花朵数据，irisClassifications中的数据项将会是{1.0, 0.0, 0.0}。
                    // 这些值将用于计算每步训练后的误差。
                    irisClassifications.add(new double[] {1.0, 0.0, 0.0});
                    break;
                case IRIS_VERSICOLOR:
                    irisClassifications.add(new double[] {0.0, 1.0, 0.0});
                    break;
                case IRIS_VIRGINICA:
                    irisClassifications.add(new double[] {0.0, 0.0, 1.0});
                    break;
            }
            irisSpecies.add(species);
        }
        Util.normalizeByFeatureScaling(irisParameters);
    }

    // 一个工具函数，将会被传给神经网络对象中的validate()方法，用于识别正确的分类。
    public String irisInterpretOutput(double[] output) {
        double max = Util.max(output);
        if(max == output[0]) {
            return IRIS_SETOSA;
        } else if (max == output[1]) {
            return IRIS_VERSICOLOR;
        } else {
            return IRIS_VIRGINICA;
        }
    }

    // 定义一个classify()方法，该方法将用来设置神经网络并训练和运行该网络。
    public Network<String>.Results classify() {
        Network<String> irisNetwork = new Network<>(
                // Network构造函数中的layerStructure参数指定了一个带有{4, 6, 3}的三层网络
                // （一个输入层、一个隐藏层和一个输出层）。
                // 输入层有4个神经元，隐藏层有6个神经元，输出层有3个神经元。
                // 输入层的4个神经元直接映射到用于对每个样本进行分类的4个参数。
                // 输出层的3个神经元直接映射到我们试图对每个输入进行分类的3个不同品种。
                // 隐藏层的6个神经元更多的是反复试验的结果，而不是某种公式。
                new int[] {4, 6, 3},
                // learningRate也是如此。如果网络的准确率是次优的，
                // 可以用这两个值（隐藏层神经元的数量和学习率）进行实验。
                0.3,
                Util::sigmoid, Util::derivativeSigmod);
        List<double[]> irisTrainers = irisParameters.subList(0, 140);
        List<double[]> irisTrainersCoreects = irisClassifications.subList(0, 140);
        int trainingIterations = 150;
        for (int i = 0; i < trainingIterations; i++) {
            irisNetwork.train(irisTrainers, irisTrainersCoreects);
        }
        List<double[]> irisTesters = irisParameters.subList(0, 140);
        List<String> irisTestersCoreects = irisSpecies.subList(140, 150);
        return irisNetwork.validate(irisTesters, irisTestersCoreects, this::irisInterpretOutput);
    }
}
