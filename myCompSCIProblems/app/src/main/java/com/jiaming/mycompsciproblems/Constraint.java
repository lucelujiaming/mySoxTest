package com.jiaming.mycompsciproblems;


import java.util.List;
import java.util.Map;

// 所有的约束都会被定义成Constraint类的子类。
// V is the variable type, and D is the domain type
public abstract class Constraint<V, D> {

    // 每个Constraint包含它所约束的variables和用来检查是否满足条件的satisfied()方法。
    // 确定是否满足约束是定义某个约束满足问题所需的主要逻辑。
    // the variables that the constraint is between
    protected List<V> variables;
    public Constraint() {

    }
    public Constraint(List<V> variables) {
        this.variables = variables;
    }


    // 默认的satisfied()方法需要被重写。这是因为Constraint类是抽象类，
    // 而抽象类不是用来实例化的，我们实际使用的是重写并实现了它的abstract方法的子类。
    // must be overridden by subclasses
    public abstract boolean satisfied(Map<V, D> assignment);
}

