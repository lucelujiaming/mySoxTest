package com.jiaming.mycompsciproblems;

import java.util.function.DoubleUnaryOperator;

// 单个神经元会储存多种状态，包括其：
public class Neuron {
    // 权重、
    public double[] weights;
    // detla、
    public double delta;
    // 学习率、
    public final double learningRate ;
    // 最近一次输出的缓存、
    public double outputCache;
    // 激活函数
    public final DoubleUnaryOperator activationFunction;
    // 以及激活函数的导数等。
    public final DoubleUnaryOperator derivativeActivationFunction;


    public Neuron(double[] weights, double learningRate,
                  DoubleUnaryOperator activationFunction,
                  DoubleUnaryOperator derivativeActivationFunction) {
        this.weights = weights;
        // 因为delta和outputCache在第一次创建神经元时未知，所以它们被初始化为0.0。
        outputCache = 0.0;
        delta = 0.0;
        // 其中几个变量（learningRate、activationFunction和derivativeActivationFunction）是预先设定好的。
        this.learningRate = learningRate;
        this.activationFunction = activationFunction;
        this.derivativeActivationFunction = derivativeActivationFunction;
    }

    // output()的参数为进入神经元的输入信号（inputs），它调用本章前面讨论过的公式（见图7.2）。
    // 输入信号通过点积操作与权重合并在一起，并缓存在outputCache中。
    public double output(double[] inputs) {
        outputCache = Util.doProduct(inputs, weights);
        return activationFunction.applyAsDouble(outputCache);
    }
}

