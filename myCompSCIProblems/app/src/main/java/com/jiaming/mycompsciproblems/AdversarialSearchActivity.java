package com.jiaming.mycompsciproblems;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Scanner;

public class AdversarialSearchActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_adversarial_search);
    }

    // Check if two values are equal and report back
    public static <T> String assertEquality(T actual, T expected) {
        String strRet = "";
        if (actual.equals(expected)) {
            System.out.println("Passed!");
            return "Passed!";
        } else {
            System.out.println("Failed!");
            strRet = "Failed!";
            System.out.println("Actual: " + actual.toString());
            strRet += "Actual: " + actual.toString();
            System.out.println("Expected: " + expected.toString());
            strRet += "Expected: " + expected.toString();
            return strRet;
        }
    }

    public String easyPosition() {
        TTTPiece[] toWinEasyPosition = new TTTPiece[] {
                TTTPiece.X, TTTPiece.O, TTTPiece.X,
                TTTPiece.X, TTTPiece.E, TTTPiece.O,
                TTTPiece.E, TTTPiece.E, TTTPiece.O };
        TTTBoard testBoard1 = new TTTBoard(toWinEasyPosition, TTTPiece.X);
        Integer answer1 = Minimax.findBestMove(testBoard1, 8);
        return assertEquality(answer1, 6);
    }

    public String blockPosition() {
        TTTPiece[] toBlockPosition = new TTTPiece[] {
                TTTPiece.X, TTTPiece.E, TTTPiece.E,
                TTTPiece.E, TTTPiece.E, TTTPiece.O,
                TTTPiece.E, TTTPiece.X, TTTPiece.O };
        TTTBoard testBoard2 = new TTTBoard(toBlockPosition, TTTPiece.X);
        Integer answer2 = Minimax.findBestMove(testBoard2, 8);
        return assertEquality(answer2, 2);
    }

    public String hardPosition() {
        TTTPiece[] toWinHardPosition = new TTTPiece[] {
                TTTPiece.X, TTTPiece.E, TTTPiece.E,
                TTTPiece.E, TTTPiece.E, TTTPiece.O,
                TTTPiece.O, TTTPiece.X, TTTPiece.E };
        TTTBoard testBoard3 = new TTTBoard(toWinHardPosition, TTTPiece.X);
        Integer answer3 = Minimax.findBestMove(testBoard3, 8);
        return assertEquality(answer3, 1);
    }

    public void testKnapSack() {
        List<KnapSack.Item> items = new ArrayList<>();
        items.add(new KnapSack.Item("candlesticks", 2, 300));
        items.add(new KnapSack.Item("laptop", 3, 1000));
        items.add(new KnapSack.Item("food", 1, 50));
        items.add(new KnapSack.Item("painting", 2, 400));
//        items.add(new Item("television", 50, 500));
//        items.add(new Item("stereo", 35, 400));
//        items.add(new Item("clothing", 20, 800));
//        items.add(new Item("jewelry", 1, 4000));
//        items.add(new Item("books", 100, 300));
//        items.add(new Item("printer", 18, 30));
//        items.add(new Item("refrigerator", 200, 700));
        List<KnapSack.Item> toSteal = KnapSack.knapSack(items, 5);
        System.out.println("The best items for the thief to steal are:");
        System.out.printf("%-15.15s %-15.15s %-15.15s%n", "Name", "Weight", "Value");
        for (KnapSack.Item item : toSteal) {
            System.out.printf("%-15.15s %-15.15s %-15.15s%n", item.name, item.weight, item.value);
        }
    }

    public void testDistances() {
        Map<String, Map<String, Integer>> vtDistances = new HashMap<>();
        Map<String, Integer> mapRutland = new HashMap<>();
        mapRutland.put("Burlington", 67);
        mapRutland.put("White River Junction", 46);
        mapRutland.put("Bennington", 55);
        mapRutland.put("Brattleboro", 75);
        vtDistances.put("Rutland", mapRutland);

        Map<String, Integer> mapWhiteRiverJunction = new HashMap<>();
        mapWhiteRiverJunction.put("Rutland", 46);
        mapWhiteRiverJunction.put("Burlington", 91);
        mapWhiteRiverJunction.put("Bennington", 98);
        mapWhiteRiverJunction.put("Brattleboro", 65);
        vtDistances.put("White River Junction", mapWhiteRiverJunction);

        Map<String, Integer> mapBennington = new HashMap<>();
        mapBennington.put("Rutland", 55);
        mapBennington.put("Burlington", 122);
        mapBennington.put("White River Junction", 98);
        mapBennington.put("Brattleboro", 40);
        vtDistances.put("Bennington", mapWhiteRiverJunction);

        Map<String, Integer> mapBrattleboro = new HashMap<>();
        mapBrattleboro.put("Rutland", 75);
        mapBrattleboro.put("Burlington", 153);
        mapBrattleboro.put("White River Junction", 65);
        mapBrattleboro.put("Bennington", 40);
        vtDistances.put("Brattleboro", mapWhiteRiverJunction);

        TSP tsp = new TSP(vtDistances);
        String[] shortestPath = tsp.findShortestPath();
        int distance = tsp.pathDistance(shortestPath);
        System.out.println("The shortest path is " + Arrays.toString(shortestPath) + " in " +
                distance + " miles.");
    }

    public void testPhoneNumberMnemonics() {
        System.out.println("Enter a phone number:");
        Scanner scanner = new Scanner(System.in);
        String phoneNumber = scanner.nextLine();
        scanner.close();
        System.out.println("The possible mnemonics are:");
        PhoneNumberMnemonics pnm = new PhoneNumberMnemonics(phoneNumber);
        System.out.println(Arrays.toString(pnm.getMnemonics()));
    }

    public void runAllTests() {

    }
}

