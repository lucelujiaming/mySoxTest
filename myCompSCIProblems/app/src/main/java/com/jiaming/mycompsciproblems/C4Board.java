package com.jiaming.mycompsciproblems;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

// 接下来，我们回到四子棋代码的核心——C4Board类。这个类定义了一些静态常量和一个静态方法。
public class C4Board implements Board<Integer> {
    private static final int NUM_COLUMNS = 7;
    private static final int NUM_ROWS = 6;
    private static final int SEGMENT_LENGTH = 4;

    // 如果能快速搜索棋盘中所有的段的话，对检查游戏是否结束（有人获胜）和对棋局进行评估都非常有帮助。
    // 因此，在下面的代码片段中，我们会在C4Board类中使用名为SEGMENTS的变量来缓存棋盘中的所有段。
    private static final ArrayList<C4Location[]> SEGMENTS =  generateSegments();

    // 静态方法generateSegments()返回一个网格位置C4Location的数组列表。
    // 列表中的每个数组包含四个网格位置。我们把这些由四个网格位置组成的数组称为段（segment）。
    // 如果棋盘上的任意段都为相同颜色的棋子，那么执这种颜色棋子的人就赢得了这个棋局。
    private static ArrayList<C4Location[]> generateSegments() {
        ArrayList<C4Location[]> segments = new ArrayList<>();
        // 所有竖向的情况。
        for (int c =0; c < NUM_COLUMNS; c++) {
            for (int r =0; r <= NUM_ROWS - SEGMENT_LENGTH; r++) {
                C4Location[] b1 = new C4Location[SEGMENT_LENGTH];
                for (int i = 0; i< SEGMENT_LENGTH; i++) {
                    b1[i] = new C4Location(c, r + i);
                }
                segments.add(b1);
            }
        }

        // 所有横向的情况。
        for (int c =0; c < NUM_COLUMNS - SEGMENT_LENGTH; c++) {
            for (int r =0; r <= NUM_ROWS; r++) {
                C4Location[] b1 = new C4Location[SEGMENT_LENGTH];
                for (int i = 0; i< SEGMENT_LENGTH; i++) {
                    b1[i] = new C4Location(c + i, r);
                }
                segments.add(b1);
            }
        }

        // 所有对角线的情况。
        for (int c =0; c < NUM_COLUMNS - SEGMENT_LENGTH; c++) {
            for (int r =0; r <= NUM_ROWS - SEGMENT_LENGTH; r++) {
                C4Location[] b1 = new C4Location[SEGMENT_LENGTH];
                for (int i = 0; i< SEGMENT_LENGTH; i++) {
                    b1[i] = new C4Location(c + i, r + i);
                }
                segments.add(b1);
            }
        }

        // 所有逆对角线的情况。
        for (int c = NUM_COLUMNS - SEGMENT_LENGTH; c >= 0 ; c--) {
            for (int r = SEGMENT_LENGTH - 1; r < NUM_ROWS ; r++) {
                C4Location[] b1 = new C4Location[SEGMENT_LENGTH];
                for (int i = 0; i< SEGMENT_LENGTH; i++) {
                    b1[i] = new C4Location(c + i, r - i);
                }
                segments.add(b1);
            }
        }
        return segments;
    }

    // 我们在position这个二维C4Piece类型数组中存储当前位置。
    // 大多数情况下，二维数组都会以行优先的方式进行索引。
    // 这是考虑到四子棋是由7列组成的并且为了使C4Board类的其余部分编写起来容易一些。
    private C4Piece[][] position;
    // 例如，数组columnCount记录了给定列中每次有多少个棋子。
    // 由于每次落子本质上都是选择没有被填充的列，因此这使得生成合法的落子方式非常容易。
    private int[] columnCount ;
    private C4Piece turn;

    // 初始化一个空白的C4Board。
    public C4Board() {
        position = new C4Piece[NUM_COLUMNS][NUM_ROWS];
        for (C4Piece[] col : position) {
            Arrays.fill(col, C4Piece.E);
        }
        columnCount = new int[NUM_COLUMNS];
        turn = C4Piece.B;
    }

    // 使用一个已经玩了一半的棋盘位置来初始化一个已经玩了一半的棋盘。
    public C4Board(C4Piece[][] position, C4Piece turn) {
        this.position = position;
        columnCount = new int[NUM_COLUMNS];
        for (int c =0; c < NUM_COLUMNS; c++) {
            int piecesInColumn = 0;
            for (int r =0; r <= NUM_ROWS; r++) {
                if(position[c][r] != C4Piece.E) {
                    piecesInColumn++;
                }
            }
            columnCount[c] = piecesInColumn;
        }
        this.turn = turn;
    }

    // 私有辅助方法countSegment()返回特定段中黑色或红色棋子的数量。
    @Override
    public Piece getTurn() {
        return turn;
    }

    @Override
    public C4Board move(Integer location) {
        C4Piece[][] tempPosition = Arrays.copyOf(position, position.length);
        for (int col = 0; col < NUM_COLUMNS; col++) {
            tempPosition[col] = Arrays.copyOf(position[col], position[col].length);
        }
        tempPosition[location][columnCount[location]] = turn;
        return new C4Board(tempPosition, turn.opposite());
    }

    @Override
    public List<Integer> getLegalMoves() {
        ArrayList<Integer> legalMoves = new ArrayList<>();
        // 一列一列查看。
        for (int i = 0; i < NUM_COLUMNS; i++) {
            // 只要这一列还有空间，就就是有效的。
            if(columnCount[i] < NUM_ROWS) {
                legalMoves.add(i);
            }
        }
        return legalMoves;
    }

    // 接下来是检查棋局胜负的方法isWin()。该方法检查棋盘中的所有段，
    // 并通过countSegment()方法来查看是否有段是由四个相同颜色的棋子所组成的，
    // 以此来确定是否分出胜负。
    private int countSegment(C4Location[] segment, C4Piece color) {
        int count = 0;
        for (C4Location location : segment) {
            if(position[location.column][location.row] == color) {
                count++;
            }
        }
        return count;
    }
    //
    @Override
    public boolean isWin() {
        for (C4Location[] segment : SEGMENTS) {
            int blackCount = countSegment(segment, C4Piece.B);
            int redCount = countSegment(segment, C4Piece.R);
            if ((blackCount == SEGMENT_LENGTH) || (redCount == SEGMENT_LENGTH)) {
                return true;
            }
        }
        return false;
    }

    private double evaluateSegment(C4Location[] segment, Piece player) {
        int blackCount = countSegment(segment, C4Piece.B);
        int redCount = countSegment(segment, C4Piece.R);
        if ((blackCount > 0) || (redCount > 0)) {
            return 0.0;
        }
        int count = Math.max(blackCount, redCount);
        double score = 0.0;
        // 最后，为了对整个棋局进行评估，我们将会对其全部的段逐一进行评估，
        // 将评估的结果进行累加并返回。
        // 含有两种颜色棋子的段被认为是毫无价值的。
        // 由两个相同颜色的棋子和两个空格所构成的段会被评估为1。
        if (count == 2) {
            score = 1.0;
        // 由3个相同颜色的棋子构成的段会被评估为100。
        } else if (count == 3) {
            score = 100.0;
        // 最后，由四个相同颜色的棋子（有人获胜）构成的段会被评估为1000000。
         // 这些评估分数可以是任意的，重点在于它们彼此之间的相对权重。
        } else if (count == 4) {
            score = 1000000.0;
        }
        C4Piece color = (redCount > blackCount) ? C4Piece.R : C4Piece.B;
        // 如果该段属于对手，那么分数为负数。
        if (color != player) {
            return -score;
        }
        return score;
    }

    @Override
    public double evaluate(Piece player) {
        double total = 0.0;
        // 私有辅助方法evaluateSegment()使用上述公式来计算每个段。
        // 所有经过evaluateSegment()评估的段的总分由evaluate()生成。
        for (C4Location[] segment : SEGMENTS) {
            total += evaluateSegment(segment, player);
        }
        return total;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (int r = NUM_ROWS - 1; r >= 0; r--) {
            sb.append("|");
            for (int c = 0; c < NUM_COLUMNS; c++) {
                sb.append(position[c][r].toString());
                sb.append("|");
            }
            sb.append(System.lineSeparator());
        }
        return sb.toString();
    }
}
