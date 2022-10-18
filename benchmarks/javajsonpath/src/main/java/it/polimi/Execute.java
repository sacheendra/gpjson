package it.polimi;

public class Execute {
    private boolean DEBUG = false;
    private int warmup_query = 5;
    private int repeat_query = 10;
    private String baseDir;

    public void execute(String dataset, Batch1.I func, String query) {
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
        }
    }
}
