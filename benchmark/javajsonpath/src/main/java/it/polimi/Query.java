package it.polimi;

import com.jayway.jsonpath.Configuration;
import com.jayway.jsonpath.JsonPath;
import com.jayway.jsonpath.PathNotFoundException;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.List;
import java.util.stream.Stream;

public class Query {
    public static int TT1(String dataset) {
        Stream<String> lines = null;
        try {
            lines = Files.lines(Paths.get(dataset)).parallel();
        } catch (IOException e) {
            System.out.println("Could not load dataset file.");
        }
        int count = lines.map(x -> {
            Object document = Configuration.defaultConfiguration().jsonProvider().parse(x);
            int results = 0;
            try {
                String result = JsonPath.read(document, "$.user.lang");
                results++;
            } catch (PathNotFoundException | ClassCastException e) {}
            return results;
        }).reduce(0, Integer::sum);
        return count;
    }

    public static int TT2(String dataset) {
        Stream<String> lines = null;
        try {
            lines = Files.lines(Paths.get(dataset)).parallel();
        } catch (IOException e) {
            System.out.println("Could not load dataset file.");
        }
        int count = lines.map(x -> {
            Object document = Configuration.defaultConfiguration().jsonProvider().parse(x);
            int results = 0;
            try {
                String result = JsonPath.read(document, "$.user.lang");
                results++;
            } catch (PathNotFoundException e) {}
            try {
                String result = JsonPath.read(document, "$.lang");
                results++;
            } catch (PathNotFoundException e) {}
            return results;
        }).reduce(0, Integer::sum);
        return count;
    }

    public static int TT3(String dataset) {
        Stream<String> lines = null;
        try {
            lines = Files.lines(Paths.get(dataset)).parallel();
        } catch (IOException e) {
            System.out.println("Could not load dataset file.");
        }
        int count = lines.map(x -> {
            Object document = Configuration.defaultConfiguration().jsonProvider().parse(x);
            int results = 0;
            try {
                List<String> result = JsonPath.read(document, "$.user[?(@.lang == 'nl')].lang");
                if (result.size() > 0) results++;
            } catch (PathNotFoundException | ClassCastException e) {}
            return results;
        }).reduce(0, Integer::sum);
        return count;
    }

    public static int TT4(String dataset) {
        Stream<String> lines = null;
        try {
            lines = Files.lines(Paths.get(dataset)).parallel();
        } catch (IOException e) {
            System.out.println("Could not load dataset file.");
        }
        int count = lines.map(x -> {
            Object document = Configuration.defaultConfiguration().jsonProvider().parse(x);
            int results = 0;
            try {
                List<String> result = JsonPath.read(document, "$.user[?(@.lang == 'en')].lang");
                if (result.size() > 0) results++;
            } catch (PathNotFoundException | ClassCastException e) {}
            return results;
        }).reduce(0, Integer::sum);
        return count;
    }

    public static int WM(String dataset) {
        Stream<String> lines = null;
        try {
            lines = Files.lines(Paths.get(dataset)).parallel();
        } catch (IOException e) {
            System.out.println("Could not load dataset file.");
        }
        int count = lines.map(x -> {
            Object document = Configuration.defaultConfiguration().jsonProvider().parse(x);
            int results = 0;
            try {
                Object obj = JsonPath.read(document, "$.bestMarketplacePrice.price");
                results++;
            } catch (PathNotFoundException e) {}
            try {
                String result = JsonPath.read(document, "$.name");
                results++;
            } catch (PathNotFoundException e) {}
            return results;
        }).reduce(0, Integer::sum);
        return count;
    }

    public static int BB(String dataset) {
        Stream<String> lines = null;
        try {
            lines = Files.lines(Paths.get(dataset)).parallel();
        } catch (IOException e) {
            System.out.println("Could not load dataset file.");
        }
        int count = lines.map(x -> {
            Object document = Configuration.defaultConfiguration().jsonProvider().parse(x);
            int results = 0;
            try {
                List<String> result = JsonPath.read(document, "$.categoryPath[1:3].id");
                results =+ result.size();
            } catch (PathNotFoundException e) {}
            return results;
        }).reduce(0, Integer::sum);
        return count;
    }
}
