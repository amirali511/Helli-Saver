project('hellisaver', 'c',
        version: '1.0',
        default_options: ['warning_level=everything'])
xcb = dependency('xcb')
xcb_keysyms = dependency('xcb-keysyms')
X11 = dependency('x11')

executable('hellisaver',
           'hellisaver.c',
           dependencies: [xcb, xcb_keysyms, X11],
           install: true)