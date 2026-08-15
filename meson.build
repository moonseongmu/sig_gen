project('sig_gen', 'c', meson_version: '>=1.1', default_options: ['c_std=c23'])

src = []
inc = []

c_args = ['-Wall', '-Wextra']

map_file_arg = '-Wl,-Map=' + meson.project_name() + '.map'
c_link_args = [map_file_arg]

subdir('sig_gen_core')
subdir('interface')
subdir('CMSIS')
subdir('external')
subdir('boards')

#build executable
executable(
    meson.project_name(),
    src,
    include_directories: inc,
    name_suffix: 'elf',
    c_args: c_args,
    link_args: c_link_args,
)

elf_file = '@BUILD_ROOT@/' + meson.project_name() + '.elf'
pyocd_config = '--config=' + '@SOURCE_ROOT@' + '/pyocd.yaml'
run_target('flash', command: ['pyocd', 'flash', elf_file, pyocd_config])