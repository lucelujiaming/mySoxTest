package com.jiaming.mycompsciproblems;

import java.util.function.DoubleUnaryOperator;

// 单个神经元会储存多种状态，包括其：
public class Neuron {
    // 权重、
    public double[] weights;
    // detla、
    // 每一个神经元的delta指的是：
    //   为了让输出层末端改变指定单位长度的值，自身需要改变多少。
    ////////////////////////////////////////////////////////
    // 这个计算方法很容易理解，现在已知预期输出，我们就可以知道误差为(预期输出 - 本层输出)。
    // 对于输出层来说，因为自己就是末端，不需要考虑下一层。因此上delta计算方法：
    //   本层输出的偏导数 * (预期输出 - 本层输出)
    ////////////////////////////////////////////////////////
    // 而对于输入层和隐藏层来说，每一层输出，会在下一层点乘权重矩阵以后通过一个激活函数。
    // 因此上，他们的delta是根据下一层的delta计算出来的，计算方法为：
    //   本层输出的偏导数 * 下一层误差delta的反向传播。
    // 而下一层误差delta的反向传播为：
    //   下一层误差delta * 下一层的权重的偏导数。
    // 这就是著名的误差反向传播。
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

