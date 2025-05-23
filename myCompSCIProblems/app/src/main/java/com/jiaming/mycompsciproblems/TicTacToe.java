package com.jiaming.mycompsciproblems;

import java.util.Scanner;

// 接下来就可以开发一个完整的能走完井字棋棋局的程序了。
// AI不再是对测试棋局进行评估，而是需要对两个棋手下棋所形成的棋局进行评估。
// 在下面的代码片段中，井字棋AI将会与执先手的人类棋手对弈。
public class TicTacToe {
    private TTTBoard board = new TTTBoard();
    private Scanner scanner = new Scanner(System.in);

    // 获得用户输入。
    private Integer getPlayerMove() {
        Integer playerMove = -1;
        while (!board.getLegalMoves().contains(playerMove)) {
            System.out.println("Enter a legal square (0-8):");
            Integer play = scanner.nextInt();
            playerMove = play;
        }
        return playerMove;
    }

    private void runGame() {
        while (true) {
            Integer humanMove = getPlayerMove();
            board = board.move(humanMove);
            if (board.isWin()) {
                System.out.println("Human wins!");
                break;
            }
            else if (board.isDraw()) {
                System.out.println("Draw!");
                break;
            }

            Integer computerMove = Minimax.findBestMove(board, 9);
            System.out.println("Computer move is " + computerMove);
            board = board.move(computerMove);
            System.out.println(board);
            if (board.isWin()) {
                System.out.println("Computer wins!");
                break;
            }
            else if (board.isDraw()) {
                System.out.println("Draw!");
                break;
            }
        }
    }
}

