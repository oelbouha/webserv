#!//usr/local/bin/python3

body = '''
<!DOCTYPE html>
<html>
    <head>
        <title>cgi test page</title>
    </head>
    <body style="margin:0; background-color: black;color:white;">
        <h1>This is a title</h1>
        <ul>
            <li>this is working</li>
            <li>i guess</li>
            <img src="/index.jpeg">
        </ul>
    </body>
</html>
'''

print("content-type: text/html\r")
print("content-length: ", end="")
print(len(body), end="")
print("\r")

print("\r")
print(body)
