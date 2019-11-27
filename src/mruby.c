#include<mruby.h>
#include<mruby/compile.h>
#include<mruby/string.h>
#include<mruby/variable.h>
#include<emscripten.h>

// From JS - Begin
extern mrb_bool _sprite_exists(mrb_int ptr);
extern mrb_int _sprite_get_int_attr(mrb_int ptr, const char* attr_name);
extern void _sprite_set_int_attr(mrb_int ptr, const char* attr_name, mrb_int value);
// From JS - End

typedef struct {
  mrb_state* state;
} Plugin;

static Plugin plugin;
struct RClass* Game;
struct RClass* Sprite;

mrb_value game_update(mrb_state* mrb, mrb_value self) {
  mrb_value block;
  mrb_value callback;
  mrb_float deltaTime;

  mrb_get_args(mrb, "|f&", &deltaTime, &block);

  if (mrb_nil_p(block)) {
    // Execute Case
    callback = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "_update"));
    if (!mrb_nil_p(callback)) {
      mrb_value args[1];
      args[0] = mrb_float_value(mrb, deltaTime);
      mrb_yield_argv(mrb, callback, 1, args);
    }
  } else {
    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "_update"), block);
  }

  return mrb_nil_value();
}

mrb_value sprite_from_js(mrb_state* mrb, mrb_value self) {
  mrb_int ptr;
  mrb_get_args(mrb, "i", &ptr);

  if (_sprite_exists(ptr)) {
    mrb_value args[1] = { mrb_fixnum_value(ptr) };
    return mrb_obj_new(mrb, Sprite, 1, args);
  }

  return mrb_nil_value();
}

mrb_value sprite_init(mrb_state* mrb, mrb_value self) {
  mrb_int ptr;
  mrb_get_args(mrb, "i", &ptr);
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "_ptr"), mrb_fixnum_value(ptr));

  return self;
}

mrb_value sprite_get_x(mrb_state* mrb, mrb_value self) {
  mrb_int ptr = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "_ptr")));
  mrb_int x = _sprite_get_int_attr(ptr, "x");
  return mrb_fixnum_value(x);
}

mrb_value sprite_get_y(mrb_state* mrb, mrb_value self) {
  mrb_int ptr = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "_ptr")));
  mrb_int y = _sprite_get_int_attr(ptr, "y");
  return mrb_fixnum_value(y);
}

mrb_value sprite_set_x(mrb_state* mrb, mrb_value self) {
  mrb_int value;
  mrb_get_args(mrb, "i", &value);

  mrb_int ptr = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "_ptr")));
  _sprite_set_int_attr(ptr, "x", value);
  return mrb_fixnum_value(value);
}

mrb_value sprite_set_y(mrb_state* mrb, mrb_value self) {
  mrb_int value;
  mrb_get_args(mrb, "i", &value);

  mrb_int ptr = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "_ptr")));
  _sprite_set_int_attr(ptr, "y", value);
  return mrb_fixnum_value(value);
}

EMSCRIPTEN_KEEPALIVE
void plugin_init() {
  plugin.state = mrb_open();

  // Game Module
  Game = mrb_define_module(plugin.state, "Game");
  mrb_define_class_method(plugin.state, Game, "update", game_update, MRB_ARGS_OPT(1)|MRB_ARGS_BLOCK());

  // Sprite
  Sprite = mrb_define_class(plugin.state, "Sprite", plugin.state->object_class);
  mrb_undef_class_method(plugin.state, Sprite, "new");
  mrb_define_class_method(plugin.state, Sprite, "[]", sprite_from_js, MRB_ARGS_REQ(1));
  mrb_define_method(plugin.state, Sprite, "initialize", sprite_init, MRB_ARGS_REQ(1));
  mrb_define_method(plugin.state, Sprite, "x", sprite_get_x, MRB_ARGS_NONE());
  mrb_define_method(plugin.state, Sprite, "y", sprite_get_y, MRB_ARGS_NONE());
  mrb_define_method(plugin.state, Sprite, "x=", sprite_set_x, MRB_ARGS_REQ(1));
  mrb_define_method(plugin.state, Sprite, "y=", sprite_set_y, MRB_ARGS_REQ(1));
}

EMSCRIPTEN_KEEPALIVE
void plugin_destroy() {
  mrb_close(plugin.state);
}

EMSCRIPTEN_KEEPALIVE
void plugin_update(double deltaTime) {
  mrb_funcall(plugin.state, mrb_obj_value(Game), "update", 1, mrb_float_value(plugin.state, deltaTime));
}

EMSCRIPTEN_KEEPALIVE
void plugin_execute(char* code) {
  mrb_load_string(plugin.state, code);
}
