package com.jiaming.mycompsciproblems;

import java.util.ArrayList;
import java.util.List;

public class MCState {
    public static final int MAX_NUM = 3;

    private final int west_missionaries ;   // west bank missionaries
    private final int west_cannibals ;      // west bank cannibals
    private final int east_missionaries ;      // east bank missionaries
    private final int east_cannibals ;      // east bank cannibals
    private final boolean is_boat_in_west; // is boat on west bank?

    public MCState(int west_missionaries, int west_cannibals, boolean is_boat_in_west) {
        this.west_missionaries = west_missionaries;
        this.west_cannibals = west_cannibals;
        this.east_missionaries = MAX_NUM - west_missionaries;
        this.east_cannibals = MAX_NUM - west_cannibals;
        this.is_boat_in_west = is_boat_in_west;
    }

    @Override
    public String toString() {
        return String.format("There are %d missionaries and %d cannibals in the west bank" +
                "and there are %d missionaries and %d cannibals in the west bank. " +
                "boat is in the %s",
                west_missionaries, west_cannibals, east_missionaries, east_cannibals,
                is_boat_in_west ? "west" : "east");
    }

    // 目标状态验证函数的实现非常简单。它只需要验证当前状态下河东岸的食人族和传教士人数是否都与所设置的最大人数相等。
    public boolean goalTest() {
        return isLegal() && east_missionaries == MAX_NUM && east_cannibals == MAX_NUM;
    }

    public boolean isLegal() {
        if (west_missionaries < west_cannibals && west_missionaries > 0) {
            return false;
        }
        if (east_missionaries < east_cannibals && east_missionaries > 0){
            return false;
        }
        return true;
    }


    public boolean isIllegal() {
        return !isLegal();
    }

    // 为了能清楚地表达逻辑，后续步骤查找函数successors()在实现上会比较烦琐。
    // 它尝试在独木舟所在河岸添加所有可能的1或2人渡河组合。
    // 添加完所有可能的步骤后，通过调用List类型临时变量的removeIf()方法过滤掉不符合要求的步骤。
    public static List<MCState> successors(MCState mcsCurrent) {
        List<MCState> successors_states_list = new ArrayList<>();
        // 如果船在西岸。
        if (mcsCurrent.is_boat_in_west) {
            // 划船从西岸到东岸过河只有五种情况。
            //   一个传教士，一个食人族，两个传教士，两个食人族和一个传教士同一个食人族各一个划船过河。
            // 下面分别处理这五种情况。
            // 第一种情况：两个传教士划船从西岸到东岸过河。此时要求传教士个数必须大于一个。
            if (mcsCurrent.west_missionaries > 1) {
                successors_states_list.add(new MCState(
                        mcsCurrent.west_missionaries - 2, mcsCurrent.west_cannibals,
                        !mcsCurrent.is_boat_in_west));
            }
            // 第二种情况：一个传教士划船从西岸到东岸过河。此时要求传教士个数必须大于零个。
            if(mcsCurrent.west_missionaries > 0) {
                successors_states_list.add(new MCState(
                        mcsCurrent.west_missionaries - 1, mcsCurrent.west_cannibals,
                        !mcsCurrent.is_boat_in_west));

            }
            // 第三种情况：两个食人族划船从西岸到东岸过河。此时要求食人族个数必须大于一个。
            if (mcsCurrent.west_cannibals > 1) {
                successors_states_list.add(new MCState(
                        mcsCurrent.west_missionaries, mcsCurrent.west_cannibals - 2,
                        !mcsCurrent.is_boat_in_west));
            }
            // 第四种情况：一个食人族划船从西岸到东岸过河。此时要求食人族个数必须大于零个。
            if(mcsCurrent.west_missionaries > 0) {
                successors_states_list.add(new MCState(
                        mcsCurrent.west_missionaries, mcsCurrent.west_cannibals - 1,
                        !mcsCurrent.is_boat_in_west));

            }
            // 第五种情况：一个传教士同一个食人族划船从西岸到东岸过河。此时要求食人族个数和传教士个数必须大于零个。
            if(mcsCurrent.west_missionaries > 0 && mcsCurrent.west_cannibals > 0) {
                successors_states_list.add(new MCState(
                        mcsCurrent.west_missionaries - 1, mcsCurrent.west_cannibals - 1,
                        !mcsCurrent.is_boat_in_west));
            }
        }
        // 如果船在东岸。
        else {
            // 划船从东岸到西岸过河只有五种情况。
            //   一个传教士，一个食人族，两个传教士，两个食人族和一个传教士同一个食人族各一个划船过河。
            // 下面分别处理这五种情况。
            // 第一种情况：两个传教士划船从东岸到西岸过河。此时要求传教士个数必须大于一个。
            if (mcsCurrent.west_missionaries > 1) {
                successors_states_list.add(new MCState(
                        // 两个传教士划船从东岸到西岸会导致西岸多两个传教士。后续同理。
                        mcsCurrent.west_missionaries + 2, mcsCurrent.west_cannibals,
                        !mcsCurrent.is_boat_in_west));
            }
            // 第二种情况：一个传教士划船从东岸到西岸过河。此时要求传教士个数必须大于零个。
            if(mcsCurrent.west_missionaries > 0) {
                successors_states_list.add(new MCState(
                        mcsCurrent.west_missionaries + 1, mcsCurrent.west_cannibals,
                        !mcsCurrent.is_boat_in_west));

            }
            // 第三种情况：两个食人族划船从东岸到西岸过河。此时要求食人族个数必须大于一个。
            if (mcsCurrent.west_cannibals > 1) {
                successors_states_list.add(new MCState(
                        mcsCurrent.west_missionaries, mcsCurrent.west_cannibals + 2,
                        !mcsCurrent.is_boat_in_west));
            }
            // 第四种情况：一个食人族划船从东岸到西岸过河。此时要求食人族个数必须大于零个。
            if(mcsCurrent.west_missionaries > 0) {
                successors_states_list.add(new MCState(
                        mcsCurrent.west_missionaries, mcsCurrent.west_cannibals + 1,
                        !mcsCurrent.is_boat_in_west));

            }
            // 第五种情况：一个传教士同一个食人族划船从东岸到西岸过河。此时要求食人族个数和传教士个数必须大于零个。
            if(mcsCurrent.west_missionaries > 0 && mcsCurrent.west_cannibals > 0) {
                successors_states_list.add(new MCState(
                        mcsCurrent.west_missionaries + 1, mcsCurrent.west_cannibals + 1,
                        !mcsCurrent.is_boat_in_west));
            }
        }
        // Only works in java 11
        successors_states_list.removeIf(MCState::isIllegal);

        // boolean isFoundIllegal = true;
        // while (isFoundIllegal) {
        //     isFoundIllegal = false;
        //     for (MCState objMCState : successors_states_list) {
        //         if (objMCState.isLegal() == false) {
        //             successors_states_list.remove(objMCState);
        //             isFoundIllegal = true;
        //             break;
        //         }
        //     }
        // }
        return  successors_states_list;
    }

    // 于传教士和食人族问题而言，我们也需要一种能将结果状态列表打印成能让人理解的一系列解答步骤的方法。
    public static void displaySolution(List<MCState> path) {
        if (path.size() == 0) { // sanity check
            return;
        }
        MCState oldState = path.get(0);
        System.out.println(oldState);
        for (MCState currentState : path.subList(1, path.size())) {
            if (currentState.is_boat_in_west) {
                System.out.printf("%d missionaries and %d cannibals moved from the east bank to the west bank.%n",
                        oldState.east_missionaries - currentState.east_missionaries,
                        oldState.east_cannibals - currentState.east_cannibals);
            } else {
                System.out.printf("%d missionaries and %d cannibals moved from the west bank to the east bank.%n",
                        oldState.west_missionaries - currentState.west_missionaries,
                        oldState.west_cannibals - currentState.west_cannibals);
            }
            System.out.println(currentState);
            oldState = currentState;
        }
    }

    // 要想让搜索函数能正确工作，就要求用于记录已探索数据的数据结构能够很容易地进行比较。
    // 因此，在main()中求解问题之前，我们需要再次让Eclipse自动生成hashCode()和equals()。
    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + (is_boat_in_west ? 1231 : 1237);
        result = prime * result + east_cannibals;
        result = prime * result + east_missionaries;
        result = prime * result + west_cannibals;
        result = prime * result + west_missionaries;
        return result;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        MCState other = (MCState) obj;
        if (is_boat_in_west != other.is_boat_in_west) {
            return false;
        }
        if (east_cannibals != other.east_cannibals) {
            return false;
        }
        if (east_missionaries != other.east_missionaries) {
            return false;
        }
        if (west_cannibals != other.west_cannibals) {
            return false;
        }
        if (west_missionaries != other.west_missionaries) {
            return false;
        }
        return true;
    }
}
