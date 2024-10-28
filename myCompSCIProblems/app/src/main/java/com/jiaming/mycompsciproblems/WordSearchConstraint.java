package com.jiaming.mycompsciproblems;

import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class WordSearchConstraint  extends Constraint<String, List<WordGrid.GridLocation>>  {
    public WordSearchConstraint(List<String> words) {
        super(words);
    }

    @Override
    public boolean satisfied(Map<String, List<WordGrid.GridLocation>> assignment) {
        // 检查为某个单词推荐的位置是否与为其他单词推荐的位置相同，这里使用Set来实现。
        // 将List转换成Set时会移除所有重复项。
        // 如果从List转换而成的Set中的数据项少于原List中的数据项，则表示原List中包含一些重复项。
        // List<WordGrid.GridLocation> allLocations = assignment.values().stream()
        //         // 为了为此项检查准备数据，我们将使用flatMap()把赋值中每个单词的多个位置子列表组合为一个大的位置列表。
        //         .flatMap(Collection::stream).collect(Collectors.toList());
        Set<WordGrid.GridLocation> allLocationsSet = new HashSet<WordGrid.GridLocation>();
        int iAssignmentCount = 0;
        for (Map.Entry<String, List<WordGrid.GridLocation>> item : assignment.entrySet()) {
            String word = item.getKey();
            List<WordGrid.GridLocation> locations = item.getValue();
            iAssignmentCount += locations.size();
            allLocationsSet.addAll(locations);
        }

        // return allLocations.size() == allLocationsSet.size();
        return iAssignmentCount == allLocationsSet.size();
    }
}
