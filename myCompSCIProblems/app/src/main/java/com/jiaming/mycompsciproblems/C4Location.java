package com.jiaming.mycompsciproblems;

import java.util.List;

// 我们还需要一个便于使用的类C4Location来追踪网格上的位置（行或列）。
// 四子棋是一款面向列的游戏，所以我们使用不同于以往的实现方式——列优先的方式——来实现所有网格代码。
public final class C4Location {
    public final int column, row;

    public C4Location(int column, int row) {
        this.column = column;
        this.row = row;
    }
}

