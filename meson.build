project('bunnymark', 'c',
    default_options: 'default_library=static')

sdl2_dep = dependency('sdl2')
sdl2_image_dep = dependency('SDL2_image')
sdl2_ttf_dep = dependency('SDL2_ttf')
sdl2_mixer_dep = dependency('SDL2_mixer')
# sdl2_net_dep = dependency('sdl2_net')

executable('bunnymark', 'main.c',
    dependencies: [sdl2_dep, sdl2_image_dep, sdl2_ttf_dep, sdl2_mixer_dep])
