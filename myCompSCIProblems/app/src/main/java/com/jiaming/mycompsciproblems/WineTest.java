package com.jiaming.mycompsciproblems;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class WineTest {
    private List<double[]> wineParameters = new ArrayList<>();
    private List<double[]> wineClassifications = new ArrayList<>();
    private List<Integer> wineSpecies = new ArrayList<>();

    public WineTest() {
        List<String[]> wineDataset = Util.loadCSV("/data/android/files/wine.csv");
        Collections.shuffle(wineDataset);
        for (String[] wine : wineDataset) {
            double[] parameters = Arrays.stream(wine).limit(4)
                    .mapToDouble(Double::parseDouble).toArray();
            wineParameters.add(parameters);

            int species = Integer.parseInt(wine[0]);
            switch (species) {
                case 1:
                    // 为了训练，我们需要知道正确类别，因此每条鸢尾花数据都带有预先标记的类别。
                    // 对于应为山鸢尾的花朵数据，irisClassifications中的数据项将会是{1.0, 0.0, 0.0}。
                    // 这些值将用于计算每步训练后的误差。
                    wineClassifications.add(new double[] {1.0, 0.0, 0.0});
                    break;
                case 2:
                    wineClassifications.add(new double[] {0.0, 1.0, 0.0});
                    break;
                case 3:
                    wineClassifications.add(new double[] {0.0, 0.0, 1.0});
                    break;
            }
            wineSpecies.add(species);
        }
        Util.normalizeByFeatureScaling(wineParameters);
    }

    public Integer wineInterpretOutput(double[] output) {
        double max = Util.max(output);
        if(max == output[0]) {
            return 1;
        } else if (max == output[1]) {
            return 2;
        } else {
            return 3;
        }
    }

    public Network<Integer>.Results classify() {
        Network<Integer> wineNetwork = new Network<>(
                new int[] {13, 7, 3},
                0.9, Util::sigmoid, Util::derivativeSigmod);
        List<double[]> wineTrainers = wineParameters.subList(0, 150);
        List<double[]> wineTrainersCoreects = wineClassifications.subList(0, 150);
        int trainingIterations = 150;
        for (int i = 0; i < trainingIterations; i++) {
            wineNetwork.train(wineTrainers, wineTrainersCoreects);
        }
        List<double[]> wineTesters = wineParameters.subList(0, 140);
        List<Integer> wineTestersCoreects = wineSpecies.subList(140, 150);
        return wineNetwork.validate(wineTesters, wineTestersCoreects, this::wineInterpretOutput);
    }
}
