project(
  'microvidya',
  ['cpp', 'c'],
  version: '0.1',
  license: 'mit',
  default_options: ['warning_level=3', 'cpp_std=c++17'],
)

cc = meson.get_compiler('c')
fs = import('fs')
l_args = []

imgui_project = subproject('imgui')
soloud_project = subproject('soloud')
pxtone_project = subproject('libpxtone')
sol_project = subproject('sol')

deps = [
  dependency('sdl2'),
  dependency('SDL2_image'),
  dependency('glm'),
  # dependency('ogg'),
  dependency('lua'),
  imgui_project.get_variable('lib_imgui_dep'),
  soloud_project.get_variable('lib_soloud_dep'),
  pxtone_project.get_variable('lib_pxtone_dep'),
  sol_project.get_variable('sol2_dep'),
]

if get_option('use_pure_gles2')
  if target_machine.system() != 'windows'
    deps += dependency('glesv2')
  else
    l_args += '-L' + meson.global_source_root() + '/lib'
    l_args += '-llibSDL2'
    l_args += '-llibGLESv2'
    l_args += '-llibEGL'
  endif
  add_project_arguments('-DMV_USE_PURE_GLES2', language: ['cpp', 'c'])
else
  deps += dependency('epoxy')
  l_args += '-L' + meson.global_source_root() + '/lib'
  # l_args += '-llibSDL2'
  l_args += '-llibGLESv2'
  # l_args += '-llibEGL'
endif

include_dirs = include_directories('include')
includes = [include_dirs]

includes += imgui_project.get_variable('imgui_inc')
includes += soloud_project.get_variable('soloud_inc')
includes += pxtone_project.get_variable('pxtone_inc')

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

  'src/engine/objects/node.cpp',
  'src/engine/objects/sprite.cpp',

  'src/engine/resource/texture.cpp',

  'src/engine/context.cpp',

  'src/microvidya.cpp',
]

exe = executable(
  'game',
  sources,
  dependencies: deps,
  include_directories: includes,
  install: true,
  install_dir: './',
  link_args: l_args,
)

install_subdir('assets', install_dir: './')

test('basic', exe)