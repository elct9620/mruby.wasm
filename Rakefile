# frozen_string_literal: true

# Prepare Build environment
PROJECT_ROOT = File.dirname(File.expand_path(__FILE__))
MRUBY_WASM_CONFIG = "#{PROJECT_ROOT}/mruby_wasm_config.rb"
EMSCRIPTEN_TOOLS = "#{ENV['EMSDK']}/fastcomp/emscripten/tools"
SOURCES = FileList['src/**/*.cpp'].exclude('**/glue.*')
BINARY_CODES = SOURCES.ext('bc')
LIBMRUBY_WASM_PATH = "#{PROJECT_ROOT}/mruby/build/wasm/lib/libmruby.bc"
OBJECTS = %w[libmruby glue] + BINARY_CODES
MINIFY = ENV['MINIFY'] && ENV['MINIFY'] != ''

ENV['MRUBY_CONFIG'] = MRUBY_WASM_CONFIG

# Helper

def create(format = :wasm)
  sources = [LIBMRUBY_WASM_PATH] + BINARY_CODES
  optimize = MINIFY ? '-Oz' : ''
  sh "emcc #{sources.join(' ')} " \
     '--post-js src/glue.js --post-js src/mruby.js ' \
     "#{optimize} -o mruby.#{format}"
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
task 'glue' do
  sh "python #{EMSCRIPTEN_TOOLS}/webidl_binder.py mruby.idl src/glue"
end

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
  task(:glue) { FileUtils.rm_rf(['**/glue.*']) }
  task(:idl) { FileUtils.rm_rf(['WebIDLGrammar.pkl', 'parser.out']) }
  task(:dist) { FileUtils.rm_rf(['mruby.wasm', 'mruby.html', 'mruby.js']) }
end

desc 'Clean compiled files'
task clean: %i[binary glue idl dist]
