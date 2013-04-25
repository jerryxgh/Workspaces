(function($) {
  $.fn.termifier = function(options) {
    options = $.extend({
      lookupResource: 'getTerm',
      flyoutClass: 'lookerUpperFlyout'
    },options||{});
    this.attr('title','Click me for my definition!');
    return this.click(function(event){
      $.ajax({
        url: options.lookupResource,
        type: 'get',
        data: {term: this.innerHTML},
        dataType: 'html',
        success: function(data) {
          $('<div></div>')
            .css({
              position: 'absolute',
              left: event.pageX,
              top: event.pageY,
              cursor: 'pointer',
              display: 'none'
            })
            .html(data)
            .addClass(options.flyoutClass)
            .click(function(){
              $(this).fadeOut(1500,function(){$(this).remove();});
             })
            .appendTo('body')
            .fadeIn();
          }
        });
        return false;
      });
  }
})(jQuery);
