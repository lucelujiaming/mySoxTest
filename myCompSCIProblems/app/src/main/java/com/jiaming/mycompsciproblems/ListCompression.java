package com.jiaming.mycompsciproblems;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Random;
import java.util.zip.GZIPOutputStream;

public class ListCompression extends Chromosome<ListCompression> {
    public static final List<String> ORIGINAL_LIST = Arrays.asList("Michael", "Sarah", "Joshua", "Narine",
            "David", "Sajid", "Melanie", "Daniel", "Wei", "Dean", "Brian", "Murat", "Lisa");
    private List<String> myList;
    private Random random;

    public ListCompression(List<String> myList) {
        this.myList = myList;
        random = new Random();
    }

    public static ListCompression randomInstance() {
        ArrayList<String> templist = new ArrayList<>(ORIGINAL_LIST);
        Collections.shuffle(templist);
        return new ListCompression(templist);
    }

    private int bytesCommpressed() {
        try {
            ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
            GZIPOutputStream gzipOutputStream = new GZIPOutputStream(byteArrayOutputStream);
            ObjectOutputStream objectOutputStream = new ObjectOutputStream(gzipOutputStream);
            objectOutputStream.writeObject(myList);
            objectOutputStream.close();
            return byteArrayOutputStream.size();
        } catch (IOException ioe) {
            System.out.println("Could not compress list!");
            ioe.printStackTrace();
            return 0;
        }
    }

    @Override
    public double fitness() {
        return 1.0 / bytesCommpressed();
    }

    @Override
    public List<ListCompression> crosssover(ListCompression other) {
        ListCompression childOne = new ListCompression(new ArrayList<>(myList));
        ListCompression childTwo = new ListCompression(new ArrayList<>(myList));

        int indexOne = random.nextInt(myList.size());
        int indexTwo = random.nextInt(other.myList.size());
        String strOne = myList.get(indexOne);
        String strTwo = myList.get(indexTwo);

        int indexThree = myList.indexOf(strTwo);
        int indexFour = other.myList.indexOf(strOne);

        Collections.swap(childOne.myList, indexOne, indexThree);
        Collections.swap(childTwo.myList, indexTwo, indexFour);
        return Arrays.asList(childOne, childTwo);
    }

    @Override
    public void mutate() {
        int indexOne = random.nextInt(myList.size());
        int indexTwo = random.nextInt(myList.size());
        Collections.swap(myList, indexOne, indexTwo);

    }

    @Override
    public ListCompression copy() {
        return new ListCompression(new ArrayList<>(myList));
    }

    @Override
    public String toString() {
        return "Order: " + myList + " Bytes: " + bytesCommpressed();
    }
}
