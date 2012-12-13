TARGET=1
PACKAGE=org.example.test
ACTIVITY=MyTest
PROJECT_NAME=Test
PROJECT_PATH=.

#default command
all: install
clean: remove uninstall

#operation command
debug:
	ant debug
release:
	ant release
remove:
	ant clean
install:
	ant install
uninstall:
	ant uninstall
tools:
	emulator -avd AVD1 &
	ddms &
update:
	android update project -p .
project:
	android create project -t $(TARGET) -k $(PACKAGE) -a $(ACTIVITY) -n $(PROJECT_NAME) -p $(PROJECT_PATH)
	vim entry.vimentry

#show help
help:
	@echo "usage:"
	@echo "\tmake --- same as install"
	@echo "\tmake project --- create a android project"
	@echo "\tmake install --- build and install apk"
	@echo "\tmake uninstall --- remove apk"
	@echo "\tmake tools --- run emulator and ddms"
	@echo "\tmake update --- update android project"
	@echo "\tmake clean --- same as uninstall"
