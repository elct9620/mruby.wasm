#include<mruby.h>
#include<mruby/compile.h>

class Ruby {
  private:
    mrb_state* state;
  public:
    Ruby() {
      this->state = mrb_open();
    }

    void Execute(char* code) {
      mrb_load_string(this->state, code);
    }

    ~Ruby() {
      mrb_close(this->state);
    }
};

#include "glue.cpp"
