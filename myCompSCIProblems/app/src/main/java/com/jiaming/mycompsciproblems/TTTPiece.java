package com.jiaming.mycompsciproblems;

// 为TTTPiece的枚举类，它实现了Piece。枚举类中可以用X、O或E来表示方格中的棋子，其中E表示没有放置棋子。
public enum  TTTPiece implements Piece {
    X, O, E;

    // 枚举类TTTPiece中定义了一个opposite方法，它的返回值为TTTPiece类型。
    // 在一步棋走完后，该方法将游戏回合从一个玩家转换到另一个玩家。
    // 每步棋可以用一个整数表示，该整数对应于棋盘上放置该棋子的方格。
    // 在之前的代码中，Move是以泛型的形式定义在Board中的，而现在我们定义TTTBoard时将Move指定为Integer类型。
    @Override
    public TTTPiece opposite() {
        switch (this) {
            case X:
                return TTTPiece.O;
            case O:
                return TTTPiece.X;
            default:
                return TTTPiece.E ;
        }
    }

    @Override
    public String toString() {
        switch (this) {
            case X:
                return "X";
            case O:
                return "O";
            default:
                return " " ;
        }
    }
}

