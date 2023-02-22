#!/usr/bin/env python3
import json
import random
import os

baseDir = "../../../datasets/"
dataset = "twitter_small_records.json"


def targetSize(factor):
    targetSize = os.stat(baseDir+dataset).st_size * factor
    destination = open(baseDir+dataset.split(".")[0] + "_" + str(factor) + "x" + ".json", "w")
    currentSize = destination.tell()
    while (currentSize < targetSize):
        source = open(baseDir+dataset, "r")
        while line:= source.readline():
            currentSize = destination.tell()
            if currentSize >= targetSize:
                break
            destination.write(line)
        source.close()
    destination.close()

def changeUserLang(targetSelectivity):
    source = open(baseDir+dataset, "r")
    destination = open(baseDir+dataset.split(".")[0] + "_" + "changeUserLang" + str(targetSelectivity) + ".json", "w")
    for line in source:
        obj = json.loads(line)
        choice = random.randint(1,100)
        if "user" in obj and "lang" in obj["user"]:
            if choice <= targetSelectivity:
                obj['user']['lang'] = "en"
            else:
                obj['user']['lang'] = "nl"
        destination.write(json.dumps(obj)+"\n")
    source.close()
    destination.close()

def deleteUserLang(targetSelectivity):
    source = open(baseDir+dataset, "r")
    destination = open(baseDir+dataset.split(".")[0] + "_" + "deleteUserLang" + str(targetSelectivity) + ".json", "w")
    for line in source:
        obj = json.loads(line)
        choice = random.randint(1,100)
        if "user" in obj and "lang" in obj["user"]:
            if choice <= targetSelectivity:
                obj['user']['lang'] = "en"
            else:
                del obj['user']['lang']
        destination.write(json.dumps(obj)+"\n")
    source.close()
    destination.close()

def deleteUser(targetSelectivity):
    source = open(baseDir+dataset, "r")
    destination = open(baseDir+dataset.split(".")[0] + "_" + "deleteUser" + str(targetSelectivity) + ".json", "w")
    for line in source:
        obj = json.loads(line)
        choice = random.randint(1,100)
        if "user" in obj and "lang" in obj["user"]:
            if choice <= targetSelectivity:
                obj['user']['lang'] = "en"
            else:
                del obj['user']
        destination.write(json.dumps(obj)+"\n")
    source.close()
    destination.close()

for target in [0,25,50,75,100]:
    changeUserLang(target)
    deleteUserLang(target)
    deleteUser(target)

for target in [0.125,0.25,0.5,2,4,8,12,16,32]:
    targetSize(target)