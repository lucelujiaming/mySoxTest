package com.jiaming.mycompsciproblems;

import java.util.HashSet;
import java.util.List;
import java.util.Map;

public class SendMoreMoneyConstraint extends Constraint<Character, Integer> {
    // 该问题中的每个字母都代表一个0～9的数字。同一个数字只会用一个字母来表示。
    // 如果同一个字母反复出现，则说明它代表的数字也在反复出现。
    private List<Character> letters;

    public SendMoreMoneyConstraint(List<Character> letters) {
        super(letters);
        this.letters = letters;
    }

    @Override
    public boolean satisfied(Map<Character, Integer> assignment) {
        HashSet temp = new HashSet(assignment.values());
        // if there are duplicate values then it's not a solution
        if (temp.size() < assignment.size()) {
            return false;
        }

        // if all variables have been assigned, check if it adds correctly
        if(assignment.size() == letters.size()) {
            int s = assignment.get('S');
            int e = assignment.get('E');
            int n = assignment.get('N');
            int d = assignment.get('D');
            int m = assignment.get('M');
            int o = assignment.get('O');
            int r = assignment.get('R');
            int y = assignment.get('Y');

            int send = s * 1000 + e * 100 + n * 10 + d;
            int more = m * 1000 + o * 100 + r * 10 + e;
            int money = m * 10000 + o * 1000 + n * 100 + e * 10 + y;
            return send + more == money;
        }
        return true;
    }
}
