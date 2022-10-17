#!/bin/bash

NODE_PATH="$HOME/node-v16.18.0/bin"

echo "Starting benchmarks"
echo "engine,dataset,query,time,results,warmup,repeat" > results.csv

# gpjson
echo "running gpjson"
cd gpjson
node --jvm --polyglot gpsjon.js >> ../results.csv
cd ..

# javajsonpath
echo "running javajsonpath"
cd javajsonpath
mvn package
java -jar target/javajsonpath-1.0-SNAPSHOT-jar-with-dependencies.jar >> ../results.csv
cd ..

# nodejsonpath
echo "running nodejsonpath"
cd nodejsonpath
$NODE_PATH/npm install jsonpath
$NODE_PATH/node async_nodejsonpath.js >> ../results.csv
cd ..

# nodejsonpathplus
echo "running nodejsonpathplus"
cd nodejsonpathplus
$NODE_PATH/npm install jsonpath-plus
$NODE_PATH/node async_nodejsonpathplus.js >> ../results.csv
cd ..

# nodemanual
echo "running nodemanual"
cd nodemanual
$NODE_PATH/node async_nodemanual.js >> ../results.csv
cd ..

# nodesimdjson
echo "running nodesimdjson"
cd nodesimdjson
$NODE_PATH/npm install simdjson
$NODE_PATH/node async_nodesimdjson.js >> ../results.csv
cd ..

# pison
echo "running pison"
cd pison
make
./bin/pison >> ../results.csv
cd ..

# rapidjson
echo "running rapidjson"
cd rapidjson
make
./bin/rapidjson >> ../results.csv
cd ..

# simdjson
echo "running simdjson"
cd simdjson
make
./bin/simdjson >> ../results.csv
cd ..

echo "Benchmarks done"