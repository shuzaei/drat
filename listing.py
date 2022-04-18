#!/usr/bin/env python3

import subprocess
import regex

DRAT_CMD = "drat"
CONTAINER = "/dev/disk4"
VOLUME = "0"
PATH = "/"
DEPTH = 3
SEARCH_TYPE = {
    "Unknown": False,
    "FIFO---": False,
    "ChrSpcl": False,
    "Dirctry": True,
    "BlkSpcl": False,
    "RegFile": True,
    "Symlink": True,
    "Socket-": False,
    "Whteout": False,
    "Unrecog": False,
}
EXCLUDE = []


stack = [(PATH, 0)]

while stack:
    path, depth = stack.pop()

    if path[-1] != "/":
        path += "/"

    if path[:-1] in EXCLUDE:
        continue

    if depth == DEPTH:
        break

    result = subprocess.run(
        [DRAT_CMD, "list", CONTAINER, VOLUME, path],
        encoding="utf-8",
        stderr=subprocess.PIPE,
    )

    filelist = []
    for line in result.stderr.splitlines():
        match = regex.match(r"^\- DIR REC \|\| (.*) \|\| name \= (.*)$", line)
        if match:
            if SEARCH_TYPE[match.group(1)[:7]]:
                print(f"{match.group(1)[:7]} {path}{match.group(2)}")
            if match.group(1)[:7] == "Dirctry":
                filelist.append((path + match.group(2), depth + 1))

    if len(filelist) != 0:
        stack.extend(filelist)
