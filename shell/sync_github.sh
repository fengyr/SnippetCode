#!/bin/bash

SRC=`pwd`
# 本地repo的路径
LOCAL_REPOS=("$HOME/Wiki" \
             "$HOME/.oh-my-zsh" \
             "$HOME/study" \
             "$HOME/workspace/adobe_demo_src/ErengineDemo" \
             "/var/ftp/mySITE/site/Wiki/学习笔记_html" \
             )
# git命令行
GIT_STATUS="git status"
GIT_PULL="git pull"
GIT_PUSH="git push"
# 未执行的repo
LEFT_TO_DO=()
# pull or push
ARG=$1

function git_pull()
{
    echo "--------------------"

    local repo=$1
    cd $repo

    # local line=`$GIT_STATUS | grep "nothing to commit"`
    # echo "Pull repo in " `pwd` " ..."

    # if [[ $line != "" ]]; then
        # $GIT_PULL
        # check_error $repo
    # else
        # echo "Please check something to commit"
        # add_to_left $repo
    # fi

    echo "Pull repo in " `pwd` " ..."
    $GIT_PULL
    check_error $repo

    cd $SRC

    echo "--------------------"
    echo ""
}

function git_push()
{
    echo "--------------------"

    local repo=$1
    cd $repo

    local line=`$GIT_STATUS | grep "nothing to commit"`
    echo "Push repo in " `pwd` " ..."

    if [[ $line != "" ]]; then
        $GIT_PUSH
        check_error $repo
    else
        echo "Please check something to commit"
        add_to_left $repo
    fi
    cd $SRC

    echo "--------------------"
    echo ""
}

function add_to_left()
{
    local len=${#LEFT_TO_DO[@]}
    LEFT_TO_DO[$len]=$1
}

function check_error()
{
    if [[ $? != 0 ]]; then
        echo "Handle " $1 " error!"
        exit 2;
    fi
}

function usage()
{
    echo "sync_github.sh [pull] [push]"
}

function main()
{
    local len=${#LOCAL_REPOS[@]}
    local i=0

    if [[ $ARG == "pull" ]]; then
        i=0
        while [ $i -lt $len ]
        do
            git_pull ${LOCAL_REPOS[$i]}
            let i++
        done
    elif [[ $ARG == "push" ]]; then
        i=0
        while [ $i -lt $len ]
        do
            git_push ${LOCAL_REPOS[$i]}
            let i++
        done
    else
        usage
        exit 1;
    fi

    len=${#LEFT_TO_DO[@]}
    if [[ $len -gt 0 ]]; then
        echo "===================="
        i=0
        while [ $i -lt $len ]
        do
            echo "Left to do: " ${LEFT_TO_DO[$i]}
            let i++
        done
        echo "===================="
    fi
}

main
