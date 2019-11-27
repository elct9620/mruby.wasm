(function() {
  var Plugin = {};

  /**
   * Initialize Plugin
   */

  Plugin.init = function() {
    ccall(
      'plugin_init',
      'null',
      [],
      []
    )
  }

  /**
   * Destroy Plugin
   */

  Plugin.destroy = function() {
    ccall(
      'plugin_destroy',
      'null',
      [],
      []
    )
  }

  /**
   * On Update
   */

  Plugin.update = function(deltaTime) {
    ccall(
      'plugin_update',
      'null',
      ['number'],
      [deltaTime]
    )
  }

  /**
   * Execute Code
   */

  Plugin.execute = function(code) {
    ccall(
      'plugin_execute',
      'null',
      ['string'],
      [code]
    )
  }

  window.Game = window.Game || {};
  window.Game.Plugin = window.Game.Plugin || Plugin;
}());

