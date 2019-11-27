# frozen_string_literal: true

# Prepare Build environment
PROJECT_ROOT = File.dirname(File.expand_path(__FILE__))
MRUBY_WASM_CONFIG = "#{PROJECT_ROOT}/mruby_wasm_config.rb"
EMSCRIPTEN_PATH = [
  ENV['EMSCRIPTEN'], # CI Environment
  "#{ENV['EMSDK']}/upstream/emscripten"
].compact.find { |path| path != '' }
EMSCRIPTEN_TOOLS = "#{EMSCRIPTEN_PATH}/tools"
SOURCES = FileList['src/**/*.c']
BINARY_CODES = SOURCES.ext('bc')
LIBMRUBY_WASM_PATH = "#{PROJECT_ROOT}/mruby/build/wasm/lib/libmruby.bc"
OBJECTS = %w[libmruby] + BINARY_CODES
MINIFY = ENV['MINIFY'] && ENV['MINIFY'] != ''
ASSERTIONS = ENV['ASSERTIONS'] && ENV['ASSERTIONS'] != ''

ENV['MRUBY_CONFIG'] = MRUBY_WASM_CONFIG

# Helper

def create(format = :wasm)
  sources = [LIBMRUBY_WASM_PATH] + BINARY_CODES
  optimize = MINIFY ? '-Oz' : ''
  assert = ASSERTIONS ? '-s ASSERTIONS=1' : ''
  sh "emcc #{sources.join(' ')} " \
     '--js-library src/js/library.js ' \
     '--pre-js src/js/object.js ' \
     '--post-js src/js/mruby.js ' \
     '--shell-file template/playground.html ' \
     "#{optimize} #{assert} -o mruby.#{format}"
end

# Load mruby tasks
begin
  namespace :mruby do
    load './mruby/Rakefile'
  end
rescue LoadError
  sh 'git submodule init'
  sh 'git submodule update --init --recursive'
  retry
end

# Build tasks
task default: :html
task 'libmruby' => 'mruby:all'

rule '.bc' => SOURCES do |task|
  sh "emcc -I mruby/include -I include -c #{task.source} -o #{task.name}"
end

%i[wasm html js].each do |format|
  task "mruby.#{format}" => OBJECTS do
    create(format)
  end

  desc "Compile sources to #{format} WebAssembly"
  task format => "mruby.#{format}"
end

namespace :clean do
  task(:binary) { FileUtils.rm_rf(BINARY_CODES) }
  task(:dist) { FileUtils.rm_rf(['mruby.wasm', 'mruby.html', 'mruby.js']) }
end

desc 'Clean compiled files'
task clean: %i[clean:binary clean:dist]
