#!/usr/bin/env python3
import json
import random

baseDir = "../../../datasets/"
dataset = "twitter_small_records.json"

def changeUserLang(targetSelectivity):
    source = open(baseDir+dataset, "r")
    destination = open(baseDir+dataset.split(".")[0] + "_" + str(targetSelectivity) + "changeUserLang" + ".json", "w")
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
    destination = open(baseDir+dataset.split(".")[0] + "_" + str(targetSelectivity) + "deleteUserLang" + ".json", "w")
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
    destination = open(baseDir+dataset.split(".")[0] + "_" + str(targetSelectivity) + "deleteUser" + ".json", "w")
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

changeUserLang(75)
deleteUserLang(75)
deleteUser(75)