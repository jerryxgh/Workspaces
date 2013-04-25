/*
* Provides common support functions for Lab pages
*/
$.fn.formatForDisplay = function() {
  if (this.size()==0) return "<em>wrapped set is empty</em>"
  var text = '';
  this.each(function(){
    text += '<div>' + this.tagName;
    if (this.id) text += '#' + this.id;
    text += '</div>';
  });
  return text;
}

$.toSource = function(target) {
   if (typeof target.toSource !== 'undefined' && typeof target.callee === 'undefined') {
      return target.toSource().slice(1,-1);
   }
   switch (typeof target) {
      case 'number':
      case 'boolean':
      case 'function':
         return target;
         break;
      case 'string':
         return '\'' + target + '\'';
         break;
      case 'object':
         var result;
         if (target instanceof Date) {
            result = 'new Date('+target.getTime()+')';
         }
         else if (target.constructor === Array || typeof target.callee !== 'undefined') {
            result = '[';
            var i, length = target.length;
            for (i = 0; i < length-1; i++) { result += $.toSource(target[i]) + ','; }
            result += $.toSource(target[i]) + ']';
         }
         else {
            result = '{';
            var key;
            for (key in target) { result += key + ':' + $.toSource(target[key]) + ','; }
            result = result.replace(/\,$/, '') + '}';
         }
         return result;
         break;
      default:
         return '?unsupported-type?';
         break;
   }
}
