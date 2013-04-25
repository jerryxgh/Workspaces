var GLOBAL = {};
GLOBAL.namespace = function(str) {
    var arr = str.split("."), o = GLOBAL;
    for (var i = (arr[0] == "GLOBAL" ? 1 : 0); i < arr.length; ++i) {
        o[arr[i]] = o[arr[i]] || {};
        o = o[arr[i]];
    }
};
GLOBAL.namespace("Dom");
GLOBAL.namespace("Event");
GLOBAL.Dom.addClass = function(node, str) {
    if (!new RegExp("(^|\\s+)" + str).test(node.className)) {
        node.className = node.className + " " + str;
    }
};
GLOBAL.Dom.removeClass = function(node, str) {
    node.className = node.className.replace(new RegExp("(^|\\s+)" + str), "");
};
GLOBAL.Event.on = function(node, eventType, handler, scope) {
    node = typeof node == "String" ? document.getElementById(node) : node;
    scope = scope || node;
    if (document.all) { // IE
        node.attachEvent("on" + eventType, function() {handler.apply(scope, arguments);});
    } else {
        node.addEventListener(eventType, function() {handler.apply(scope, arguments);}, false);
    }
};
GLOBAL.Dom.getElementsByClassName = function(str, root, tag) {
    if (root) {
        root = typeof root == "String" ? document.getElementById(root) : root;
    } else {
        root = document.body;
    }

    tag = tag || "*";
    var els = root.getElementsByTagName(tag), arr = [];
    for (var i = 0; i < els.length; ++i) {
        for (var k = els[i].className.split(" "), l = k.length, j = 0; j < l; ++j) {
            if (k[j] == str) {
                arr.push(els[i]);
                break;
            }
        }
    }

    return arr;
};