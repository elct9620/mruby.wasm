# frozen_string_literal: true

# Prepare Build environment
PROJECT_ROOT = File.dirname(File.expand_path(__FILE__))
MRUBY_WASM_CONFIG = "#{PROJECT_ROOT}/mruby_wasm_config.rb"
EMSCRIPTEN_TOOLS = "#{ENV['EMSDK']}/fastcomp/emscripten/tools"
SOURCES = FileList['src/**/*.cpp'].exclude('**/glue.*')
BINARY_CODES = SOURCES.ext('bc')
LIBMRUBY_WASM_PATH = "#{PROJECT_ROOT}/mruby/build/wasm/lib/libmruby.bc"
MINIFY = ENV['MINIFY'] && ENV['MINIFY'] != ''

ENV['MRUBY_CONFIG'] = MRUBY_WASM_CONFIG

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
task default: 'mruby.wasm'
task 'libmruby' => 'mruby:all'
task 'glue' do
  sh "python #{EMSCRIPTEN_TOOLS}/webidl_binder.py mruby.idl src/glue"
end

rule '.bc' => SOURCES do |task|
  sh "emcc -I mruby/include -I include -c #{task.source} -o #{task.name}"
end

task 'mruby.wasm' => %w[libmruby glue] + BINARY_CODES do
  sources = [LIBMRUBY_WASM_PATH] + BINARY_CODES
  optimize = MINIFY ? '-Oz' : ''
  sh "emcc #{sources.join(' ')} " \
     '--post-js src/glue.js --post-js src/mruby.js ' \
     "#{optimize} -o mruby.wasm"
end

task 'mruby.html' => %w[libmruby glue] + BINARY_CODES do
  sources = [LIBMRUBY_WASM_PATH] + BINARY_CODES
  sh "emcc #{sources.join(' ')} " \
     '--post-js src/glue.js --post-js src/mruby.js -o mruby.html'
end

task :clean do
  FileUtils.rm_rf(BINARY_CODES)
  FileUtils.rm_rf(['**/glue.*'])
  FileUtils.rm_rf(['WebIDLGrammar.pkl', 'parser.out'])
  FileUtils.rm_rf(['mruby.wasm', 'mruby.html', 'mruby.js'])
end
