package com.jiaming.mycompsciproblems;

import java.util.List;

// Board是用于维护位置状态的类的接口。针对本章搜索算法将要计算的游戏，我们需要解决以下4个问题：
// Board中的每个方法和属性分别代表了上述某个问题或操作。
// 在游戏中，Board接口也可以被命名为棋局（Position），但这里我们会用该命名来表示更为具体的子类。
public interface Board<
        // Board拥有一个泛型Move。Move类型代表游戏中的一步棋，本章中它是一个整数。
        // 在井字棋和四子棋等游戏中，整数可以通过指示棋子应该放置的方格或列来表示一步棋的走法。
        // 在更为复杂的游戏中，可能需要更多的整数来表述一步棋的走法。使用Move泛型可以使Board适用于多种游戏。
        Move> {
    // 1. 轮到谁走棋？
    Piece getTurn();

    // 2. 在当前位置有哪些符合规则的走法？
    List<Move> getLegalMoves();
    // 3. 是否有人赢得游戏？
    boolean isWin();
    // 4. 游戏是否平局？
    // 对于很多游戏而言，最后一个问题其实是前两个问题的结合。
    // 如果游戏没有人赢，也没有符合规则的移动棋子的方法，那么就是平局。
    // 因此，在接口Board中定义了isDraw()的默认实现。
    default boolean isDraw() {
        return !isWin() && getLegalMoves().isEmpty();
    }

    // 此外，我们还要实现以下操作：
    // 1. 从当前位置移动到新的位置。
    Board<Move> move(Move location) ;
    // 2. 评估当前位置，看哪位玩家占据了优势。
    double evaluate(Piece player);
}

