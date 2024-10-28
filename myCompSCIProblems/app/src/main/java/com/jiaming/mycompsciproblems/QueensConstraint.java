package com.jiaming.mycompsciproblems;

import java.util.Map;

public class QueensConstraint extends Constraint<Integer, Integer> {

    @Override
    public boolean satisfied(Map<Integer, Integer> assignment) {
        return false;
    }
}
