#!/usr/bin/env python3

import os
import subprocess
import regex

DRAT_CMD = "drat"
CONTAINER = "/dev/disk4"
VOLUME = "0"
PATH = ["/"]
OUT = "/"
EXCLUDE = []
# NAME = "/"

stack = PATH

while stack:
    path = stack.pop()

    if path[-1] != "/":
        path += "/"
    if path[:-1] in EXCLUDE:
        continue

    result = subprocess.run(
        [DRAT_CMD, "list", CONTAINER, VOLUME, path],
        encoding="utf-8",
        stderr=subprocess.PIPE,
    )

    dirs = []
    for line in result.stderr.splitlines():
        match = regex.match(r"^\- DIR REC \|\| (.*) \|\| name \= (.*)$", line)
        if match:
            nextpath = path + match.group(2)
            if nextpath[-1] != "/":
                nextpath += "/"
            if path[:-1] in EXCLUDE:
                continue

            if match.group(1)[:7] == "Dirctry":
                dirs.append(nextpath)
                os.makedirs(OUT + nextpath, exist_ok=True, mode=0o777)
            elif match.group(1)[:7] == "Symlink":
                #     file = subprocess.run(
                #         [DRAT_CMD, "recover", CONTAINER, VOLUME, nextpath],
                #         encoding="utf-8",
                #         stdout=subprocess.PIPE,
                #     )
                #     if file[0][:len(NAME)] == NAME:
                #         os.symlink(OUT + file[0][len(NAME):], OUT + nextpath)
                #     else:
                #         os.symlink(file[0], OUT + nextpath)
                os.makedirs(OUT + nextpath, exist_ok=True, mode=0o777)
            else:
                nextpath = nextpath[:-1]
                dir = os.path.dirname(nextpath)
                os.makedirs(OUT + dir, exist_ok=True, mode=0o777)
                file = subprocess.run(
                    [DRAT_CMD, "recover", CONTAINER, VOLUME, nextpath],
                    stdout=subprocess.PIPE,
                )
                with open(OUT + nextpath, "wb") as f:
                    f.write(file.stdout)
                os.chmod(OUT + nextpath, 0o777)

    stack.extend(dirs)
