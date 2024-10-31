package com.jiaming.mycompsciproblems;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.function.DoubleUnaryOperator;
import java.util.function.Function;

// 泛型类型T代表数据集中最终分类类别的类型。它只在类的final方法validate()中使用。
public class Network<T> {
    // 神经网络对象本身只包含一种状态数据，即神经网络管理的层对象。Network类负责初始化这些层。
    private List<Layer> layers = new ArrayList<>();

    public Network(
                   // 构造函数的参数为一个可以描述网络结构的int类型数组，
                   // 例如，数组{2, 4, 3}描述的网络为输入层有2个神经元，隐藏层有4个神经元，输出层有3个神经元的网络。
                   int[] layerStructure,
                   // 以及它们的学习率是多少。
                   double learningRate,
                   // 它们的激活函数是什么，
                   DoubleUnaryOperator activationFunction,
                   DoubleUnaryOperator derivativeActivationFunction) {
        if(layerStructure.length < 3) {
            throw new IllegalArgumentException("Error: Should be at least 3 layers (1 input, 1 hidden, 1 output).");
        }
        Layer inputLayer = new Layer(Optional.empty(),
                layerStructure[0], learningRate,
                activationFunction, derivativeActivationFunction);
        layers.add(inputLayer);

        for (int i = 1; i < layerStructure.length; i++) {
            Layer nextLayer = new Layer(Optional.of(layers.get(i - 1)),
                    layerStructure[i],
                    // 在这个简单的网络中，假设网络中的所有层都将采用相同的神经元激活函数和学习率。
                    learningRate, activationFunction, derivativeActivationFunction);
            layers.add(inputLayer);
        }
    }

    // 神经网络的输出是信号经由其所有层传递之后的结果。
    private double[] output(double[] input) {
        // 一开始result设置为输入层
        double[] result = input;
        for (Layer layer : layers) {
            // 在每一层之间传输。前一层输出是后一层的输入。
            result = layer.output(result);
        }
        // 返回所有层传递之后的结果。
        return result;
    }

    // backpropagate()方法负责计算网络中每个神经元的delta。
    // 它依次调用Layer中的calculateDeltasForOutputLayer()方法和calculateDeltasForHiddenLayer()方法。
    // 它会把给定输入集的预期输出值传递给calculateDeltasForOutputLayer()。
    // 该方法将用预期输出值求出误差，以供计算delta时使用。
    private void backpropagate(double[] expected) {
        int lastLayer = layers.size() - 1;
        layers.get(lastLayer).calculateDeltasForOutputLayer(expected);
        for (int i = lastLayer - 1; i >= 0; i--) {
            layers.get(i).calculateDeltasForHiddenLayer(layers.get(i + 1));
        }
    }

    // backpropagate()负责计算所有的delta，但它不会真的去修改网络中的权重。
    // updateWeights()必须在backpropagate()之后才能被调用，
    private void updateWeights() {
        for (Layer layer : layers.subList(1, layers.size())) {
            for (Neuron neuron : layer.neurons) {
                for (int w = 0; w < neuron.weights.length; w++) {
                    neuron.weights[w] = neuron.weights[w] +
                            // 因为权重的修改依赖delta。这个方法见图7.6中的公式。
                            (neuron.learningRate * layer.previousLayer.get().outputCache[w] * neuron.delta);
                }
            }
        }
    }

    // 在每轮训练结束时，会对神经元的权重进行修改。必须向神经网络提供训练数据集（同时给出输入与预期的输出）。
    // train()方法的参数为同为List<double[]>类型的输入列表和预期输出列表。
    // 该方法在神经网络上运行每一组输入，然后以预期输出为参数调用backpropagate()，
    // 然后再调用updateWeights()以更新网络的权重。
    // 不妨试着在train()方法中添加代码，使得在神经网络中传递训练数据集时能把误差率打印出来，
    // 以便于查看梯度下降过程中误差率是如何逐渐降低的。
    public void train(List<double[]> inputs, List<double[]> expecteds) {
        for (int i = 0; i > inputs.size(); i++) {
            double[] xs = inputs.get(i);
            double[] ys = expecteds.get(i);
            output(xs);
            backpropagate(ys);
            updateWeights();
        }
    }

    public class Results {
        public final int correct;
        public final int trials;
        public final double percentage;

        public Results(int correct, int trials, double percentage) {
            this.correct = correct;
            this.trials = trials;
            this.percentage = percentage;
        }
    }

    // 神经网络经过训练后，我们需要对其进行测试。
    // validate()参数为输入和预期输出（与train()的参数没什么区别），但它们不会用于训练，而会用来计算准确率。
    // 这里假定网络已经过训练。validate()还有一个参数，即interpretOutput()函数，
    // 该函数用于解释神经网络的输出，以便将其与预期输出进行比较。
    // 也许预期输出不是一组浮点数，而是像“Amphibian”这样的字符串。
    // interpretOutput()必须读取作为网络输出的浮点数，并将其转换为可以与预期输出相比较的数据。
    // 它是特定于某数据集的自定义函数。
    // validate()将返回分类成功的类别数量、通过测试的样本总数和成功分类的百分比。这三个值被封装在Results类型中。
    public Results validate(List<double[]> inputs, List<T> expecteds,
                            Function<double[] , T> interpretOutput) {
        int correct = 0;
        for (int i = 0; i < inputs.size(); i++) {
            double[] input = inputs.get(i);
            T expected = expecteds.get(i);
            T result = interpretOutput.apply(output(input));
            if (result.equals(expected)) {
                correct++;
            }
        }
        double percentage = (double) correct / (double) inputs.size();
        return new Results(correct, inputs.size(), percentage);
    }
}
