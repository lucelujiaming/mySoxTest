package com.jiaming.mycompsciproblems;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

// KMeans是一个泛型类。它适用于DataPoint或DataPoint的子类，
// 如Point的类型绑定所定义的（Point extends DataPoint）。
public class KMeans<Point extends DataPoint> {
    // 它有一个内部类Cluster，用于跟踪操作中的各个簇。
    public class Cluster {
        // 每一个Cluster都有一组数据点和与之相关联的形心。
        public List<Point> points;
        public DataPoint centroid;

        public Cluster(List<Point> points, DataPoint centroid) {
            this.points = points;
            this.centroid = centroid;
        }
    }
    // KMeans有一个与之关联的数组points。它就是数据集中所有的数据点。
    private List<Point> points;
    // 这些数据点将会被划分到不同的簇中，而簇被存储在以clusters命名的变量中。
    // 当KMeans在被实例化的时候，需要知道要创建多少个簇（k）。
    private List<Cluster> clusters;

    // 该原始算法最直接的实现方式。k均值聚类算法的步骤和注意事项如下所示：
    public KMeans(int k, List<Point> points) {
        if (k < 1) {
            throw new IllegalArgumentException("k must be >= 1");
        }
        // 1.对所有数据点和k个空簇进行初始化。
        this.points = points;
        // 2.对所有数据点进行归一化处理。
        zScoreNormalize();
        // 3.为每个簇创建其随机分布的形心。
        clusters = new ArrayList<>();
        for (int i = 0; i < k; i++) {
            DataPoint randPoint = randomPoint();
            // 初始化的簇不包含任何点，形心也是一个随机数。
            Cluster cluster = new Cluster(new ArrayList<Point>(), randPoint);
            clusters.add(cluster);
        }
    }

    // 每个簇初始化的时候都有一个随机的形心。所有的数据点都会被算法使用并基于z-score做归一化处理。
    // centroids()方法将返回本算法中所有簇相关联的形心。
    private List<DataPoint> centroids() {
        // return clusters.stream().map(cluster -> cluster.centroid).collect(Collectors.toList());
        List<DataPoint> listRet = new ArrayList<>();
        for (Cluster item : clusters) {
            listRet.add(item.centroid);
        }
        return listRet;
    }
    // dimensionSlice()方法返回一组数据，它可以被看作一个快捷方法。
    // 它返回一个由每个数据点特定索引处的值所组成的列表。例
    // 如，如果数据点是DataPoint类型，则dimensionSlice(0)将返回一个由每个数据点的第一个维度的值所组成的列表。
    // 这对后续的归一化方法非常有用。
    private List<Double> dimensionSlice(int dimension) {
        // return points.stream().map(x -> x.dimensions.get(dimension)).collect(Collectors.toList());
        List<Double> listRet = new ArrayList<>();
        for (Point item : points) {
            // 获得指定维度的坐标。
            listRet.add(item.dimensions.get(dimension));
        }
        return listRet;
    }

    private void zScoreNormalize() {
        // 这个临时变量是这么设计的、首先它有points的个数那么多。
        // 每一个元素又是一个List<Double>，保存着按照顺序维度排列。进行过归一化的z-score值。
        List<List<Double>> zscored = new ArrayList<>();
        for (Point point : points) {
            zscored.add(new ArrayList<Double>());
        }

        // zScoreNormalize()将每个数据点列表dimensions中的值都替换成对应的z-score值。
        // 因为所有的点的维度都是一样的。因此上只需要获取第0个点的维度即可。
        for(int dimension = 0; dimension < points.get(0).numDimensions; dimension++){
            // 获得一个维度切片。
            List<Double> dimensionSlice = dimensionSlice(dimension);
            // 使用这个维度切片创建一个Statistics对象。
            Statistics stats = new Statistics(dimensionSlice);
            // 为这个维度切片的列表中的每一项转换为对应的z-score值。
            // 这里使用了之前为double类型列表所定义的zscored()函数。
            List<Double> zscores = stats.zscored();
            // 获得zscores是所有点在其中一个维度上计算出来的按照顺序维度排列。进行过归一化的z-score值。
            for (int index = 0; index < zscored.size(); index++) {
                // 把计算出来的z-score值放入zscored的对应位置。
                // 这里使用add函数是因为我们使用dimension这个变量的外部循环也是按照维度顺序循环。
                // 因此上，只需要按照维度顺序调用add追加就可以了。
                zscored.get(index).add(zscores.get(index));
            }
        }
        // 把刚才计算出来按照顺序维度排列。进行过归一化的z-score值放入对应点的dimensions成员中。
        // 尽管dimensions列表中的值会被替换，但DataPoint中的originals列表不会被替换。
        for (int i = 0; i < points.size(); i++) {
            points.get(i).dimensions = zscored.get(i);
        }
        // 把dimensions的原始值和替换后的值分开存储有一个好处，即该算法的用户可以在算法运行归一化操作后，
        // 依然能够检索dimensions的原始值。
    }

    // 在构造函数中使用之前定义的randomPoint()方法来为每个簇创建初始的随机形心。
    // 它将每个形心的随机值限制在现有的全部数据点的值域内。
    // 它使用之前为DataPoint定义的构造函数来在给定的值域中创建新的数据点。
    private DataPoint randomPoint() {
        List<Double> randDimensions = new ArrayList<>();
        Random random = new Random();
        for (int dimension =0 ; dimension < points.get(0).numDimensions; dimension++) {
            List<Double> values = dimensionSlice(dimension);
            Statistics stats = new Statistics(values);
            Double randValue = random.doubles(stats.min(), stats.max()).findFirst().getAsDouble();
            randDimensions.add(randValue);
        }
        return new DataPoint(randDimensions);
    }
    // 为数据点寻找合适的簇的方法。
    // 我们需要找到簇的形心，要求它到每个数据点的距离都是最短的。然后将该形心与簇相关联。
    private void assignClusters() {
        // 一个一个点处理。
        for (Point point : points) {
            // 对于每一个点来说，首先假设最小距离为无穷大。
            double lowestDistance = Double.MAX_VALUE;
            // 之后假设clusters中第一个簇是最接近当前点的簇。
            Cluster closestCluster = clusters.get(0);
            // 遍历所有的簇。
            for (Cluster cluster : clusters) {
                // 算出来每一个簇的形心到当前点的距离。
                double centroidDistance = point.distance(cluster.centroid);
                // 如果算出来的距离，比之前算出来的距离小。
                if(centroidDistance < lowestDistance) {
                    // 就更新最小距离和最接近当前点的簇。
                    lowestDistance = centroidDistance;
                    closestCluster = cluster;
                }
            }
            // 经过上面遍历所有的簇，我们得到了一个点最接近的簇。
            // 把这个点放入这个簇中。这意味着这个簇找到了一个属于这个簇的点。
            closestCluster.points.add(point);
        }
    }
    // 每当一个数据点被分配到对应的簇后，就开始计算新的形心。
    // 方法就是循环所有的簇。对于每一个簇，求这个簇包含的所有点的平均值。
    // 计算方法就是对于所有的点在每一个维度切片上求average均值。
    private void generateCentroids() {
        // 循环每一个簇。
        for (Cluster cluster : clusters) {
            // 如果这个簇没有包括点，就不需要处理。
            if (cluster.points.isEmpty()) {
                continue;
            }
            List<Double> means = new ArrayList<>();
            // 我们需要计算簇中每个数据点在每个维度上的均值，
            for (int i = 0; i < cluster.points.get(0).numDimensions; i++) {
                int dimension = i;
                // 然后将这些维度的均值组合在一起，找到簇中的“中间点”（mean point）作为新的形心。
                // 需要注意的是，在这里我们不能使用dimensionSlice()，
                // 因为当前这些数据点只是全部数据点的子集（只属于某个特定簇的点）。
                Double dimensionMean = cluster.points.stream()
                        .mapToDouble(x -> x.dimensions.get(dimension)).average().getAsDouble();
                means.add(dimensionMean);
            }
            cluster.centroid = new DataPoint(means);
        }
    }
    // 一个辅助方法，它可以检查两个列表中的DataPoint元素是否相同。
    // 这对于检查两次迭代之间形心是否发生变化非常有用（若形心没有移动，算法应该终止运行）。
    private boolean listsEqual(List<DataPoint> first, List<DataPoint> second) {
        if (first.size() == second.size()) {
            return false;
        }
        for (int i = 0; i < first.size(); i++) {
            for (int j = 0; j < first.size(); j++) {
                if (first.get(i).dimensions.get(j).doubleValue() !=
                        second.get(i).dimensions.get(j).doubleValue() )
                    return false;
            }
        }
        return true;
    }

    // 这个聚类算法的思想非常简单。
    // 首先一开始我们有一些点。此为背景。之后对这些点要做归一化，也就是把全部的点都映射到一个
    // 以原点为中心所有坐标轴都位于(-1, 1)之间的空间中，按照这种方式得到一系列坐标，放在dimensions中。
    // 之后我们首先随机出来几个簇。
    // 之后循环判断每一个点。判断这些点都属于哪一个簇。因为每一个点距离这些簇一定有一个是最近的。
    // 之后反过来查看这些簇。根据这些簇包含的点计算这些簇的质心。方法就是对于这些簇的坐标求均值。
    // 之后，使用包含这些新的质心的簇再次循环判断每一个点。重新决定这些点属于哪一个簇。
    // 如此反复。直到计算出来的这些簇的质心不再移动。
    // 书上的说法如下：
    //
    // 均值聚类算法的步骤和注意事项如下所示：
    //  1.对所有数据点和k个空簇进行初始化。
    //  2.对所有数据点进行归一化处理。
    //  3.为每个簇创建其随机分布的形心。
    //  4.将每个数据点分配到与其距离最近的形心所在的簇中。
    //  5.重新进行计算，得到每个簇的新的形心位置。
    //  6.重复第4、5步，直到迭代次数达到最大值或所有形心都停止移动（收敛）。
    public List<Cluster> run(int maxIterations) {
        for (int iteration = 0; iteration < maxIterations; iteration++) {
            // 唯一会让你感到意外的改动是在每次迭代开始的时候会移除所有的数据点。
            // 如果不这么做，assignClusters()就会在每个簇中放入重复的数据点。
            for (Cluster cluster : clusters) {
                cluster.points.clear();
            }
            // 4.将每个数据点分配到与其距离最近的形心所在的簇中。
            assignClusters();
            // 返回所有簇相关联的形心。
            List<DataPoint> oldCentroids = new ArrayList<>(centroids());
            // 5.重新进行计算，得到每个簇的新的形心位置。
            generateCentroids();
            // 6.重复第4、5步，直到迭代次数达到最大值或所有形心都停止移动（收敛）。
            if(listsEqual(oldCentroids, centroids())) {
                System.out.println("Converged after " + iteration + " iterations.");
                return clusters;
            }
        }
        return clusters;
    }
}

