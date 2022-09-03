#!/bin/bash

SCRIPT_DIR=$(cd $(dirname $0); pwd)

# Usage:
#  get_submits.sh [id]
#
#  get_submits.sh
#    標準入出力に数値を入力すると対応する submit ID の情報を取ってid.jsonに保存する
#    大量に仕入れたいときに便利
#      $ cat ids.txt
#      123
#      1234
#      $ cat ids.txt | get_submits.sh
#      $ ls
#      123.json  1234.json
#
#  get_submits.sh id
#    指定した id の submit 情報を id.json に保存する
#      $ get_submits.sh 314
#      $ ls
#      314.json

function get_submit () {
  id=$1
  ${SCRIPT_DIR}/rest_api.sh record ${id} > ${id}.json
  echo "Saved problem ${id} into ${id}.json"
}

if [ $# -eq 0 ] ; then
  while read id ; do
    get_submit $id
  done
else
  id=$1
  get_submit $id 
fi
