#!/usr/bin/env python3

import subprocess
import regex

DRAT_CMD = "drat"
CONTAINER = "/dev/disk4"
VOLUME = "0"
PATH = "/"
TYPE = ["Symlink"]  # "Directory", "RegFile", "Symlink", ...
# Learn more at line 245 on https://github.com/jivanpal/drat/blob/567fa16f359659df84bd70c3556f4d446a0b981a/include/drat/string/j.c

stack = [PATH]

while stack:
    path = stack.pop()

    if path[-1] != "/":
        path += "/"

    result = subprocess.run(
        [DRAT_CMD, "list", CONTAINER, VOLUME, path],
        encoding="utf-8",
        stderr=subprocess.PIPE,
    )

    filelist = []
    for line in result.stderr.splitlines():
        match = regex.match(r"^\- DIR REC \|\| (.*) \|\| name \= (.*)$", line)
        if match:
            if match.group(1)[:7] in TYPE:
                print(f"{match.group(1)[:7]} {path}{match.group(2)}")
            if match.group(1)[:7] == "Dirctry":
                filelist.append(path + match.group(2))

    if len(filelist) != 0:
        stack.extend(filelist)
