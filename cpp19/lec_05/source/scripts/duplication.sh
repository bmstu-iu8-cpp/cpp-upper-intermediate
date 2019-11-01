#!/bin/bash

set -e

jscpd --mode "weak" --format "cpp" --ignore "_builds/**" .

REPORT_DATA=$(jscpd --mode "weak" --format "cpp" --ignore "_builds/**" . | sed -r "s/[[:cntrl:]]\[[0-9]{1,3}m//g" | base64 | tr -d '\n')
POST_DATA="{\"report\": \"$REPORT_DATA\", \"slug\": \"$TRAVIS_REPO_SLUG\", \"head_branch\": \"$TRAVIS_BRANCH\", \"head_sha\": \"$TRAVIS_COMMIT\"}"

if [[ $TRAVIS_PULL_REQUEST == 'false' ]]; then
  curl -H "Content-Type: application/json" -d "$POST_DATA" -X POST http://borodin.dev.bmstu.cloud/api/duplication/
fi
