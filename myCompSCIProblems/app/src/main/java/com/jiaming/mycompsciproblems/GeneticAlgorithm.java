package com.jiaming.mycompsciproblems;

// 我们先回顾一下本章开始时对遗传算法的描述，并明确定义遗传算法所采取的步骤：
// 1.创建随机染色体的初始种群，作为算法的第一代数据。
// 2.测量这一代种群中每条染色体的适应度。如果有超过阈值的就将其返回，算法结束。
// 3.选择一些个体进行繁殖，适应度最高的个体被选中的概率更大。
// 4.某些被选中的染色体以一定的概率进行交换（结合），产生代表下一代种群的后代。
// 5.通常某些染色体发生突变的概率比较小。新一代的种群已经创建完成，并取代了上一代的种群。
// 6.返回步骤2继续执行，直到代的数量达到最大值，然后返回当前找到的最优染色体。
//   以上对遗传算法的概述（见图5.1）缺少很多重要的细节。种群中应该包含多少染色体？
//   停止算法的阈值是多少？如何选择染色体进行繁殖？它们该以多大的概率以及如何进行结合（交换）？
//   突变发生的概率是多少？应该运行多少代？

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;

// GeneticAlgorithm的参数名为c，是符合Chromosome类的泛型类型。
public class GeneticAlgorithm<C extends Chromosome<C>> {

    // 枚举SelectionType是一个内部类型，用于指定算法使用的选择方法。
    // 最常见的两种遗传算法的选择方法称为轮盘式选择法（roulette-wheel selection）和
    // 锦标赛选择法（tournament selection），
    // 轮盘式选择法有时也称为适应度比例选择法（fitness proportionate selection）。
    // 轮盘式选择法让每条染色体都有机会被选中，与其适应度成正比。
    // 在锦标赛选择法中，一定数量的随机染色体会互相挑战，适应度最佳的那个染色体将会被选中。
    public enum SelectionType {
        ROULETTE, TOURNAMENT;
    }

    private ArrayList<C> population;
    private double mutationChance;
    private double crossoverChance;
    private SelectionType selectionType;
    private Random random;

    public GeneticAlgorithm(
            // initialPopulation是算法第一代中的染色体。
            List<C> initialPolpulation,
            // mutationChance是每一代中每条染色体突变的概率，
            double mutationChance,
            // crossoverChance是被选中繁殖的双亲生育出带有它们的混合基因的后代的概率，
            // 若无混合基因的后代，则后代只是其双亲的副本。
            double crossoverChance,
            // 最后，selectionType是要使用的选择方法的类型，由枚举selectionType描述。
            SelectionType selectionType) {
        this.population = new ArrayList<C>(initialPolpulation);
        this.mutationChance = mutationChance;
        this.crossoverChance = crossoverChance;
        this.selectionType = selectionType;
        random = new Random();
    }

    // 轮盘式选择法基于每条染色体的适应度占同一代中所有适应度之和的比例进行选择。
    // 适应度最高的染色体被选中的概率会更大。
    private List<C> pickRoulette(
            // 代表每条染色体适应度的值由参数wheel给出。这些百分比由0～1之间的浮点数表示。
            double[] wheel, int numPicks) {
        List<C> picks = new ArrayList<C>();
        for (int i =0; i < numPicks; i++) {
            // 用一个0～1之间的随机数pick即可算出应该选择哪一条染色体。
            double pick = random.nextDouble();
            for (int j = 0; j < wheel.length; j++) {
                // 依次使pick减去每条染色体的相对适应度，本算法即能正常工作。
                pick -= wheel[j];
                // 当pick小于0时，就遇到了要选择的染色体。
                if (pick <= 0) {
                    picks.add(population.get(j));
                    break;
                }
            }
        }
        return picks;
    }

    private List<C> pickTournment(int numParticipants, int numPicks) {
        // 首先使用shuffle()来打乱种群顺序，
        Collections.shuffle(population);
        // 然后从中获取第一组numParticipants。这是一种随机获得numParticipants条染色体的简单方法。
        List<C> tourment = population.subList(0, numParticipants);
        // 接下来，它根据适应度对参与的染色体进行排序，
        Collections.sort(tourment, Collections.reverseOrder());
        // 并返回numPicks条最适合的染色体。
        return tourment.subList(0, numPicks);
    }

    // reproduceAndReplace()大致实现了以下步骤：
    // 3.如果nextPopulation拥有和population一样多的染色体，那么nextPopulation就会替换population。否则，返回第1步。
    private void reproduceAndReplace() {
        ArrayList<C> nextGeneration = new ArrayList<C>();
        while (nextGeneration.size() < population.size()) {
            List<C> parents;
            // 1.用两种选择法之一选出两条名为parents的染色体，以进行繁殖。
            if(selectionType == SelectionType.ROULETTE) {
                double totalFitness = population.stream().mapToDouble(C::fitness).sum();
                double[] wheel = population.stream().mapToDouble(C -> C.fitness()/totalFitness).toArray();
                parents = pickRoulette(wheel, 2);
            }
            else {
                // 若采用锦标赛选择法，则始终在整个种群的半数个体中进行竞赛，不过这是一个可配置的选项。
                parents = pickTournment(population.size() / 2, 2);
            }
            // 2.双亲染色体以一定概率结合（crossoverChance）并产生两条新的染色体，
            if(random.nextDouble() < crossoverChance) {
                C parentOne = parents.get(0);
                C parentTwo = parents.get(1);
                // 这些新的染色体会被添加到nextPopulation中。
                nextGeneration.addAll(parentOne.crosssover(parentTwo));
            }
            else {
                // 如果没有后代，则把parents直接加入nextPopulation中。
                nextGeneration.addAll(parents);
            }
        }
        if (nextGeneration.size() > population.size()) {
            nextGeneration.remove(0);
        }
        population = nextGeneration;
    }

    // 实现突变的方法mutate()非常简单，突变的细节留给单条染色体去实现。
    private void mutate() {
        for (C individual : population) {
            if (random.nextDouble() < mutationChance) {
                individual.mutate();
            }
        }
    }
    // best用来存储到目前为止发现的最佳染色体。
    public C run(int maxGenerations, double threshold) {
        C best = Collections.max(population).copy();
        // 主循环最多执行maxGenerations次。
        for (int generation = 0; generation < maxGenerations; generation++) {
            // 只要有染色体的适应度超过threshold，就会返回该染色体，循环也就结束运行；
            if (best.fitness() >= threshold) {
                return best;
            }
            // Debug printout
            Double doubleAverage = 0.0;
            for (C item : population) {
                doubleAverage += item.fitness();
            }
            doubleAverage = doubleAverage / population.size();

            // System.out.println("Generation " + generation +
            //         " Best " + best.fitness() +
            //         " Avg " + population.stream()
            //         .mapToDouble(C::fitness).average().orElse(0.0));
            System.out.println("Generation " + generation +
                    " Best " + best.fitness() +
                    " Avg " + doubleAverage.toString());
            // 否则就会调用reproduceAndReplace()和mutate()来创建下一代并再次运行循环。
            reproduceAndReplace();
            mutate();
            C highest = Collections.max(population);
            if(highest.fitness() > best.fitness()) {
                best = highest.copy();
            }
        }
        // 如果循环次数达到maxGenerations，则返回目前为止找到的最佳染色体。
        return best;
    }
}

