var sys3 = require('../index.js'),
    tap = require('tap');

tap.test('', function (t) {
    t.ok(sys3.system);
    t.ok(sys3.qx);
    t.end();
});
