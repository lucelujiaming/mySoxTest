package com.jiaming.mycompsciproblems;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

// CSP - Constraint Satisfaction Problem
// 该约束满足问题框架的核心是CSP类。CSP类中包含了变量、值域和约束。
// 类中使用泛型来保证足够的灵活性，从而能处理任意类型的变量和域值（主键V和域值D）。
public class CSP<V, D> {
    // 在CSP中，variables、domains和constraints可以是任意类型。
    // 集合variables是一个用来存放变量的List类型，
    // 例如参加会议的三个人
    private List<V> variables;

    // Map类型的domains则把变量映射为可取值的列表（这些变量的值域），
    // 例如参加会议的每一个人各自的可用时间。每一个人都有一系列可用时间。
    private Map<V, List<D>> domains;

    // Map类型的constraints则把每个变量映射为其所受约束的列表。
    // 每一个变量都有一个映射出来的其所受约束的列表。
    // 也就是说，有一个Map，Map中每一个变量都有一个约束列表。约束列表中每一个元素都是一个Constraint对象。
    // 包含它所约束的variables和用来检查是否满足条件的satisfied()方法。
    private Map<V, List<Constraint<V, D>>> constraints = new HashMap<V, List<Constraint<V, D>>>();

    // 使用传入的变量列表和每一个变量的可取值列表初始化CSP类。
    // 初始化的结果就是，我们初始化了变量列表和每一个变量的可取值列表。但是没有任何约束。
    public CSP(List<V> variables, Map<V, List<D>> domains) {
        this.variables = variables;
        this.domains = domains;
        for (V variable : variables) {
            // 为每一个变量生成一个对应的空约束对象。
            constraints.put(variable, new ArrayList<>());
            // 如果传入的domains中包含了variables中没有的变量，说明传入的变量可取值列表有问题。起码是有一些冗余。
            if(!domains.containsKey(variable)) {
                throw new IllegalArgumentException("Every variable should have a domain assigned to it.");
            }
        }
    }

    // 初始化的结果就是，我们初始化了变量列表和每一个变量的可取值列表。但是没有任何约束。
    // 我们使用addConstraint函数添加约束。
    public void addConstraint(Constraint<V, D> constraint){
        // 遍历传入的约束中所有的它所约束的variables。也就是遍历给定约束涉及的所有变量。
        for (V variable : constraint.variables) {
            // 如果它所约束的variables中存在CSP中没有的变量。
            if (!variables.contains(variable)) {
                // 说明传入的约束有问题。起码是有一些冗余。
                throw new IllegalArgumentException("Variable in constraint not in CSP.");
            }
            // 否则把传入的约束添加到每个变量的constraints映射中。
            constraints.get(variable).add(constraint);
        }
    }
    // 如何判断给定的变量配置和所选域值是否满足约束呢？
    // 为此，我们需要定义一个函数，该函数根据assignment来检查给定变量的每个约束，
    // 以查看assignment中变量的值是否与约束一致。
    public boolean consistent (
                               // 给定的变量
                               V variable,
                               // 其中，我们将给定的变量配置称为assignment。
                               // 也就是说，assignment是为每个变量所选择的特定域值。
                               Map<V, D> assignment) {
        // 根据assignment来检查给定变量的每个约束，以查看assignment中变量的值是否与约束一致。
        // 遍历给定变量（一定是刚刚加入assignment中的变量）的每个约束，以检查是否满足约束。
        for (Constraint<V, D> constraint : constraints.get(variable)) {
            // 只要有一条不满足，则返回false。
            if (!constraint.satisfied(assignment)) {
                return false;
            }
        }
        // 如果满足所有约束，则返回true；
        return true;
    }

    // 该约束满足问题框架使用简单的回溯搜索来寻找问题的解。回溯的思路如下：
    // 在搜索中一旦碰到障碍，就回到碰到障碍之前最后一次做出判断的已知点，然后选择另一条路径。
    // 敏锐的你应该发现这与第2章中的深度优先搜索非常相似。
    public Map<V, D> backtrackingSearch(
            // 参数为初始化空Map类型。也就是说这个函数第一次被调用的时候，为一个空Map。
            // 后续会在函数内经过多次迭代，被多次填充。
            Map<V, D> assignment) {
        // 递归搜索的基准条件是为每个变量都找到满足条件的赋值，一旦找到就返回满足条件的解的第一个实例，而不会继续搜索下去。
        if(assignment.size() == variables.size()) {
            return assignment;
        }
        // 为了选出一个新变量并搜索其值域，只需遍历所有变量并找出第一个未赋值的变量。
        // 为此，我们为variables创建一个Stream过滤器，根据变量是否在assignment中来进行筛选，
        // 我们通过findFirst()方法获取第一个没在assignment中的变量。
        // get the first variable in the CSP but not in the assignment
        // V unassignedVariable = variables.stream().filter(
        //         // filter()接收一个Predicate参数。Predicate是一个函数接口，说明该函数拥有一个参数并返回一个boolean类型的值。
        //         // 这里的Predicate是一个lambda表达式，即v->!assignment.containsKey(v)。
        //         // 如果assignment中不包含参数，则返回true。返回true的参数在这里是就CSP的一个变量。
        //         v -> !assignment.containsKey(v)).findFirst().get();
        // V unassignedVariable = variables.stream().filter(v -> !assignment.containsKey(v)).findFirst().get();
        V unassignedVariable = null;
        for(V var : variables)   {
            if(assignment.containsKey(var) == false) {
                unassignedVariable = var;
                break;
            }
        }

        if(unassignedVariable == null) {
            return null;
        }

        // 我们一次一个地为该变量分配所有可能的域值。
        // get the every possible domain value of the first unassigned variable
        for (D value : domains.get(unassignedVariable)) {
            // 每个新assignment存储在名为Map类型的localAssignment局部变量中。
            // shallow copy of assignment that we can change
            Map<V, D> localAssignment = new HashMap<>(assignment);
            localAssignment.put(unassignedVariable, value);
            // 如果localAssignment中的新assignment与所有约束一致（使用consistent()进行检查），
            // if we're still consistent, we recurse (continue)
            if(consistent(unassignedVariable, localAssignment)) {
                // 则继续递归搜索新的assignment。
                Map<V, D> result = backtrackingSearch(localAssignment);
                // 如果result不为空（基准情况），我们就将新assignment返回到递归链上。
                // if we didn't find the result, we will end up backtracking
                if (result != null) {
                    return result;
                }
            }
        }
        // 最后，如果已经对某变量遍历了每一种可能的域值，并且用现有的一组assignment没有找到解，就返回null，表示无解。
        // 这将导致递归调用链回溯到之前成功做出上一次赋值的位置。
        return null;
    }

    // backtrackingSearch这个函数第一次被调用的时候，为一个空Map。
    // helper for backtrackingSearch when nothing known yet
    public Map<V, D> backtrackingSearch() {
        return backtrackingSearch(new HashMap<>());
    }
}

