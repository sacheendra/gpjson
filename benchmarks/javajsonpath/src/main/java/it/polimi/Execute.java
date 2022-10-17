package it.polimi;

public class Execute {
    public void execute(final int warmup_query, final int repeat_query, String dataset, String baseDir, Main.I func, String query, boolean DEBUG) {
        if (DEBUG) System.out.println("Starting warmup queries on dataset " + dataset);
        int num_results = 0;
        for (int i=0; i < warmup_query; i++)
            num_results = func.query(baseDir + dataset);
        if (DEBUG) System.out.println("Starting query on dataset " + dataset);
        long start = System.currentTimeMillis();
        for (int i=0; i < repeat_query; i++)
            func.query(baseDir + dataset);
        long delay = System.currentTimeMillis() - start;
        if (DEBUG) System.out.println("Executed query " + query + " on dataset " + dataset + " in " + delay/repeat_query + "ms; results: " + num_results);
        System.out.println("javajsonpath,"+dataset+","+query+","+delay/repeat_query+","+num_results+","+warmup_query+","+repeat_query);
    }
}
