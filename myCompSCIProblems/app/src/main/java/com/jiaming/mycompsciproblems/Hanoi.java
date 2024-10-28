package com.jiaming.mycompsciproblems;

import java.util.Stack;

public class Hanoi {
    private final int numDiscs ;
    public final Stack<Integer> towerA = new Stack<>();
    public final Stack<Integer> towerB = new Stack<>();
    public final Stack<Integer> towerC = new Stack<>();

    public Hanoi(int iDiscs) {
        numDiscs = iDiscs;
        for (int i = 0; i < iDiscs; i++){
            towerA.push(i);
        }
    }

    private void  move (Stack<Integer> begin, Stack<Integer> end, Stack<Integer> temp, int n){
        if (n == 1){
            end.push(begin.pop());
        }
        else {
            move(begin, temp, end, n - 1);
            move(begin, end, temp, 1);
            move(temp, end,  begin, n - 1);
        }
    }

    public void  solve() {
        move(towerA, towerC, towerB, numDiscs);
    }
}
