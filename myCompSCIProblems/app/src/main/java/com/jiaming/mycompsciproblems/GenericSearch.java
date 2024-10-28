package com.jiaming.mycompsciproblems;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.Set;
import java.util.Stack;
import java.util.function.Function;
import java.util.function.Predicate;
import java.util.function.ToDoubleFunction;

public class GenericSearch {
    //  线性搜索按照原始数据结构的元素顺序来遍历搜索空间中的每个元素，
    //  直到找到要搜索的内容或达到数据结构的末尾。
    public static <T extends Comparable<T>> boolean linearContains(List<T> list, T key){
        for (T item : list) {
            if(item.compareTo(key) == 0){
                return true;
            }
        }
        return false;
    }

    // 有一种比需要遍历每个元素更快的搜索方法，但是这种方法需要我们提前了解数据结构的元素顺序。
    // 如果数据结构是有序的，而且可以通过索引直接访问任意元素，那我们就可以使用二分搜索。
    public static <T extends Comparable<T>> boolean binaryContains(List<T> list, T key){
        ArrayList<T> sortedArrayList = new ArrayList<>(list);
        Collections.sort(sortedArrayList);
        int lowPos = 0;
        int highPos = sortedArrayList.size() - 1;
        while (lowPos <= highPos) {
            int middlePos = (lowPos + highPos) / 2;
            int comparisonResult = sortedArrayList.get(middlePos).compareTo(key);
            if (comparisonResult < 0) {
                lowPos = middlePos + 1;
            }
            else if (comparisonResult > 0) {
                highPos = middlePos - 1;
            }
            else {
                return true;
            }
        }
        return false;
    }

    // 深度优先搜索（Depth-First Search, DFS）顾名思义就是尽可能地往路径深处进行搜索，
    // 如果遇到了死胡同，则回溯到最后一次做出决策的那个位置。

    // 在实现DFS算法之前，还需要添加一些额外的组件。
    // 我们需要使用一个Node类来记录进行搜索时的状态变化，也就是位置变化。
    // Node类可以看作对状态的一种封装。
    public static class Node<T> implements Comparable<Node<T>> {
        private final T state;
        private Node<T> parent;
        private double cost;
        private double heuristic;

        public Node(T state, Node<T> parent) {
            this.state = state;
            this.parent = parent;
        }

        public Node(T state, Node<T> parent, double cost, double heuristic) {
            this.state = state;
            this.parent = parent;
            this.cost = cost;
            this.heuristic = heuristic;
        }

        // 为了能够在frontier上选出f(n)最小的那个状态，A*搜索使用优先队列（priority queue）这种数据结构来存储frontier。
        // 优先队列能使其数据元素维持某种内部顺序，使得首先弹出的元素始终是优先级最高的元素。
        // 在本例中，优先级最高的数据项是f(n)最小的那个。
        // 为此，通常会在其内部使用二叉堆，使得入队和出队的算法复杂度均为O(lgn)。
        @Override
        public int compareTo(Node<T> o) {
            // Double 有compareTo方法。
            Double mine = cost + heuristic;
            Double theirs = o.cost + o.heuristic;
            return mine.compareTo(theirs);
        }
    }

    public static <T> Node<T> dfs(T initial,
                                  // goalTest被声明为Predicate<T>函数，它接收任意的类型T作为参数并返回一个boolean值。
                                  // 在当前例子中，T是MazeLocation类型。
                                  Predicate<T> goalTest,
                                  // successors被声明为接收一个T类型参数并返回一个List类型值的函数。
                                  Function<T, List<T>> successors){
        // 深度优先搜索需要使用两种数据结构来记录状态：
        // 用栈来记录当前要搜索的状态，也就是位置，命名为frontier；
        // frontier is where we've yet to go
        Stack<Node<T>> frontier = new Stack<>();
        // 如果一个Node没有parent，则使用null来表示。
        frontier.push(new Node<T>(initial, null));

        // 用集合来记录已经搜索过的状态，命名为explored。
        // explored is where we've been
        Set<T> explored = new HashSet<>();
        explored.add(initial);

        // keep going while there is more to explore
        // 只要frontier中还有状态需要访问，DFS就将持续检查这些状态是不是终点状态，
        // 如果frontier为空，则意味着已经没有可以搜索的地方了。
        while (! frontier.isEmpty()) {
            Node<T> currentNode = frontier.pop();
            T currentState = currentNode.state;
            // if we found the goal, we're done
            // 如果是终点状态，则停止搜索并将终点状态返回；
            if(goalTest.test(currentState)){
                return  currentNode;
            }
            // 如果不是，则将在该状态之后可以继续访问的那些状态successors添加到frontier中。
            // check where we can go next and haven't explored
            for (T child : successors.apply(currentState)) {
                // 为了避免原地打转，需要将已经搜索过的状态添加到explored中，防止重复访问之前已经访问过的状态。
                if (explored.contains(child)) {
                    continue;  // skip children we already explored
                }
                explored.add(child);
                frontier.push(new Node<>(child, currentNode));
            }
        }
        return null;   // went through everything and never found goal
    }

    // 如果dfs()执行成功，将会返回封装了目标状态的Node。
    // 从该Node开始，利用parent属性向前遍历，即可重现由起点到目标点的路径。
    public static <T> List<T> nodeToPath(Node<T> node) {
        List<T> path = new ArrayList<>();
        path.add(node.state);
        // work backwards from end to front
        while (node.parent != null){
            node = node.parent;
            path.add(0, node.state);
        }
        return path;
    }

    // 或许你会注意到，使用深度优先搜索解得的迷宫路径看起来似乎有些别扭，并且通常不是最短路径。
    // 而广度优先搜索（Breadth-First Search, BFS）找到的路径总是最短的。
    // 它在每次迭代中都会从起始状态开始由近至远地搜索每一层级的所有节点。
    // 在某些特定的问题中，深度优先搜索可能比广度优先搜索更快地找到解，反之亦然。
    public static <T> Node<T> bfs(T initial,
                                  // goalTest被声明为Predicate<T>函数，它接收任意的类型T作为参数并返回一个boolean值。
                                  // 在当前例子中，T是MazeLocation类型。
                                  Predicate<T> goalTest,
                                  // successors被声明为接收一个T类型参数并返回一个List类型值的函数。
                                  Function<T, List<T>> successors){
        // 广度优先搜索需要使用两种数据结构来记录状态：
        // 用队列来记录当前要搜索的状态，也就是位置，命名为frontier；
        // frontier is where we've yet to go
        Queue<Node<T>> frontier = new LinkedList<>();
        // 如果一个Node没有parent，则使用null来表示。
        frontier.offer(new Node<>(initial, null));

        // 用集合来记录已经搜索过的状态，命名为explored。
        // explored is where we've been
        Set<T> explored = new HashSet<>();
        explored.add(initial);

        // keep going while there is more to explore
        // 只要frontier中还有状态需要访问，DFS就将持续检查这些状态是不是终点状态，
        // 如果frontier为空，则意味着已经没有可以搜索的地方了。
        while (! frontier.isEmpty()) {
            Node<T> currentNode = frontier.poll();
            T currentState = currentNode.state;
            // if we found the goal, we're done
            // 如果是终点状态，则停止搜索并将终点状态返回；
            if(goalTest.test(currentState)){
                return  currentNode;
            }
            // 如果不是，则将在该状态之后可以继续访问的那些状态successors添加到frontier中。
            // check where we can go next and haven't explored
            for (T child : successors.apply(currentState)) {
                // 为了避免原地打转，需要将已经搜索过的状态添加到explored中，防止重复访问之前已经访问过的状态。
                if (explored.contains(child)) {
                    continue;  // skip children we already explored
                }
                explored.add(child);
                frontier.offer(new Node<>(child, currentNode));
            }
        }
        return null;   // went through everything and never found goal
    }

    // 与之前广度优先搜索算法的实现不同的是，A*搜索使用了成本函数和启发函数的组合，将搜索聚焦在最有可能快速抵达目标的路径上。
    //   1. 成本函数g(n)会检查到达指定状态的成本。在求解迷宫的场景中，成本是指在到达当前状态时，已经走过了多少个单元格。
    //   2. 启发函数h(n)则给出了从当前状态到达目标状态的成本估算。
    // 可以证明，如果h(n)是一个可接受的启发式信息（admissible heuristic），那么找到的最终路径将是最优解。
    // 可接受的启发式信息永远不会高估抵达目标的成本。
    // 在二维平面上，直线距离启发式信息就是一个很好的例子，因为直线总是最短路径[1]。
    // 到达任一状态所需的总成本为f(n)，它是g(n)与h(n)之和，即f(n)=g(n)+h(n)。
    // 当从frontier选取要探索的下一个状态时，A*搜索会选择f(n)最小的那个状态。
    // 这也就是它与广度优先搜索和深度优先搜索的不同之处。
    public static <T> Node<T> astar(T initial,
                                    // goalTest被声明为Predicate<T>函数，它接收任意的类型T作为参数并返回一个boolean值。
                                    // 在当前例子中，T是MazeLocation类型。
                                    Predicate<T> goalTest,
                                    // successors被声明为接收一个T类型参数并返回一个List类型值的函数。
                                    Function<T, List<T>> successors,
                                    ToDoubleFunction<T> heuristic){
        // A*优先搜索需要使用两种数据结构来记录状态：
        // 用PriorityQueue来记录当前要搜索的状态，也就是位置，命名为frontier；
        // 为了能够在frontier上选出f(n)最小的那个状态，A*搜索使用优先队列（priority queue）这种数据结构来存储frontier。
        // frontier is where we've yet to go
        PriorityQueue<Node<T>> frontier = new PriorityQueue<>();
        // 如果一个Node没有parent，则使用null来表示。
        // 对于起点来说，除了parent为null以外，本身的cost也就是最低成本g(n)。也就是步数为零。
        // 使用传入的启发函数h(n)计算启发值。
        frontier.offer(new Node<T>(initial, null, 0, heuristic.applyAsDouble(initial)));

        // 用HashMap来记录已经搜索过的状态，命名为explored。
        // explored is where we've been
        Map<T, Double> explored = new HashMap<>();
        // HashMap可以用来跟踪每一个可能被访问的节点的最低成本g(n)。也就是步数。
        // 对于起点来说，步数为零。
        explored.put(initial, 0.0);

        // keep going while there is more to explore
        // 只要frontier中还有状态需要访问，DFS就将持续检查这些状态是不是终点状态，
        // 如果frontier为空，则意味着已经没有可以搜索的地方了。
        while (! frontier.isEmpty()) {
            Node<T> currentNode = frontier.poll();
            T currentState = currentNode.state;
            // if we found the goal, we're done
            // 如果是终点状态，则停止搜索并将终点状态返回；
            if(goalTest.test(currentState)){
                return  currentNode;
            }
            // 如果不是，则将在该状态之后可以继续访问的那些状态successors添加到frontier中。
            // check where we can go next and haven't explored
            for (T child : successors.apply(currentState)) {
                // 1 here assumes a grid, need a cost function for more sophisticated apps
                // 可以继续访问的那些状态successors的最低成本g(n)。也就是步数等于当前状态的步数加一。
                double newCost = currentNode.cost + 1;
                // 节点没有包含。
                if (!explored.containsKey(child) ||
                         // 或者是包含了，但是这个点的启发式信息比当前代价大。也就是更靠近终点。
                        (explored.get(child) > newCost)) {
                    explored.put(child, newCost);
                    // 把这个新点加入路径队列。
                    frontier.offer(new Node<>(child, currentNode, newCost, heuristic.applyAsDouble(child)));
                }
            }
        }
        return null;   // went through everything and never found goal
    }

}

