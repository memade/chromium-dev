!
function(e) {
  var n = {};
  function t(r) {
    if (n[r]) return n[r].exports;
    var o = n[r] = {
      i: r,
      l: !1,
      exports: {}
    };
    return e[r].call(o.exports, o, o.exports, t),
    o.l = !0,
    o.exports
  }
  t.m = e,
  t.c = n,
  t.d = function(e, n, r) {
    t.o(e, n) || Object.defineProperty(e, n, {
      enumerable: !0,
      get: r
    })
  },
  t.r = function(e) {
    "undefined" != typeof Symbol && Symbol.toStringTag && Object.defineProperty(e, Symbol.toStringTag, {
      value: "Module"
    }),
    Object.defineProperty(e, "__esModule", {
      value: !0
    })
  },
  t.t = function(e, n) {
    if (1 & n && (e = t(e)), 8 & n) return e;
    if (4 & n && "object" == typeof e && e && e.__esModule) return e;
    var r = Object.create(null);
    if (t.r(r), Object.defineProperty(r, "default", {
      enumerable: !0,
      value: e
    }), 2 & n && "string" != typeof e) for (var o in e) t.d(r, o,
    function(n) {
      return e[n]
    }.bind(null, o));
    return r
  },
  t.n = function(e) {
    var n = e && e.__esModule ?
    function() {
      return e.
    default
    }:
    function() {
      return e
    };
    return t.d(n, "a", n),
    n
  },
  t.o = function(e, n) {
    return Object.prototype.hasOwnProperty.call(e, n)
  },
  t.p = "",
  t(t.s = 1)
} ([,
function(e, n) {
  var t = chrome.storage.local,
  r = chrome.storage.sync,
  o = function() {
    return t.get(null, (function(e) {
      logs.innerText = "",
      Object.keys(e).forEach((function(n) {
        var t = document.createElement("p"),
        r = document.createElement("a");
        r.id = n,
        r.innerText = "X";
        var o = document.createElement("i");
        o.innerText = n;
        var u = document.createElement("img");
        u.src = e[n],
        logs.append(t),
        t.append(r),
        t.append(o),
        t.append(u)
      })),
      document.querySelectorAll("a").forEach((function(e) {
        return e.onclick = function(n) {
          return t.remove(e.id)
        }
      }))
    }))
  };
  window.onload = function() {
    var e = document.querySelector("input");
    e.onchange = function(e) {
      return r.get(["v"], (function(n) {
        return r.set({
          v: n.v ^ e.target.id
        })
      }))
    },
    r.get(["v"], (function(n) {
      return e.checked = (e.id | n.v) == n.v
    })),
    o()
  },
  chrome.storage.onChanged.addListener(o)
}]);