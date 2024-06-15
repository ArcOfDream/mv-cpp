project(
  'microvidya',
  ['cpp', 'c'],
  version: '0.1',
  license: 'mit',
  default_options: ['warning_level=3', 'cpp_std=c++17'],
)

cc = meson.get_compiler('c')
fs = import('fs')

imgui_project = subproject('imgui')
soloud_project = subproject('soloud')
pxtone_project = subproject('libpxtone')
flecs_project = subproject('flecs')
sol_project = subproject('sol')

deps = [
  dependency('sdl2'),
  dependency('SDL2_image'),
  dependency('epoxy'),
  dependency('glm', required: false),
  dependency('ogg'),
  dependency('lua'),
  imgui_project.get_variable('lib_imgui_dep'),
  soloud_project.get_variable('lib_soloud_dep'),
  pxtone_project.get_variable('lib_pxtone_dep'),
  flecs_project.get_variable('flecs_dep'),
  sol_project.get_variable('sol2_dep'),
]

include_dirs = include_directories(
  [
    'include',
  ],
)
include_imgui = imgui_project.get_variable('imgui_inc')
include_soloud = soloud_project.get_variable('soloud_inc')
include_pxtone = pxtone_project.get_variable('pxtone_inc')
include_flecs = flecs_project.get_variable('flecs_inc')

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

  'src/engine/resource/texture.cpp',

  'src/engine/context.cpp',

  'src/microvidya.cpp',
]

exe = executable(
  'game',
  sources,
  dependencies: deps,
  include_directories: [include_dirs, include_imgui, include_soloud, include_pxtone, include_flecs],
  install: true,
  install_dir:'./'
)

install_subdir('assets', install_dir: './')

test('basic', exe)