from SCons.Script import DefaultEnvironment

ENV = DefaultEnvironment()
ENV.Execute('$PROJECT_DIR/pre.sh -f')
