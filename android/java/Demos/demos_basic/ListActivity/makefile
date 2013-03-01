TARGET=1
PACKAGE=org.example.listactivity
ACTIVITY=MyListTest
PROJECT_NAME=ListActivityTest
PROJECT_PATH=.

#default command
all: install
clean: uninstall

#operation command
debug:
	ant debug
release:
	ant release
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
	@echo "\tmake install --- build and install apk"
	@echo "\tmake uninstall --- remove apk"
	@echo "\tmake tools --- run emulator and ddms"
	@echo "\tmake update --- update android project"
	@echo "\tmake clean --- same as uninstall"

