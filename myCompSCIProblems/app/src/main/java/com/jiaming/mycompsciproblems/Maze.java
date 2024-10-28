package com.jiaming.mycompsciproblems;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Maze {
    // 我们的迷宫是由一些Cell组成的二维网格。
    // Cell是一个能够将自身转换成String的enum类型。
    // 例如：“　”表示空白单元格，“X”表示路障单元格。
    // 还有一些在打印迷宫时供演示用的其他单元格。
    public enum Cell {
        EMPTY(" "),
        BLOCKED("X"),
        START("S"),
        GOAL("G"),
        PATH("*");

        private final String code;

        private Cell(String c) {
            code = c;
        }

        @Override
        public String toString() {
            return code;
        }
    }

    // 还需要一种表示迷宫中各个位置的方法。只需要使用MazeLocation类即可，其属性表示当前位置对应的行和列。
    public static class MazeLocation {
        public final int rowPos;
        public final int columnPos;

        public MazeLocation(int rowPos, int columnPos) {
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
            MazeLocation other = (MazeLocation)obj;
            if (columnPos != other.columnPos) {
                return false;
            }
            if (rowPos != other.rowPos) {
                return false;
            }
            return true;
        }
    }
    // Maze类将在内部保存一个表示其状态的网格（二维数组）。
    private   Cell[][] grid;
    // 除此之外，还有用于记录行号、列号、起始位置和终点位置的实例变量。路障单元格会随机填入这个网格中。
    private final int rows, columns;
    private final MazeLocation start, goal;

    public Cell[][] getGrid() {
        return grid;
    }

    public int getRows() {
        return rows;
    }

    public int getColumns() {
        return columns;
    }

    public MazeLocation getStart() {
        return start;
    }

    public MazeLocation getGoal() {
        return goal;
    }

    public Maze(int rows, int columns, MazeLocation start, MazeLocation goal, double sparseness) {
        this.rows = rows;
        this.columns = columns;
        this.start = start;
        this.goal = goal;
        // new grid
        grid = new Cell[rows][columns];
        // Init grid
        for (Cell[] row : grid) {
            Arrays.fill(row, Cell.EMPTY);
        }
        // populate the grid with blocked cells
        randomlyFill(sparseness);
        // fill the start and goal locations
        grid[start.rowPos][start.columnPos] = Cell.START;
        grid[goal.rowPos][goal.columnPos] = Cell.GOAL;
    }

    public Maze() {
        this(20, 20, new MazeLocation(0, 0), new MazeLocation(19, 19), 0.2);
    }

    private void randomlyFill(double sparseness) {
        for (int row = 0; row < rows; row++) {
            for (int column = 0; column < columns; column++) {
                // 只有小于这个阈值才会创建墙壁
                if (Math.random() < sparseness) {
                    grid[row][column] = Cell.BLOCKED;
                }
            }
        }
    }

    @Override
    public String toString() {
        StringBuffer strBuffer = new StringBuffer();
        for (Cell[] row : grid){
            for (Cell cell : row) {
                strBuffer.append(cell);
            }
            // Add CR
            strBuffer.append(System.lineSeparator());
        }
        return strBuffer.toString();
    }

    // 如果有一个方法能够在搜索过程中检查我们是否已经抵达目的地，将会便利很多。
    public boolean goalTest(MazeLocation mazeLocation) {
        return goal.equals(mazeLocation);
    }
    // 怎么才能在迷宫内移动呢？假设我们一次可以从给定的迷宫单元格开始向水平或垂直方向移动一格。
    // 按照这个规则，可以用successors()方法寻找给定MazeLocation的下一个可到达的位置。
    // 但是，每个Maze的大小和路障数量都不尽相同，所以每个Maze的successors()方法也会有所不同。
    // 因此，我们在Maze类中定义这个移动到下一步的方法。
    public List<MazeLocation> successors(MazeLocation mazeLocation) {
        List<MazeLocation> locations = new ArrayList<>();
        // 如果在行方向上没有正向越界，而在行方向上可以前进一步，则向行方向上前进一步。
        if(mazeLocation.rowPos + 1 < rows &&
                grid[mazeLocation.rowPos + 1][mazeLocation.columnPos] != Cell.BLOCKED) {
            locations.add(new MazeLocation(mazeLocation.rowPos + 1, mazeLocation.columnPos));
        }
        // 如果在行方向上没有反向越界，而在行方向上可以后退一步，则向行方向上后退一步。
        if(mazeLocation.rowPos - 1 > 0 &&
            grid[mazeLocation.rowPos - 1][mazeLocation.columnPos] != Cell.BLOCKED) {
            locations.add(new MazeLocation(mazeLocation.rowPos - 1, mazeLocation.columnPos));
        }
        // 如果在列方向上没有正向越界，而在列方向上可以前进一步，则向列方向上前进一步。
        if(mazeLocation.columnPos + 1 < rows &&
                grid[mazeLocation.rowPos][mazeLocation.columnPos + 1] != Cell.BLOCKED) {
            locations.add(new MazeLocation(mazeLocation.rowPos, mazeLocation.columnPos + 1));
        }
        // 如果在列方向上没有反向越界，而在列方向上可以后退一步，则向列方向上后退一步。
        if(mazeLocation.columnPos - 1 > 0 &&
                grid[mazeLocation.rowPos][mazeLocation.columnPos - 1] != Cell.BLOCKED) {
            locations.add(new MazeLocation(mazeLocation.rowPos, mazeLocation.columnPos - 1));
        }

        return locations;
    }

    // 为了便于展示，我们可以将迷宫中的成功路径、起始状态和终点状态标记出来。
    public void mark(List<MazeLocation> path) {
        for (MazeLocation loc : path) {
            grid[loc.rowPos][loc.columnPos] = Cell.PATH;
        }
        grid[start.rowPos][start.columnPos] = Cell.START;
        grid[goal.rowPos][goal.columnPos] = Cell.GOAL;
    }

    // 同时移除路径功能也很有用，这样我们就可以在同一个迷宫中尝试不同的搜索算法。
    public void clear(List<MazeLocation> path) {
        for (MazeLocation loc : path) {
            grid[loc.rowPos][loc.columnPos] = Cell.EMPTY;
        }
        grid[start.rowPos][start.columnPos] = Cell.START;
        grid[goal.rowPos][goal.columnPos] = Cell.GOAL;
    }

    // 众所周知，几何学中两点之间直线距离最短。
    // 因此在求解迷宫问题时，可以采用直线启发式信息。由勾股定理推导出来的欧氏距离（Euclidean distance）。
    public double euclideanDistance(MazeLocation ml) {
        int xdist = ml.columnPos - goal.columnPos;
        int ydist = ml.rowPos - goal.rowPos;
        return Math.sqrt((xdist * xdist) + (ydist * ydist));
    }

    // 计算迷宫中两个点之间的行列差之和得到曼哈顿距离。
    public double manhattanDistance(MazeLocation ml) {
        int xdist = Math.abs(ml.columnPos - goal.columnPos);
        int ydist = Math.abs(ml.rowPos - goal.rowPos);
        return (xdist + ydist);
    }
}

