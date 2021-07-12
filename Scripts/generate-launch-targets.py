#!/usr/bin/env python3

import os
import sys
import json
import pathlib

isWindows = (os.name == 'nt')

executable = sys.argv[1]    # Path to executable
sourceDir = sys.argv[2]     # CMAKE_CURRENT_SOURCE_DIR
assetPath = sys.argv[3]     # Asset search path, separated by ';'
runtimePath = sys.argv[4]   # Runtime search path, separated by ';'

name = pathlib.Path(executable).stem

assetPath = assetPath.replace(';', os.pathsep)
runtimePath = runtimePath.replace(';', os.pathsep)

def add_or_update_config(data, configurations):
    found = False
    for i in range(0, len(configurations)):
        if configurations[i]['name'] == data['name']:
            configurations[i] = data
            found = True
            break
    
    if not found:
        configurations.append(data)

if os.path.isdir('.vscode'):
    filename = '.vscode/launch.json'

    launch = {}
    try:
        file = open(filename, 'r')
        launch = json.load(file)
    except:
        pass

    if 'version' not in launch:
        launch['version'] = '0.2.0'

    if 'configurations' not in launch:
        launch['configurations'] = []

    data = {
        'name': name,
        'type': 'cppdbg',
        'request': 'launch',
        'program': executable,
        'args': [],
        'cwd': sourceDir,
        'environment': [
            {
                'name': 'ASSET_PATH',
                'value': assetPath
            }
        ],
        'console': 'internalConsole'
    }
    
    if isWindows:
        data['type'] = 'cppvsdbg'
        data['environment'].append({
            'name': 'PATH',
            'value': '${env:PATH};' + runtimePath
        })
    else:
        data['environment'].append({
            'name': 'LD_LIBRARY_PATH',
            'value': runtimePath
        })

    add_or_update_config(data, launch['configurations'])

    file = open(filename, 'w')
    json.dump(launch, file, indent=4)

# if isWindows and os.path.isdir('.vs'):
#     filename = '.vs/launch.vs.json'

#     launch = {}
#     try:
#         file = open(filename, 'r')
#         launch = json.load(file)
#     except:
#         pass

#     if 'version' not in launch:
#         launch['version'] = '0.2.1'

#     if 'configurations' not in launch:
#         launch['configurations'] = []

#     default = {
#         'name': '',
#         'type': 'default', # dll
#         # 'exe': executable,
#         'project': 'CMakeLists.txt',
#         'projectTarget': '{} ({})'.format(os.path.basename(executable), executable),
#         'cwd': projectDirectory,
#         'env': {
#             'PATH': '${env.PATH};' + runtimePath,
#             'DUSK_ASSET_PATH': assetPath,
#         },
#     }

#     if 'configurations' in project:
#         for name,config in project['configurations'].items():
#             data = copy.deepcopy(default)
#             data['name'] = "%s (%s)" % (project['name'], name)
#             data['args'] = [ duskproj, '-c', name ]

#             add_or_update_config(data, launch['configurations'])
#     else:
#         data = copy.copy(default)
#         data['name'] = project['name']
#         data['args'] = [ duskproj ]

#         add_or_update_config(data, launch['configurations'])

#     file = open(filename, 'w')
#     json.dump(launch, file, indent=4)
