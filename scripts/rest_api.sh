#!/bin/bash

BASE_DIR=$(cd $(dirname $0)/..; pwd)
API_HOST=https://robovinci.xyz/api
PROBLEM_HOST=https://cdn.robovinci.xyz/imageframes

if [ "${SANMA_TOKEN}" = "" ] ; then
  echo "Get API token fromm https://robovinci.xyz/dashboard and set it as SANME_TOKEN"
  echo "export SANMA_TOKEN=api_token_is_here"
  exit
fi

function access_api () {
  curl --header "Authorization: Bearer ${SANMA_TOKEN}" $@
}

function display_usage () {
  echo "Usage: ${0} [submit|initial|target|best|record] <other options>"
  exit
}

function get_problem_target () {
  if [ $# = 0 ] ; then
    echo "Usage: $0 target <problem_id>"
    echo "  Shows the target problem in JSON format"
    exit
  fi
  id=$1
  uri=${PROBLEM_HOST}/${id}.json
  curl ${uri}
}

function get_problem_init () {
  if [ $# = 0 ] ; then
    echo "Usage: $0 initial <problem_id>"
    echo "  Shows the initial state in JSON format"
    exit
  fi
  id=$1
  uri=${PROBLEM_HOST}/${id}.initial.json
  curl ${uri}
}

function best_submits () {
  access_api ${API_HOST}/results/user
}

function get_submission_record () {
  if [ $# = 0 ] ; then
    echo "Usage: $0 record <submission_id>"
    echo "  Shows the submission status"
    exit
  fi
  id=$1
  uri=${API_HOST}/submissions/${id}
  access_api $uri
}

function submit () {
  if [ $# -lt 2 ] ; then
    echo "Usage: $0 submit <problem_id> <ISL file>"
    echo "  Submit the ISL file to the problem"
    exit
  fi
  id=$1
  isl=$2
  uri=${API_HOST}/problems/${id}
  access_api -F file=@${isl} ${uri}
}

if [ $# = 0 ] ; then
  display_usage
fi

command=$1
case $command in
  "submit")
    submit $2 $3;;
  "target")
    get_problem_target $2 ;;
  "initial")
    get_problem_init $2 ;;
  "best")
    best_submits ;;
  "record")
    get_submission_record $2 ;;
  *)
    display_usage
esac