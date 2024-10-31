package com.jiaming.mycompsciproblems;

import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.PriorityQueue;

public class WeightedGraph<V> extends Graph<V, WeightedEdge> {
    public WeightedGraph(List<V> vertices) {
        super(vertices);
    }

    public void addEdge(WeightedEdge edge) {
        // 首先将一条边添加到顶点u的邻接表中，
        // 接着将该条边反转，然后添加到顶点v的邻接表中。
        edges.get(edge.u).add(edge);
        edges.get(edge.v).add(edge.reversed());

    }

    public void addEdge(int u, int v, float weight) {
        addEdge(new WeightedEdge(u, v, weight));
    }

    public void addEdge(V first, V second, float weight) {
        addEdge(new WeightedEdge(indexOf(first), indexOf(second), weight));
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < getVertexCount(); i++) {
            sb.append(vertexAt(i));
            sb.append(" -> ");
            // sb.append(Arrays.toString(edgesOf(i).stream()
            //         .map(we -> "(" + vertexAt(we.v) + ", " + we.weights + ")").toArray()));
            List<WeightedEdge> objWeightedEdge = edgesOf(i);
            for (WeightedEdge edge : objWeightedEdge){
                sb.append("(" + vertexAt(edge.v) + ", " + edge.weight + ")");
            }
            sb.append(System.lineSeparator());
        }
        return sb.toString();
    }

    public static double totalWeight(List<WeightedEdge> path) {
        int sumPath = 0;
        for (WeightedEdge item : path) {
            sumPath += item.weight;
        }
        return sumPath;
    }

    public List<WeightedEdge> mst(int start) {
         // 该算法返回一个表示最小生成树的加权路径（List<WeightedEdge>）。
         // 随着权重最小的边不断被弹出以及图中新的区域被不断遍历，
         // WeightedEdge对象会不断被添加到result中，最终result会包含最小生成树的加权路径。
         LinkedList<WeightedEdge> result = new LinkedList<>();
         // 如果参数start不合法，则mst()返回一个空的列表。
         if (start < 0 || start > (getVertexCount() - 1)){
             return result;
         }
         // Jarník算法总是选择权重最小的边，所以被视为一种贪心算法。
         // pq用于存储新发现的边并弹出权重第二小的边。
         PriorityQueue<WeightedEdge> pq = new PriorityQueue<>();
         // visited用于记录已经到过的顶点索引，它可通过Set来实现，类似于bfs()中的explored。
         boolean[] visited = new boolean[getVertexCount()];

         //  // this is like a "visit" inner function
         //  IntConsumer visit = index -> {
         //      visited[index] = true; // mark as visited
         //      for (WeightedEdge edge : edgesOf(index)) {
         //          // add all edges coming from here to pq
         //          if (!visited[edge.v]) {
         //              pq.offer(edge);
         //          }
         //      }
         //  };

        // Replace  "visit" inner function
        // 将一个顶点标记为已访问，并把尚未访问过的顶点连接的边都加入pq中。
         visited[start] = true;
         for (WeightedEdge edgeVisit : edgesOf(start)) {
             if(!visited[edgeVisit.v]) {
                 pq.offer(edgeVisit);
             }
         }
        // 如果已经没有边可供探索，则返回结果。
         while(!pq.isEmpty()) {
             // 只要优先队列中还有边存在，我们就将它们弹出并检查它们是否会引出尚未加入树的顶点。
             // 因为优先队列是以升序排列的，所以会先弹出权重最小的边。这就确保了结果确实具有最小总权重。
             WeightedEdge edgePoll = pq.poll();
             // 如果弹出的边不会引出未探索过的顶点，那么就会被忽略，
             if (visited[edgePoll.v]) {
                 continue;
             }
             // 否则，因为该条边是目前为止权重最小的边，所以会被添加到结果集中，并且对其引出的新顶点进行探索。
             result.add(edgePoll);

             // Replace  "visit" inner function
             // 将一个顶点标记为已访问，并把尚未访问过的顶点连接的边都加入pq中。
             visited[edgePoll.v] = true;
             for (WeightedEdge edgeVisit : edgesOf(edgePoll.v)) {
                 if(!visited[edgeVisit.v]) {
                     pq.offer(edgeVisit);
                 }
             }
         }
         return result;
    }

    public String printWeightedPath(List<WeightedEdge> wp) {
        StringBuilder sb = new StringBuilder();
        for (WeightedEdge edge : wp) {
            System.out.println(vertexAt(edge.u) + " " + edge.weight + "> " + vertexAt(edge.v));
            sb.append(vertexAt(edge.u) + " " + edge.weight + "> " + vertexAt(edge.v));
            sb.append(System.lineSeparator());
        }
        System.out.println("Total Weight: " + totalWeight(wp));
        sb.append("Total Weight: " + totalWeight(wp));
        return sb.toString();
    }

    public static final class DijkstraNode implements Comparable<DijkstraNode> {
        public final int vertex;
        public final double distance;

        public DijkstraNode(int vertex, double distance) {
            this.vertex = vertex;
            this.distance = distance;
        }

        @Override
        public int compareTo(DijkstraNode other) {
            Double mine = distance;
            Double theirs = other.distance;
            return mine.compareTo(theirs);
        }
    }

    // 记录了最小权重路径和记录了路径上每一个点的权重的最小权重数组。
    public static final class DijkstraResult {
        public final double[] distances;
        public final Map<Integer, WeightedEdge> pathMap;

        public DijkstraResult(double[] distances, Map<Integer, WeightedEdge> pathMap) {
            this.distances = distances;
            this.pathMap = pathMap;
        }
    }

    public DijkstraResult dijkstra(V root) {
        int first = indexOf(root); // find starting index
        // distances是从root到图中每个顶点的距离的占位符。起初所有这些距离都是0，
        // 因为我们并不知道这些距离有多长，所以这正是要使用Dijkstra算法解决的问题！
        // 这个就是后面准备返回的最小权重。
        // distances are unknown at first
        double[] distances = new double[getVertexCount()];
        // 第一个放入优先队列的顶点包括顶节点。
        distances[first] = 0; // root's distance to root is 0
        // 用于记录点是否被访问过。
        boolean[] visited = new boolean[getVertexCount()]; // where we've been
        visited[first] = true;
        // 这个就是后面准备返回的最小权重路径。记录了从哪一根边访问指定顶点最短。
        // how we got to each vertex
        HashMap<Integer, WeightedEdge> pathMap = new HashMap<>();
        PriorityQueue<DijkstraNode> pq = new PriorityQueue<>();
        pq.offer(new DijkstraNode(first, 0));

        // 继续执行Dijkstra算法，直到优先队列为空。
        while (!pq.isEmpty()) {
            // u是我们正要搜索的当前顶点，distanceU是已记录下来的沿着已知路径到达u的距离。
            // 当前探索过的每个顶点都是已经被找到的，因此它们必须带有已知的距离。
            // 按照权重顺序依次取出距离最小的点。
            int u = pq.poll().vertex;  // explore the next closest vertex
            // 得到当前这个点的距离。这个肯定存在，
            // 因为每次把点加入到优先队列的时候，会更新距离数组distances。
            double distanceU = distances[u];  // should already have seen it
            // 接下来，对连接到u的每条边进行探索。
            // look at every edge/vertex from the vertex in question
            for (WeightedEdge we :edgesOf(u)) {
                // distV是从u到任意已知与之相连的顶点的距离。
                // the old distance to this vertex

                // 进一步取出另一个顶点的距离。起初所有这些距离都是0，因为我们并不知道这些距离有多长。
                double distanceV = distances[
                        // 取出每一个边的另一个顶点。这个边的类WeightEdge是有方向的。
                        // 就是一个点的所有的边指的是一个WeightEdge列表，
                        // 每一个WeightEdge类的成员u表示这个点的索引，成员v表示另一个顶点。
                        we.v];
                // pathWeight是正在探索的新路径的距离。
                // the new distance to this vertex
                double pathWeight = we.weight + distanceU;
                // 如果我们找到了一个尚未被探索的顶点（!visited[we.v]）或者一条新的最短路径（distV>pathWeight），
                // new vertex or found shorter path?
                if(!visited[we.v] || (distanceV > pathWeight)) {
                    // 把这个顶点设置为被访问过。
                    visited[we.v] = true;
                    // 就会记录到达v的新的最短距离和到达那里的边。
                    // update the distance to this vertex
                    // 更新前面计算出来的这条边的另一个顶点的距离。
                    distances[we.v] = pathWeight;
                    // update the edge on the shortest path to this vertex
                    // 把这个顶点放入最小权重路径。对于HashMap对象pathMap来说，如果不存在就是添加，如果存在就是更新。
                    // 也就是说，如果不知道从哪一根边访问这个顶点，那么我们就使用这个边。
                    // 如果以前使用其他边访问这个顶点，但是使用新的边访问这个顶点距离更短。那就使用这个边。
                    pathMap.put(we.v, we);
                    // 最后，我们把新发现的路径所达到的顶点全部放入优先队列。
                    // explore it in the future
                    pq.offer(new DijkstraNode(we.v, pathWeight));
                }
            }
        }
        // dijkstra()返回从根顶点到加权图中每个顶点的距离，以及能够揭示到达这些顶点的最短路径pathMap。
        // return distances and pathMap
        // 当我们退出循环的时候，我们就得到了最小权重路径和记录了路径上每一个点的权重的最小权重数组。
        return new DijkstraResult(distances, pathMap);
    }

    // 一个帮助者函数。把距离数组转换成距离Map。
    public Map<V, Double> distanceArrayToDistanceMap(double[] distances) {
        HashMap<V, Double> distanceMap = new HashMap<V, Double>();
        for (int i = 0; i< distances.length; i++) {
            distanceMap.put(vertexAt(i), distances[i]);
        }
        return distanceMap;
    }
    // 一个帮助者函数。给出指定两个点的路径。
    public static List<WeightedEdge> pathMapToPath(int start, int end, Map<Integer, WeightedEdge> pathMap) {
        // 如果传入的路径为空，就直接返回空列表
        if (pathMap.size() == 0) {
            return Arrays.asList();
        }
        LinkedList<WeightedEdge> path = new LinkedList<>();
        // 找到终点对应的距离最短边。
        WeightedEdge edge = pathMap.get(end);
        path.add(edge);
        // 从这个边开始找起点。
        while (edge.u != start) {
            edge = pathMap.get(edge.u);
            path.add(edge);
        }
        // 上面找到的路径是从终点到起点的。因此上需要反过来。
        Collections.reverse(path);
        return path;
    }
}

