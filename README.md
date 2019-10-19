mruby.wasm
===

Binding mruby to WebAssembly use WebIDL

## Requirements

* Emscripten SDK
* Ruby
* Rake
* Python

## Compile

To compile project, please make sure your `emsdk` is already configured with `$EMSDK` environment and `emcc` command.

### libmruby

Compile mruby and generate `libmruby.bc`

```
rake libmruby
```

### WebIDL Glue

Generate `glue.cpp` and `glue.js` for WebIDL

```
rake mruby.idl
```

### mruby.wasm

Generate mruby's WebAssembly file

```
rake mruby.wasm
```

> This is the default task

If you want to minify for production, please add `MINIFY` environment

```
MINIFY=1 rake mruby.wasm
```

## References

* [Emscripten](https://emscripten.org/)
* [mrubyをブラウザで実行するまで (WebAssembly)](https://qiita.com/noontage/items/da846e78eebd1d3b2a78)
* [Creating Javascript bindings for C/C++ libraries with Emscripten](https://adamrehn.com/articles/creating-javascript-bindings-for-c-cxx-libraries-with-emscripten/)
