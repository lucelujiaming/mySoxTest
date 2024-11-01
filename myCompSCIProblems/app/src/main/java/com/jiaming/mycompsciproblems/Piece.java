package com.jiaming.mycompsciproblems;

// Piece是游戏中棋盘上的棋子接口。
public interface Piece {
    // 它还兼有回合指示器的作用，因此需要带有opposite方法。
    // 我们需要知道在给定的回合后轮到谁来走棋。
    Piece opposite();
}

