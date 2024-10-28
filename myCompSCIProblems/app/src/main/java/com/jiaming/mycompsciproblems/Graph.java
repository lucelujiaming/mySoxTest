package com.jiaming.mycompsciproblems;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public abstract class Graph<V, E extends Edge> {
    // vertices列表是Graph类的核心。
    // 每个顶点都会被存储在该列表中，稍后我们将通过它们在列表中的整数索引来引用它们。
    // 顶点本身可能是一个复杂的数据类型，但它的索引始终是一个int类型，以方便使用。
    private ArrayList<V> vertices = new ArrayList<V>();
    // 而在本示例中，我们将使用邻接表来进行表示。
    // 在邻接表中，每个顶点都有一个与其相连接的顶点的列表。
    // 这里采用由边的列表组成的列表，因此每个顶点都带有一个含有多条边的列表，
    // 顶点通过该列表展示了其与哪些顶点相连接。edges就是这个由列表元素组成的列表。
    protected ArrayList<ArrayList<E>> edges = new ArrayList<>();

    public Graph() {

    }

    public Graph(List<V> vertices) {
        this.vertices.addAll(vertices);
        for (V vetex : vertices) {
            edges.add(new ArrayList<>());
        }
    }

    public int getVertexCount() {
        return vertices.size();
    }

    public int getEdgeCount() {
        // return edges.stream().mapToInt(ArrayList::size).sum();x
        int iEdgeCount = 0;
        for (ArrayList<E> item : edges) {
            iEdgeCount += item.size();
        }
        return iEdgeCount;
    }

    // Add a vertex to the graph and return its index
    public int addVertex(V vertex) {
        // Add a vertex to the graph
        vertices.add(vertex);
        edges.add(new ArrayList<>());
        // Areturn its index
        return getVertexCount() - 1;
    }

    // Find the vertex at a specific index
    public V vertexAt(int index) {
        return vertices.get(index);
    }

    // Find the index of a vertex in the graph
    public int indexOf(V vertex) {
        return vertices.indexOf(vertex);
    }

    // Find the vertices that a vertex at some index is connected to
    public List<V> neighborsOf(int index) {
        // return edges.get(index).stream()
        //         .map(edge -> vertexAt(edge.v))
        //         .collect(Collectors.toList());
        List<V> listReturn = new ArrayList<V>();
        for (E item : edges.get(index)) {
            listReturn.add(vertexAt(item.v));
        }
        return listReturn;
    }

    public List<V> neighborsOf(V vertex) {
        return neighborsOf(indexOf(vertex));
    }

    public List<E> edgesOf(int index) {
        return edges.get(index);
    }

    public List<E> edgesOf(V vertex) {
        return edges.get(indexOf(vertex));
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < getVertexCount(); i++){
            sb.append(vertexAt(i));
            sb.append(" -> ");
            sb.append(Arrays.toString(neighborsOf(i).toArray()));
            sb.append(System.lineSeparator());
        }
        return sb.toString();
    }
}

