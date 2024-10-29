package com.jiaming.mycompsciproblems;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Random;

public class SendMoreMoneyGenetic extends Chromosome<SendMoreMoneyGenetic>  {
    // 该问题中的每个字母都代表一个0～9的数字。同一个数字只会用一个字母来表示。
    // 如果同一个字母反复出现，则说明它代表的数字也在反复出现。
    private List<Character> letters;
    private Random random;

    public SendMoreMoneyGenetic(List<Character> letters) {
        this.letters = letters;
        random = new Random();
    }

    public static SendMoreMoneyGenetic randomInstance() {
        List<Character> letters = new ArrayList<>(
                Arrays.asList('S', 'E', 'N', 'D', 'M', 'O', 'R', 'Y'));
        Collections.shuffle(letters);
        return new SendMoreMoneyGenetic(letters);
    }

    @Override
    public double fitness() {
        int s = letters.indexOf('S');
        int e = letters.indexOf('E');
        int n = letters.indexOf('N');
        int d = letters.indexOf('D');
        int m = letters.indexOf('M');
        int o = letters.indexOf('O');
        int r = letters.indexOf('R');
        int y = letters.indexOf('Y');

        int send = s * 1000 + e * 100 + n * 10 + d;
        int more = m * 1000 + o * 100 + r * 10 + e;
        int money = m * 10000 + o * 1000 + n * 100 + e * 10 + y;
        // 在这里我们返回1/(difference+1)。
        // difference是MONEY和SEND+MORE差值的绝对值，表示染色体离解决这个问题还有多远。
        int difference = Math.abs(money - (send + more));
        return 1.0 / (difference + 1.0);
    }

    @Override
    public List<SendMoreMoneyGenetic> crosssover(SendMoreMoneyGenetic other) {
        SendMoreMoneyGenetic childOne = new SendMoreMoneyGenetic(new ArrayList<>(letters));
        SendMoreMoneyGenetic childTwo = new SendMoreMoneyGenetic(new ArrayList<>(other.letters));

        int indexOne = random.nextInt(letters.size());
        int indexTwo = random.nextInt(other.letters.size());
        Character letterOne = letters.get(indexOne);
        Character letterTwo = letters.get(indexTwo);

        int indexThree = random.nextInt(letters.size());
        int indexFour = random.nextInt(other.letters.size());
        Collections.swap(childOne.letters, indexOne, indexThree);
        Collections.swap(childTwo.letters, indexTwo, indexFour);
        return Arrays.asList(childOne, childTwo);
    }

    @Override
    public void mutate() {
        int indexOne = random.nextInt(letters.size());
        int indexTwo = random.nextInt(letters.size());
        Collections.swap(letters, indexOne, indexTwo);
    }

    @Override
    public SendMoreMoneyGenetic copy() {
        return new SendMoreMoneyGenetic(new ArrayList<>(letters));
    }

    @Override
    public String toString() {
        int s = letters.indexOf('S');
        int e = letters.indexOf('E');
        int n = letters.indexOf('N');
        int d = letters.indexOf('D');
        int m = letters.indexOf('M');
        int o = letters.indexOf('O');
        int r = letters.indexOf('R');
        int y = letters.indexOf('Y');

        int send = s * 1000 + e * 100 + n * 10 + d;
        int more = m * 1000 + o * 100 + r * 10 + e;
        int money = m * 10000 + o * 1000 + n * 100 + e * 10 + y;
        // 在这里我们返回1/(difference+1)。
        // difference是MONEY和SEND+MORE差值的绝对值，表示染色体离解决这个问题还有多远。
        int difference = Math.abs(money - (send + more));
        return (send + " + " + more + " = " + money + " Difference: " + difference);
    }
}
