#!/bin/bash

SRC=`pwd`
# 本地repo的路径
LOCAL_REPOS=("$HOME/Wiki" \
             "$HOME/.oh-my-zsh" \
             "$HOME/study" \
             "$HOME/tools" \
             "$HOME/workspace/adobe_demo_src/ErengineDemo" \
             "/var/ftp/mySITE/site/Wiki/学习笔记_html" \
             )
# git命令行
GIT_STATUS="git status"
GIT_PULL="git pull"
GIT_PUSH="git push"
GIT_ADD_ALL="git add -A"
GIT_COMMIT_FORCE="git commit -m"
# 未执行的repo
LEFT_TO_DO=()
# 总仓库大小
REPO_TOTAL_SIZE=0
# pull or push
CMD=$1
CMD_ARG=$2

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
    local force=$2
    cd $repo

    local line=`$GIT_STATUS | grep "nothing to commit"`
    echo "Push repo in " `pwd` " ..."

    if [[ $line != "" ]]; then
        $GIT_PUSH
        check_error $repo
    elif [[ $force != "" ]]; then
        $GIT_ADD_ALL
        $GIT_COMMIT_FORCE "Force commit by sync_github.sh"
        check_error $repo
    else
        echo "Please check something to commit"
        add_to_left $repo
    fi
    cd $SRC

    echo "--------------------"
    echo ""
}

function git_status()
{
    local repo=$1
    cd $repo

    local line=`$GIT_STATUS | grep "nothing to commit"`

    if [[ $line == "" ]]; then
        add_to_left $repo
    fi
    cd $SRC
}

function du_repo()
{
    local repo=$1
    cd $repo

    local size=`du . -s | awk '{print $1;}'`
    check_error $repo

    echo Size: " $size "K" "-Repo: " $repo " 

    let REPO_TOTAL_SIZE=$REPO_TOTAL_SIZE+$size
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
    echo "sync_github.sh [pull] [push] [list] [status]"
    echo "               list..[|ls]    --- show repos index"
    echo "               status..[|st]  --- show what is modified"
    echo "               pull [index]   --- pull repo for index"
    echo "               push [index|-f]   --- push repo for index"
}

function main()
{
    local len=${#LOCAL_REPOS[@]}
    local i=0

    if [[ $CMD == "pull" ]]; then
        if [[ $CMD_ARG == "" ]]; then
            i=0
            while [ $i -lt $len ]
            do
                git_pull ${LOCAL_REPOS[$i]}
                let i++
            done
        elif [[ $CMD_ARG -lt $len ]]; then
            git_pull ${LOCAL_REPOS[$CMD_ARG]}
        fi
    elif [[ $CMD == "push" ]]; then
        if [[ $CMD_ARG == "" ]]; then
            i=0
            while [ $i -lt $len ]
            do
                git_push ${LOCAL_REPOS[$i]}
                let i++
            done
        elif [[ $CMD_ARG == "-f" ]]; then
            i=0
            while [ $i -lt $len ]
            do
                git_push ${LOCAL_REPOS[$i]} -f
                let i++
            done
        elif [[ $CMD_ARG -lt $len ]]; then
            git_push ${LOCAL_REPOS[$CMD_ARG]}
        fi
    elif [[ $CMD == "list" ]] || [[ $CMD == "ls" ]]; then
        i=0
        while [ $i -lt $len ]
        do
            echo "$i: " ${LOCAL_REPOS[$i]}
            let i++
        done    
        exit 0;
    elif [[ $CMD == "status" ]] || [[ $CMD == "st" ]]; then
        echo "Scan Projects ..."
        if [[ $CMD_ARG == "" ]]; then
            i=0
            while [ $i -lt $len ]
            do
                git_status ${LOCAL_REPOS[$i]}
                let i++
            done    
        elif [[ $CMD_ARG -lt $len ]]; then
            cd ${LOCAL_REPOS[$CMD_ARG]}
            $GIT_STATUS
            cd $SRC

            exit 0;
        fi
    elif [[ $CMD == "du" ]]; then
        echo "Calculate Size ..."
        echo "--------------------"

        i=0
        while [ $i -lt $len ]
        do
            du_repo ${LOCAL_REPOS[$i]}
            let i++
        done

        echo "--------------------"
        
        let total_size=$REPO_TOTAL_SIZE/1000
        echo "Total size: " $total_size "M"
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
