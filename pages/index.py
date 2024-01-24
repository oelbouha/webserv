body = "this is a text from python script (cgi)"

print("HTTP/1.1 200 OK\r")
print("content-type: text/plain\r")
print("content-length: ", end="")
print(len(body), end="")
print("\r")

print("\r")
print(body)
