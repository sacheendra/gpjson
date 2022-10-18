#!/bin/bash

# exit when any command fails
set -e

# keep track of the last executed command
trap 'last_command=$current_command; current_command=$BASH_COMMAND' DEBUG
# echo an error message before exiting
trap 'echo "\"${last_command}\" command exited with exit code $?."' EXIT

NODE_PATH="$NODE_DIR/bin"
GRAAL_PATH="$GRAAL_DIR/bin"
WARMUP=1
REPEAT=2

echo "Starting benchmarks"
echo "engine,dataset,query,time,results,warmup,repeat" > results.csv

# gpjson
echo "running gpjson"
cd gpjson
$GRAAL_PATH/node --jvm --polyglot batch1.js warmup=$WARMUP repeat=$REPEAT >> ../results.csv
cd ..

# javajsonpath
echo "running javajsonpath"
cd javajsonpath
mvn package > /dev/null 2>&1
java -cp target/javajsonpath-1.0-SNAPSHOT-jar-with-dependencies.jar it.polimi.Batch1 warmup=$WARMUP repeat=$REPEAT >> ../results.csv
cd ..

# nodejsonpath
echo "running nodejsonpath"
cd nodejsonpath
$NODE_PATH/npm install jsonpath > /dev/null 2>&1
$NODE_PATH/node batch1.js warmup=$WARMUP repeat=$REPEAT >> ../results.csv
cd ..

# nodejsonpathplus
echo "running nodejsonpathplus"
cd nodejsonpathplus
$NODE_PATH/npm install jsonpath-plus > /dev/null 2>&1
$NODE_PATH/node batch1.js warmup=$WARMUP repeat=$REPEAT >> ../results.csv
cd ..

# nodemanual
echo "running nodemanual"
cd nodemanual
$NODE_PATH/node batch1.js warmup=$WARMUP repeat=$REPEAT >> ../results.csv
cd ..

# nodesimdjson
echo "running nodesimdjson"
cd nodesimdjson
$NODE_PATH/npm install simdjson > /dev/null 2>&1
$NODE_PATH/node batch1.js warmup=$WARMUP repeat=$REPEAT >> ../results.csv
cd ..

# pison
echo "running pison"
cd pison
make batch1 > /dev/null 2>&1
./bin/batch1 warmup=$WARMUP repeat=$REPEAT >> ../results.csv
cd ..

# rapidjson
echo "running rapidjson"
cd rapidjson
make batch1 > /dev/null 2>&1
./bin/batch1 warmup=$WARMUP repeat=$REPEAT >> ../results.csv
cd ..

# simdjson
echo "running simdjson"
cd simdjson
make batch1 > /dev/null 2>&1
./bin/batch1 warmup=$WARMUP repeat=$REPEAT >> ../results.csv
cd ..

echo "Benchmarks done"