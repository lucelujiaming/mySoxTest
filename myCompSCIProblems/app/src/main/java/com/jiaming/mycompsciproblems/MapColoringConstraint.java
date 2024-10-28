package com.jiaming.mycompsciproblems;

import java.util.Arrays;
import java.util.List;
import java.util.Map;

// 由于不允许对两个相邻地区使用同一种颜色，因此约束将取决于有哪些地区是彼此相邻的。
// 可以采用二元约束，即两个变量间的约束。
// 具有相同边界的两个地区共用一个二元约束，表示不能给它们赋予相同的颜色。
public final class MapColoringConstraint extends Constraint<String, String> {
    private String strPlaceOne;
    private String strPlaceTwo;

    public MapColoringConstraint(String strPlaceOne, String strPlaceTwo){
        // super(List.of(place1, place2));
        super(Arrays.asList(strPlaceOne, strPlaceTwo));
        this.strPlaceOne = strPlaceOne;
        this.strPlaceTwo = strPlaceTwo;
    }

    public MapColoringConstraint(List<String> variables) {
        super(variables);
    }

    @Override
    public boolean satisfied(Map<String, String> assignment) {
        // 首先检查两个地区是否已经赋值（颜色）。如果其中一个地区没有着色，则不会发生冲突。
        if(!assignment.containsKey(strPlaceOne) || !assignment.containsKey(strPlaceTwo)) {
            return true;
        }
        // 然后检查两个地区是否被赋予了同样的颜色。显然，如果颜色相同，则存在冲突，也就不满足约束。
        // return !assignment.get(strPlaceOne).equals(assignment.get(strPlaceTwo));
        String strColorOne = assignment.get(strPlaceOne);
        String strColorTwo = assignment.get(strPlaceTwo);
        if(strColorOne.equals(strColorTwo)) {
            return false;
        }
        else {
            return true;
        }
    }
}
