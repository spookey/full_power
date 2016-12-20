from SCons.Script import DefaultEnvironment
env = DefaultEnvironment()
env.Execute('$PROJECT_DIR/pre.sh -f')
