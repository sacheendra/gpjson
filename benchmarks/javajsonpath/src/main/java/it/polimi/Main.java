package it.polimi;

public class Main {
    private static int warmup_query = 5;
    private static int repeat_query = 10;
    private static boolean DEBUG = false;
    private final static String baseDir = "../../../datasets/";

    interface I {
        public int query(String dataset);
    }

    public static void main(String[] args) {
        for (int i=0; i<args.length; i++) {
            if (args[i].equals("DEBUG")) {
                DEBUG = true;
            }
            else if (args[i].contains("warmup=")) {
                warmup_query = Integer.parseInt(args[i].substring(7));
            }
            else if (args[i].contains("repeat=")) {
                repeat_query = Integer.parseInt(args[i].substring(7));
            }
        }
        
        Execute exe = new Execute();
        String test = "test_large_record.json";
        String twitter_small = "twitter_small_records.json";
        String twitter_smaller = "twitter_small_records_smaller.json";
        String bestbuy_small = "bestbuy_small_records.json";
        String walmart_small = "walmart_small_records.json";

        exe.execute(warmup_query, repeat_query, twitter_small, baseDir, Query::TT1, "TT1", DEBUG);
        exe.execute(warmup_query, repeat_query, twitter_small, baseDir, Query::TT2, "TT2", DEBUG);
        exe.execute(warmup_query, repeat_query, twitter_small, baseDir, Query::TT3, "TT3", DEBUG);
        exe.execute(warmup_query, repeat_query, twitter_small, baseDir, Query::TT4, "TT4", DEBUG);

        exe.execute(warmup_query, repeat_query, walmart_small, baseDir, Query::WM, "WM", DEBUG);

        exe.execute(warmup_query, repeat_query, bestbuy_small, baseDir, Query::BB, "BB", DEBUG);
    }
}