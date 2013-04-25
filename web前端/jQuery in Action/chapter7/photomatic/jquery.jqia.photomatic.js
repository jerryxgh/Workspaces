(function($){
  var settings;

  $.fn.photomatic = function(callerSettings) {
    settings = $.extend({
      photoElement: '#photomaticPhoto',
      transformer: function(name) {
                     return name.replace(/thumbnail/,'photo');
                   },
      nextControl: null,
      previousControl: null,
      firstControl: null,
      lastControl: null
    },callerSettings||{});
    settings.photoElement = $(settings.photoElement);
    settings.thumbnails = this.filter('img');
    settings.thumbnails.each(function(n){this.index = n;});
    settings.current = 0;
    settings.thumbnails.click(function(){ showPhoto(this.index); });
    settings.photoElement.click(function(){
      showPhoto((settings.current+1) % settings.thumbnails.length);
    });
    $(settings.nextControl).click(function(){
      showPhoto((settings.current+1) % settings.thumbnails.length);
    });
    $(settings.previousControl).click(function(){
      showPhoto((settings.thumbnails.length+settings.current-1) %
                settings.thumbnails.length);
    });
    $(settings.firstControl).click(function(){
      showPhoto(0);
    });
    $(settings.lastControl).click(function(){
      showPhoto(settings.thumbnails.length-1);
    });
    showPhoto(0);
    return this;
  };

  var showPhoto = function(index) {
    settings.photoElement
      .attr('src',
            settings.transformer(settings.thumbnails[index].src));
    settings.current = index;
  };

})(jQuery);
