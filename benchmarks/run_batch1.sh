#!/bin/bash

NODE_PATH="$HOME/node-v16.18.0/bin"
WARMUP=1
REPEAT=2

echo "Starting benchmarks"
echo "engine,dataset,query,time,results,warmup,repeat" > results.csv

# gpjson
echo "running gpjson"
cd gpjson
node --jvm --polyglot batch1.js warmup=$WARMUP repeat=$REPEAT >> ../results.csv
cd ..

# javajsonpath
echo "running javajsonpath"
cd javajsonpath
mvn package
java -cp target/javajsonpath-1.0-SNAPSHOT-jar-with-dependencies.jar it.polimi.Batch1 warmup=$WARMUP repeat=$REPEAT >> ../results.csv
cd ..

# nodejsonpath
echo "running nodejsonpath"
cd nodejsonpath
$NODE_PATH/npm install jsonpath
$NODE_PATH/node batch1.js warmup=$WARMUP repeat=$REPEAT >> ../results.csv
cd ..

# nodejsonpathplus
echo "running nodejsonpathplus"
cd nodejsonpathplus
$NODE_PATH/npm install jsonpath-plus
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
$NODE_PATH/npm install simdjson
$NODE_PATH/node batch1.js warmup=$WARMUP repeat=$REPEAT >> ../results.csv
cd ..

# pison
echo "running pison"
cd pison
make batch1
./bin/batch1 warmup=$WARMUP repeat=$REPEAT >> ../results.csv
cd ..

# rapidjson
echo "running rapidjson"
cd rapidjson
make batch1
./bin/batch1 warmup=$WARMUP repeat=$REPEAT >> ../results.csv
cd ..

# simdjson
echo "running simdjson"
cd simdjson
make batch1
./bin/batch1 warmup=$WARMUP repeat=$REPEAT >> ../results.csv
cd ..

echo "Benchmarks done"