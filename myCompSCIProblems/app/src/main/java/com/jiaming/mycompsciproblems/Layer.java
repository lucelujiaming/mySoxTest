package com.jiaming.mycompsciproblems;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.Random;
import java.util.function.DoubleUnaryOperator;

// 输出缓存类似于神经元的缓存，但高一个级别。它缓存了层中每一个神经元在调用激活函数之后的输出。
public class Layer {
    // 本章神经网络中的层对象需要维护三种状态数据：
    // 上一层
    public Optional<Layer> previousLayer;
    // 所含神经元、
    public List<Neuron> neurons  = new ArrayList<>();
    // 输出缓存。
    public double[] outputCache;

    // 在创建时，层对象的主要职责是初始化其内部的神经元。因此，Layer类的构造函数需要知道：
    public Layer(Optional<Layer> perviousLayer,
                 // 应该初始化多少个神经元，
                 int numNeurons,
                 // 以及它们的学习率是多少。
                 double learningRate,
                 // 它们的激活函数是什么，
                 DoubleUnaryOperator activationFunction,
                 DoubleUnaryOperator derivativeActivationFunction) {
        this.previousLayer = perviousLayer;
        Random random = new Random();
        for (int i = 0; i < numNeurons; i++) {
            double[] randomWeights = null;
            if(previousLayer.isPresent()) {
                randomWeights = random.doubles(
                        previousLayer.get().neurons.size()).toArray();
            }
            // 在这个简单的神经网络中，层中的每个神经元都有相同的激活函数和学习率。
            Neuron neuron = new Neuron(randomWeights, learningRate,
                    activationFunction, derivativeActivationFunction);
            neurons.add(neuron);
        }
        outputCache = new double[numNeurons];
    }
    // 当信号在神经网络中前馈时，Layer必须让每个神经元都对其进行处理。
    public double[] output(double[] inputs) {
        // 请注意，层中的每个神经元都会接收到上一层中每个神经元传入的信号。outputs()正是如此处理的。
        if(previousLayer.isPresent()) {
            // outputs()还会返回处理后的结果（以便经由网络传递到下一层）并将输出缓存一份。
            outputCache = neurons.stream().mapToDouble(
                    n -> n.output(inputs)
            ).toArray();
        }
        // 如果不存在上一层，则表示本层为输入层，只要将信号向前传递给下一层即可。
        else {
            outputCache = inputs;
        }
        return outputCache;
    }

    // 在反向传播时，需要计算两种不同类型的delta：输出层中神经元的delta和隐藏层中神经元的delta。
    // 图7.4和图7.5中分别给出了公式的描述。
    // 下面的两个方法只是机械地将公式转换成了代码。稍后在反向传播过程中神经网络对象将会调用这两个方法。
    public void calculateDeltasForOutputLayer(double[] expected) {
        for (int n = 0; n < neurons.size(); n++) {
            neurons.get(n).delta = neurons.get(n).
                    derivativeActivationFunction.applyAsDouble(neurons.get(n).outputCache)
                    * (expected[n] - outputCache[n]);
        }
    }

    public void calculateDeltasForHiddenLayer(Layer nextlayer) {
        for (int i = 0; i < neurons.size(); i++) {
            int index = i;
            double[] nextWeights = nextlayer.neurons.stream().mapToDouble(
                            n -> n.weights[index]
                    ).toArray();
            double[] nextDeltas = nextlayer.neurons.stream().mapToDouble(
                            n -> n.delta
                    ).toArray();
            double sumWeightsAndDeltas = Util.doProduct(nextWeights, nextDeltas);
            neurons.get(i).delta = neurons.get(i).derivativeActivationFunction
                    .applyAsDouble(neurons.get(i).outputCache) * sumWeightsAndDeltas;
        }

    }
}

