import os
import subprocess
import sys
import regex

DRAT_CMD = "drat"
CONTAINER = "/dev/disk3"
VOLUME = "0"
PATH = "/"
OUT = "/Volumes/Files/Recovery/"

stack = [PATH]
indent = 0

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
        path = path[:-1]
        dir = os.path.dirname(path)
        os.makedirs(OUT + dir, exist_ok=True, mode=0o777)
        file = subprocess.run(
            [DRAT_CMD, "recover", CONTAINER, VOLUME, path], stdout=subprocess.PIPE
        )
        with open(OUT + path, "wb") as f:
            f.write(file.stdout)
        os.chmod(OUT + path, 0o777)

        print(path)
    else:
        stack.extend(filelist)
