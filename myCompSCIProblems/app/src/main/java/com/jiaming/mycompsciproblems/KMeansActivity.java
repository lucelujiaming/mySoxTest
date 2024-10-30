package com.jiaming.mycompsciproblems;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class KMeansActivity extends AppCompatActivity implements View.OnClickListener{

    private Button btnKMeansClustering;
    private Button btnGovernorsClustering;
    private Button btnAlbumClustering;

    private TextView textKMeansResult;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_kmeans);

        btnKMeansClustering = (Button)findViewById(R.id.test_kmeans_clustering);
        btnKMeansClustering.setOnClickListener(this);
        btnGovernorsClustering = (Button)findViewById(R.id.test_governors_clustering);
        btnGovernorsClustering.setOnClickListener(this);
        btnAlbumClustering = (Button)findViewById(R.id.test_album_clustering);
        btnAlbumClustering.setOnClickListener(this);

        textKMeansResult = (TextView)findViewById(R.id.test_kmeans_result);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.test_kmeans_clustering:
                DataPoint point1 = new DataPoint(Arrays.asList(2.0, 1.0, 1.0));
                DataPoint point2 = new DataPoint(Arrays.asList(2.0, 2.0, 5.0));
                DataPoint point3 = new DataPoint(Arrays.asList(3.0, 1.5, 2.5));
                KMeans<DataPoint> kmeansTest = new KMeans<>(2, Arrays.asList(point1, point2, point3));
                List<KMeans<DataPoint>.Cluster> testClusters = kmeansTest.run(100);

                String strKMeansOutResult = new String();
                for (int clusterIndex = 0; clusterIndex < testClusters.size(); clusterIndex++) {
                    System.out.println("Cluster " + clusterIndex + ": "
                            + testClusters.get(clusterIndex).points);
                    strKMeansOutResult += "Cluster " + clusterIndex + ": "
                            + testClusters.get(clusterIndex).points + "\n";
                }
                textKMeansResult.setText(strKMeansOutResult);
                break;
            case R.id.test_governors_clustering:
                List<Governor> governors = new ArrayList<>();
                governors.add(new Governor(-86.79113, 72, "Alabama"));
                governors.add(new Governor(-152.404419, 66, "Alaska"));
                governors.add(new Governor(-111.431221, 53, "Arizona"));
                governors.add(new Governor(-92.373123, 66, "Arkansas"));
                governors.add(new Governor(-119.681564, 79, "California"));
                governors.add(new Governor(-105.311104, 65, "Colorado"));
                governors.add(new Governor(-72.755371, 61, "Connecticut"));
                governors.add(new Governor(-75.507141, 61, "Delaware"));
                governors.add(new Governor(-81.686783, 64, "Florida"));
                governors.add(new Governor(-83.643074, 74, "Georgia"));
                governors.add(new Governor(-157.498337, 60, "Hawaii"));
                governors.add(new Governor(-114.478828, 75, "Idaho"));
                governors.add(new Governor(-88.986137, 60, "Illinois"));
                governors.add(new Governor(-86.258278, 49, "Indiana"));
                governors.add(new Governor(-93.210526, 57, "Iowa"));
                governors.add(new Governor(-96.726486, 60, "Kansas"));
                governors.add(new Governor(-84.670067, 50, "Kentucky"));
                governors.add(new Governor(-91.867805, 50, "Louisiana"));
                governors.add(new Governor(-69.381927, 68, "Maine"));
                governors.add(new Governor(-76.802101, 61, "Maryland"));
                governors.add(new Governor(-71.530106, 60, "Massachusetts"));
                governors.add(new Governor(-84.536095, 58, "Michigan"));
                governors.add(new Governor(-93.900192, 70, "Minnesota"));
                governors.add(new Governor(-89.678696, 62, "Mississippi"));
                governors.add(new Governor(-92.288368, 43, "Missouri"));
                governors.add(new Governor(-110.454353, 51, "Montana"));
                governors.add(new Governor(-98.268082, 52, "Nebraska"));
                governors.add(new Governor(-117.055374, 53, "Nevada"));
                governors.add(new Governor(-71.563896, 42, "New Hampshire"));
                governors.add(new Governor(-74.521011, 54, "New Jersey"));
                governors.add(new Governor(-106.248482, 57, "New Mexico"));
                governors.add(new Governor(-74.948051, 59, "New York"));
                governors.add(new Governor(-79.806419, 60, "North Carolina"));
                governors.add(new Governor(-99.784012, 60, "North Dakota"));
                governors.add(new Governor(-82.764915, 65, "Ohio"));
                governors.add(new Governor(-96.928917, 62, "Oklahoma"));
                governors.add(new Governor(-122.070938, 56, "Oregon"));
                governors.add(new Governor(-77.209755, 68, "Pennsylvania"));
                governors.add(new Governor(-71.51178, 46, "Rhode Island"));
                governors.add(new Governor(-80.945007, 70, "South Carolina"));
                governors.add(new Governor(-99.438828, 64, "South Dakota"));
                governors.add(new Governor(-86.692345, 58, "Tennessee"));
                governors.add(new Governor(-97.563461, 59, "Texas"));
                governors.add(new Governor(-111.862434, 70, "Utah"));
                governors.add(new Governor(-72.710686, 58, "Vermont"));
                governors.add(new Governor(-78.169968, 60, "Virginia"));
                governors.add(new Governor(-121.490494, 66, "Washington"));
                governors.add(new Governor(-80.954453, 66, "West Virginia"));
                governors.add(new Governor(-89.616508, 49, "Wisconsin"));
                governors.add(new Governor(-107.30249, 55, "Wyoming"));

                KMeans<Governor> kmeans = new KMeans<>(2, governors);
                List<KMeans<Governor>.Cluster> govClusters = kmeans.run(100);
                String strGovernorOutResult = new String();
                for (int clusterIndex = 0; clusterIndex < govClusters.size(); clusterIndex++) {
                    System.out.printf("Cluster %d: %s%n", clusterIndex, govClusters.get(clusterIndex).points);
                    strGovernorOutResult += "Cluster "+ clusterIndex +
                            ": " + govClusters.get(clusterIndex).points + ". "  + "\n";
                }
                textKMeansResult.setText(strGovernorOutResult);
                break;
            case R.id.test_album_clustering:
                List<Album> albums = new ArrayList<>();
                albums.add(new Album("Got to Be There", 1972, 35.45, 10));
                albums.add(new Album("Ben", 1972, 31.31, 10));
                albums.add(new Album("Music & Me", 1973, 32.09, 10));
                albums.add(new Album("Forever, Michael", 1975, 33.36, 10));
                albums.add(new Album("Off the Wall", 1979, 42.28, 10));
                albums.add(new Album("Thriller", 1982, 42.19, 9));
                albums.add(new Album("Bad", 1987, 48.16, 10));
                albums.add(new Album("Dangerous", 1991, 77.03, 14));
                albums.add(new Album("HIStory: Past, Present and Future, Book I", 1995, 148.58, 30));
                albums.add(new Album("Invincible", 2001, 77.05, 16));
                KMeans<Album> kmeansAlbum = new KMeans<>(2, albums);
                List<KMeans<Album>.Cluster> clusters = kmeansAlbum.run(100);
                String strAlbumOutResult = new String();
                for (int clusterIndex = 0; clusterIndex < clusters.size(); clusterIndex++) {
                    System.out.printf("Cluster %d Avg Length %f Avg Tracks %f: %s%n",
                            clusterIndex, clusters.get(clusterIndex).centroid.dimensions.get(0),
                            clusters.get(clusterIndex).centroid.dimensions.get(1),
                            clusters.get(clusterIndex).points);
                    strAlbumOutResult += String.format("Cluster %d Avg Length %f Avg Tracks %f: %s%n",
                            clusterIndex, clusters.get(clusterIndex).centroid.dimensions.get(0),
                            clusters.get(clusterIndex).centroid.dimensions.get(1),
                            clusters.get(clusterIndex).points);
                }
                textKMeansResult.setText(strAlbumOutResult);
                break;
        }
    }
}

