package com.jiaming.mycompsciproblems;

import java.util.Arrays;

public class Governor extends DataPoint {
    private double longitude;
    private double age;

    private String state;

    public Governor(double longitude, double age, String state) {
        super(Arrays.asList(longitude, age));
        this.longitude = longitude;
        this.age = age;
        this.state = state;
    }

    @Override
    public String toString() {
        return state + ": (longitude: " + longitude + ", age: " + age + ")";
    }
}

