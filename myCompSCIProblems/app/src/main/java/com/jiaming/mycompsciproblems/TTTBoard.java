package com.jiaming.mycompsciproblems;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

// 井字棋的棋盘由3行3列共9个位置组成。为简单起见，可以使用一维数组来表示这9个位置。
// 每个位置的数字标识（也就是数组中的索引）可以随意设定。
public class TTTBoard implements Board<Integer> {
    // 棋盘的状态保存在TTTBoard类中。TTTBoard中会记录两种不同的状态：位置（由上述一维数组来表示）和轮到哪位玩家下棋。
    private static final int NUM_SQUARES = 9;
    private TTTPiece[] position;
    private TTTPiece turn;

    public TTTBoard(TTTPiece[] position, TTTPiece turn) {
        this.position = position;
        this.turn = turn;
    }
    // 默认棋盘是一个尚未开始下棋的空棋盘。TTTBoard的无参构造函数用来初始化这样的空棋盘，
    public TTTBoard() {
        position = new TTTPiece[NUM_SQUARES];
        Arrays.fill(position, TTTPiece.E);
        // 并且由X先走棋（井字棋中通常由X玩家先走棋）。
        turn = TTTPiece.X;
    }

    // getTurn()表明当前位置是由X下棋还是由O下棋。
    @Override
    public Piece getTurn() {
        return turn;
    }

    // 在井字棋游戏中，空的方格都是可以落子的。
    @Override
    public TTTBoard move(Integer location) {
        TTTPiece[] tempPosition = Arrays.copyOf(position, position.length);
        tempPosition[location] = turn;
        return new TTTBoard(tempPosition, turn.opposite());
    }

    // getLegalMoves()用来查找棋盘上所有空着的方格并以列表的形式返回。
    @Override
    public List<Integer> getLegalMoves() {
        ArrayList<Integer> legalMoves = new ArrayList<>();
        for (int i = 0; i < NUM_SQUARES; i++) {
            if(position[i] == TTTPiece.E) {
                legalMoves.add(i);
            }
        }
        return legalMoves;
    }

    // 有许多方法可以对井字棋的行、列和对角线进行扫描，以便检查游戏是否分出胜负。
    // 下面对isWin()及其辅助方法checkPos()的代码实现都采用了硬编码的方式。
    @Override
    public boolean isWin() {
        // three row, three column, and then two diagonal checks
        return checkPos(0, 1, 2) || checkPos(3, 4, 5) || checkPos(6, 7, 8)         // 同一行。
                || checkPos(0, 3, 6) || checkPos(1, 4, 7) || checkPos(2, 5, 8)      // 同一列。
                || checkPos(0, 4, 8) || checkPos(2, 4, 6);                          // 同一对角线。
    }

    // 检查三个位置是否都是一样的。
    private boolean checkPos(int p0, int p1, int p2) {
        return position[p0] == position[p1] && position[p0] == position[p2]
                && position[p0] != TTTPiece.E;
    }

    // 如果游戏没有分出胜负并且没有能够落子的方格，那么游戏就以平局结束。
    // 这种情况已经被Board接口中的默认方法isDraw()覆盖了。
    // 最后，我们需要一个能够评估棋局地方法和一个能将棋盘美观地打印出来的方法。
    @Override
    public double evaluate(Piece player) {
        if (isWin() && turn == player) {
            return -1;      // 败
        } else if (isWin() && turn != player) {
            return 1;       // 胜
        } else {
            return 0.0;     // 平
        }
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                sb.append(position[row * 3 + col].toString());
                if (col != 2) {
                    sb.append("|");
                }
            }
            sb.append(System.lineSeparator());
            if (row != 2) {
                sb.append("-----");
                sb.append(System.lineSeparator());
            }
        }
        return sb.toString();
    }
}

