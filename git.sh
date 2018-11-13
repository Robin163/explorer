#!/bin/bash

msg=$1
if [ -n "$msg" ]; then
	git add -A
	git commit -m"${msg}"
	git pull
	git status
	echo "finished add、commit、pull !"
	echo "when push, please input name and password"
	git push
else
	echo "input again, for no comments!"
fi
