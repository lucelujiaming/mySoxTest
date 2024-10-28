package com.jiaming.mycompsciproblems;

import java.util.ArrayList;
import java.util.Collections;

public class Gene {
    // 我们可以用含有四个常量的enum来定义核苷酸类Nucleotide。
    public enum Nucleotide {
        A, C, G, T
    }
    // 密码子可以被定义成由三个Nucleotide类型的属性所构成的类，Codon。
    public static class Codon implements Comparable<Codon> {
        public final Nucleotide first, second, third;
        // Error:Jack is required to support java 8 language features.
        // private final Comparator<Codon> comparator = Comparator.comparing(
        //         (Codon c)->c.first).thenComparing(
        //                 (Codon c)->c.second).thenComparing(
        //                         (Codon c)->c.third);

        // 可以通过Codon类的构造函数将三个String类型的字母传入其中。
        public Codon(String codonStr) {
            first = Nucleotide.valueOf(codonStr.substring(0, 1));
            second = Nucleotide.valueOf(codonStr.substring(1, 2));
            third = Nucleotide.valueOf(codonStr.substring(2, 3));
        }
        //  实现Comparable接口需要重写compareTo()方法。
        //  如果问题中的项小于其比较的项，则compareTo()应当返回负数；
        //  如果两项相等，则返回零；
        //  如果该项大于其比较的项，则返回正数。
        @Override
        public int compareTo(Codon o) {
            if(first.compareTo(o.first) != 0)
            {
                return first.compareTo(o.first);
            }
            else  if(second.compareTo(o.second) != 0)
            {
                return second.compareTo(o.second);
            }
            else if(third.compareTo(o.third) != 0)
            {
                return third.compareTo(o.third);
            }
            return 0;
        }
    }

    // Gene类的唯一状态就是持有一个Codon类的ArrayList。
    private ArrayList<Codon> codonArrayList = new ArrayList<>();
    // 我们还需要一个构造函数来将基因字符串转换成Gene类实例
    public Gene(String geneStr) {
        for (int i = 0 ; i < geneStr.length() - 3; i += 3){
            codonArrayList.add(new Codon(geneStr.substring(i, i + 3)));
        }
    }
    //  对于一个基因，我们想要执行的一个基本操作就是搜索其中的特定密码子。
    //  线性搜索按照原始数据结构的元素顺序来遍历搜索空间中的每个元素，
    //  直到找到要搜索的内容或达到数据结构的末尾。
    public boolean linearContains(Codon key) {
        for (Codon codon : codonArrayList){
            if(codon.compareTo(key) == 0) {
                return true;
            }
        }
        return false;
    }

    // 有一种比需要遍历每个元素更快的搜索方法，但是这种方法需要我们提前了解数据结构的元素顺序。
    // 如果数据结构是有序的，而且可以通过索引直接访问任意元素，那我们就可以使用二分搜索。
    public boolean binaryContains(Codon key) {
        ArrayList<Codon> sortedCodonArrayList = new ArrayList<>(codonArrayList);
        Collections.sort(sortedCodonArrayList);
        int lowPos = 0;
        int highPos = sortedCodonArrayList.size() - 1;
        while (lowPos <= highPos) {
            int middlePos = (lowPos + highPos) / 2;
            int comparisonResult = sortedCodonArrayList.get(middlePos).compareTo(key);
            if (comparisonResult < 0) {
                lowPos = middlePos + 1;
            }
            else if (comparisonResult > 0) {
                highPos = middlePos - 1;
            }
            else {
                return true;
            }
        }
        return false;
    }
}
