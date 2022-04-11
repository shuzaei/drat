#!/usr/bin/env python3

import os
import subprocess
import regex

DRAT_CMD = "drat"
CONTAINER = "/dev/disk4"
VOLUME = "0"
PATH = "/"
OUT = "/Volumes/Volume01/Recovery/"

stack = [(PATH, True)]

while stack:
    path, isDir = stack.pop()

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
            print(match.group(1))
            if regex.match(r"^Dirctry \|\| .*$", match.group(1)):
                filelist.append((path + match.group(2), True))
                os.makedirs(OUT + path + match.group(2), exist_ok=True, mode=0o777)
            else:
                filelist.append((path + match.group(2), False))

    if len(filelist) == 0:
        path = path[:-1]
        if isDir:
            os.makedirs(OUT + path, exist_ok=True, mode=0o777)
        else:
            dir = os.path.dirname(path)
            os.makedirs(OUT + dir, exist_ok=True, mode=0o777)
            file = subprocess.run(
                [DRAT_CMD, "recover", CONTAINER, VOLUME, path], stdout=subprocess.PIPE
            )
            with open(OUT + path, "wb") as f:
                f.write(file.stdout)
            os.chmod(OUT + path, 0o777)

        # print(path)
    else:
        stack.extend(filelist)
