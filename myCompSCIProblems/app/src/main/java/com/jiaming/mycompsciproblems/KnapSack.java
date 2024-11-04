package com.jiaming.mycompsciproblems;

import java.util.ArrayList;
import java.util.List;

public final class KnapSack {

    public static final class Item {
        public final String name;
        public final int weight;
        public final double value;

        public Item(String name, int weight, double value) {
            this.name = name;
            this.weight = weight;
            this.value = value;
        }
    }

    public static List<Item> knapSack(List<Item> items, int maxCapacity) {
        // 创建一个表格，用于保存每种容量和物品数量组合的最优解。
        // 这个表格在填充的时候，是从1开始的。因此上，第0行和第0列始终都为全0。
        // build up dynamic programming table
        double[][] table = new double[items.size() + 1][maxCapacity + 1];

        System.out.println("The Items are:");
        for (int i = 0; i < items.size(); i++) {
            Item item = items.get(i);
            System.out.printf("name: %-15s     Weight: %-15d     value: %-15f \n", 
                    item.name, item.weight, item.value);
        }

        // 遍历每种可能的物品数量，
        // 请注意，这里是“每种可能的物品数量”，而不是每一件物品。i等于2不代表第2件物品，
        // 而是代表在每个已搜索的容量以内前两件物品的可能组合。
        for (int i = 0; i < items.size(); i++) {
            // item是正要被窃取的下一件物品：
            Item item = items.get(i);
            // 对于每种可能的物品数量，我们都将线性遍历所有可能的重量，一直到背包的最大容纳重量。
            for (int capacity = 1; capacity <= maxCapacity; capacity++) {

                // prevItemValue是正在探索的当前capacity（容量）以内最后一种物品组合的价值。
                // 对于每种可能的物品组合，我们都要考虑是否还有可能加入“新”的物品。
                double prevItemValue = table[i][capacity];

                // 否则，我们就要考虑在当前容量以内，加入“新”物品能否产生比最后一种物品组合更高的价值。
                if (capacity >= item.weight) { // item fits in knapsack
                    // 表中已算出的价值是指从当前容量中减去该物品重量后的容量对应的最近一次物品组合的价值。
                    double valueFreeingWeightForItem = table[i][capacity - item.weight];

                    // 只要将该物品的价值加上表中已算出的价值即可得知，
                    // only take if more valuable than previous item
                    double doubleWeightWithCurrentItem = valueFreeingWeightForItem + item.value;
                    // 如果总价值高于当前容量下的最后一种物品组合的价值，就将其插入表中，
                    // table[i + 1][capacity] = Math.max(valueFreeingWeightForItem + item.value, prevItemValue);
                    if(doubleWeightWithCurrentItem > prevItemValue) {
                        table[i + 1][capacity] = doubleWeightWithCurrentItem;
                    }
                    // 否则插入最后一种组合的价值：
                    else {
                        table[i + 1][capacity] = prevItemValue;
                    }
                }
                // 如果可以容纳的重量没有超过下一个物品的重量，说明可以把当前物品放入。
				else { // no room for this item
                    table[i + 1][capacity] = prevItemValue;
                }
            }

            System.out.println("------------------------------------------------------------------------------------------------------------------------");
            System.out.printf(
            		"The current table's contents after put (name: %s, Weight: %d, value: %.1f)  are: \n", 
                    item.name, item.weight, item.value);
            System.out.print(
                    "Check weight is   : ");
            for(int k = 0; k <= maxCapacity; k++) {
                System.out.printf("%d 磅     ", k );
            }
            System.out.println(" ");
            for(int j = 0; j <= items.size(); j++) {
                System.out.print(
                        "可以放入"+ j +"个物体时: ");
                for(int k = 0; k <= maxCapacity; k++) {
                    System.out.printf("%-7.1f  ", table[j][k] );
                }
                System.out.println(" ");
            }
            
        }

        System.out.println("------------------------------------------------------------------------------------------------------------------------");
        System.out.println("As the result, the table's contents are:");
        System.out.print(
                "Check weight is   : ");
        for(int k = 0; k <= maxCapacity; k++) {
            System.out.printf("%d 磅     ", k );
        }
        System.out.println(" ");
        for(int j = 0; j <= items.size(); j++) {
            System.out.print(
                    "可以放入"+ j +"个物体时: ");
            for(int k = 0; k <= maxCapacity; k++) {
                System.out.printf("%-7.1f  ", table[j][k] );
            }
            System.out.println(" ");
        }
        
        

        // 至此，建表的工作就完成了。但是，要想真正得到结果中的物品，
        // 需要从最高容量值及最终求得的物品组合开始往回找：
        // figure out solution from table
        List<Item> solution = new ArrayList<>();
        int capacity = maxCapacity;
        // 我们从最终位置开始，从右到左遍历缓存表，
        for (int i = items.size(); i > 0; i--) { // work backwards
            // 检查插入表中的总价值是否有变化。
            // was this item used?
            if (table[i - 1][capacity] != table[i][capacity]) {
                // 如果有，就意味着在计算某组合时加入了新的物品，因为该组合比前一组合价值高。
                solution.add(items.get(i - 1));
                // 于是我们把该物品加入解中。同时，要从总容量中减去该物品的重量，这可以被认为是在表中向上移动：
                // if the item was used, remove its weight
                capacity -= items.get(i - 1).weight;
            }
        }
        return solution;
    }
}
