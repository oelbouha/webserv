#!/usr/local/bin/python3
import os
import sys
import fileinput
import urllib.parse

input = ""
for line in fileinput.input():
    input += line

# print(input, file=sys.stderr)
# print('===========\n', file=sys.stderr)

start = input.find("\r\n\r\n") + 4;
end = input.find("\r\n", start, len(input))
name = input[start:end]
input = input[start:len(input)]

start = input.find("\r\n\r\n") + 4;
end = input.find("\r\n", start, len(input))
content_type = input[start:end]
input = input[start:len(input)]

start = input.find("\r\n\r\n") + 4;
end = input.find("\r\n", start, len(input))
path = input[start:end]
input = input[start:len(input)]

splited = content_type.split('/')

stype = splited[0]
gtype = splited[1]

end = path.rfind('/upload')
new_path = path[0:end] + '/pages/upload_dir/' + name

# print( 'new_path', stype, file=sys.stderr)

os.replace(path, new_path)


path = new_path
upload = {
	"name": name,
	"gtype": gtype,
	"stype": stype,
	"path": path
}

if upload['stype'] != 'image':
	pass


print('location: /upload_dir\r\n\r')
