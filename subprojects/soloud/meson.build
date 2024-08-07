project('SoLoud', ['c', 'cpp'], 
  default_options: ['default_library=static', 'warning_level=1'])
# add_project_arguments('-DWITH_SDL2_STATIC', language : 'cpp')
# add_project_arguments('-DWITH_MINIAUDIO', language : 'cpp')
# add_project_arguments('-DWITH_ALSA', language : 'cpp')
# add_project_arguments('-DWITH_NULL', language : 'cpp')
# add_project_arguments('-DWITH_NOSOUND', language : 'cpp')

soloud_inc = include_directories('./include')

deps = [
  dependency('sdl2'),
  dependency('threads'),
]

sources = [
# backends, including universally compileable ones here
'src/backend/null/soloud_null.cpp',

'src/backend/nosound/soloud_nosound.cpp',

#  'src/backend/sdl2_static/soloud_sdl2_static.cpp',

# audiosource
'src/audiosource/ay/chipplayer.cpp',
'src/audiosource/ay/sndbuffer.cpp',
'src/audiosource/ay/sndchip.cpp',
'src/audiosource/ay/sndrender.cpp',
'src/audiosource/ay/soloud_ay.cpp',

'src/audiosource/monotone/soloud_monotone.cpp',

'src/audiosource/noise/soloud_noise.cpp',

'src/audiosource/sfxr/soloud_sfxr.cpp',

'src/audiosource/speech/darray.cpp',
'src/audiosource/speech/klatt.cpp',
'src/audiosource/speech/resonator.cpp',
'src/audiosource/speech/tts.cpp',
'src/audiosource/speech/soloud_speech.cpp',

'src/audiosource/tedsid/sid.cpp',
'src/audiosource/tedsid/ted.cpp',
'src/audiosource/tedsid/soloud_tedsid.cpp',

'src/audiosource/vic/soloud_vic.cpp',

'src/audiosource/vizsn/soloud_vizsn.cpp',

'src/audiosource/wav/dr_impl.cpp',
'src/audiosource/wav/stb_vorbis.c',
'src/audiosource/wav/soloud_wav.cpp',
'src/audiosource/wav/soloud_wavstream.cpp',

# filter
'src/filter/soloud_bassboostfilter.cpp',
'src/filter/soloud_biquadresonantfilter.cpp',
'src/filter/soloud_dcremovalfilter.cpp',
'src/filter/soloud_duckfilter.cpp',
'src/filter/soloud_echofilter.cpp',
'src/filter/soloud_eqfilter.cpp',
'src/filter/soloud_fftfilter.cpp',
'src/filter/soloud_flangerfilter.cpp',
'src/filter/soloud_freeverbfilter.cpp',
'src/filter/soloud_lofifilter.cpp',
'src/filter/soloud_robotizefilter.cpp',
'src/filter/soloud_waveshaperfilter.cpp',

# core
'src/core/soloud_core_voiceops.cpp',
'src/core/soloud_core_voicegroup.cpp',
'src/core/soloud_core_setters.cpp',
'src/core/soloud_core_getters.cpp',
'src/core/soloud_core_filterops.cpp',
'src/core/soloud_core_faderops.cpp',
'src/core/soloud_core_basicops.cpp',
'src/core/soloud_thread.cpp',
'src/core/soloud_queue.cpp',
'src/core/soloud_misc.cpp',
'src/core/soloud_filter.cpp',
'src/core/soloud_file.cpp',
'src/core/soloud_fft_lut.cpp',
'src/core/soloud_fft.cpp',
'src/core/soloud_fader.cpp',
'src/core/soloud_core_3d.cpp',
'src/core/soloud_bus.cpp',
'src/core/soloud_audiosource.cpp',
'src/core/soloud.cpp',

]

if target_machine.system() not in ['windows', 'emscripten']
  # we will try to exclude windows and emscripten from including the SDL2 backend for now
  add_project_arguments('-DWITH_SDL2', language : ['cpp', 'c'])
  sources += 'src/backend/sdl/soloud_sdl2.cpp'
  sources += 'src/backend/sdl/soloud_sdl2_dll.c'

  deps += dependency('sdl2')
endif

# If there's a system that can't compile this, we'll exclude it from using miniaudio.
if target_machine.system() not in ['emscripten']
  add_project_arguments('-DWITH_MINIAUDIO', language : ['cpp', 'c'])
  sources += 'src/backend/miniaudio/soloud_miniaudio.cpp'
endif

if target_machine.system() in ['windows']
  add_project_arguments('-DWITH_WINMM', language : ['cpp', 'c'])
  sources += 'src/backend/winmm/soloud_winmm.cpp'
endif

if target_machine.system() in ['emscripten']
  add_project_arguments('-DWITH_SDL_STATIC', language : ['cpp', 'c'])
  sources += 'src/backend/sdl_static/soloud_sdl_static.cpp'
endif

if target_machine.system() in ['linux']
  add_project_arguments('-DWITH_ALSA', language : ['cpp', 'c'])
  sources += 'src/backend/alsa/soloud_alsa.cpp'

  deps += dependency('alsa')
endif

lib_soloud = library('SoLoud', sources, include_directories: soloud_inc, dependencies: deps)
lib_soloud_dep = declare_dependency(link_with: lib_soloud, include_directories: soloud_inc)
meson.override_dependency('soloud', lib_soloud_dep)