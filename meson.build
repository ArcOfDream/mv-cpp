project(
  'microvidya',
  ['cpp', 'c'],
  version: '0.1',
  license: 'mit',
  default_options: ['warning_level=2', 'cpp_std=c++17'],
)

cc = meson.get_compiler('c')
l_args = []

imgui_project = subproject('imgui')
soloud_project = subproject('soloud')
pxtone_project = subproject('libpxtone')

include_dirs = include_directories('include')
includes = [include_dirs]
includes += imgui_project.get_variable('imgui_inc')
includes += soloud_project.get_variable('soloud_inc')
includes += pxtone_project.get_variable('pxtone_inc')

deps = [
  imgui_project.get_variable('lib_imgui_dep'),
  soloud_project.get_variable('lib_soloud_dep'),
  pxtone_project.get_variable('lib_pxtone_dep'),
  dependency('glm'),
  dependency('threads'),
  dependency('wren'),
]

if target_machine.system() not in ['emscripten']
  deps += dependency('SDL2_image')
endif

if target_machine.system() not in ['windows', 'emscripten']
  deps += dependency('glesv2')
else
  if target_machine.system() in ['windows']
    l_args += '-L' + meson.global_source_root() + '/lib'
    l_args += '-llibGLESv2'
    l_args += '-llibEGL'
  endif
endif

if get_option('use_freetype')
  add_project_arguments('-DMV_USE_FREETYPE', language: ['cpp', 'c'])
  deps += dependency('freetype2')
endif

if get_option('single_thread')
  add_project_arguments('-DMV_SINGLE_THREAD', language: ['cpp', 'c'])
endif

sources = [
  'src/engine/audio/pxtone_source.cpp',

  'src/engine/graphics/indexbuffer.cpp',
  # 'src/engine/vertexarray.cpp',
  'src/engine/graphics/vertexbuffer.cpp',
  'src/engine/graphics/gl_debug.cpp',
  'src/engine/graphics/quad.cpp',
  'src/engine/graphics/shader.cpp',
  'src/engine/graphics/camera2d.cpp',
  'src/engine/graphics/renderer.cpp',
  'src/engine/graphics/fontrender.cpp',

  'src/engine/objects/node.cpp',
  'src/engine/objects/sprite.cpp',

  'src/engine/resource/texture.cpp',
  'src/engine/resource/material.cpp',

  'src/engine/scripting/register_types.cpp',

  'src/engine/context.cpp',

  'src/microvidya.cpp',
]

if target_machine.system() in ['emscripten']
  exe = executable(
    'index',
    sources,
    dependencies: deps,
    include_directories: includes,
    install: true,
    install_dir: './',
    link_args: l_args,
    name_suffix: 'html',
  )
else
  exe = executable(
    'game',
    sources,
    dependencies: deps,
    include_directories: includes,
    install: true,
    install_dir: './',
    link_args: l_args,
  )
endif

install_subdir('assets', install_dir: './')

test('basic', exe)