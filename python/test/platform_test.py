import platform

print platform.platform()
print platform.machine()
print platform.architecture()
print platform.processor()
print platform.node()
print platform.python_version()
print platform.python_compiler()
print platform.python_version_tuple()
print platform.release()
print platform.system()
print platform.popen('ls').read()