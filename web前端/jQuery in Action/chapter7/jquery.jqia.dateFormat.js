(function($){
  $.formatDate = function(date,pattern) {
    var result = [];
    while (pattern.length>0) {
      $.formatDate.patternParts.lastIndex = 0;
      var matched = $.formatDate.patternParts.exec(pattern);
      if (matched) {
        result.push($.formatDate.patternValue[matched[0]].call(this,date));
        pattern = pattern.slice(matched[0].length);
      }
      else {
        result.push(pattern.charAt(0));
        pattern = pattern.slice(1);
      }
    }
    return result.join('');
  };

  $.formatDate.patternParts =
    /^(yy(yy)?|M(M(M(M)?)?)?|d(d)?|EEE(E)?|a|H(H)?|h(h)?|m(m)?|s(s)?|S)/;

  $.formatDate.monthNames = [
    'January','February','March','April','May','June','July',
    'August','September','October','November','December'
  ];

  $.formatDate.dayNames = [
    'Sunday','Monday','Tuesday','Wednesday','Thursday','Friday',
    'Saturday'
  ];

  $.formatDate.patternValue = {
    yy: function(date) {
      return $.toFixedWidth(date.getFullYear(),2);
    },
    yyyy: function(date) {
      return date.getFullYear().toString();
    },
    MMMM: function(date) {
      return $.formatDate.monthNames[date.getMonth()];
    },
    MMM: function(date) {
      return $.formatDate.monthNames[date.getMonth()].substr(0,3);
    },
    MM: function(date) {
      return $.toFixedWidth(date.getMonth()+1,2);
    },
    M: function(date) {
      return date.getMonth()+1;
    },
    dd: function(date) {
      return $.toFixedWidth(date.getDate(),2);
    },
    d: function(date) {
      return date.getDate();
    },
    EEEE: function(date) {
      return $.formatDate.dayNames[date.getDay()];
    },
    EEE: function(date) {
      return $.formatDate.dayNames[date.getDay()].substr(0,3);
    },
    HH: function(date) {
      return $.toFixedWidth(date.getHours(),2);
    },
    H: function(date) {
      return date.getHours();
    },
    hh: function(date) {
      var hours = date.getHours();
      return $.toFixedWidth(hours>12 ? hours - 12 : hours,2);
    },
    h: function(date) {
      return date.getHours()%12;
    },
    mm: function(date) {
      return $.toFixedWidth(date.getMinutes(),2);
    },
    m: function(date) {
      return date.getMinutes();
    },
    ss: function(date) {
      return $.toFixedWidth(date.getSeconds(),2);
    },
    s: function(date) {
      return date.getSeconds();
    },
    S: function(date) {
      return $.toFixedWidth(date.getMilliseconds(),3);
    },
    a: function(date) {
      return date.getHours() < 12 ? 'AM' : 'PM';
    }
  };

  $.toFixedWidth = function(value,length,fill) {
    if (!fill) fill = '0';
    var result = value.toString();
    var padding = length - result.length;
    if (padding < 0) {
      result = result.substr(-padding);
    }
    else {
      for (var n = 0; n < padding; n++) result = fill + result;
    }
    return result;
  };

})(jQuery);
