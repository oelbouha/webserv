#!//usr/local/bin/python3

body = '''
<!DOCTYPE html>
<html>
    <head>
        <title>cgi test - index.py </title>
        <style>
            * { margin: 0; padding 0;}
            body {
                background: #282828; 
                padding: 10px; 
                display: flex;
                flex-direction: column;
                justify-content: center;
                align-items: center;
                color: white;
                width: 1080px;
                max-width: 100%;
                margin: 0 auto;
            }
            ul {
                max-width: 100%;
                list-style-type: none;
            }
            img {
                max-width: 100%;
            }
        </style>
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
