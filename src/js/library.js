var LibraryPlugin = {
  $Plugin__postset: 'Plugin.init()',
  $Plugin: {
    sprites: [],
    init: function() {
      window.Game = window.Game || {};
      Game.AddSprite = function(sprite) {
        return Plugin.sprites.push(sprite) - 1;
      }
    }
  },
  _sprite_exists__deps: ['$Plugin'],
  _sprite_exists: function(idx) {
    if (Plugin.sprites[idx]) {
      return true;
    }

    return false;
  },
  _sprite_get_attr__deps: ['$Plugin'],
  _sprite_get_int_attr: function(idx, attrNamePtr) {
    var sprite = Plugin.sprites[idx]
    if (!sprite) {
      return;
    }

    var attrName = UTF8ToString(attrNamePtr);
    return sprite[attrName];
  },
  _sprite_set_attr__deps: ['$Plugin'],
  _sprite_set_int_attr: function(idx, attrNamePtr, value) {
    var sprite = Plugin.sprites[idx]
    if (!sprite) {
      return;
    }

    var attrName = UTF8ToString(attrNamePtr);
    sprite[attrName] = value;
  }
}

mergeInto(LibraryManager.library, LibraryPlugin);
