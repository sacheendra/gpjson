package it.polimi;
import java.util.*;

public class Sizes {
    private final static String baseDir = "../../../datasets/";

    interface I {
        public int query(String dataset);
    }

    public static void main(String[] args) {
        Execute exe = new Execute();
        exe.init(args, baseDir);

        List<String> twitter = new ArrayList<>();
        twitter.add("twitter_small_records.json");
        twitter.add("twitter_small_records_0.125x.json");
        twitter.add("twitter_small_records_0.25x.json");
        twitter.add("twitter_small_records_0.5x.json");
        twitter.add("twitter_small_records_1.5x.json");
        twitter.add("twitter_small_records_2x.json");
        twitter.add("twitter_small_records_2.5x.json");

        for (int i=0; i<twitter.size(); i++) {
            exe.execute(twitter.get(i), Query::TT4, "TT4");
        }
    }
}