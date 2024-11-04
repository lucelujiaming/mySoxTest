package com.jiaming.mycompsciproblems;

import java.util.Scanner;

public class ConnectFour {
    private C4Board board = new C4Board();
    private Scanner scanner = new Scanner(System.in);

    // 获得用户输入。
    private Integer getPlayerMove() {
        Integer playerMove = -1;
        while (!board.getLegalMoves().contains(playerMove)) {
            System.out.println("Enter a legal column (0-6):");
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
            // 我们为井字棋开发的minimax()和findBestMove()函数不做任何改动就可以直接在四子棋的代码中进行复用。
            // 下面的代码片段与井字棋AI代码只有少许的不同。其中最大的区别就是把maxDepth设置为5。
            Integer computerMove = Minimax.findBestMove(board, 5);
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
