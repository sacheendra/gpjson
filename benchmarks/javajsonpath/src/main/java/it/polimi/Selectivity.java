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
        twitter.add("twitter_small_records_changeUserLang0.json");
        twitter.add("twitter_small_records_deleteUser0.json");
        twitter.add("twitter_small_records_deleteUserLang0.json");
        twitter.add("twitter_small_records_changeUserLang25.json");
        twitter.add("twitter_small_records_deleteUser25.json");
        twitter.add("twitter_small_records_deleteUserLang25.json");
        twitter.add("twitter_small_records_changeUserLang50.json");
        twitter.add("twitter_small_records_deleteUser50.json");
        twitter.add("twitter_small_records_deleteUserLang50.json");
        twitter.add("twitter_small_records_changeUserLang75.json");
        twitter.add("twitter_small_records_deleteUser75.json");
        twitter.add("twitter_small_records_deleteUserLang75.json");
        twitter.add("twitter_small_records_changeUserLang100.json");
        twitter.add("twitter_small_records_deleteUser100.json");
        twitter.add("twitter_small_records_deleteUserLang100.json");

        for (int i=0; i<twitter.size(); i++) {
            exe.execute(twitter.get(i), Query::TT1, "TT1");
            exe.execute(twitter.get(i), Query::TT2, "TT2");
            exe.execute(twitter.get(i), Query::TT3, "TT3");
            exe.execute(twitter.get(i), Query::TT4, "TT4");
        }
    }
}
