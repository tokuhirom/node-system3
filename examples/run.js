var addon = require('../index.js');

console.log(addon.system("ls -l"));
console.log(addon.system("heh"));
console.log("=================");
console.log(addon.qx("ls -l").split(/\n/).join("\n****\n"));
