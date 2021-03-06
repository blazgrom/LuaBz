#! /bin/bash
# Bash Script that builds project

RED='\033[0;31m' # Red
BB='\033[0;34m'  # Blue
NC='\033[0m' # No Color
BG='\033[0;32m' # Green

error() { >&2 echo -e "${RED}$1${NC}"; }
showinfo() { echo -e "${BG}$1${NC}"; }
workingprocess() { echo -e "${BB}$1${NC}"; }
allert () { echo -e "${RED}$1${NC}"; }
function run()
{
    workingprocess "Running test"
    ./$1
    if [ $? -eq 0 ]; then
    workingprocess "All tests compile and pass."
    else
        COREFILE=$(find . -maxdepth 1 -name "core*" | head -n 1) # find core file
        if [[ -f "$COREFILE" ]]; then gdb -c "$COREFILE" ./$1 -ex "thread apply all bt" -ex "set pagination 0" -batch; fi
        exit -1;
    fi
}
# Building project
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_CLANG_TIDY=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON  -DENABLE_CODE_COVERAGE=ON ..
make -j8
# Checks if last comand didn't output 0
# $? checks what last command outputed
# If output is 0 then command is succesfuly executed
# If command fails it outputs number between 0 to 255
if [ $? -ne 0 ]; then
    error "Error: there are compile errors!"
	# Terminate script and outputs 3
    exit 3
fi
cd ../bin
run_tests luabz_tests
run luabz

#Uncomment if you want to run cppcheck
#workingprocess "Running cppcheck"
#cppcheck --version
#cd ..
#cppcheck  --suppress=missingIncludeSystem --force --enable=all --std=c++11 --language=c++ -i gtest --quiet --error-exitcode=1  .

workingprocess "Running clang-tidy"
cd ../build
make clang-tidy > output.txt
if [[ -n $(grep "error: " output.txt) ]]; then
     echo "clang-tidy failed"
     echo ""
     grep --color -E '^|warning: |error: ' output.txt
     exit 1
 else
     grep --color -E '^|warning: ' output.txt
     echo -e "\033[1;32m\xE2\x9C\x93 passed:\033[0m $1";
     exit 0
fi