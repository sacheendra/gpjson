package it.polimi;

import java.util.Arrays;
import java.util.List;
import java.util.ArrayList;

public class Execute {
    private boolean DEBUG = false;
    private int warmup_query = 5;
    private int repeat_query = 10;
    private String baseDir;

    public void execute(String dataset, Batch1.I func, String query) {
        if (DEBUG) System.out.println("Starting warmup queries on dataset " + dataset);
        List<String> result = new ArrayList();
        for (int i=0; i < warmup_query; i++)
            result = func.query(baseDir + dataset);
        int numResults = result.size();
        if (DEBUG) System.out.println("Starting query on dataset " + dataset);
        long start = 0;
        double[] delays = new double[repeat_query];
        for (int i=0; i < repeat_query; i++) {
            start = System.currentTimeMillis();
            func.query(baseDir + dataset);
            delays[i] = System.currentTimeMillis() - start;
        }
        double average = Arrays.stream(delays).sum() / repeat_query;
        double std = Math.sqrt(Arrays.stream(delays).reduce(0, (tot, elem) -> tot + (elem-average)*(elem-average) / (repeat_query-1)));
        if (DEBUG) System.out.println("Executed query " + query + " on dataset " + dataset + " in " + average + "ms; results: " + numResults);
        System.out.println("javajsonpath,"+dataset+","+query+","+average+","+std+","+numResults+","+warmup_query+","+repeat_query);
    }

    public void init(String[] args, String baseDir) {
        this.baseDir = baseDir;
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
            else if (args[i].contains("threads=")) {
                System.setProperty("java.util.concurrent.ForkJoinPool.common.parallelism", args[i].substring(8));
            }
        }
    }
}
