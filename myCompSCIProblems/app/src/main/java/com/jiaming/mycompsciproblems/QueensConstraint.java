package com.jiaming.mycompsciproblems;

import java.util.List;
import java.util.Map;

public class QueensConstraint extends Constraint<Integer, Integer> {
    // 皇后所在格子的列号可以作为此约束满足问题中的变量。皇后可能被摆放的格子的行号（1～8）可以作为值域。
    private List<Integer> columns;

    public QueensConstraint(List<Integer> columns) {
        super(columns);
        this.columns = columns;
    }

    @Override
    public boolean satisfied(Map<Integer, Integer> assignment) {
        // 由于在一开始每个皇后都被赋予了不同的列号，因此为了解决这一问题，
        // 我们需要有一个检查任意两个皇后是否位于同一行或同一对角线上的约束。
        // 想要检查是否位于同一行是非常容易的，但检查是否位于同一条对角线就需要用到一些数学知识
        //   ——如果任意两个皇后位于同一条对角线，则它们所在的行差和列差必定相同。
        // 具体逻辑如下：
        // 逐个检查分配方案的中的每一个皇后。
        for (Map.Entry<Integer, Integer> item : assignment.entrySet()) {
            // 得到皇后所在的行和列。
            // queenOneColumn means queen onw column, queenOneRow means queen onw row
            int queenOneColumn = item.getKey();
            int queenOneRow = item.getValue();
            // 循环后续的列。因为这个assignment是有次序的，也就是元素是从左到右排列的。
            // 按照顺序元素的column也是从大到小的。
            // 因此上，这里从queenOneColumn + 1开始循环，就已经处理了同一列的问题。
            // q2c = queen 2 column
            for (int queenTwoColumn = queenOneColumn + 1;
                 queenTwoColumn <= columns.size(); queenTwoColumn++) {
                // 如果对应的列上有皇后。
                if (assignment.containsKey(queenTwoColumn)) {
                    // 检查是否位于同一行是非常容易的。
                    // q2r = queen 2 row
                    int queenTwoRow = assignment.get(queenTwoColumn);
                    if (queenOneRow == queenTwoRow) {
                        return false;
                    }
                    // 如果任意两个皇后位于同一条对角线，则它们所在的行差和列差必定相同。
                    // same diagonal?
                    if (Math.abs(queenOneRow - queenTwoRow) == Math.abs(queenOneColumn - queenTwoColumn)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
}
