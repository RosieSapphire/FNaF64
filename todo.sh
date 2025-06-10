#!/usr/bin/fish

grep -E 'todo|fixme' include/** src/** -i --color=always
