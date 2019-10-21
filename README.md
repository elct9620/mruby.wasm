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

> Please make sure you are use `LLVM` version instead Fastcomp version.
> The `LLVM` version can install by use `emsdk install latest-upstream`.

### libmruby

Compile mruby and generate `libmruby.bc`

```bash
rake libmruby
```

### WebIDL Glue

Generate `glue.cpp` and `glue.js` for WebIDL

```bash
rake mruby.idl
```

### WebAssemtly

Generate mruby's WebAssembly file

```bash
rake wasm
rake js
rake html
```

> The `rake html` is default task, you can specify which output you wants

If you want to minify for production, please add `MINIFY` environment

```bash
MINIFY=1 rake html
```

## Usage

### Local PlayGround

Prepare WebAssembly

```bash
# Create mruby.html
rake html

# Serve mruby.wasm
ruby -run -e httpd .
```

Goto [http://localhost:8080/mruby.html](http://localhost:8080/mruby.html) and open the console to execute ruby code.

```js
var ruby = new Ruby()
ruby.Execute('puts "Hello World"')
```

### Production

Prepare WebAssembly

```
rake js
```

Copy `mruby.js` and `mruby.wasm` to your project and load WebAssembly by yourself.

> Currently this project is for prototype to test use mruby on browser, it is not stable!

## References

* [Emscripten](https://emscripten.org/)
* [mrubyをブラウザで実行するまで (WebAssembly)](https://qiita.com/noontage/items/da846e78eebd1d3b2a78)
* [Creating Javascript bindings for C/C++ libraries with Emscripten](https://adamrehn.com/articles/creating-javascript-bindings-for-c-cxx-libraries-with-emscripten/)
