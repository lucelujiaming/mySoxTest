package com.jiaming.mycompsciproblems;

import java.util.Arrays;

public class Album extends DataPoint {
    private String name;
    private int year;

    public Album(String name, int year, double length, double track) {
        super(Arrays.asList(length, track));
        this.name = name;
        this.year = year;
    }

    @Override
    public String toString() {
        return "(" + name + ", " + year + ")";
    }
}
