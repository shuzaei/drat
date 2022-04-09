import sys
import subprocess
import regex

DRAT_CMD = "drat"
CONTAINER = "/dev/disk3"
VOLUME = "0"
PATH = "/"

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
        match = regex.match(r"^\- DIR REC \|\| .* \|\| name \= (.*)$", line)
        if match:
            filelist.append(path + match.group(1))

    if len(filelist) == 0:
        print(path[:-1])
    else:
        stack.extend(filelist)
