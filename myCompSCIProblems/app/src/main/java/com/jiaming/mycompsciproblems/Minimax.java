package com.jiaming.mycompsciproblems;

// 极小化极大是一种用来在双人、零和、全信息的对弈游戏（如井字棋、跳棋或国际象棋）中找到最佳走法的经典算法。
// 它还针对其他类型的游戏进行了扩展和修改。
// 极小化极大算法通常使用递归函数实现，这时两个玩家要么是极大化玩家，要么是极小化玩家。
public class Minimax {

    // 调用minimax()将返回极大化玩家的起始位置的评估。
    public static <Move> double minimax(Board<Move> board,
              boolean maxmizing, Piece originalPlayer, int maxDepth) {
        // 如果胜负已分，或者出现了平局，或者达到了迭代次数。就可以直接调用评估函数了，
        if(board.isWin() || board.isDraw() || maxDepth == 1) {
            // 就TTTBoard类的evaluate()方法而言，如果双方的最佳走法将导致极大化玩家获胜，则返回1；
            // 如果将导致极大化玩家输棋，则返回-1；如果导致平局，则返回0。
            return board.evaluate(originalPlayer);
        }

        // 这些数字将会在达到基线条件时返回，然后再沿着到达基线条件的各层递归调用逐级向上返回。
        // 对于每次求极大化的递归调用，向上返回的是下一步走法的最佳评分。
        if (maxmizing) {
            double bestEval = Double.NEGATIVE_INFINITY;
            for (Move move : board.getLegalMoves()) {
                double result = minimax(board.move(move), false, originalPlayer, maxDepth - 1);
                bestEval = Math.max(result, bestEval);
            }
            return bestEval;
        }
        // 对于每次求极小化的递归调用，向上返回的是下一步走法的最差评分。
        // 这样，决策树就建立起来了。
        else {
            double worstEval = Double.POSITIVE_INFINITY;
            for (Move move : board.getLegalMoves()) {
                double result = minimax(board.move(move), true, originalPlayer, maxDepth - 1);
                worstEval = Math.min(result, worstEval);
            }
            return worstEval;
        }
    }

    // 可惜我们无法使用已经实现的minimax()方法来寻找给定棋局的最佳走法。
    // 该方法返回一个double类型的估计值，无法给出生成该评分的最佳走法的第一步该如何走。
    // 因此，我们需要定义一个辅助函数findBestMove()，
    // 我们可以将findBestMove()视作对minimax()的第一次求最大值调用，只是带上了初始的棋步而已。
    public static <Move> Move findBestMove(Board<Move> board, int maxDepth) {
        double bestEval = Double.NEGATIVE_INFINITY;
        Move bestMove = null;
        // 它会对每个合法的走法都调用一次minimax()，以便找出评分最高的走法。
        for (Move move : board.getLegalMoves()) {
            double result = minimax(board.move(move), false, board.getTurn(), maxDepth);
            if(result > bestEval) {
                bestEval = result;
                bestMove = move;
            }
        }
        return bestMove;
    }

    // 极小化极大算法的效果很好，但目前还无法实现很深的搜索。
    // 极小化极大算法有一个扩展算法，被称为alpha-beta剪枝算法，
    // 该算法在搜索时可以排除掉那些不会生成更优结果的棋局，由此来增加搜索的深度。
    // 该算法可以看作是对于函数minimax的优化替代。
    public static <Move> double alphabeta(Board<Move> board,
                                          boolean maxmizing, Piece originalPlayer, int maxDepth) {
        return alphabeta(board, maxmizing, originalPlayer, maxDepth,
                Double.NEGATIVE_INFINITY, Double.POSITIVE_INFINITY);
    }
    // 通过跟踪递归极小化极大算法调用间的alpha和beta值就可以实现这一神奇的优化效果。
    public static <Move> double alphabeta(Board<Move> board,
                                          boolean maxmizing, Piece originalPlayer, int maxDepth,
                                          // alpha表示搜索树当前找到的最优极大化走法的评分，
                                          double alpha,
                                          // 而beta表示当前找到的对手的最优极小化走法的评分。
                                          double beta) {

        // 如果胜负已分，或者出现了平局，或者达到了迭代次数。就可以直接调用评估函数了，
        if(board.isWin() || board.isDraw() || maxDepth == 1) {
            // 就TTTBoard类的evaluate()方法而言，如果双方的最佳走法将导致极大化玩家获胜，则返回1；
            // 如果将导致极大化玩家输棋，则返回-1；如果导致平局，则返回0。
            return board.evaluate(originalPlayer);
        }

        // 这些数字将会在达到基线条件时返回，然后再沿着到达基线条件的各层递归调用逐级向上返回。
        // 对于每次求极大化的递归调用，向上返回的是下一步走法的最佳评分。
        if (maxmizing) {
            for (Move move : board.getLegalMoves()) {
                double result = alphabeta(board.move(move), false, originalPlayer, maxDepth - 1, alpha, beta);
                alpha = Math.max(result, alpha);
                // 如果beta小于或等于alpha，则不值得对该搜索分支做更进一步搜索，
                // 因为已经发现的走法不会比继续沿着该分支搜索得到的走法更差。这种启发式算法大大减少了搜索空间。
                if (beta <= alpha) {
                    break;
                }
            }
            return alpha;
        }
        // 对于每次求极小化的递归调用，向上返回的是下一步走法的最差评分。
        // 这样，决策树就建立起来了。
        else {
            for (Move move : board.getLegalMoves()) {
                double result = alphabeta(board.move(move), true, originalPlayer, maxDepth - 1, alpha, beta);
                beta = Math.min(result, beta);
                // 如果beta小于或等于alpha，则不值得对该搜索分支做更进一步搜索，
                // 因为已经发现的走法不会比继续沿着该分支搜索得到的走法更差。这种启发式算法大大减少了搜索空间。
                if (beta <= alpha) {
                    break;
                }
            }
            return beta;
        }

    }

    public static <Move> Move findBestAlphabetaMove(Board<Move> board, int maxDepth) {
        double bestEval = Double.NEGATIVE_INFINITY;
        Move bestMove = null;
        // 它会对每个合法的走法都调用一次minimax()，以便找出评分最高的走法。
        for (Move move : board.getLegalMoves()) {
            double result = alphabeta(board.move(move), false, board.getTurn(), maxDepth);
            if(result > bestEval) {
                bestEval = result;
                bestMove = move;
            }
        }
        return bestMove;
    }

}

