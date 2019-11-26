#include<mruby.h>
#include<mruby/compile.h>
#include<mruby/string.h>
#include <mruby/variable.h>
#include<emscripten.h>

typedef struct {
  mrb_state* state;
} Plugin;

static Plugin plugin;
struct RClass* Game;

mrb_value game_update(mrb_state* mrb, mrb_value self) {
  mrb_value block;
  mrb_value callback;

  mrb_get_args(mrb, "&", &block);

  if (mrb_nil_p(block)) {
    // Execute Case
    callback = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "_update"));
    if (!mrb_nil_p(callback)) {
      mrb_yield_argv(mrb, callback, 0, NULL);
    }
  } else {
    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "_update"), block);
  }

  return mrb_nil_value();
}

EMSCRIPTEN_KEEPALIVE
void plugin_init() {
  plugin.state = mrb_open();

  // Game Module
  Game = mrb_define_module(plugin.state, "Game");
  mrb_define_class_method(plugin.state, Game, "update", game_update, MRB_ARGS_BLOCK());
}

EMSCRIPTEN_KEEPALIVE
void plugin_destroy() {
  mrb_close(plugin.state);
}

EMSCRIPTEN_KEEPALIVE
void plugin_update() {
  mrb_funcall(plugin.state, mrb_obj_value(Game), "update", 0);
}

EMSCRIPTEN_KEEPALIVE
void plugin_execute(char* code) {
  mrb_load_string(plugin.state, code);
}
