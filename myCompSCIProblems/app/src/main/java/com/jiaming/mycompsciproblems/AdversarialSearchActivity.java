package com.jiaming.mycompsciproblems;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

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

    public void runAllTests() {

    }
}

