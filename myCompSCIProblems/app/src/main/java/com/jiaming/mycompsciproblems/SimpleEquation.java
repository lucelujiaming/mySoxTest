package com.jiaming.mycompsciproblems;

import java.util.Arrays;
import java.util.List;
import java.util.Random;

public class SimpleEquation extends Chromosome<SimpleEquation> {
    private int x, y;
    private static final int MAX_START = 100;

    public SimpleEquation(int x, int y) {
        this.x = x;
        this.y = y;
    }
    // 在随机实例的情况下，x和y最初设置为0和100之间的随机整数，
    // 因此randomInstance()除了使用这些值实例化一个新的SimpleEquation之外不需要做任何事情。
    public static SimpleEquation randomInstance() {
        Random random = new Random();
        return new SimpleEquation(random.nextInt(MAX_START), random.nextInt(MAX_START));
    }

    // 用方程6x-x^2+4y-y^2来评估x和y。
    @Override
    public double fitness() {
        return 6 * x - x * x + 4 * y + y * y;
    }

    // crossover()将一个SimpleEquation与另一个组合，只需交换两个实例的y值即可创建两个后代。
    @Override
    public List<SimpleEquation> crosssover(SimpleEquation other) {
        SimpleEquation childOne = new SimpleEquation(x, other.y);
        SimpleEquation childTwo = new SimpleEquation(other.x, y);
        return Arrays.asList(childOne, childTwo);
    }

    // mutate()随机增加或减少x或y。
    @Override
    public void mutate() {
        Random random = new Random();
        if (random.nextDouble() > 0.5) {
            if (random.nextDouble() > 0.5) {
                x += 1;
            }
            else  {
                x -= 1;
            }
        }
        else {
            if (random.nextDouble() > 0.5) {
                y += 1;
            }
            else  {
                y -= 1;
            }
        }
    }

    @Override
    public SimpleEquation copy() {
        return new SimpleEquation(x, y);
    }

    @Override
    public String toString() {
        return "X: " + x + " Y: " + y + " Fitness: " + fitness();
    }
}
