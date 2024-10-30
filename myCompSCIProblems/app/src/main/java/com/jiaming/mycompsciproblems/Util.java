package com.jiaming.mycompsciproblems;

public final class Util {
    public static double doProduct(double[] xs, double[] ys) {
        double sum = 0.0;
        for (int i = 0 ; i < xs.length; i++) {
            sum += xs[i] * ys[i];
        }
        return sum;
    }


}
