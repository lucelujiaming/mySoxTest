package com.xungao.xungaoblectrl.utils;
/**
 * 字符串转字节工具类
 */
public class HexUtil {
    // 十六进制字符串转字节
    public static byte[] hexStringToBytes(String hexString) {
        if (hexString == null || hexString.equals("")) {
            return null;
        }
        hexString = hexString.toUpperCase();
        int length = hexString.length() / 2;
        // 将一个字符串内容转换为字符数组，例如
        //     String str="abc";
        //     System.out.println(str.toCharArray()+" ");
        // 将输出a b c
        char[] hexChars = hexString.toCharArray();
        byte[] d = new byte[length];
        for (int i = 0; i < length; i++) {
            int pos = i * 2;
            d[i] = (byte) (charToByte(hexChars[pos]) << 4 | charToByte(hexChars[pos + 1]));
        }
        return d;
    }
    // String 类的 indexOf() 方法返回指定字符串中指定字符或字符串第一次出现的位置。
    private static byte charToByte(char c) {
        return (byte) "0123456789ABCDEF".indexOf(c);
    }
    // 字节转十六进制字符串
    public static String bytesToHexString(byte[] b) {
        if (b.length == 0) {
            return null;
        }
        StringBuilder sb = new StringBuilder("");
        for (int i = 0; i < b.length; i++) {
            int value = b[i] & 0xFF;
            // toHexString 是 Java 中的一个方法,用于将一个整数转换为十六进制字符串。
            String hv = Integer.toHexString(value);
            if (hv.length() < 2) {
                sb.append(0);
            }

            sb.append(hv);
        }
        return sb.toString();
    }

    public static float bytesToFloat(byte[] b) {
        int accum = 0;
        accum= accum|(b[0] & 0xff) << 0;
        accum= accum|(b[1] & 0xff) << 8;
        accum= accum|(b[2] & 0xff) << 16;
        accum= accum|(b[3] & 0xff) << 24;
        return Float.intBitsToFloat(accum);
    }

    /**
     * byte数组到int的转换(小端)
     * @param bytes
     * @return
     */
    public static int bytes2IntLittle(byte[] bytes )
    {
        int int1=bytes[0]&0xff;
        int int2=(bytes[1]&0xff)<<8;
        int int3=(bytes[2]&0xff)<<16;
        int int4=(bytes[3]&0xff)<<24;
        return int1|int2|int3|int4;
    }
    /**
     * byte数组到int的转换(大端)
     * @param bytes
     * @return
     */
    public static int bytes2IntBig(byte[] bytes )
    {
        int int1=bytes[3]&0xff;
        int int2=(bytes[2]&0xff)<<8;
        int int3=(bytes[1]&0xff)<<16;
        int int4=(bytes[0]&0xff)<<24;
        return int1|int2|int3|int4;
    }

    /**
     * 转换short为byte
     *
     * @param b
     * @param s
     *            需要转换的short
     * @param index
     *            第几位开始存放
     */
    public static void short2byteArray(byte b[], short s, int index) {
        b[index + 1] = (byte) (s >> 8);
        b[index + 0] = (byte) (s >> 0);
    }

    /**
     * 通过byte数组取到short
     *
     * @param b
     * @param index
     *            第几位开始取
     * @return
     */
    public static short byteArray2short(byte[] b, int index) {
        return (short) (((b[index + 1] << 8) | b[index + 0] & 0xff));
    }
}
