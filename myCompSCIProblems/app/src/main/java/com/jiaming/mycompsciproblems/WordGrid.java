package com.jiaming.mycompsciproblems;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

// 单词搜索问题的网格与第2章中的迷宫有点类似。
// WordGrid类似于Maze，而GridLocation与MazeLocation类似。
public class WordGrid {
    public static class GridLocation {
        public final int rowPos;
        public final int columnPos;

        public GridLocation(int rowPos, int columnPos) {
            this.rowPos = rowPos;
            this.columnPos = columnPos;
        }

        @Override
        public int hashCode() {
            final int prime = 31;
            int result = 1;
            result = prime * result + columnPos;
            result = prime * result + rowPos;
            return result;
        }

        @Override
        public boolean equals(Object obj) {
            if(this == obj){
                return true;
            }
            if (obj == null) {
                return false;
            }
            if (getClass() != obj.getClass()){
                return false;
            }
            GridLocation other = (GridLocation)obj;
            if (columnPos != other.columnPos) {
                return false;
            }
            if (rowPos != other.rowPos) {
                return false;
            }
            return true;
        }
    }

    private final char ALPHABET_LENGTH = 26;
    private final char FIRST_LETTER = 'A';
    // WordGrid类将在内部保存一个表示其状态的网格（二维数组）。
    private   char[][] grid;
    // 除此之外，还有用于记录行号、列号、起始位置和终点位置的实例变量。路障单元格会随机填入这个网格中。
    private final int rows, columns;

    public WordGrid(int rows, int columns) {
        this.rows = rows;
        this.columns = columns;
        grid = new char[rows][columns];
        randomlyFill();
    }

    private void randomlyFill() {
        Random random = new Random();
        for (int row = 0; row < rows; row++) {
            for (int column = 0; column < columns; column++) {
                char randomLetter = (char)(random.nextInt(ALPHABET_LENGTH) + FIRST_LETTER);
                grid[row][column] = randomLetter;
            }
        }
    }

    // 为了便于展示，我们可以将迷宫中的成功路径、起始状态和终点状态标记出来。
    public void mark(String word, List<GridLocation> locations) {
        for (int i = 0; i < word.length(); i++) {
            GridLocation location = locations.get(i);
            grid[location.rowPos][location.columnPos] = word.charAt(i);
        }
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (char[] rowArray :grid){
            sb.append(rowArray);
            sb.append(System.lineSeparator());
        }
        return sb.toString();
    }

    // 为了将单词在网格中的位置标识出来，需要生成其值域。
    // 单词的域值是其全部字母可能放置的位置的列表的列表（List<List<GridLocation>>）。
    // 但是单词不能随意放置，必须位于网格范围内的行、列或对角线上。也就是说，单词长度不能超过网格的边界。
    public List<List<GridLocation>> generateDomain(String word) {
        List<List<GridLocation>>domain = new ArrayList<>();
        int length = word.length();
        // 首先我们认为一个单词网格的左上角为原点。横轴为不同的column，纵轴为不同的row。
        // 包括四种情况。
        for (int row = 0; row < rows; row++) {
            for (int column = 0; column < columns; column++) {
                if (column + length <= columns) {
                    //   1.第一种是沿着一行从左到右排列。也就是row不变，column累加。实现函数为fillRight函数。
                    // 因为，我们只是为单词生成位置信息。因此上，只需要知道单词长度就好了。不需要把单词本身传进去。
                    fillRight(domain, row, column, length);
                    // 2.第二种是沿着对角线从左上角到右下角。也就是row累加，column累加。实现函数为fillDiagonalRight函数。
                    if (row + length <= rows) {
                        fillDiagonalRight(domain, row, column, length);
                    }
                }
                if (row + length <= rows) {
                    // 3.第三种是沿着一列从上到下排列。也就是row累加，column不变。实现函数为fillDown函数。
                    fillDown(domain, row, column, length);
                    if (column - length >= 0) {
                        // 4.第四种是沿着对角线从右上角到左下角。也就是row累加，column累次递减。实现函数为fillDiagonalLeft函数。
                        fillDiagonalLeft(domain, row, column, length);
                    }
                }
            }
        }
        return domain;
    }

    private void fillRight(List<List<GridLocation>> domain, int row, int column, int length) {
        List<GridLocation> locations = new ArrayList<>();
        for(int c = column; c < (column + length); c++) {
            locations.add(new GridLocation(row, c));
        }
        domain.add(locations);
    }

    private void fillDiagonalRight(List<List<GridLocation>> domain, int row, int column, int length) {
        List<GridLocation> locations = new ArrayList<>();
        int r = row;
        for(int c = column; c < (column + length); c++) {
            locations.add(new GridLocation(row, c));
            r++;
        }
        domain.add(locations);
    }
    private void fillDown(List<List<GridLocation>> domain, int row, int column, int length) {
        List<GridLocation> locations = new ArrayList<>();
        for(int r = row; r < (row + length); r++) {
            locations.add(new GridLocation(r, column));
        }
        domain.add(locations);
    }
    private void fillDiagonalLeft(List<List<GridLocation>> domain, int row, int column, int length) {
        List<GridLocation> locations = new ArrayList<>();
        int c = column;
        for(int r = row; r < (row + length); r++) {
            locations.add(new GridLocation(r, c));
            c--;
        }
        domain.add(locations);
    }
}

