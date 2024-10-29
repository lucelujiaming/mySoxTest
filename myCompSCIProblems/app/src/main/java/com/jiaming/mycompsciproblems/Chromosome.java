package com.jiaming.mycompsciproblems;

import java.util.List;

// 需要注意的是，我们会把泛型类型T与Chromosome进行绑定（Chromosome<T extends Chromosome<T>>）。
// 这意味着任何填入T类型变量的对象都必须是Chromosome的子类。
public abstract class Chromosome<T extends Chromosome<T>>  implements Comparable<T>{
    public abstract double fitness();
    public abstract List<T> crosssover(T other);
    public abstract void mutate();
    public abstract T copy();

    @Override
    public int compareTo(T other) {
        Double mine = this.fitness();
        Double theirs = other.fitness();
        return mine.compareTo(theirs);
    }
}

