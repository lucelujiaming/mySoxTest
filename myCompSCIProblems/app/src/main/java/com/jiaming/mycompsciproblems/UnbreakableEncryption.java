package com.jiaming.mycompsciproblems;

import java.util.Random;

public class UnbreakableEncryption {
    private static byte[] randomKey(int length) {
        byte[] dummy = new byte[length];
        Random random = new Random();
        random.nextBytes(dummy);
        return dummy;
    }

    public static KeyPair encrypt(String original){
        byte[] originalBytes = original.getBytes();
        byte[] dummyKey = randomKey(originalBytes.length);
        byte[] encryptedData = new byte[originalBytes.length];
        for (int i = 0; i < originalBytes.length; i++) {
            encryptedData[i] = (byte)(originalBytes[i] ^ dummyKey[i]);
        }
        return new KeyPair(dummyKey, encryptedData);
    }

    public static String decrypt(KeyPair kp) {
        byte[] decryptedData = new byte[kp.key1.length];
        for (int i = 0; i < kp.key1.length; i++) {
            decryptedData[i] = (byte)(kp.key1[i] ^ kp.key2[i]);
        }
        return new String(decryptedData);
    }
}
