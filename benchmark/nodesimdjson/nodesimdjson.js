const fs = require(`fs`);
const simdjson = require('simdjson');

console.time("json read");
var file = fs.readFileSync(`../../../datasets/twitter_small_records.json`, `utf-8`).toString().split("\n");
console.timeEnd("json read");

console.time("json parse and query");
var count = 0;
for (var i = 0; i < file.length; i++) {
    try {
        const jsonTape = simdjson.lazyParse(file[i]);
        let value = jsonTape.valueForKeyPath(`user.lang`);
        count++;
    } catch (error) {
        // console.log(error);
    }
}
console.timeEnd("json parse and query");
console.log(count);

// ### Not lazy version ###
// console.time("json parse and query");
// var count = 0;
// for (var i = 0; i < file.length; i++) {
//     try {
//         const jsonTape = simdjson.parse(file[i]);
//         let value = jsonTape["user"]["lang"];
//         count++;
//     } catch (error) {
//         // console.log(error);
//     }
// }
// console.timeEnd("json parse and query");
// console.log(count);