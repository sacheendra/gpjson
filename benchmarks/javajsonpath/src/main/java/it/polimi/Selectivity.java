package it.polimi;
import java.util.*;

public class Selectivity {
    private final static String baseDir = "../../../datasets/";

    interface I {
        public int query(String dataset);
    }

    public static void main(String[] args) {
        Execute exe = new Execute();
        exe.init(args, baseDir);

        List<String> twitter = new ArrayList<>();
        twitter.add("twitter_small_records_0changeUserLang.json");
        twitter.add("twitter_small_records_0deleteUser.json");
        twitter.add("twitter_small_records_0deleteUserLang.json");
        twitter.add("twitter_small_records_25changeUserLang.json");
        twitter.add("twitter_small_records_25deleteUser.json");
        twitter.add("twitter_small_records_25deleteUserLang.json");
        twitter.add("twitter_small_records_50changeUserLang.json");
        twitter.add("twitter_small_records_50deleteUser.json");
        twitter.add("twitter_small_records_50deleteUserLang.json");
        twitter.add("twitter_small_records_75changeUserLang.json");
        twitter.add("twitter_small_records_75deleteUser.json");
        twitter.add("twitter_small_records_75deleteUserLang.json");
        twitter.add("twitter_small_records_100changeUserLang.json");
        twitter.add("twitter_small_records_100deleteUser.json");
        twitter.add("twitter_small_records_100deleteUserLang.json");

        for (int i=0; i<twitter.size(); i++) {
            exe.execute(twitter.get(i), Query::TT1, "TT1");
            exe.execute(twitter.get(i), Query::TT2, "TT2");
            exe.execute(twitter.get(i), Query::TT3, "TT3");
            exe.execute(twitter.get(i), Query::TT4, "TT4");
        }
    }
}
