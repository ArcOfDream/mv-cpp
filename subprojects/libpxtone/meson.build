project(
  'libpxtone',
  ['c', 'cpp'],
  default_options: ['default_library=static', 'warning_level=1', 'cpp_std=c++11'],
)

add_project_arguments('-DpxINCLUDE_OGGVORBIS', language: ['cpp', 'c'])
add_project_arguments('-DpxINCLUDE_OGGVORBIS_STB', language: ['cpp', 'c'])

pxtone_inc = include_directories('./src', './stb_vorbis')

deps = [
  dependency('ogg'),
]

sources = [
  'src/pxtnData.cpp',
  'src/pxtnDelay.cpp',
  'src/pxtnError.cpp',
  'src/pxtnEvelist.cpp',
  'src/pxtnMaster.cpp',
  'src/pxtnMem.cpp',
  'src/pxtnOverDrive.cpp',
  'src/pxtnPulse_Frequency.cpp',
  'src/pxtnPulse_Noise.cpp',
  'src/pxtnPulse_NoiseBuilder.cpp',
  'src/pxtnPulse_Oggv.cpp',
  'src/pxtnPulse_Oscillator.cpp',
  'src/pxtnPulse_PCM.cpp',
  'src/pxtnService.cpp',
  'src/pxtnService_moo.cpp',
  'src/pxtnText.cpp',
  'src/pxtnUnit.cpp',
  'src/pxtnWoice.cpp',
  'src/pxtnWoice_io.cpp',
  'src/pxtnWoicePTV.cpp',
  'src/pxtoneNoise.cpp',
]

lib_pxtone = library('libpxtone', sources, include_directories: pxtone_inc, dependencies: deps)
lib_pxtone_dep = declare_dependency(link_with: lib_pxtone, include_directories: pxtone_inc)
meson.override_dependency('pxtone', lib_pxtone_dep)