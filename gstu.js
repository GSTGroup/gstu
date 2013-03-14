/* $Id:$ */
/* 
    Document   : gstu.js
    Created on : 8/19/2011
    Author     : fields
    Description:
        GSTMain JavaScript Util Library        
*/

(function ($) {


  //
  // Collapsible Regions
  //
  // TO USE:
  //  HTLM Structure should be:
  //  <div class="gst-collapsible-region">
  //    <h2 class="block-title">Title</h2>
  //    <div class="block-content">This is the content</div>
  //  </div>
  // Optional:
  //  <div class="gst-collapsible-region" region-title=".mytitle" region-content=".mycontent" region-slidespeed="100">
  //  region-title and region-content MUST be JQuery "selectors" (don't forget the "." or put a "#" in for an ID)
  //  If you want the region to be "collapsed" upon load, add the class "gst-region-collapsed" to the OUTER div
  //
  //ADFTODO: Add ability to store collapsed/expanded in local COOKIES so it persists across page-loads (see collapsiblock.js for example)
  function gstu_collapsible_region_process(context) {
    var $ele = $(context),
      group = $ele.attr('region-group'),
      title_sel = $ele.attr('region-title'),
      content_sel = $ele.attr('region-content'),
      slidespeed = $ele.attr('region-slidespeed');
    //console.log('gst_collapsible_region_process', $ele, title_sel, content_sel);
    title_sel = (!title_sel) ? '.block-title:first' : title_sel;
    content_sel = (!content_sel) ? '.block-content:first' : content_sel;
    //console.log('gst_collapsible_region_process', $ele, title_sel, content_sel);
    slidespeed = (!slidespeed) ? 500 : slidespeed;
    
    var eTitle = $(title_sel, context);
    var eContent = $(content_sel, context);
    //console.log('gst_collapsible_region_process', eTitle, eContent);
        
    eTitle.region = context;
    eTitle.content = eContent;
    
    var is_collapsed = $(eTitle.region).hasClass('gst-region-collapsed') ? 1 : 0;
    var is_active = $(eTitle.region).hasClass('gst-region-active') ? 1 : 0;
    if (is_collapsed) { // Show Collapsed
      $(eTitle.content).hide();
      $(eTitle.region).removeClass('gst-region-active');
      $(eTitle.region).removeClass('active');
      $(eTitle.region).addClass('gst-region-collapsed');
    } else if (is_active) { // Show Active
      $(eTitle.content).show();
      $(eTitle.region).addClass('gst-region-active');
      $(eTitle.region).removeClass('active');
      $(eTitle.region).removeClass('gst-region-collapsed');
    } else { // Default is Collapsed (if nothing set) - so set proper classes and HIDE
      $(eTitle.content).hide();
      $(eTitle.region).removeClass('gst-region-active');
      $(eTitle.region).removeClass('active');
      $(eTitle.region).addClass('gst-region-collapsed');      
    }
    
    // Attach Click Event
    $(eTitle)
    .wrapInner('<a href="#" role="link" />')
    .click(function(e) {
      e.preventDefault();
      
      var is_collapsed = $(eTitle.region).hasClass('gst-region-collapsed') ? 1 : 0;
      var action = (is_collapsed) ? 'show' : 'hide';
      if (group && action == 'show') {
        // Hide Siblings
        $(eTitle.region).siblings('.gst-collapsible-region').each(function() {
          gstu_collapsible_region_hide(this);
        });
      }
      if (slidespeed > 100) {
        $(eTitle.content).animate({
          height: action,
          opacity: action,
          }, slidespeed, function() {
            $(eTitle.region).toggleClass('gst-region-active');
            $(eTitle.region).toggleClass('active');
            $(eTitle.region).toggleClass('gst-region-collapsed');        
        });
      } else {
        gstu_collapsible_region_action(context, action);
      }
      
    });
  }
  
  function gstu_collapsible_region_hide(context) {
    var $ele = $(context),
      group = $ele.attr('region-group'),
      title_sel = $ele.attr('region-title'),
      content_sel = $ele.attr('region-content'),
      slidespeed = $ele.attr('region-slidespeed');
    title_sel = (!title_sel) ? '.title:first' : title_sel;
    content_sel = (!content_sel) ? '.content:first' : content_sel;
    slidespeed = (!slidespeed) ? 500 : slidespeed;        
    var eContent = $(content_sel, context);
    $ele.removeClass('gst-region-active');
    $ele.removeClass('active');
    $ele.addClass('gst-region-collapsed');
    $(eContent).hide();
  }

  function gstu_collapsible_region_action(context, action) {
    var $ele = $(context),
      group = $ele.attr('region-group'),
      title_sel = $ele.attr('region-title'),
      content_sel = $ele.attr('region-content'),
      slidespeed = $ele.attr('region-slidespeed');
    title_sel = (!title_sel) ? '.title:first' : title_sel;
    content_sel = (!content_sel) ? '.content:first' : content_sel;
    slidespeed = (!slidespeed) ? 500 : slidespeed;    
    var eContent = $(content_sel, context);
    switch (action) {
      case 'show':
        $ele.addClass('gst-region-active');
        $ele.addClass('active');
        $ele.removeClass('gst-region-collapsed');
        $(eContent).show();
        break;
      case 'hide':
        $ele.removeClass('gst-region-active');
        $ele.removeClass('active');
        $ele.addClass('gst-region-collapsed');
        $(eContent).hide();
        break;
    }

  }  
  
  Drupal.behaviors.gst_collapsible_region = {
      attach: function(context) {
        $(".gst-collapsible-region:not(.gst-collapsible-region-processed)", context)
        .addClass("gst-collapsible-region-processed")
        .each(function() {
          gstu_collapsible_region_process(this); // pass in the context of the element being processed
        });
      }
  };


// BeautyTip
  if (jQuery.bt) {
    jQuery.bt.options.postShow = function(box) {    
      gstu_beautytip_postShow(this, box);
    }
  }
  
  function gstu_beautytip_postShow(bt, box) {
    //console.log('gst_beautytip_postShow', box);
    var opts = jQuery.extend(false, jQuery.bt.defaults, jQuery.bt.options, Drupal.settings.beautytips[bt.id]);
    var gstu_postShow = opts.gst_postShow;
    switch (gstu_postShow) {
      case 'gstu_beautytips_close_on_mouseleave':
        gstu_beautytips_close_on_mouseleave(bt, box, opts);
        break;
    }
  }
  
  function gstu_beautytips_close_on_mouseleave(bt, box, opts) {
    //console.log('gst_beautytips_close_on_mouseleave', box);
    $('.bt-content').bind('mouseleave', function() {
      //console.log('hide', box);
      $(box).hide();
    });
  }
  
})(jQuery);